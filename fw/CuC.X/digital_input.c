/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Komunikacijski modul kontrolnog kruga
 *
 *******************************************************************************
 * Ime fajla:       control_comm.c
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

#include "digital_input.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars
BYTE digital_input;

BYTE input_0_7;
BYTE input_8_15;
BYTE input_16_23;
BYTE input_24_31;

BYTE input_0_7_raw;
BYTE input_8_15_raw;
BYTE input_16_23_raw;
BYTE input_24_31_raw;


unsigned char  input_0_7_tmr_0;
unsigned char  input_0_7_tmr_1;
unsigned char  input_0_7_tmr_2;
unsigned char  input_0_7_tmr_3;
unsigned char  input_0_7_tmr_4;
unsigned char  input_0_7_tmr_5;
unsigned char  input_0_7_tmr_6;
unsigned char  input_0_7_tmr_7;

unsigned char  input_8_15_tmr_0;
unsigned char  input_8_15_tmr_1;
unsigned char  input_8_15_tmr_2;
unsigned char  input_8_15_tmr_3;
unsigned char  input_8_15_tmr_4;
unsigned char  input_8_15_tmr_5;
unsigned char  input_8_15_tmr_6;
unsigned char  input_8_15_tmr_7;

unsigned char  input_16_23_tmr_0;
unsigned char  input_16_23_tmr_1;
unsigned char  input_16_23_tmr_2;
unsigned char  input_16_23_tmr_3;
unsigned char  input_16_23_tmr_4;
unsigned char  input_16_23_tmr_5;
unsigned char  input_16_23_tmr_6;
unsigned char  input_16_23_tmr_7;

unsigned char  input_24_31_tmr_0;
unsigned char  input_24_31_tmr_1;
unsigned char  input_24_31_tmr_2;
unsigned char  input_24_31_tmr_3;
unsigned char  input_24_31_tmr_4;
unsigned char  input_24_31_tmr_5;
unsigned char  input_24_31_tmr_6;
unsigned char  input_24_31_tmr_7;
//
//
//
#pragma code

