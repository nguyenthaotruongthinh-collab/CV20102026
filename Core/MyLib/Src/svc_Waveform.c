#include "svc_Waveform.h"
#include "bsp_OutputDigital.h"
#include "tim.h"
#include <stdint.h>
#include <math.h>

#define _100A 		250U //mV
#define _us			1000000U //us
#define JUMP_TIME	50 //us
#define DAC_1A		3
#define SIZE_BUFFER	256 // 256 Samples
#define SIMPSON_N	20
#define SAMPLES_TIMES	20 // 20 Samples for 1ms

extern TIM_HandleTypeDef htim3; //1ms

typedef struct{
	uint32_t CMD;
	bool IsEN;
} ValueBuffer_t;

typedef struct {
	ValueBuffer_t data;

} RingBuffer_t;

typedef struct{
	uint32_t RMS[2];

	uint32_t EP[2];
	uint32_t EN[2];

	uint32_t MinAC;

	uint32_t CyclePulse;
	uint32_t T_Peak;
	uint32_t T_Base;

	uint32_t CycleAC;
	uint32_t T_EN;
	uint32_t T_EP;
	uint32_t RaitoNP;
} Data_t;

typedef struct{
	uint32_t Max;

	uint32_t Min;

	uint32_t T_AC;
	uint32_t T_Pulse;

	bool IsBase;
}DataBuffer_t;

static uint16_t s_RMS[1600][AC_SHAPE_ID_COUNT];

static RingBuffer_t s_RingBuffer[SIZE_BUFFER];
static uint32_t s_RingBufferHead = 0;
static uint32_t s_RingBufferTail = 0;

static bool s_IsAC = false;
static bool s_ISPulse = false;

static MIDMenuCOMA_t s_COMA;

static MIDMenuACShapeId_t s_ACShape;
static MIDMenuPulseShape_t s_PulseShape;

static Data_t s_Data;
static DataBuffer_t s_Buffer;

static Data_t s_Data2;

static uint32_t s_CounterAC = 0;
static uint32_t s_TimeAC = 0;

static uint32_t s_CounterPulse = 0;
static uint32_t s_TimePulse = 0;

/* **************** PRIVATE FUNCTION **************** */
// static uint16_t partitionLE_GT(uint16_t a[][AC_SHAPE_ID_COUNT], uint16_t n, uint16_t X, MIDMenuACShapeId_t Shape){
//     uint16_t lo = 0, hi = n;
//     while (lo < hi) {
//         uint16_t mid = lo + (hi - lo) / 2;
//         if (a[mid][Shape] <= X) lo = mid + 1;   /* mid thuộc vùng ≤ → biên phải nằm sau */
//         else             hi = mid;       /* mid thuộc vùng > → biên phải nằm tại/trước */
//     }
//     return lo;   /* = firstGT = lastLE + 1 */
// }

static uint16_t LastLE(uint16_t a[][AC_SHAPE_ID_COUNT], uint16_t n, uint16_t X, MIDMenuACShapeId_t Shape){
	uint16_t lo = 0, hi = n;
	while (lo < hi) {
		uint16_t mid = lo + (hi - lo) / 2;
		if (a[mid][Shape] <= X) lo = mid + 1;   /* mid thuộc vùng ≤ → biên phải nằm sau */
		else             hi = mid;       /* mid thuộc vùng > → biên phải nằm tại/trước */
	}
	return lo - 1;   /* = lastLE */
}

// static uint16_t Interpolate(uint16_t x, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
// 	if (x1 == x0) return y0;  /* tránh chia cho 0 */
// 	return y0 + (uint32_t)(y1 - y0) * (x - x0) / (x1 - x0);
// }

static uint32_t mAtomV (uint32_t mA){
	return (mA*(_100A))/100000;
}

static uint32_t mVtomA (uint32_t mV){
	return (mV*100000)/(_100A);
}

static uint32_t mVtoDAC (uint32_t mV){
	return (mV*MAX_DAC)/VREF;
}

static uint32_t DACtomV (uint32_t dac){
	return (dac*VREF)/MAX_DAC;
}

static uint32_t mAtoDAC (uint32_t mA){
	return mVtoDAC(mAtomV(mA));
}

static uint32_t DACtomA (uint32_t dac){
	return mVtomA((DACtomV(dac)));
}

static void InitRingBuffer(void){
	s_RingBufferHead = 0;
	s_RingBufferTail = SAMPLES_TIMES; // Add 20 Sample for start
	for (uint32_t i = 0; i < SIZE_BUFFER; i++){
		s_RingBuffer[i].data.CMD = 0;
		s_RingBuffer[i].data.IsEN = false;
	}
}

