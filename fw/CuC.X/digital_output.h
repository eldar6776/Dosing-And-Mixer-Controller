/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Zaglavlje modula digitalnih izlazi
 *
 *******************************************************************************
 * Ime fajla:       digital_output.h
 *
 * Procesor:        PIC18F66J11
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

#ifndef DIGITAL_OUTPUT_H
#define DIGITAL_OUTPUT_H
//
//
/** U K L J U C E N I  ********************************************************/
//
//
#include "defines.h"
#include "io_cfg.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
extern unsigned char digital_output;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
extern BYTE digital_output_0_7;
#define DRIVE_1_RUN     digital_output_0_7.b0
#define DRIVE_2_RUN     digital_output_0_7.b1
#define DRIVE_3_RUN     digital_output_0_7.b2
#define DRIVE_4_RUN     digital_output_0_7.b3
#define DOUT_AUX0       digital_output_0_7.b4
#define DOUT_AUX1       digital_output_0_7.b5
#define DOUT_AUX2       digital_output_0_7.b6
#define DOUT_AUX3       digital_output_0_7.b7
//
//
/********* M A C R O S   ******************************************************/
//
//

//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void DigitalOutputInit(void);
void WriteDigitalOutput(void);

#endif	// End of digital output