void DigitalInputInit(void) {
    DATA_PORT_DIR = INPUT_PORT;
    INPUT_0_7 = HIGH;
    INPUT_8_15 = HIGH;
    INPUT_16_23 = HIGH;
    INPUT_24_31 = HIGH;
    INPUT_32_49 = HIGH;
    INPUT_40_47 = HIGH;
    INPUT_0_7_DIR = OUTPUT_PIN;
    INPUT_8_15_DIR = OUTPUT_PIN;
    INPUT_16_23_DIR = OUTPUT_PIN;
    INPUT_24_31_DIR = OUTPUT_PIN;
    INPUT_32_39_DIR = OUTPUT_PIN;
    INPUT_40_47_DIR = OUTPUT_PIN;
    
}// End of digital input init

    void ReadDigitalInput(void) {
    
    if(!DIN_10_MS_TMR) return;
    else DIN_10_MS_TMR = FALSE;
    
    SetDataPortAsInput();
    SelectDigitalInput_0_7();
    input_0_7_raw._byte = DATA_PORT_RD;
    DeselectDigitalInput_0_7();
    SelectDigitalInput_8_15();
    input_8_15_raw._byte = DATA_PORT_RD;
    DeselectDigitalInput_8_15();
    SelectDigitalInput_16_23();
    input_16_23_raw._byte = DATA_PORT_RD;
    DeselectDigitalInput_16_23();
    SelectDigitalInput_24_31();
    input_24_31_raw._byte = DATA_PORT_RD;
    DeselectDigitalInput_24_31();

    // <editor-fold defaultstate="collapsed" desc="debounce din 0 - prekostrujna zastita VP 1">
    if (input_0_7.b0 != input_0_7_raw.b0) {

        if (input_0_7_tmr_0 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_0;
        else input_0_7.b0 = input_0_7_raw.b0;

    } else input_0_7_tmr_0 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 1 - prekostrujna zastita VP 2">
    if (input_0_7.b1 != input_0_7_raw.b1) {

        if (input_0_7_tmr_1 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_1;
        else input_0_7.b1 = input_0_7_raw.b1;

    } else input_0_7_tmr_1 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 2 - prekostrujna zastita VP 3">
    if (input_0_7.b2 != input_0_7_raw.b2) {

        if (input_0_7_tmr_2 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_2;
        else input_0_7.b2 = input_0_7_raw.b2;

    } else input_0_7_tmr_2 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 3 - temperaturna zastita VP 1">
    if (input_0_7.b3 != input_0_7_raw.b3) {

        if (input_0_7_tmr_3 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_3;
        else input_0_7.b3 = input_0_7_raw.b3;

    } else input_0_7_tmr_3 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 4 - prekostrujna zastita MIKSER">
    if (input_0_7.b4 != input_0_7_raw.b4) {

        if (input_0_7_tmr_4 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_4;
        else input_0_7.b4 = input_0_7_raw.b4;

    } else input_0_7_tmr_4 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 5 - nuzni isklop NOT AUS EMERGENCY">
    if (input_0_7.b5 != input_0_7_raw.b5) {

        if (input_0_7_tmr_5 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_5;
        else input_0_7.b5 = input_0_7_raw.b5;

    } else input_0_7_tmr_5 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 6 - ekstruder RUN">
    if (input_0_7.b6 != input_0_7_raw.b6) {

        if (input_0_7_tmr_6 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_6;
        else input_0_7.b6 = input_0_7_raw.b6;

    } else input_0_7_tmr_6 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 7 - temperaturna zastita VP 2">
    if (input_0_7.b7 != input_0_7_raw.b7) {

        if (input_0_7_tmr_7 != INPUT_DEBOUNCE_TIME) ++input_0_7_tmr_7;
        else input_0_7.b7 = input_0_7_raw.b7;

    } else input_0_7_tmr_7 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 8 - drive 1 OK">
    if (input_8_15.b0 != input_8_15_raw.b0) {

        if (input_8_15_tmr_0 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_0;
        else input_8_15.b0 = input_8_15_raw.b0;

    } else input_8_15_tmr_0 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 9 - drive 2 OK">
    if (input_8_15.b1 != input_8_15_raw.b1) {

        if (input_8_15_tmr_1 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_1;
        else input_8_15.b1 = input_8_15_raw.b1;

    } else input_8_15_tmr_1 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 10 - drive 3 OK">
    if (input_8_15.b2 != input_8_15_raw.b2) {

        if (input_8_15_tmr_2 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_2;
        else input_8_15.b2 = input_8_15_raw.b2;

    } else input_8_15_tmr_2 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 11 - drive 4 OK">
    if (input_8_15.b3 != input_8_15_raw.b3) {

        if (input_8_15_tmr_3 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_3;
        else input_8_15.b3 = input_8_15_raw.b3;

    } else input_8_15_tmr_3 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 12 - mikser nivo materijala max.">
    if (input_8_15.b4 != input_8_15_raw.b4) {

        if (input_8_15_tmr_4 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_4;
        else input_8_15.b4 = input_8_15_raw.b4;

    } else input_8_15_tmr_4 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 13 - mikser nivo materijala min.">
    if (input_8_15.b5 != input_8_15_raw.b5) {

        if (input_8_15_tmr_5 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_5;
        else input_8_15.b5 = input_8_15_raw.b5;

    } else input_8_15_tmr_5 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 14 - ciklon nivo materijala max.">
    if (input_8_15.b6 != input_8_15_raw.b6) {

        if (input_8_15_tmr_6 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_6;
        else input_8_15.b6 = input_8_15_raw.b6;

    } else input_8_15_tmr_6 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 15 - temperaturna zastita VP 3">
    if (input_8_15.b7 != input_8_15_raw.b7) {

        if (input_8_15_tmr_7 != INPUT_DEBOUNCE_TIME) ++input_8_15_tmr_7;
        else input_8_15.b7 = input_8_15_raw.b7;

    } else input_8_15_tmr_7 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 16 - feeder 1 nivo max.">
    if (input_16_23.b0 != input_16_23_raw.b0) {

        if (input_16_23_tmr_0 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_0;
        else input_16_23.b0 = input_16_23_raw.b0;

    } else input_16_23_tmr_0 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 17 - feeder 1 zahtjev za punjenje">
    if (input_16_23.b1 != input_16_23_raw.b1) {

        if (input_16_23_tmr_1 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_1;
        else input_16_23.b1 = input_16_23_raw.b1;

    } else input_16_23_tmr_1 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 18 - feeder 1 nivo min.">
    if (input_16_23.b2 != input_16_23_raw.b2) {

        if (input_16_23_tmr_2 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_2;
        else input_16_23.b2 = input_16_23_raw.b2;

    } else input_16_23_tmr_2 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 19 - feeder 2 nivo max.">
    if (input_16_23.b3 != input_16_23_raw.b3) {

        if (input_16_23_tmr_3 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_3;
        else input_16_23.b3 = input_16_23_raw.b3;

    } else input_16_23_tmr_3 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 20 - feeder 2 zahtjev za punjenje">
    if (input_16_23.b4 != input_16_23_raw.b4) {

        if (input_16_23_tmr_4 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_4;
        else input_16_23.b4 = input_16_23_raw.b4;

    } else input_16_23_tmr_4 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 21 - feeder 2 nivo min.">
    if (input_16_23.b5 != input_16_23_raw.b5) {

        if (input_16_23_tmr_5 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_5;
        else input_16_23.b5 = input_16_23_raw.b5;

    } else input_16_23_tmr_5 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 22 - kontrola vakuuma VP 1">
    if (input_16_23.b6 != input_16_23_raw.b6) {

        if (input_16_23_tmr_6 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_6;
        else input_16_23.b6 = input_16_23_raw.b6;

    } else input_16_23_tmr_6 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 23 - kontrola vakuuma VP3">
    if (input_16_23.b7 != input_16_23_raw.b7) {

        if (input_16_23_tmr_7 != INPUT_DEBOUNCE_TIME) ++input_16_23_tmr_7;
        else input_16_23.b7 = input_16_23_raw.b7;

    } else input_16_23_tmr_7 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 24 - feeder 3 nivo max.">
    if (input_24_31.b0 != input_24_31_raw.b0) {

        if (input_24_31_tmr_0 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_0;
        else input_24_31.b0 = input_24_31_raw.b0;

    } else input_24_31_tmr_0 = 0; // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="debounce din 25 - feeder 3 zahtjev za punjenje">
    if (input_24_31.b1 != input_24_31_raw.b1) {

        if (input_24_31_tmr_1 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_1;
        else input_24_31.b1 = input_24_31_raw.b1;

    } else input_24_31_tmr_1 = 1; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 26 - feeder 3 nivo min.">
    if (input_24_31.b2 != input_24_31_raw.b2) {

        if (input_24_31_tmr_2 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_2;
        else input_24_31.b2 = input_24_31_raw.b2;

    } else input_24_31_tmr_2 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 27 - feeder 4 nivo max.">
    if (input_24_31.b3 != input_24_31_raw.b3) {

        if (input_24_31_tmr_3 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_3;
        else input_24_31.b3 = input_24_31_raw.b3;

    } else input_24_31_tmr_3 = 3; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 28 - feeder 4 zahtjev za punjenje">
    if (input_24_31.b4 != input_24_31_raw.b4) {

        if (input_24_31_tmr_4 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_4;
        else input_24_31.b4 = input_24_31_raw.b4;

    } else input_24_31_tmr_4 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 29 - feeder 4 nivo min.">
    if (input_24_31.b5 != input_24_31_raw.b5) {

        if (input_24_31_tmr_5 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_5;
        else input_24_31.b5 = input_24_31_raw.b5;

    } else input_24_31_tmr_5 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 30 - kontrola vakuuma VP 3">
    if (input_24_31.b6 != input_24_31_raw.b6) {

        if (input_24_31_tmr_6 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_6;
        else input_24_31.b6 = input_24_31_raw.b6;

    } else input_24_31_tmr_6 = 0; // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="debounce din 31 - aux 0">
    if (input_24_31.b7 != input_24_31_raw.b7) {

        if (input_24_31_tmr_7 != INPUT_DEBOUNCE_TIME) ++input_24_31_tmr_7;
        else input_24_31.b7 = input_24_31_raw.b7;

    } else input_24_31_tmr_7 = 0; // </editor-fold>
    
}// End of read digital input
//
//----------------------------------------------- end of file
//
