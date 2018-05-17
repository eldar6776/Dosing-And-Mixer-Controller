/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                 Zaglavlje modula relejni izlazi
 *
 *******************************************************************************
 * Ime fajla:       relay_output.h
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

#ifndef RELAY_OUTPUT_H
#define	RELAY_OUTPUT_H
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
extern unsigned char relay_output, rout_tmp, rout_tmp2;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
extern BYTE relay_output_0_7;
#define VP1_MOTOR               relay_output_0_7.b0
#define VP2_MOTOR               relay_output_0_7.b1
#define DOSER4_MOTOR_FAN        relay_output_0_7.b2
#define VP1_VACUM_VALVE         relay_output_0_7.b3
#define MIXER_MOTOR             relay_output_0_7.b4
#define DRIVE_POWER_CTRL        relay_output_0_7.b5
#define VIBRATOR                relay_output_0_7.b6
#define VP3_MOTOR               relay_output_0_7.b7

extern BYTE relay_output_8_15;
#define FEEDER1_VACUM_VALVE     relay_output_8_15.b0
#define FEEDER2_VACUM_VALVE     relay_output_8_15.b1
#define FEEDER3_VACUM_VALVE     relay_output_8_15.b2
#define FEEDER4_VACUM_VALVE     relay_output_8_15.b3
#define CLEANING_VALVE          relay_output_8_15.b4
#define MILL_ENABLE             relay_output_8_15.b5
#define ALARM                   relay_output_8_15.b6
#define OUTPUT_AUX1             relay_output_8_15.b7

#define RelayOutputClear()     (relay_output_0_7._byte = 0, relay_output_8_15._byte = 0)
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
void RelayOutputInit(void);
void WriteRelayOutput(void);

#endif	/* RELAY_OUTPUT_H */

