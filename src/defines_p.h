/**
 * SpaIot Library (c) by espilonrt - epsilonrt@gmail.com
 * This file is part of SpaIot library <https://github.com/epsilonrt/spaiot-lib>
 * This file is based on DIYSCIP (c) by Geoffroy HUBERT
 *
 * SpaIot library is licensed under a
 * Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 *
 * You should have received a copy of the license along with this
 * work. If not, see <http://creativecommons.org/licenses/by-nc-sa/4.0/>.
 *
 * SpaIot library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;
 */

/*
 * Frame constant / Serial frame on 16bits.
 *
 * POWER        = P
 * FILTER       = F
 * HEATER       = H
 * HEATER_RED   = HR
 * HEATER_GREEN = HG
 * BUBBLE       = B
 * JET          = J
 * SANITIZER    = S
 * DOWN         = D
 * UP           = U
 * FC           = C
 * BUZZER       = BZ (0 for buzz)
 *          
 *         |-------------------------------------------------------------------------------|
 *         | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | A7 | A6 | A5 | A4 | A3 | A2 | A1 | A0 |
 * BIT     | 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
 *
 * DISPLAY | DP |  0 |  A |  B | S3 |  D |  C | BZ |  E | S1 | S2 |  G |  F | S4 |  0 |  0 |
 *
 * LED_SSP |  X |  1 |  X |  F |  X |  B | HG | BZ | HR |  X |  X |  X |  X |  X |  X |  P |
 *
 * LED_SJB |  X |  1 |  S |  F |  X |  J | HG | BZ | HR |  X |  X |  X |  X |  X |  B |  P |
 *
 * BUT_SSP |  H |  0 |  C |  U |  0 |  P |  X | BZ |  D |  0 |  0 |  X |  B |  0 |  F |  X |
 *
 * BUT_SJB |  H |  0 | *C |  U |  0 |  P |  D | BZ |  F |  0 |  0 |  X | *B |  0 | *J |  S |
 *        * hypothesis that must be verified !
 * 
 *
 * S1 | S2 | S3 | S4  =>  7-segment display     
 *                                              A
 *                                     DP °   -----
 *                                           |     |
 *                                         F |     | B
 *                                           |     |
 *                                            --G--
 *                                           |     |
 *                                         E |     | C
 *                                           |     |
 *                                            -----    
 *                                              D
 *
 *
 *
 */
#define FRAME_BITS_SIZE           16

#define FRAME_CUE                 0x0100
#define FRAME_BP                  0xB58A

#define FRAME_DISPLAY_1           0x0040
#define FRAME_DISPLAY_2           0x0020
#define FRAME_DISPLAY_3           0x0800
#define FRAME_DISPLAY_4           0x0004
#define FRAME_DISPLAY             (FRAME_DISPLAY_1 | FRAME_DISPLAY_2 | FRAME_DISPLAY_3 | FRAME_DISPLAY_4)

#define FRAME_DISPLAY_FRAGMENT_A  0x2000
#define FRAME_DISPLAY_FRAGMENT_B  0x1000
#define FRAME_DISPLAY_FRAGMENT_C  0x0200
#define FRAME_DISPLAY_FRAGMENT_D  0x0400
#define FRAME_DISPLAY_FRAGMENT_E  0x0080
#define FRAME_DISPLAY_FRAGMENT_F  0x0008
#define FRAME_DISPLAY_FRAGMENT_G  0x0010
#define FRAME_DISPLAY_FRAGMENT_DP 0x8000

#define FRAME_DISPLAY_DIGIT_MASK  (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G)

#define FRAME_DISPLAY_OFF         0x0000
#define FRAME_DISPLAY_DIGIT0      (FRAME_DISPLAY_FRAGMENT_A | FRAME_DISPLAY_FRAGMENT_B | FRAME_DISPLAY_FRAGMENT_C | FRAME_DISPLAY_FRAGMENT_D | FRAME_DISPLAY_FRAGMENT_E | FRAME_DISPLAY_FRAGMENT_F)
#define FRAME_DISPLAY_DIGIT1      (FRAME_DISPLAY_FRAGMENT_B | FRAME_DISPLAY_FRAGMENT_C)
#define FRAME_DISPLAY_DIGIT2      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_G|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_D)
#define FRAME_DISPLAY_DIGIT3      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGIT4      (FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C)
#define FRAME_DISPLAY_DIGIT5      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_D)
#define FRAME_DISPLAY_DIGIT6      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGIT7      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C)
#define FRAME_DISPLAY_DIGIT8      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGIT9      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGITA      (FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGITC      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_D)
#define FRAME_DISPLAY_DIGITE      (FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_D|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGITF      (FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_A|FRAME_DISPLAY_FRAGMENT_G)
#define FRAME_DISPLAY_DIGITH      (FRAME_DISPLAY_FRAGMENT_B|FRAME_DISPLAY_FRAGMENT_C|FRAME_DISPLAY_FRAGMENT_E|FRAME_DISPLAY_FRAGMENT_F|FRAME_DISPLAY_FRAGMENT_G)

#define DIGITOFF_VALUE            0xB
#define DIGITH_VALUE              0xD

#define FRAME_LED                 0x4000

#define FRAME_BEEP_BIT            0x0100

#define DISPLAY_DIGIT1            0x0008
#define DISPLAY_DIGIT2            0x0004
#define DISPLAY_DIGIT3            0x0002
#define DISPLAY_DIGIT4            0x0001
#define DISPLAY_ALLDIGITS         DISPLAY_DIGIT1 | DISPLAY_DIGIT2 | DISPLAY_DIGIT3 | DISPLAY_DIGIT4

#define DISPLAY_OFF               0xBBBB
#define DISPLAY_UNIT_F            0x000F
#define DISPLAY_UNIT_C            0x000C

#define DIGITS2UINT(v)            ((((v >> 12) & 0x000F)*100) + (((v >> 8) & 0x000F)*10) + ((v >> 4) & 0x000F))
#define DISPLAY_UNIT(v)           (v & 0x000F)
#define NO_ERROR_ON_DISPLAY(v)    ((v & 0xF000) != 0xE000)
#define DISPLAY2ERROR(v)          ((v >> 4) & 0x0FFF)
#define TEMP_ON_DISPLAY(v)        ((DISPLAY_UNIT(v) == DISPLAY_UNIT_C) || (DISPLAY_UNIT(v) == DISPLAY_UNIT_F))
#define TIMING_ON_DISPLAY(v)      (DISPLAY_UNIT(v) == DIGITH_VALUE)

#define INIT_STABLE_VALUE_COUNTER   10
#define INIT_STABLE_WATER_COUNTER   500

#define BLINK_DESIRED_FRAME_MAX       600
#define BLINK_RESET_FRAME_MIN         1300

#define UNITCHANGE_FRAME_COUNTER_MAX  2500

#define RST_ERROR_FRAME_COUNTER       10000
#define MIN_SANITIZER_FRAME_COUNTER   1500

#define PUSH_COUNTER_MAX          25
