/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                    Modul digitalnih izlaza
 *
 *******************************************************************************
 * Ime fajla:       digital_output.c
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

#include "main.h"
#include "digital_output.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars
unsigned char digital_output;

BYTE digital_output_0_7;
//
//
//
#pragma code

void DigitalOutputInit(void) {
    DigitalOutputClear();
    DOUT_CLK = LOW;
    DOUT_CLK_DIR = OUTPUT_PIN;
}// End of digital output init

void WriteDigitalOutput(void){
    DATA_PORT_WR = digital_output_0_7._byte;
    SetDataPortAsOutput();
    DigitalOutputClock();
    OutputEnable();
}// End of write digital output
//
//----------------------------------------------- end of file
//
