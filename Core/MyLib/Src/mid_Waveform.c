#include "mid_Waveform.h"

#define JUMP_TIME	50 //us

#define TABLE_SIZE_SQRT 257U
#define TABLE_SIZE_SIN 91U
#define TABLE_SIZE_TRI 91U

#define PI 314U
#define _us 1000000U
#define MAX_DAC_OUTPUT 4095U
#define ECLIP_LUT_SIZE 129U

typedef struct {
	uint32_t JUp1;
	uint32_t JUp2;
	uint32_t JDown1;
	uint32_t JDown2;
} RampWaveform_t; // mA/us

static RampWaveform_t s_RampSoftSquare;
static RampWaveform_t s_RampSquare;

static const uint16_t s_EclipTable[ECLIP_LUT_SIZE] = {
	10000U, 10000U, 10000U, 9999U, 9999U, 9998U, 9998U, 9997U,
	9996U, 9995U, 9994U, 9993U, 9991U, 9990U, 9988U, 9986U,
	9984U, 9982U, 9980U, 9978U, 9975U, 9973U, 9970U, 9967U,
	9965U, 9961U, 9958U, 9955U, 9952U, 9948U, 9944U, 9940U,
	9936U, 9932U, 9928U, 9924U, 9919U, 9915U, 9910U, 9905U,
	9900U, 9895U, 9889U, 9884U, 9878U, 9872U, 9866U, 9860U,
	9854U, 9848U, 9841U, 9834U, 9828U, 9820U, 9813U, 9806U,
	9798U, 9791U, 9783U, 9775U, 9767U, 9758U, 9750U, 9741U,
	9732U, 9723U, 9714U, 9704U, 9694U, 9685U, 9674U, 9664U,
	9654U, 9643U, 9632U, 9621U, 9609U, 9598U, 9586U, 9574U,
	9561U, 9549U, 9536U, 9523U, 9509U, 9495U, 9481U, 9467U,
	9452U, 9437U, 9422U, 9407U, 9391U, 9374U, 9357U, 9340U,
	9323U, 9305U, 9287U, 9268U, 9248U, 9229U, 9208U, 9187U,
	9166U, 9144U, 9121U, 9098U, 9073U, 9049U, 9023U, 8996U,
	8968U, 8939U, 8909U, 8878U, 8845U, 8811U, 8775U, 8737U,
	8696U, 8652U, 8605U, 8554U, 8496U, 8430U, 8352U, 8250U, 8000U
};

static const uint32_t s_SQRTTable[TABLE_SIZE_SQRT] = {
	2560000, 2564995, 2569981, 2574956, 2579922, 2584879, 2589826, 2594764,
	2599692, 2604611, 2609521, 2614422, 2619313, 2624195, 2629068, 2633932,
	2638788, 2643634, 2648471, 2653300, 2658120, 2662931, 2667733, 2672527,
	2677312, 2682089, 2686857, 2691617, 2696368, 2701111, 2705846, 2710572,
	2715290, 2720000, 2724702, 2729396, 2734081, 2738759, 2743429, 2748090,
	2752744, 2757390, 2762028, 2766659, 2771281, 2775896, 2780504, 2785103,
	2789695, 2794280, 2798857, 2803426, 2807989, 2812543, 2817091, 2821631,
	2826163, 2830689, 2835207, 2839718, 2844222, 2848719, 2853209, 2857691,
	2862167, 2866636, 2871097, 2875552, 2880000, 2884441, 2888875, 2893303,
	2897723, 2902137, 2906544, 2910945, 2915339, 2919726, 2924107, 2928481,
	2932848, 2937210, 2941564, 2945912, 2950254, 2954590, 2958919, 2963241,
	2967558, 2971868, 2976172, 2980470, 2984761, 2989047, 2993326, 2997599,
	3001866, 3006127, 3010382, 3014631, 3018874, 3023111, 3027342, 3031567,
	3035787, 3040000, 3044208, 3048409, 3052605, 3056796, 3060980, 3065159,
	3069332, 3073500, 3077661, 3081818, 3085968, 3090113, 3094253, 3098387,
	3102515, 3106638, 3110756, 3114868, 3118974, 3123075, 3127171, 3131262,
	3135347, 3139427, 3143501, 3147570, 3151634, 3155693, 3159747, 3163795,
	3167838, 3171876, 3175909, 3179937, 3183960, 3187977, 3191990, 3195997,
	3200000, 3203998, 3207990, 3211978, 3215960, 3219938, 3223911, 3227879,
	3231842, 3235800, 3239753, 3243702, 3247645, 3251584, 3255518, 3259448,
	3263372, 3267292, 3271208, 3275118, 3279024, 3282926, 3286822, 3290714,
	3294602, 3298485, 3302363, 3306237, 3310106, 3313970, 3317831, 3321686,
	3325538, 3329384, 3333227, 3337065, 3340898, 3344727, 3348552, 3352372,
	3356188, 3360000, 3363807, 3367610, 3371409, 3375204, 3378994, 3382780,
	3386562, 3390339, 3394113, 3397882, 3401647, 3405407, 3409164, 3412917,
	3416665, 3420409, 3424150, 3427886, 3431618, 3435346, 3439070, 3442790,
	3446505, 3450217, 3453925, 3457629, 3461329, 3465025, 3468717, 3472406,
	3476090, 3479770, 3483447, 3487119, 3490788, 3494453, 3498114, 3501771,
	3505424, 3509074, 3512720, 3516362, 3520000, 3523634, 3527265, 3530892,
	3534516, 3538135, 3541751, 3545363, 3548972, 3552577, 3556178, 3559775,
	3563369, 3566959, 3570546, 3574129, 3577709, 3581285, 3584857, 3588426,
	3591991, 3595553, 3599111, 3602666, 3606217, 3609765, 3613309, 3616849, 3620387
};

