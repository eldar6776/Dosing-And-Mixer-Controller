/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                    Modul relejnih izlaza
 *
 *******************************************************************************
 * Ime fajla:       relay_output.c
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

#include "control_comm.h"
#include "digital_input.h"
#include "relay_output.h"
#include "main.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars
unsigned char relay_output;

BYTE relay_output_0_7;
BYTE relay_output_8_15;
//
//
//
#pragma code

void RelayOutputInit(void) {
    OutputDisable();
    OUTPUT1_CLK = LOW;/*set pin o1_clk low*/
    OUTPUT2_CLK = LOW;/*set pin o2_clk low*/
    OUTPUT1_CLK_DIR = OUTPUT_PIN;/*set o1_clk pin as output*/
    OUTPUT2_CLK_DIR = OUTPUT_PIN;/*set o2_clk pin as output*/
}// End of relay output init

void WriteRelayOutput(void){
    DATA_PORT_WR = relay_output_0_7._byte;/*write data to data port latch*/
    SetDataPortAsOutput();/*set port as output*/
    Output2Clock();/*clock data into second latch register*/
    DATA_PORT_WR = relay_output_8_15._byte;
    Output1Clock();/*clock data into first latch register*/
    OutputEnable();
}// End of write relay output
//
//----------------------------------------------- end of file
//

