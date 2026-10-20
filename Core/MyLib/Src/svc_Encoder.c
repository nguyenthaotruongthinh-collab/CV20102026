#include "svc_Encoder.h"
#include "svc_Timer.h"

static int8_t s_rawCount = 0;

typedef enum
{
    ENCODER_STATE_00 = 0U,
    ENCODER_STATE_01,
    ENCODER_STATE_10,
    ENCODER_STATE_11
} EncoderState_t;


/* Previous encoder state */
static EncoderState_t s_prevState = ENCODER_STATE_00;


/**
 * @brief Decode quadrature encoder using x2 decoding.
 *
 * Only transitions of channel A are counted.
 *
 * Clockwise:
 *     00 -> 10
 *     11 -> 01
 *
 * Anti-clockwise:
 *     00 -> 01
 *     11 -> 10
 *
 * @return  1  : Clockwise
 * @return -1  : Anti-clockwise
 * @return  0  : No step
 */
static int8_t DeCodeEncoder(void) {
    EncoderState_t currentState;
    int8_t result = 0;

    /* Read encoder */
    bool chA = bsp_InputDigital_EncoderA_ReadPin();
    bool chB = bsp_InputDigital_EncoderB_ReadPin();

    /* Convert A/B to state */
    if ((chA == false) && (chB == false)) {
        currentState = ENCODER_STATE_00;
    } else if ((chA == false) && (chB == true)) {
        currentState = ENCODER_STATE_01;
    } else if ((chA == true) && (chB == false)) {
        currentState = ENCODER_STATE_10;
    } else {
        currentState = ENCODER_STATE_11;
    }

    /*
     * Only process transitions where A changes.
     *
     * CW:
     *     00 -> 10
     *     11 -> 01
     *
     * CCW:
     *     00 -> 01
     *     11 -> 10
     */

    if ((s_prevState == ENCODER_STATE_00) &&
        (currentState == ENCODER_STATE_10)) {
        result = 1;
    } else if ((s_prevState == ENCODER_STATE_11) &&
             (currentState == ENCODER_STATE_01)) {
        result = 1;
    } else if ((s_prevState == ENCODER_STATE_00) &&
             (currentState == ENCODER_STATE_01)) {
        result = -1;
    } else if ((s_prevState == ENCODER_STATE_11) &&
             (currentState == ENCODER_STATE_10)) {
        result = -1;
    } else {
        result = 0;
    }

    /* Save current state */
    s_prevState = currentState;

    return result;
}

static void Encoder_Init(void){
    bool chA = bsp_InputDigital_EncoderA_ReadPin();
    bool chB = bsp_InputDigital_EncoderB_ReadPin();

    if ((chA == false) && (chB == false)) {
        s_prevState = ENCODER_STATE_00;
    } else if ((chA == false) && (chB == true)) {
        s_prevState = ENCODER_STATE_01;
    } else if ((chA == true) && (chB == false)) {
        s_prevState = ENCODER_STATE_10;
    } else {
        s_prevState = ENCODER_STATE_11;
    }
}

void svc_Encoder_Init(void) {
	Encoder_Init();
    s_rawCount = 0;
}

void svc_Encoder_Poll(void) {
	s_rawCount = DeCodeEncoder();
}

int8_t svc_Encoder_getRawCount(void){
	return s_rawCount;
}