// Look-Up Table (LUT) for fsin(x) = 10000*sin(x)
static const uint32_t s_SinTable[TABLE_SIZE_SIN] = {
	   0,  175,  349,  523,  698,  872, 1045, 1219, 1392, 1564,
	1736, 1908, 2079, 2250, 2419, 2588, 2756, 2924, 3090, 3256,
	3420, 3584, 3746, 3907, 4067, 4226, 4384, 4540, 4695, 4848,
	5000, 5150, 5299, 5446, 5592, 5736, 5878, 6018, 6157, 6293,
	6428, 6561, 6691, 6820, 6947, 7071, 7193, 7314, 7431, 7547,
	7660, 7771, 7880, 7986, 8090, 8192, 8290, 8387, 8480, 8572,
	8660, 8746, 8829, 8910, 8988, 9063, 9135, 9205, 9272, 9336,
	9397, 9455, 9511, 9563, 9613, 9659, 9703, 9744, 9781, 9816,
	9848, 9877, 9903, 9925, 9945, 9962, 9976, 9986, 9994, 9998, 10000
};

// Look-Up Table (LUT) for fTri(x) = 10000*Tri(x)
static const uint32_t s_TriangleTable[TABLE_SIZE_TRI] = {
		0,   111,   222,   333,   444,   556,   667,   778,   889,  1000,
	 1111,  1222,  1333,  1444,  1556,  1667,  1778,  1889,  2000,  2111,
	 2222,  2333,  2444,  2556,  2667,  2778,  2889,  3000,  3111,  3222,
	 3333,  3444,  3556,  3667,  3778,  3889,  4000,  4111,  4222,  4333,
	 4444,  4556,  4667,  4778,  4889,  5000,  5111,  5222,  5333,  5444,
	 5556,  5667,  5778,  5889,  6000,  6111,  6222,  6333,  6444,  6556,
	 6667,  6778,  6889,  7000,  7111,  7222,  7333,  7444,  7556,  7667,
	 7778,  7889,  8000,  8111,  8222,  8333,  8444,  8556,  8667,  8778,
	 8889,  9000,  9111,  9222,  9333,  9444,  9556,  9667,  9778,  9889, 10000
};

// Trả về floor(10000 * sqrt(n)) cho uint32_t
uint32_t isqrt_lut32_x10000(uint32_t n) {
    if (n == 0) return 0;

    // 1. Tìm vị trí bit cao nhất
    int e = 31 - __builtin_clz(n);  // GCC/Clang

    // 2. Chuẩn hóa m về 16-bit fixed point [65536, 131071]
    uint32_t m_fixed;
    if (e >= 16) {
        m_fixed = n >> (e - 16);
    } else {
        m_fixed = n << (16 - e);
    }

    // 3. Lấy chỉ số và phần thập phân
    uint32_t idx = (m_fixed >> 8) - 256;  // 0..255
    uint32_t frac = m_fixed & 0xFF;       // 0..255

    // 4. Nội suy tuyến tính: sqrt_m_scaled ≈ 10000 * 256 * sqrt(m)
    uint32_t v0 = s_SQRTTable[idx];
    uint32_t v1 = s_SQRTTable[idx + 1];
    uint32_t sqrt_m_scaled = v0 + (((v1 - v0) * frac) >> 8);

    // 5. Tính 10000 * sqrt(n) = sqrt_m_scaled * 2^{e/2} / 256
    int k = e >> 1;           // e/2
    int odd = e & 1;          // e lẻ?

    uint64_t temp = sqrt_m_scaled;
    if (odd) {
        temp = (temp * 362) >> 8;  // nhân thêm sqrt(2) ≈ 362/256
    }

    uint64_t result64;
    if (k >= 8) {
        result64 = temp << (k - 8);
    } else {
        result64 = temp >> (8 - k);
    }

    return (uint32_t)result64;  // = floor(10000 * sqrt(n))
}