static void AddRingBuffer(uint32_t CMD, bool IsEN){
	if (s_RingBufferTail == s_RingBufferHead){
		s_RingBufferHead++;
		if (s_RingBufferHead >= SIZE_BUFFER){
			s_RingBufferHead = 0;
		}
	}
	s_RingBuffer[s_RingBufferTail].data.CMD = CMD;
	s_RingBuffer[s_RingBufferTail].data.IsEN = IsEN;

	s_RingBufferTail++;
	if (s_RingBufferTail >= SIZE_BUFFER){
		s_RingBufferTail = 0;
	}
}

static void getRingBuffer(ValueBuffer_t *Value){
	Value->CMD = s_RingBuffer[s_RingBufferHead].data.CMD;
	Value->IsEN = s_RingBuffer[s_RingBufferHead].data.IsEN;

	s_RingBufferHead++;
	if (s_RingBufferHead >= SIZE_BUFFER){
		s_RingBufferHead = 0;
	}
}

static void UpdateCounterAC(bool *SetEN){
	if (s_IsAC == false){
		s_Buffer.Max = s_Data.RMS[s_Buffer.IsBase];
		s_CounterAC = 0;
		s_TimeAC = 0;
		return;
	}

	s_CounterAC += JUMP_TIME;

	if (s_CounterAC >= s_Data.CycleAC){
		s_CounterAC = 0;
	}

	if (s_CounterAC >= s_Data.T_EP){
		s_TimeAC = s_CounterAC - s_Data.T_EP;
		s_Buffer.Max = s_Data.EN[s_Buffer.IsBase];
		s_Buffer.T_AC = s_Data.T_EN;

		if (s_TimeAC >= JUMP_TIME)
		{
			*SetEN = true;
		}

		return;
	}

	s_TimeAC = s_CounterAC;
	s_Buffer.Max = s_Data.EP[s_Buffer.IsBase];
	s_Buffer.T_AC = s_Data.T_EP;

	if (s_TimeAC >= JUMP_TIME)
	{
		*SetEN = false;
	}
}

static void UpdateCounterPulse(void){
	if (s_ISPulse == false){
		s_Buffer.IsBase = false;
		s_CounterPulse = 0;
		s_TimePulse = 0;
		return;
	}

	s_CounterPulse += JUMP_TIME;

	if (s_CounterPulse >= s_Data.CyclePulse){
		s_CounterPulse = 0;
	}

	if (s_CounterPulse >= s_Data.T_Peak){
		s_TimePulse = s_CounterPulse - s_Data.T_Peak;
		s_Buffer.IsBase = true;
		s_Buffer.T_Pulse = s_Data.T_Base;
		return;
	}

	s_TimePulse = s_CounterPulse;
	s_Buffer.IsBase = false;
	s_Buffer.T_Pulse = s_Data.T_Peak;
}

static uint32_t ShapeLiner (uint32_t data){
	return data;
}

//AC Shape

static void RampSoftSquare(uint32_t JUp, uint32_t JDown){
	mid_Waveform_setRampSoftSquareAC(mAtoDAC(JUp), mAtoDAC(JDown));
}

static uint32_t ShapeSoftSquareAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t TempMin = (Max*80)/100;

	if (s_COMA == COMA_OFF){
		return mid_Waveform_SoftSquareAC(Min, Max, TMax, t_us);
	}

	if(TempMin >= mAtoDAC(300000)){
		TempMin = mAtoDAC(300000);
	} else if (TempMin >= mAtoDAC(100000)){
		TempMin = mAtoDAC(100000);
	}

	return mid_Waveform_SoftSquareAC(TempMin, Max, TMax, t_us);
}

void RampSquare(uint32_t JUp1, uint32_t JUp2, uint32_t JDown1){
	mid_Waveform_setRampSquareAC(mAtoDAC(JUp1), mAtoDAC(JUp2), mAtoDAC(JDown1));
}

static uint32_t ShapeSquareAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t TempMin = (Max*80)/100;

	if (s_COMA == COMA_OFF){
		return mid_Waveform_SquareAC(Min, Max, TMax, t_us);
	}

	if (s_COMA == COMA_2){
		return ShapeLiner(Max);
	}

	if(TempMin >= mAtoDAC(300000)){
		TempMin = mAtoDAC(300000);
	} else if (TempMin >= mAtoDAC(100000)){
		TempMin = mAtoDAC(100000);
	}

	return mid_Waveform_SquareAC(TempMin, Max, TMax, t_us);
}

static uint32_t ShapeTriAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	return mid_Waveform_TriAC(Min, Max, TMax, t_us);
}

