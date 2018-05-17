/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Signal and Keys Controller
 *
 *                      Hardwerske definicije
 *
 *******************************************************************************
 * Ime fajla:       io_cfg.h
 *
 * Procesor:        PIC18F23K20
 *
 * Kompajler:       MCC18 v3.47
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
#include "typedefs.h"

/******** D E F I N I C I J E    P O R T O V A *******************************/

//----------------------------------------------- porta
#define DISP1_LATCH         LATAbits.LATA0
#define DISP2_LATCH         LATAbits.LATA1
#define DISP3_LATCH         LATAbits.LATA2
#define DISP4_LATCH         LATAbits.LATA3
#define DISP5_LATCH         LATAbits.LATA4
#define DISP_DIG_100        LATAbits.LATA5
#define DISP_DIG_10         LATAbits.LATA6
#define DISP_DIG_1          LATAbits.LATA7
#define DISP_CTRL_PORT      LATA

//----------------------------------------------- portb
#define DATA0               PORTBbits.RB0
#define DATA1               PORTBbits.RB1
#define DATA2               PORTBbits.RB2
#define DATA3               PORTBbits.RB3
#define DATA4               PORTBbits.RB4
#define DATA5               PORTBbits.RB5
#define DATA6               PORTBbits.RB6
#define DATA7               PORTBbits.RB7
#define DATA_PORT           PORTB
//----------------------------------------------- portc
#define SH1_LATCH           LATCbits.LATC0
#define SH2_LATCH           LATCbits.LATC1
#define COMM_DATA_DIR       LATCbits.LATC2
#define SH_CLK              LATCbits.LATC3
#define SH1_OE              LATCbits.LATC4
#define SH1_OE_TRIS         TRISCbits.TRISC4
#define SH_DATA             LATCbits.LATC5


#endif	//	IO_CFG_H