static uint32_t Interpolate_LUT (uint32_t Angle_x100, const uint32_t LUT[]){
	uint32_t Deg = Angle_x100;

	if (Angle_x100 > 9000){
		Deg = 18000 - Deg;
	}

	uint32_t Index = Deg/100;
	uint32_t Y0 = LUT[Index];
	uint32_t Y1 = LUT[Index + 1];
	uint32_t X0 = Index*100;
	uint32_t X1 = (Index + 1)*100;

	return (Y0 + ((Deg - X0)*(Y1 -Y0))/(X1 - X0));
}

static uint32_t Sin_x10000 (uint32_t Deg_x100){
	return Interpolate_LUT(Deg_x100, s_SinTable);
}

static uint32_t Tri_x10000 (uint32_t Deg_x100){
	return Interpolate_LUT(Deg_x100, s_TriangleTable);
}

static uint32_t Eclip_x10000(uint32_t TMax, uint32_t t_us){
    uint32_t half;
    uint32_t distance;
    uint32_t scaled_distance;
    uint32_t index;
    uint32_t remainder;
    uint32_t y0;
    uint32_t y1;
    uint32_t delta;

    if (TMax < 2U)
    {
        return 8000U;
    }

    if (t_us >= TMax)
    {
        t_us = TMax - 1U;
    }

    half = TMax / 2U;
    if (half == 0U)
    {
        return 8000U;
    }

    distance = (t_us >= half) ? (t_us - half) : (half - t_us);

    /*
     * Fast integer LUT lookup and linear interpolation.
     * Uses 32-bit arithmetic for normal welding periods (for example,
     * TMax <= 125000 us). The table has 129 entries: 10000 at center,
     * decreasing to 8000 at either edge.
     */
    if (distance >= half)
    {
        return 8000U;
    }

    scaled_distance = distance * (ECLIP_LUT_SIZE - 1U);
    index = scaled_distance / half;
    remainder = scaled_distance % half;

    if (index >= (ECLIP_LUT_SIZE - 1U))
    {
        return 8000U;
    }

    y0 = (uint32_t)s_EclipTable[index];
    y1 = (uint32_t)s_EclipTable[index + 1U];
    delta = y0 - y1;

    /* 32-bit interpolation; valid for normal welding-waveform periods. */
    return y0 - ((delta * remainder) / half);
}

void mid_Waveform_setRampSoftSquareAC(uint32_t JUp, uint32_t JDown){
	s_RampSoftSquare.JUp1 = JUp;
	s_RampSoftSquare.JDown1 = JDown;
}

uint32_t mid_Waveform_SoftSquareAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
    uint32_t target;
    uint32_t delta;
    uint32_t j_up;
    uint32_t j_down;
    uint32_t t_up;
    uint32_t t_down;
    uint32_t active_duration;
    uint32_t active_time;
    uint32_t output;
    uint32_t elapsed;

    if ((TMax == 0U) || (t_us >= TMax) || (Max <= Min))
    {
        return (Min <= MAX_DAC_OUTPUT) ? Min : MAX_DAC_OUTPUT;
    }

    if (Max > MAX_DAC_OUTPUT)
    {
        Max = MAX_DAC_OUTPUT;
    }
    if (Min > Max)
    {
        Min = Max;
    }

    target = (Max * 80U) / 100U;
    if (target < Min)
    {
        target = Min;
    }
    delta = target - Min;

    j_up = s_RampSoftSquare.JUp1;
    j_down = s_RampSoftSquare.JDown1;

    /* Zero ramp rates are invalid; return safely instead of dividing by zero. */
    if ((j_up == 0U) || (j_down == 0U) || (delta == 0U))
    {
        return Min;
    }

    /* JUp/JDown are DAC-units per microsecond; t_us is in microseconds. */
    t_up = (delta / j_up) + (((delta % j_up) != 0U) ? 1U : 0U);
    t_down = (delta / j_down) + (((delta % j_down) != 0U) ? 1U : 0U);

    /* Align ramp durations to the 50-us timer period. */
    t_up = ((t_up + JUMP_TIME - 1U) / JUMP_TIME) * JUMP_TIME;
    t_down = ((t_down + JUMP_TIME - 1U) / JUMP_TIME) * JUMP_TIME;

    /* If the requested ramps cannot fit, fail safely rather than underflow. */
    if ((t_up >= TMax) || (t_down >= TMax) || (t_up > (TMax - t_down)))
    {
        return Min;
    }

    if (t_us < t_up)
    {
        /* Saturate before multiplication can overflow. */
        if ((t_us != 0U) && (j_up > (delta / t_us)))
        {
            return target;
        }
        output = Min + (j_up * t_us);
        return (output > target) ? target : output;
    }

    if (t_us >= (TMax - t_down))
    {
        elapsed = t_us - (TMax - t_down);
        if ((elapsed != 0U) && (j_down > (delta / elapsed)))
        {
            return Min;
        }
        output = target - (j_down * elapsed);
        return (output < Min) ? Min : output;
    }

    active_duration = TMax - t_up - t_down;
    active_time = t_us - t_up;
    output = (Max * Eclip_x10000(active_duration, active_time)) / 10000U;

    /* Preserve the 80%-of-Max floor throughout the central section. */
    if (output < target)
    {
        output = target;
    }

    return (output > MAX_DAC_OUTPUT) ? MAX_DAC_OUTPUT : output;
}