static uint32_t ShapeSineAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	return mid_Waveform_SinAC(Min, Max, TMax, t_us);
}

static uint32_t ShapeAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t Result = 0;
	switch ((uint16_t)s_ACShape) {
		case (uint16_t)AC_SOFTSQUARE:
			Result = ShapeSoftSquareAC(Min, Max, TMax, t_us);
		break;
		case (uint16_t)AC_SQUARE:
			Result =  ShapeSquareAC(Min, Max, TMax, t_us);
		break;
		case (uint16_t)AC_TRIANGLE:
			Result =  ShapeTriAC(Min, Max, TMax, t_us);
		break;
		case (uint16_t)AC_SINE:
			Result =  ShapeSineAC(Min, Max, TMax, t_us);
		break;
		default:
		break;
	}

	return Result;
}

static uint64_t simpson13(uint32_t a, uint32_t b, uint32_t n, uint32_t  i[]){
	uint64_t h = (b - a) / n;
	uint64_t sum = 0;

	for (uint32_t j = 0; j <= n; j++) {
		uint64_t i_squared = i[j] * i[j]; // i^2

		if (j == 0 || j == n) {
			sum += i_squared;
		} else if (j % 2 == 0) {
			sum += 2 * i_squared;
		} else {
			sum += 4 * i_squared;
		}
	}

	return (h / 3) * sum;
}

static uint32_t RMS_ShapeSoftSquare(uint32_t Min, uint32_t NMax, uint32_t RaitoNP){
	uint32_t Negative[SIMPSON_N + 1];
	uint32_t Positive[SIMPSON_N + 1];

	for (uint32_t i = 0; i <= SIMPSON_N; i++){
		Negative[i] = ShapeSoftSquareAC(Min, NMax, s_Data.T_EN, (s_Data.T_EN*i)/SIMPSON_N);
		Positive[i] = ShapeSoftSquareAC(Min, (NMax*RaitoNP)/100, s_Data.T_EP, (s_Data.T_EP*i)/SIMPSON_N);
	}

	uint64_t NRMS_squared = simpson13(0, s_Data.T_EN, SIMPSON_N, Negative);
	uint64_t PRMS_squared = simpson13(0, s_Data.T_EP, SIMPSON_N, Positive);

	uint64_t TotalRMS_squared = (NRMS_squared + PRMS_squared)/(s_Data.T_EN + s_Data.T_EP);
	return (uint32_t)sqrt(TotalRMS_squared);
}

static uint32_t RMS_ShapeSquare(uint32_t Min, uint32_t NMax, uint32_t RaitoNP){
	uint32_t Negative[SIMPSON_N + 1];
	uint32_t Positive[SIMPSON_N + 1];

	for (uint32_t i = 0; i <= SIMPSON_N; i++){
		Negative[i] = ShapeSquareAC(Min, NMax, s_Data.T_EN, (s_Data.T_EN*i)/SIMPSON_N);
		Positive[i] = ShapeSquareAC(Min, (NMax*RaitoNP)/100, s_Data.T_EP, (s_Data.T_EP*i)/SIMPSON_N);
	}

	uint64_t NRMS_squared = simpson13(0, s_Data.T_EN, SIMPSON_N, Negative);
	uint64_t PRMS_squared = simpson13(0, s_Data.T_EP, SIMPSON_N, Positive);

	uint64_t TotalRMS_squared = (NRMS_squared + PRMS_squared)/(s_Data.T_EN + s_Data.T_EP);
	return (uint32_t)sqrt(TotalRMS_squared);
}

static uint32_t RMS_ShapeTri(uint32_t Min, uint32_t NMax, uint32_t RaitoNP){
	uint32_t Negative[SIMPSON_N + 1];
	uint32_t Positive[SIMPSON_N + 1];

	for (uint32_t i = 0; i <= SIMPSON_N; i++){
		Negative[i] = ShapeTriAC(Min, NMax, s_Data.T_EN, (s_Data.T_EN*i)/SIMPSON_N);
		Positive[i] = ShapeTriAC(Min, (NMax*RaitoNP)/100, s_Data.T_EP, (s_Data.T_EP*i)/SIMPSON_N);
	}

	uint64_t NRMS_squared = simpson13(0, s_Data.T_EN, SIMPSON_N, Negative);
	uint64_t PRMS_squared = simpson13(0, s_Data.T_EP, SIMPSON_N, Positive);

	uint64_t TotalRMS_squared = (NRMS_squared + PRMS_squared)/(s_Data.T_EN + s_Data.T_EP);
	return (uint32_t)sqrt(TotalRMS_squared);
}

