/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                      Hardwerske definicije
 *
 *******************************************************************************
 * Ime fajla:       io_cfg.h
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

#ifndef IO_CFG_H
#define IO_CFG_H

#include <p18cxxx.h>
#include "defines.h"

/******** D E F I N I C I J E    P O R T O V A ********************************/

//
//----------------------------------------------- porta
//

//
//----------------------------------------------- portb
//
#define STATUS_LED              LATBbits.LATB0
#define STATUS_LED_TRIS         TRISBbits.TRISB0
#define SDA                     LATBbits.LATB2
#define SDA_PIN                 PORTBbits.RB2
#define SDA_TRIS                TRISBbits.TRISB2
#define SCL                     LATBbits.LATB4
#define SCL_TRIS                TRISBbits.TRISB4
//
//----------------------------------------------- portc
//
#define OUTPUT1_CLK             LATCbits.LATC0
#define OUTPUT1_CLK_DIR         TRISCbits.TRISC0
#define OUTPUT2_CLK             LATCbits.LATC1
#define OUTPUT2_CLK_DIR         TRISCbits.TRISC1
#define PWM0                    PORTCbits.CCP1
#define DOUT_CLK                LATCbits.LATC3
#define DOUT_CLK_DIR            TRISCbits.TRISC3
#define OUTPUT_ENABLE           LATCbits.LATC4
#define OUTPUT_ENABLE_DIR       TRISCbits.TRISC4
#define TERM_DATA_DIR           LATCbits.LATC5
#define TERM_DATA_DIR_TRIS      TRISCbits.TRISC5
#define TERMINAL_TX             PORTCbits.TX
#define TERMINAL_RX             PORTCbits.RX

#define TERM_TX_TRIS            TRISCbits.TRISC6
#define TERM_RX_TRIS            TRISCbits.TRISC7

#define TerminalDataDirRX()     (TERM_DATA_DIR = PIN_LOW)
#define TerminalDataDirTX()     (TERM_DATA_DIR = PIN_HIGH)

#define OutputDisable()         (OUTPUT_ENABLE = HIGH, OUTPUT_ENABLE_DIR = OUTPUT_PIN)
#define OutputEnable()          (OUTPUT_ENABLE = LOW, OUTPUT_ENABLE_DIR = OUTPUT_PIN)
//
//----------------------------------------------- portd
//
#define DATA0                   PORTDbits.RD0
#define DATA1                   PORTDbits.RD1
#define DATA2                   PORTDbits.RD2
#define DATA3                   PORTDbits.RD3
#define DATA4                   PORTDbits.RD4
#define DATA5                   PORTDbits.RD5
#define DATA6                   PORTDbits.RD6
#define DATA7                   PORTDbits.RD7

#define DATA_PORT_RD            PORTD
#define DATA_PORT_WR            LATD
#define DATA_PORT_DIR           TRISD

#define SetDataPortAsInput()    (DATA_PORT_DIR = 0xff)
#define SetDataPortAsOutput()   (DATA_PORT_DIR = 0x00)

//
//----------------------------------------------- porte
//
#define CTRL_DATA_DIR           LATEbits.LATE0
#define CTRL_DATA_DIR_TRIS      TRISEbits.TRISE0
#define INPUT_0_7               LATEbits.LATE1
#define INPUT_0_7_DIR           TRISEbits.TRISE1
#define INPUT_8_15              LATEbits.LATE2
#define INPUT_8_15_DIR          TRISEbits.TRISE2
#define INPUT_16_23             LATEbits.LATE3
#define INPUT_16_23_DIR         TRISEbits.TRISE3
#define INPUT_24_31             LATEbits.LATE4
#define INPUT_24_31_DIR         TRISEbits.TRISE4
#define INPUT_32_49             LATEbits.LATE5
#define INPUT_32_39_DIR         TRISEbits.TRISE5
#define INPUT_40_47             LATEbits.LATE6
#define INPUT_40_47_DIR         TRISEbits.TRISE6
#define PWM1                    PORTEbits.CCP2

#define ControlCommDataDirRX()     (CTRL_DATA_DIR = PIN_LOW)
#define ControlCommDataDirTX()     (CTRL_DATA_DIR = PIN_HIGH)

//
//----------------------------------------------- portf
//

//
//----------------------------------------------- portg
//
#define PWM2                    PORTGbits.CCP3
#define PWM3                    PORTGbits.CCP4
#define PWM4                    PORTGbits.CCP5
#define CONTROL_TX              PORTGbits.TX2
#define CONTROL_RX              PORTGbits.RX2

#define CONTROL_TX_TRIS         TRISGbits.TRISG1
#define CONTROL_RX_TRIS         TRISGbits.TRISG2
//
//
#endif	//	IO_CFG_H