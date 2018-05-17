/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                 Lista adresa u eepromu
 *
 *******************************************************************************
 * Ime fajla:       eeprom_address_list.h
 *
 * Procesor:        PIC18F66J11
 *
 * Kompajler:       MCC18 v3.46
 *
 * IDE:             MPLAB X IDE v1.95 (java 1.7.0_17)
 *
 * Firma:           PROVEN d.o.o. Sarajevo
 *
 * Autor:           <mailto> eldar6776@hotmail.com
 *
 *
 ******************************************************************************/

#ifndef EEPROM_ADDRESS_LIST_H
#define	EEPROM_ADDRESS_LIST_H

//
//  ADDRESS_LABEL   0 x 0 0 0
//                      | | |
//                      | | |--- program variable address
//                      | |----- program number offset
//                      |------- eeprom page offset
//
//
//
//----------------------------------------------- first page
//
#define EE_FIRST_PAGE           0x00
//
//----------------------------------------------- program 1 eeprom address space
//
#define EE_PGM1_DOS1_REF        0x010
#define EE_PGM1_DOS2_REF        0x012
#define EE_PGM1_DOS3_REF        0x014
#define EE_PGM1_DOS4_REF        0x016
#define EE_PGM1_CTRL            0x018
//
//----------------------------------------------- program 2 eeprom address space
//
#define EE_PGM2_DOS1_REF        0x020
#define EE_PGM2_DOS2_REF        0x022
#define EE_PGM2_DOS3_REF        0x024
#define EE_PGM2_DOS4_REF        0x026
#define EE_PGM2_CTRL            0x028
//
//----------------------------------------------- program 3 eeprom address space
//
#define EE_PGM3_DOS1_REF        0x030
#define EE_PGM3_DOS2_REF        0x032
#define EE_PGM3_DOS3_REF        0x034
#define EE_PGM3_DOS4_REF        0x036
#define EE_PGM3_CTRL            0x038
//
//----------------------------------------------- program 4 eeprom address space
//
#define EE_PGM4_DOS1_REF        0x040
#define EE_PGM4_DOS2_REF        0x042
#define EE_PGM4_DOS3_REF        0x044
#define EE_PGM4_DOS4_REF        0x046
#define EE_PGM4_CTRL            0x048
//
//----------------------------------------------- program 5 eeprom address space
//
#define EE_PGM5_DOS1_REF        0x050
#define EE_PGM5_DOS2_REF        0x052
#define EE_PGM5_DOS3_REF        0x054
#define EE_PGM5_DOS4_REF        0x056
#define EE_PGM5_CTRL            0x058
//
//----------------------------------------------- program 6 eeprom address space
//
#define EE_PGM6_DOS1_REF        0x060
#define EE_PGM6_DOS2_REF        0x062
#define EE_PGM6_DOS3_REF        0x064
#define EE_PGM6_DOS4_REF        0x066
#define EE_PGM6_CTRL            0x068
//
//----------------------------------------------- program 7 eeprom address space
//
#define EE_PGM7_DOS1_REF        0x070
#define EE_PGM7_DOS2_REF        0x072
#define EE_PGM7_DOS3_REF        0x074
#define EE_PGM7_DOS4_REF        0x076
#define EE_PGM7_CTRL            0x078
//
//----------------------------------------------- program 8 eeprom address space
//
#define EE_PGM8_DOS1_REF        0x080
#define EE_PGM8_DOS2_REF        0x082
#define EE_PGM8_DOS3_REF        0x084
#define EE_PGM8_DOS4_REF        0x086
#define EE_PGM8_CTRL            0x088
//
//----------------------------------------------- program 9 eeprom address space
//
#define EE_PGM9_DOS1_REF        0x090
#define EE_PGM9_DOS2_REF        0x092
#define EE_PGM9_DOS3_REF        0x094
#define EE_PGM9_DOS4_REF        0x096
#define EE_PGM9_CTRL            0x098
//
//----------------------------------------------- program 10 eeprom address space
//
#define EE_PGM10_DOS1_REF       0x0a0
#define EE_PGM10_DOS2_REF       0x0a2
#define EE_PGM10_DOS3_REF       0x0a4
#define EE_PGM10_DOS4_REF       0x0a6
#define EE_PGM10_CTRL           0x0a8
//
//----------------------------------------------- program 11 eeprom address space
//
#define EE_PGM11_DOS1_REF       0x0b0
#define EE_PGM11_DOS2_REF       0x0b2
#define EE_PGM11_DOS3_REF       0x0b4
#define EE_PGM11_DOS4_REF       0x0b6
#define EE_PGM11_CTRL           0x0b8
//
//----------------------------------------------- program 12 eeprom address space
//
#define EE_PGM12_DOS1_REF       0x0c0
#define EE_PGM12_DOS2_REF       0x0c2
#define EE_PGM12_DOS3_REF       0x0c4
#define EE_PGM12_DOS4_REF       0x0c6
#define EE_PGM12_CTRL           0x0c8
//
//----------------------------------------------- program 13 eeprom address space
//
#define EE_PGM13_DOS1_REF       0x0d0
#define EE_PGM13_DOS2_REF       0x0d2
#define EE_PGM13_DOS3_REF       0x0d4
#define EE_PGM13_DOS4_REF       0x0d6
#define EE_PGM13_CTRL           0x0d8
//
//----------------------------------------------- program 14 eeprom address space
//
#define EE_PGM14_DOS1_REF       0x0e0
#define EE_PGM14_DOS2_REF       0x0e2
#define EE_PGM14_DOS3_REF       0x0e4
#define EE_PGM14_DOS4_REF       0x0e6
#define EE_PGM14_CTRL           0x0e8
//
//----------------------------------------------- program 15 eeprom address space
//
#define EE_PGM15_DOS1_REF       0x0f0
#define EE_PGM15_DOS2_REF       0x0f2
#define EE_PGM15_DOS3_REF       0x0f4
#define EE_PGM15_DOS4_REF       0x0f6
#define EE_PGM15_CTRL           0x0f8
//
//----------------------------------------------- second page
//
#define EE_SECOND_PAGE          0x02
//
//----------------------------------------------- program 16 eeprom address space
//
#define EE_PGM16_DOS1_REF       0x200
#define EE_PGM16_DOS2_REF       0x202
#define EE_PGM16_DOS3_REF       0x204
#define EE_PGM16_DOS4_REF       0x206
#define EE_PGM16_CTRL           0x208
//
//----------------------------------------------- program 17 eeprom address space
//
#define EE_PGM17_DOS1_REF       0x210
#define EE_PGM17_DOS2_REF       0x212
#define EE_PGM17_DOS3_REF       0x214
#define EE_PGM17_DOS4_REF       0x216
#define EE_PGM17_CTRL           0x218
//
//----------------------------------------------- program 18 eeprom address space
//
#define EE_PGM18_DOS1_REF       0x220
#define EE_PGM18_DOS2_REF       0x222
#define EE_PGM18_DOS3_REF       0x224
#define EE_PGM18_DOS4_REF       0x226
#define EE_PGM18_CTRL           0x228
//
//----------------------------------------------- program 19 eeprom address space
//
#define EE_PGM19_DOS1_REF       0x230
#define EE_PGM19_DOS2_REF       0x232
#define EE_PGM19_DOS3_REF       0x234
#define EE_PGM19_DOS4_REF       0x236
#define EE_PGM19_CTRL           0x238
//
//----------------------------------------------- program 20 eeprom address space
//
#define EE_PGM20_DOS1_REF       0x240
#define EE_PGM20_DOS2_REF       0x242
#define EE_PGM20_DOS3_REF       0x244
#define EE_PGM20_DOS4_REF       0x246
#define EE_PGM20_CTRL           0x248
//
//----------------------------------------------- program 21 eeprom address space
//
#define EE_PGM21_DOS1_REF       0x250
#define EE_PGM21_DOS2_REF       0x252
#define EE_PGM21_DOS3_REF       0x254
#define EE_PGM21_DOS4_REF       0x256
#define EE_PGM21_CTRL           0x258
//
//----------------------------------------------- program 22 eeprom address space
//
#define EE_PGM22_DOS1_REF       0x260
#define EE_PGM22_DOS2_REF       0x262
#define EE_PGM22_DOS3_REF       0x264
#define EE_PGM22_DOS4_REF       0x266
#define EE_PGM22_CTRL           0x268
//
//----------------------------------------------- program 23 eeprom address space
//
#define EE_PGM23_DOS1_REF       0x270
#define EE_PGM23_DOS2_REF       0x272
#define EE_PGM23_DOS3_REF       0x274
#define EE_PGM23_DOS4_REF       0x276
#define EE_PGM23_CTRL           0x278
//
//----------------------------------------------- program 24 eeprom address space
//
#define EE_PGM24_DOS1_REF       0x280
#define EE_PGM24_DOS2_REF       0x282
#define EE_PGM24_DOS3_REF       0x284
#define EE_PGM24_DOS4_REF       0x286
#define EE_PGM24_CTRL           0x288
//
//----------------------------------------------- program 25 eeprom address space
//
#define EE_PGM25_DOS1_REF       0x290
#define EE_PGM25_DOS2_REF       0x292
#define EE_PGM25_DOS3_REF       0x294
#define EE_PGM25_DOS4_REF       0x296
#define EE_PGM25_CTRL           0x298
//
//----------------------------------------------- program 26 eeprom address space
//
#define EE_PGM26_DOS1_REF       0x2a0
#define EE_PGM26_DOS2_REF       0x2a2
#define EE_PGM26_DOS3_REF       0x2a4
#define EE_PGM26_DOS4_REF       0x2a6
#define EE_PGM26_CTRL           0x2a8
//
//----------------------------------------------- program 27 eeprom address space
//
#define EE_PGM27_DOS1_REF       0x2b0
#define EE_PGM27_DOS2_REF       0x2b2
#define EE_PGM27_DOS3_REF       0x2b4
#define EE_PGM27_DOS4_REF       0x2b6
#define EE_PGM27_CTRL           0x2b8
//
//----------------------------------------------- program 28 eeprom address space
//
#define EE_PGM28_DOS1_REF       0x2c0
#define EE_PGM28_DOS2_REF       0x2c2
#define EE_PGM28_DOS3_REF       0x2c4
#define EE_PGM28_DOS4_REF       0x2c6
#define EE_PGM28_CTRL           0x2c8
//
//----------------------------------------------- program 29 eeprom address space
//
#define EE_PGM29_DOS1_REF       0x2d0
#define EE_PGM29_DOS2_REF       0x2d2
#define EE_PGM29_DOS3_REF       0x2d4
#define EE_PGM29_DOS4_REF       0x2d6
#define EE_PGM29_CTRL           0x2d8
//
//----------------------------------------------- program 30 eeprom address space
//
#define EE_PGM30_DOS1_REF       0x2e0
#define EE_PGM30_DOS2_REF       0x2e2
#define EE_PGM30_DOS3_REF       0x2e4
#define EE_PGM30_DOS4_REF       0x2e6
#define EE_PGM30_CTRL           0x2e8
//
//----------------------------------------------- program 31 eeprom address space
//
#define EE_PGM31_DOS1_REF       0x2f0
#define EE_PGM31_DOS2_REF       0x2f2
#define EE_PGM31_DOS3_REF       0x2f4
#define EE_PGM31_DOS4_REF       0x2f6
#define EE_PGM31_CTRL           0x2f8
//
//----------------------------------------------- third page
//
#define EE_THIRD_PAGE           0x04
//
//----------------------------------------------- program 32 eeprom address space
//
#define EE_PGM32_DOS1_REF       0x400
#define EE_PGM32_DOS2_REF       0x402
#define EE_PGM32_DOS3_REF       0x404
#define EE_PGM32_DOS4_REF       0x406
#define EE_PGM32_CTRL           0x408
//
//----------------------------------------------- program 33 eeprom address space
//
#define EE_PGM33_DOS1_REF       0x410
#define EE_PGM33_DOS2_REF       0x412
#define EE_PGM33_DOS3_REF       0x414
#define EE_PGM33_DOS4_REF       0x416
#define EE_PGM33_CTRL           0x418
//
//----------------------------------------------- program 34 eeprom address space
//
#define EE_PGM34_DOS1_REF       0x420
#define EE_PGM34_DOS2_REF       0x422
#define EE_PGM34_DOS3_REF       0x424
#define EE_PGM34_DOS4_REF       0x426
#define EE_PGM34_CTRL           0x428
//
//----------------------------------------------- program 35 eeprom address space
//
#define EE_PGM35_DOS1_REF       0x430
#define EE_PGM35_DOS2_REF       0x432
#define EE_PGM35_DOS3_REF       0x434
#define EE_PGM35_DOS4_REF       0x436
#define EE_PGM35_CTRL           0x438
//
//----------------------------------------------- program 36 eeprom address space
//
#define EE_PGM36_DOS1_REF       0x440
#define EE_PGM36_DOS2_REF       0x442
#define EE_PGM36_DOS3_REF       0x444
#define EE_PGM36_DOS4_REF       0x446
#define EE_PGM36_CTRL           0x448
//
//----------------------------------------------- program 37 eeprom address space
//
#define EE_PGM37_DOS1_REF       0x450
#define EE_PGM37_DOS2_REF       0x452
#define EE_PGM37_DOS3_REF       0x454
#define EE_PGM37_DOS4_REF       0x456
#define EE_PGM37_CTRL           0x458
//
//----------------------------------------------- program 38 eeprom address space
//
#define EE_PGM38_DOS1_REF       0x460
#define EE_PGM38_DOS2_REF       0x462
#define EE_PGM38_DOS3_REF       0x464
#define EE_PGM38_DOS4_REF       0x466
#define EE_PGM38_CTRL           0x468
//
//----------------------------------------------- program 39 eeprom address space
//
#define EE_PGM39_DOS1_REF       0x470
#define EE_PGM39_DOS2_REF       0x472
#define EE_PGM39_DOS3_REF       0x474
#define EE_PGM39_DOS4_REF       0x476
#define EE_PGM39_CTRL           0x478
//
//----------------------------------------------- program 40 eeprom address space
//
#define EE_PGM40_DOS1_REF       0x480
#define EE_PGM40_DOS2_REF       0x482
#define EE_PGM40_DOS3_REF       0x484
#define EE_PGM40_DOS4_REF       0x486
#define EE_PGM40_CTRL           0x488
//
//----------------------------------------------- program 41 eeprom address space
//
#define EE_PGM41_DOS1_REF       0x490
#define EE_PGM41_DOS2_REF       0x492
#define EE_PGM41_DOS3_REF       0x494
#define EE_PGM41_DOS4_REF       0x496
#define EE_PGM41_CTRL           0x498
//
//----------------------------------------------- program 42 eeprom address space
//
#define EE_PGM42_DOS1_REF       0x4a0
#define EE_PGM42_DOS2_REF       0x4a2
#define EE_PGM42_DOS3_REF       0x4a4
#define EE_PGM42_DOS4_REF       0x4a6
#define EE_PGM42_CTRL           0x4a8
//
//----------------------------------------------- program 43 eeprom address space
//
#define EE_PGM43_DOS1_REF       0x4b0
#define EE_PGM43_DOS2_REF       0x4b2
#define EE_PGM43_DOS3_REF       0x4b4
#define EE_PGM43_DOS4_REF       0x4b6
#define EE_PGM43_CTRL           0x4b8
//
//----------------------------------------------- program 44 eeprom address space
//
#define EE_PGM44_DOS1_REF       0x4c0
#define EE_PGM44_DOS2_REF       0x4c2
#define EE_PGM44_DOS3_REF       0x4c4
#define EE_PGM44_DOS4_REF       0x4c6
#define EE_PGM44_CTRL           0x4c8
//
//----------------------------------------------- program 45 eeprom address space
//
#define EE_PGM45_DOS1_REF       0x4d0
#define EE_PGM45_DOS2_REF       0x4d2
#define EE_PGM45_DOS3_REF       0x4d4
#define EE_PGM45_DOS4_REF       0x4d6
#define EE_PGM45_CTRL           0x4d9
//
//----------------------------------------------- program 46 eeprom address space
//
#define EE_PGM46_DOS1_REF       0x4e0
#define EE_PGM46_DOS2_REF       0x4e2
#define EE_PGM46_DOS3_REF       0x4e4
#define EE_PGM46_DOS4_REF       0x4e6
#define EE_PGM46_CTRL           0x4e8
//
//----------------------------------------------- program 47 eeprom address space
//
#define EE_PGM47_DOS1_REF       0x4f0
#define EE_PGM47_DOS2_REF       0x4f2
#define EE_PGM47_DOS3_REF       0x4f4
#define EE_PGM47_DOS4_REF       0x4f6
#define EE_PGM47_CTRL           0x4f8
//
//----------------------------------------------- fourth page
//
#define EE_FOURTH_PAGE          0x06
//
//----------------------------------------------- program 48 eeprom address space
//
#define EE_PGM48_DOS1_REF       0x600
#define EE_PGM48_DOS2_REF       0x602
#define EE_PGM48_DOS3_REF       0x604
#define EE_PGM48_DOS4_REF       0x606
#define EE_PGM48_CTRL           0x608
//
//----------------------------------------------- program 49 eeprom address space
//
#define EE_PGM49_DOS1_REF       0x610
#define EE_PGM49_DOS2_REF       0x612
#define EE_PGM49_DOS3_REF       0x614
#define EE_PGM49_DOS4_REF       0x616
#define EE_PGM49_CTRL           0x618
//
//----------------------------------------------- program 50 eeprom address space
//
#define EE_PGM50_DOS1_REF       0x620
#define EE_PGM50_DOS2_REF       0x622
#define EE_PGM50_DOS3_REF       0x624
#define EE_PGM50_DOS4_REF       0x626
#define EE_PGM50_CTRL           0x628
//
//----------------------------------------------- fifth page
//
#define EE_FIFTH_PAGE           0x08
//
//
//
#define EE_VP1_DELAY_TMR                    0x00
#define EE_MIXER_STOP_DELAY_TMR             0x02
#define EE_MIXER_RAW_MAX_ALARM_DELAY_TMR    0x04
#define EE_MIXER_RAW_MAX_LOAD_DELAY_TMR     0x06
#define EE_VIBRATOR_TMR                     0x08
#define EE_VIBRATOR_CYC                     0x0a
#define EE_FEED1_CYC_TIME                   0x0c
#define EE_FEED2_CYC_TIME                   0x0e
#define EE_FEED3_CYC_TIME                   0x10
#define EE_FEED4_CYC_TIME                   0x12
#define EE_CYCLON_LEVEL_MAX_ALARM_DELAY_TMR 0x14
#define EE_MILL_STOP_DELAY_TMR              0x16
#define EE_CLEANING_AIR_CYC                 0x18
#define EE_CLEANING_AIR_TMR                 0x1a
#define EE_TERM_COMM_ADDRESS                0x1c
#define EE_LAST_PGM_IN_USE                  0x1e
#define EE_FW_VERSION                       0x20
//
//
//
#define EE_SIXTH_PAGE                       0x0a
#define EE_SEVENTH_PAGE                     0x0c
#define EE_EIGHT_PAGE                       0x0e
//
//
//
#endif	/* EEPROM_ADDRESS_LIST_H */