void mid_Waveform_setRampSquareAC(uint32_t JUp1, uint32_t JUp2, uint32_t JDown1){
	s_RampSquare.JUp1 = JUp1;
	s_RampSquare.JDown1 = JDown1;
	s_RampSquare.JUp2 = JUp2;
}

uint32_t mid_Waveform_SquareAC (uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t OutputDAC = 0;

	uint32_t JUp1 = s_RampSquare.JUp1;
	uint32_t TUp1 = (((80*Max)/100 - Min)/JUp1)/JUMP_TIME;

	uint32_t JUp2 = s_RampSquare.JUp2;
	uint32_t TUp2 = (((20*Max)/100)/JUp2)/JUMP_TIME;

	uint32_t JDown = s_RampSquare.JDown1;
	uint32_t TDown = (((80*Max)/100 - Min)/JDown)/JUMP_TIME;;


	if (TUp1 <= JUMP_TIME) TUp1 = JUMP_TIME;
	if (TUp2 <= JUMP_TIME) TUp2 = JUMP_TIME;
	if (TDown <= JUMP_TIME) TDown = JUMP_TIME;

	if (t_us < TUp1){
		OutputDAC = Min + JUp1*t_us;

		if (OutputDAC > (80*Max)/100) OutputDAC = (80*Max)/100;

		return OutputDAC;
	}

	if (t_us < TUp2){
		OutputDAC = (80*Max)/100 + JUp2*(t_us - TUp1);

		if (OutputDAC > Max) OutputDAC = Max;

		return OutputDAC;
	}

	if (t_us >= TMax - TDown){
		OutputDAC = Max - JDown*(TMax - t_us - TDown);

		if (OutputDAC < Min) OutputDAC = Min;

		return OutputDAC;
	}

	OutputDAC = Max;

	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}

uint32_t mid_Waveform_SinAC (uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t OutputDAC = 0;

	uint32_t Deg_x100 = (18000*t_us)/TMax;

	if (Max < Min) Max = Min;

	OutputDAC = Min + ((Max - Min)*Sin_x10000(Deg_x100))/10000;

	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}

uint32_t mid_Waveform_TriAC (uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us){
	uint32_t OutputDAC = 0;

	uint32_t Deg_x100 = (18000*t_us)/TMax;

	if (Max < Min) Max = Min;

	OutputDAC = Min + ((Max - Min)*Tri_x10000(Deg_x100))/10000;

	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}

uint32_t mid_Waveform_SquareDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	uint32_t OutputDAC = 0;

	if (IsPeak == true){
		OutputDAC = Max;
	} else {
		OutputDAC = Min;
	}

	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}

uint32_t mid_Waveform_SinDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	uint32_t OutputDAC = 0;
	uint32_t MidZero = (Max + Min)/2;
	uint32_t Deg_x100 = ((180*t_us)/TMax)*100;

	if (IsPeak == true){
		OutputDAC = MidZero + ((Max - MidZero)*Sin_x10000(Deg_x100))/10000;
	} else {
		OutputDAC = MidZero - ((MidZero - Min)*Sin_x10000(Deg_x100))/10000;
	}


	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}

uint32_t mid_Waveform_TriDC (uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us){
	uint32_t OutputDAC = 0;
	uint32_t MidZero = (Max + Min)/2;
	uint32_t Deg_x100 = ((180*t_us)/TMax)*100;

	if (IsPeak == true){
		OutputDAC = MidZero + ((Max - MidZero)*Tri_x10000(Deg_x100))/10000;
	} else{
		OutputDAC = MidZero - ((MidZero - Min)*Tri_x10000(Deg_x100))/10000;
	}
	if(OutputDAC > MAX_DAC_OUTPUT){
		OutputDAC = MAX_DAC_OUTPUT;
	}

	return OutputDAC;
}
