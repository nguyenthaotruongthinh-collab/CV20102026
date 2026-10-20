#ifndef APPLICATION_USER_MYLIB_INC_MID_FONT14SEG_H_
#define APPLICATION_USER_MYLIB_INC_MID_FONT14SEG_H_

#include <stdint.h>

#define SEG_A      (1U << 0)
#define SEG_B      (1U << 1)
#define SEG_C      (1U << 2)
#define SEG_D      (1U << 3)
#define SEG_E      (1U << 4)
#define SEG_F      (1U << 5)
#define SEG_G1     (1U << 6)
#define SEG_G2     (1U << 7)
#define SEG_H      (1U << 8)
#define SEG_I      (1U << 9)
#define SEG_J      (1U << 10)
#define SEG_K      (1U << 11)
#define SEG_L      (1U << 12)
#define SEG_M      (1U << 13)
#define SEG_DP     (1U << 14)

uint16_t mid_Font14Seg_getCode(char ch);

#endif /* APPLICATION_USER_MYLIB_INC_MID_FONT14SEG_H_ */
