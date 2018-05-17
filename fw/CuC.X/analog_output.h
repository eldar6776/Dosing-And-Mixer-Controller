/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Zaglavlje modula analognih izlazi
 *
 *******************************************************************************
 * Ime fajla:       analog_output.h
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
//
#ifndef ANALOG_OUTPUT_H
#define ANALOG_OUTPUT_H
//
//
/** U K L J U C E N I  ********************************************************/
//
//
#include "defines.h"
#include "io_cfg.h"
#include "control_comm.h"
#include "process_control.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
//
#pragma udata doser_int_vars
extern unsigned int dos1_set_value, dos2_set_value, dos3_set_value, dos4_set_value;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
//
//
//
/*********   D E F I N I C I J E   ********************************************/
//
//
//
//
/********* M A C R O S   ******************************************************/
//
//
#define AnalogOutputClear() (dos1_set_value = 0, dos2_set_value = 0,\
                             dos3_set_value = 0, dos4_set_value = 0,\
                             WriteAnalogOutput())
//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void AnalogOutputInit(void);
void WriteAnalogOutput(void);

#endif	// End of analog output