static uint32_t RMS_ShapeSine(uint32_t Min, uint32_t NMax, uint32_t RaitoNP){
	uint32_t Negative[SIMPSON_N + 1];
	uint32_t Positive[SIMPSON_N + 1];

	for (uint32_t i = 0; i <= SIMPSON_N; i++){
		Negative[i] = ShapeSineAC(Min, NMax, s_Data.T_EN, (s_Data.T_EN*i)/SIMPSON_N);
		Positive[i] = ShapeSineAC(Min, (NMax*RaitoNP)/100, s_Data.T_EP, (s_Data.T_EP*i)/SIMPSON_N);
	}

	uint64_t NRMS_squared = simpson13(0, s_Data.T_EN, SIMPSON_N, Negative);
	uint64_t PRMS_squared = simpson13(0, s_Data.T_EP, SIMPSON_N, Positive);

	uint64_t TotalRMS_squared = (NRMS_squared + PRMS_squared)/(s_Data.T_EN + s_Data.T_EP);
	return (uint32_t)sqrt(TotalRMS_squared);
}

// DC Shape

static uint32_t ShapeSquareDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	return mid_Waveform_SquareDC(Min, Max, TMax, IsPeak, t_us);
}

static uint32_t ShapeTriDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	return mid_Waveform_TriDC(Min, Max, TMax, IsPeak, t_us);
}

static uint32_t ShapeSineDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	return mid_Waveform_SinDC(Min, Max, TMax, IsPeak, t_us);
}

static uint32_t ShapeDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	if (s_ISPulse == false){
		return ShapeLiner(Max);
	}

	uint32_t Result = 0;
	switch ((uint16_t)s_PulseShape) {
		case (uint16_t)PSHAPE_SQUARE:
			Result = ShapeSquareDC(Min, Max, TMax, IsPeak, t_us);
		break;
		case (uint16_t)PSHAPE_TRIANGLE:
			Result = ShapeTriDC(Min, Max, TMax, IsPeak, t_us);
		break;
		case (uint16_t)PSHAPE_SINE:
			Result = ShapeSineDC(Min, Max, TMax, IsPeak, t_us);
		break;
		default:
		break;
	}

	return Result;
}

// static void UpdateDAC1(Data_t Data, uint32_t tDC_us, DataBuffer_t Buffer, uint32_t tAC_us){
// 	uint32_t CMD = 0;

// 	if (s_IsAC == true){
// 		CMD = ShapeAC(Buffer.Min, Buffer.Max, Buffer.T_AC, tAC_us);
// 	} else {
// 		CMD = ShapeDC(Data.RMS[1], Data.RMS[0], Buffer.T_Pulse, !Buffer.IsBase, tDC_us);
// 	}

// 	bsp_OutputAnalog_Reffersh_DAC1(CMD);
// }

static void UpdateDAC1(void){
	ValueBuffer_t Value;
	getRingBuffer(&Value);
	
	if (Value.IsEN == true){
		bsp_OutputDigital_setNormalOutput(OUTPUT_ID_EN);
		bsp_OutputDigital_clearNormalOutput(OUTPUT_ID_EP);
	} else {
		bsp_OutputDigital_setNormalOutput(OUTPUT_ID_EP);
		bsp_OutputDigital_clearNormalOutput(OUTPUT_ID_EN);
	}

	bsp_OutputAnalog_Reffersh_DAC1(Value.CMD);
}

static void UpdateDAC2(uint32_t Data){
	uint32_t CMD = 0;

	CMD = ShapeLiner(Data);

	bsp_OutputAnalog_Reffersh_DAC2(CMD);
}
/* **************** PUBLIC FUNCTION **************** */
void svc_Waveform_UpdateRMSArr(void){
	for(uint32_t i = 0; i < 1600; i++){
		s_RMS[i][AC_SOFTSQUARE] = RMS_ShapeSoftSquare(s_Data.MinAC, i*3, s_Data.RaitoNP);
		s_RMS[i][AC_SQUARE] = RMS_ShapeSquare(s_Data.MinAC, i*3, s_Data.RaitoNP);
		s_RMS[i][AC_TRIANGLE] = RMS_ShapeTri(s_Data.MinAC, i*3, s_Data.RaitoNP);
		s_RMS[i][AC_SINE] = RMS_ShapeSine(s_Data.MinAC, i*3, s_Data.RaitoNP);
	}
}

uint16_t svc_Waveform_getEN_DAC(uint32_t RMS, MIDMenuACShapeId_t Shape){
	if (RMS >= 1600*3){
		RMS = 1600*3 - 1;
	}

	uint16_t Index = LastLE(s_RMS, 1600 - 1, RMS, Shape);
	
	return Index*3;
}

