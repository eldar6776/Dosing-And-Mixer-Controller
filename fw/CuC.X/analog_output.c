/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                    Modul analognih izlaza
 *
 *******************************************************************************
 * Ime fajla:       analog_output.c
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

#include <pwm.h>
#include <timers.h>
#include "analog_output.h"

//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
//#pragma udata char_vars
//
//
#pragma udata doser_int_vars
unsigned int dos1_set_value, dos2_set_value, dos3_set_value, dos4_set_value;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
//
//
//
//
#pragma code

// <editor-fold defaultstate="collapsed" desc="analog output init">

void AnalogOutputInit(void) {
        //
    //------------------------------------------- open timer 2
    //
    OpenTimer2(TIMER_INT_OFF &
            T2_PS_1_1 &
            T2_POST_1_1);
    //
    //------------------------------------------- open pwm 1
    //
    OpenPWM1(0xff);
    SetOutputPWM1(SINGLE_OUT, PWM_MODE_1);
    SetDCPWM1(0);
    //
    //------------------------------------------- open pwm 2
    //
    OpenPWM2(0xff);
    SetOutputPWM2(SINGLE_OUT, PWM_MODE_1);
    SetDCPWM2(0);
    //
    //------------------------------------------- open pwm 3
    //
    OpenPWM3(0xff);
    SetOutputPWM3(SINGLE_OUT, PWM_MODE_1);
    SetDCPWM3(0);
    //
    //------------------------------------------- open pwm 4
    //
    OpenPWM4(0xff);
    SetDCPWM4(0);
    //
    //------------------------------------------- open pwm 5
    //
    //OpenPWM5(0xff);
    //SetDCPWM5(0);
}// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="write analog output">

void WriteAnalogOutput(void) {
    SetDCPWM1(dos1_set_value & 0x3ff);
    SetDCPWM2(dos2_set_value & 0x3ff);
    SetDCPWM3(dos3_set_value & 0x3ff);
    SetDCPWM4(dos4_set_value & 0x3ff);
    SetDCPWM5(sys_tick._word & 0x3ff);
}// </editor-fold>

//
//----------------------------------------------- end of file
//
