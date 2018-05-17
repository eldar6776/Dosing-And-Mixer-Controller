/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                          Digitalni ulazi
 *
 *******************************************************************************
 * Ime fajla:       input.h
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
//
#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H
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
extern BYTE digital_input;
#define DIN_1_MS_TMR        digital_input.b0
#define DIN_10_MS_TMR       digital_input.b1
//
//
//
/*********   F L A G S   ******************************************************/
//
//
extern BYTE input_0_7;
#define VP1_CURRENT_PROT    input_0_7.b0
#define VP2_CURRENT_PROT    input_0_7.b1
#define VP3_CURRENT_PROT    input_0_7.b2
#define VP1_THERMO_PROT     input_0_7.b3
#define MIKSER_CURRENT_PROT input_0_7.b4
#define EMERGENCY           input_0_7.b5
#define EXTRUDER_RUN        input_0_7.b6
#define VP2_THERMO_PROT     input_0_7.b7
//
extern BYTE input_8_15;
#define DRIVE1_OK           input_8_15.b0
#define DRIVE2_OK           input_8_15.b1
#define DRIVE3_OK           input_8_15.b2
#define DRIVE4_OK           input_8_15.b3
#define MIX_SEN_MAX         input_8_15.b4
#define MIX_SEN_MIN         input_8_15.b5
#define CYC_SEN_MAX         input_8_15.b6
#define VP3_THERMO_PROT     input_8_15.b7
//
extern BYTE input_16_23;
#define FEEDER1_SEN_MAX     input_16_23.b0
#define FEEDER1_SEN_REQ     input_16_23.b1
#define FEEDER1_SEN_MIN     input_16_23.b2
#define FEEDER2_SEN_MAX     input_16_23.b3
#define FEEDER2_SEN_REQ     input_16_23.b4
#define FEEDER2_SEN_MIN     input_16_23.b5
#define VP1_VACUM_OK        input_16_23.b6
#define VP2_VACUM_OK        input_16_23.b7
//
extern BYTE input_24_31;
#define FEEDER3_SEN_MAX     input_24_31.b0
#define FEEDER3_SEN_REQ     input_24_31.b1
#define FEEDER3_SEN_MIN     input_24_31.b2
#define FEEDER4_SEN_MAX     input_24_31.b3
#define FEEDER4_SEN_REQ     input_24_31.b4
#define FEEDER4_SEN_MIN     input_24_31.b5
#define VP3_VACUM_OK        input_24_31.b6
#define INPUT_AUX0          input_24_31.b7
//
//
/********* M A C R O S   ******************************************************/
//
//
#define SelectDigitalInput_0_7()    (INPUT_0_7 = LOW)
#define DeselectDigitalInput_0_7()  (INPUT_0_7 = HIGH)
#define SelectDigitalInput_8_15()   (INPUT_8_15 = LOW)
#define DeselectDigitalInput_8_15() (INPUT_8_15 = HIGH)
#define SelectDigitalInput_16_23()  (INPUT_16_23 = LOW)
#define DeselectDigitalInput_16_23()(INPUT_16_23 = HIGH)
#define SelectDigitalInput_24_31()  (INPUT_24_31 = LOW)
#define DeselectDigitalInput_24_31()(INPUT_24_31 = HIGH)
//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void DigitalInputInit(void);
void ReadDigitalInput(void);

#endif	// End of digital input