uint32_t svc_Waveform_getEN(uint32_t A_RMS, MIDMenuACShapeId_t Shape){
	uint32_t RMS = mAtoDAC(A_RMS*1000);
	
	return DACtomA(svc_Waveform_getEN_DAC(RMS, Shape))/1000;
}

uint32_t svc_Waveform_getRMS(uint32_t A_EN, MIDMenuACShapeId_t Shape){
	uint32_t EN = mAtoDAC(A_EN*1000);

	if (EN >= 1600*3){
		EN = 1600*3 - 1;
	}

	uint16_t Index = EN/3;
	return DACtomA(s_RMS[Index][Shape])/1000;
}

void svc_Waveform_setPeak(uint32_t Peak, uint32_t MinAC){
	s_Data.RMS[0] = mAtoDAC(Peak);

	s_Data.MinAC = mAtoDAC(MinAC);
	s_Buffer.Min = s_Data.MinAC;
}

void svc_Waveform_setPeak2(uint32_t Peak){
	s_Data2.RMS[0] = mAtoDAC(Peak);
}

void svc_Waveform_setPulse (uint32_t fx10, uint32_t d, uint32_t Base){
	s_Data.CyclePulse 	= (_us*10)/fx10;
	s_Data.T_Peak 		= (s_Data.CyclePulse*d)/100;
	s_Data.T_Base 		= s_Data.CyclePulse - s_Data.T_Peak;

	s_Data.RMS[1] 		= (s_Data.RMS[0]*Base)/100;
}

void svc_Waveform_setAC(uint32_t f, uint32_t Balance, uint32_t RaitoNP){
	s_Data.CycleAC 	= _us/f;
	s_Data.T_EP 	= (s_Data.CycleAC*Balance)/100;
	s_Data.T_EN 	= s_Data.CycleAC - s_Data.T_EP;
	s_Data.RaitoNP  = RaitoNP;
	s_Data.EN[0] 	= svc_Waveform_getEN_DAC(s_Data.RMS[0], s_ACShape);
	s_Data.EP[0] 	= (s_Data.EN[0]*RaitoNP)/100;
	s_Data.EN[1] 	= svc_Waveform_getEN_DAC(s_Data.RMS[1], s_ACShape);
	s_Data.EP[1] 	= (s_Data.EN[1]*RaitoNP)/100;
}

void svc_Waveform_setIsAC(void){
	s_IsAC = true;
}

void svc_Waveform_setIsPulse(void){
	s_ISPulse = true;
}

void svc_Waveform_clearIsAC(void){
	s_IsAC = false;
}

void svc_Waveform_clearIsPulse(void){
	s_ISPulse = false;
}

void svc_Waveform_setShapeAC(MIDMenuACShapeId_t Shape, MIDMenuCOMA_t COMA){
	s_ACShape = Shape;
	s_COMA = COMA;

}

void svc_Waveform_setShapePulse(MIDMenuPulseShape_t Shape){
	s_PulseShape = Shape;
}

void svc_Waveform_Init(void){
	svc_Waveform_clearIsAC();
	svc_Waveform_clearIsPulse();

	RampSoftSquare(2000, 2000);
	RampSquare(100000, 2000, 1000);

	svc_Waveform_setShapeAC(AC_SQUARE, COMA_OFF);
	svc_Waveform_setShapePulse(PSHAPE_TRIANGLE);

	svc_Waveform_setPeak(100000, 100000);
	svc_Waveform_setPulse(20, 50, 50);
	svc_Waveform_setAC(200, 50, 100);

	svc_Waveform_setPeak2(100000);

	InitRingBuffer();
	HAL_TIM_Base_Start_IT(&htim3);
}

void svc_Waveform_UpdateRingBuffer(void){
	ValueBuffer_t Value;

	for (uint32_t i = 0; i < SAMPLES_TIMES; i++){

		UpdateCounterAC(&Value.IsEN);
		UpdateCounterPulse();

		if (s_IsAC == true){
			Value.CMD = ShapeAC(s_Buffer.Min, s_Buffer.Max, s_Buffer.T_AC, s_TimeAC);
		} else {
			Value.CMD = ShapeDC(s_Data.RMS[1], s_Data.RMS[0], s_Buffer.T_Pulse, !s_Buffer.IsBase, s_TimePulse);
		}

		AddRingBuffer(Value.CMD, Value.IsEN);
	}
}

void svc_Waveform_UpdateDAC(void){
	UpdateDAC1();
	UpdateDAC2(s_Data2.RMS[0]);
}
