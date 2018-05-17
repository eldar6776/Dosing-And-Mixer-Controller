/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                          Glavni program
 *
 *******************************************************************************
 * Ime fajla:       main.c
 *
 * Procesor:        PIC18F66J11
 *
 * Kompajler:       MCC18 v3.47
 *
 * IDE:             MPLAB X IDE v1.95 (java 1.7.0_25)
 *
 * Firma:           PROVEN d.o.o. Sarajevo
 *
 * Autor:           <mailto> eldar6776@hotmail.com
 *
 *
 ******************************************************************************/
//
//
/** U K L J U C E N I *********************************************************/
//
//
#include <p18cxxx.h>
#include <stdlib.h>
#include <delays.h>
#include <usart.h>
#include <timers.h>
#include <pwm.h>
#include "defines.h"
#include "io_cfg.h"
#include "control_comm.h"
#include "terminal_comm.h"
#include "analog_output.h"
#include "digital_input.h"
#include "digital_output.h"
#include "relay_output.h"
#include "process_control.h"
#include "analog_output.h"
#include "main.h"
#include "eeprom_address_list.h"
#include "i2c.h"
//
//
//
//
/** K O N F I G U R A C I J A  M I K R O K O N T R O L E R A  *****************/
//
//
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
//
// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)
//
// CONFIG2L
#pragma config FOSC = INTOSCPLL // Oscillator Selection bits (INTOSC with PLL enabled, port function on RA6 and RA7)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = OFF       // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up disabled)
//
// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscaler Select bits (1:32768)
//
// CONFIG3L
//
// CONFIG3H
#pragma config CCP2MX = ALTERNATE // ECCP2 MUX bit (ECCP2/P2A is multiplexed with RC1)
#pragma config MSSPMSK = MSK7   // MSSP Address Masking Mode Select bit (7-Bit Address Masking mode enable)
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
//#pragma udata sys_char_vars
unsigned char program_select, program_in_use;
unsigned char dsp1_unit, dsp2_unit, dsp3_unit, dsp4_unit;
unsigned char sys_svc_pcnt, dsp_svc_pcnt, comm_svc_pcnt;
unsigned char sig_svc_pcnt, sys_state_pcnt, timer_10_ms;
unsigned char comm_svc_pcnt, comm_cyc_cnt, skc_comm_address;
unsigned char tst_dos1_speed_up_tmr, tst_dos1_speed_dn_tmr;
unsigned char tst_dos2_speed_up_tmr, tst_dos2_speed_dn_tmr;
unsigned char tst_dos3_speed_up_tmr, tst_dos3_speed_dn_tmr;
unsigned char tst_dos4_speed_up_tmr, tst_dos4_speed_dn_tmr;
unsigned char tst_pgm_up_speed_up_tmr, tst_pgm_dn_speed_up_tmr;
unsigned char tst_dos1_speed_up_pcnt, tst_dos1_speed_dn_pcnt;
unsigned char tst_dos2_speed_up_pcnt, tst_dos2_speed_dn_pcnt;
unsigned char tst_dos3_speed_up_pcnt, tst_dos3_speed_dn_pcnt;
unsigned char tst_dos4_speed_up_pcnt, tst_dos4_speed_dn_pcnt;
unsigned char tst_pgm_up_speed_up_pcnt, tst_pgm_dn_speed_up_pcnt;
unsigned char char1, char10, char100, dsp_char_cnt;
unsigned char dsp1_dig100, dsp1_dig10, dsp1_dig1;
unsigned char dsp2_dig100, dsp2_dig10, dsp2_dig1;
unsigned char dsp3_dig100, dsp3_dig10, dsp3_dig1;
unsigned char dsp4_dig100, dsp4_dig10, dsp4_dig1;
unsigned char dsp5_dig100, dsp5_dig10, dsp5_dig1;
unsigned char err_dsp_cnt;
//
//
//#pragma udata sys_int_vars
unsigned int fw_version, program_select_tmr;
unsigned int dsp_tmr, sig_tmr, mnu_cnt, err_dsp_tmr;
unsigned int dos1_ref, dos2_ref, dos3_ref, dos4_ref;
//
/*********   F L A G S   ******************************************************/
//
BYTE sys_flags1;
BYTE sys_flags2;
BYTE sys_flags3;
BYTE sys_flags4;
BYTE sys_flags5;
BYTE sys_flags6;
BYTE sys_flags7;
BYTE sys_flags8;
BYTE sys_flags9;
BYTE sys_flags10;
WORD sys_tick;
DEVICE_STATE control1;
//
//
/**  E N U M E R A T O R S   **************************************************/
//
//
//
//----------------------------------------------- setup menu items

enum menu_items {
    PROG_DSP, SETUP_ENTER, VP1_DELAY_TMR, MIXER_STOP_DELAY_TMR, MIXER_RAW_MAX_ALARM_DELAY_TMR,
    MIXER_RAW_MAX_LOAD_DELAY_TMR, VIBRATOR_TMR, VIBRATOR_CYC, FEED1_CYC_TIME, FEED2_CYC_TIME, FEED3_CYC_TIME,
    FEED4_CYC_TIME, CYCLON_LEVEL_MAX_ALARM_DELAY_TMR, MILL_STOP_DELAY_TMR, CLEANING_AIR_CYC, CLEANING_AIR_TMR,
    TERM_COMM_ADDRESS
} menu_item;
//
//----------------------------------------------- system state enumerators

enum sys_states {
    SYS_STOP, SYS_RUN, SYS_ERROR
} sys_state;
//
//
//*****************************************************************************
//*****************************************************************************
//
//*******************     P R O G R A M  C O D E      *************************
//
//*****************************************************************************
//*****************************************************************************
//
//
#pragma code high_vector = 0x08
//
// <editor-fold defaultstate="collapsed" desc="asm high vector">
//

void interrupt_at_high_vector(void) {
    _asm goto high_isr _endasm
}// End of interrupt_at_high_vector
// </editor-fold>
//
#pragma code low_vector = 0x18
//
// <editor-fold defaultstate="collapsed" desc="asm low vector">
//

void interrupt_at_low_vector(void) {
    _asm goto low_isr _endasm
}// End of interrupt_at_low_vector
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="high interrupt">
#pragma interrupt high_isr
//
//
//

void high_isr(void) {
    //
    //------------------------------------------- check USART 1 RX IRQ
    //
    if (PIE1bits.RC1IE && PIR1bits.RC1IF) {
        term_rx_tmp = getc1USART();
        if ((!term_rx_lenght) && (term_rx_tmp == term_comm_address)) {
            TerminalCommRxAddressingDisable();
            term_rx_buff[0] = term_rx_tmp;
            term_comm_tmr = 15;
            TERM_COM_TEX = FALSE;
            TERM_RX_PENDING = TRUE;
        } else if (!TERM_RX_DATA_RDY && TERM_RX_PENDING) {
            ++term_rx_lenght;
            term_rx_buff[term_rx_lenght] = term_rx_tmp;
            if (term_rx_lenght == 9) TERM_RX_DATA_RDY = TRUE;
        }// End of else if...
    }// End of if USART 1 RX interrupt
    //
    //------------------------------------------- check USART 2 RX IRQ
    //
    if (PIE3bits.RC2IE && PIR3bits.RC2IF) {
        ctrl_rx_tmp = getc2USART();
        if (!IsControlCommReceivingInProgress() && (ctrl_rx_tmp == ctrl_comm_address)) {
            SetControlCommReceivingInProgress();
            SetControlCommReceivingTimer();
            ctrl_rx_buff[0] = ctrl_rx_tmp;
        } else if (!IsControlCommReceivedDataReady() && IsControlCommReceivingInProgress()) {
            ++ctrl_rx_lenght;
            ctrl_rx_buff[ctrl_rx_lenght] = ctrl_rx_tmp;
            if ((ctrl_rx_lenght == 3) && ((ctrl_rx_buff[2] == ACK) || (ctrl_rx_buff[2] == NAK))) {
                SetControlCommReceivedDataReady();
            } else if (ctrl_rx_lenght == 7) {
                if (ctrl_rx_buff[2] == GET_CMD) SetControlCommReceivedDataReady();
                else {
                    ResetControlCommReceivingInProgress();
                    ResetControlCommReceivedDataReady();
                    ctrl_rx_lenght = 0;
                }// End of else...
            }// End of else...
        }// End of else if... 
    }// End of if USART 2 RX interrupt
    //
    //------------------------------------------- check USART 1 TX IRQ
    //
    if (PIE1bits.TX1IE && PIR1bits.TX1IF) {
        if (term_tx_cnt <= term_tx_lenght) {
            putc1USART(term_tx_buff[term_tx_cnt]);
            term_tx_cnt++;
        } else if (TXSTA1bits.TRMT1) {
            TerminalCommTxInterruptDisable();
            TerminalDataDirRX();
            TERM_TX_PENDING = FALSE;
            term_tx_cnt = 0;
        }// End of else if....
    }// End of USART 1 TX INTERRUPT
    //
    //------------------------------------------- check USART 2 TX IRQ
    //
    if (PIE3bits.TX2IE && PIR3bits.TX2IF) {
        if (ctrl_tx_cnt <= ctrl_tx_lenght) {
            putc2USART(ctrl_tx_buff[ctrl_tx_cnt]);
            ctrl_tx_cnt++;
        } else if (TXSTA2bits.TRMT2) {
            ControlCommTxInterruptDisable();
            ControlCommDataDirRX();
            ResetControlCommTransmitInProgress();
            ctrl_tx_cnt = 0;
        }// End of else if....
    }// End of USART 2 TX INTERRUPT
}// End of high_isr
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="low interrupt">
#pragma interruptlow low_isr
//
//
//

void low_isr(void) {
    //00000000
    //------------------------------------------- check interrupt source
    //
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = FALSE; //clear interrupt flag
        TMR0L = 0x06; //
        //--------------------------------------- sys tick 1 ms
        ++sys_tick._word;
        DIN_1_MS_TMR = 1;
        //--------------------------------------- timer 10 ms
        if (++timer_10_ms == 10) {
            timer_10_ms = 0;
            PROCESS_CTRL_TMR_10MS_CLK = TRUE;
            CLOCK_10_MS = TRUE;
            SYS_TMR_FLAG = TRUE;
            DIN_10_MS_TMR = TRUE;
        }// End of data process timer clock
        //--------------------------------------- usart 1 message timeout
        if (term_comm_tmr) {
            --term_comm_tmr;
            TERM_COM_TEX = FALSE;
        } else TERM_COM_TEX = TRUE;
        //--------------------------------------- usart 2 transfer timeout
        if (ctrl_comm_timeout_tmr) {
            --ctrl_comm_timeout_tmr;
            CTRL_COMM_TIMEOUT_TEX = FALSE;
        } else CTRL_COMM_TIMEOUT_TEX = TRUE;
        //--------------------------------------- usart 2 rx pending timeout
        if (ctrl_comm_rx_tmr) {
            --ctrl_comm_rx_tmr;
            CTRL_COMM_RX_TEX = FALSE;
        } else CTRL_COMM_RX_TEX = TRUE;
    }// End of if(INTCONbits.TMR0IE && .....
}// End of low_isr
// </editor-fold>
//
//
//
#pragma code
//
// <editor-fold defaultstate="collapsed" desc="init system">

void InitSYS(void) {
    //
    //------------------------------------------- config oscilator frequency
    //
    //OSCCONbits.IRCF0 = 1;
    //OSCCONbits.IRCF1 = 1;
    //OSCCONbits.IRCF2 = 1;
    OSCTUNEbits.PLLEN = TRUE;
    //
    //------------------------------------------- config ports
    //
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0b00010000;
    LATE = 0b01111110;
    LATF = 0x00;
    LATG = 0x00;
    PORTB = 0;
    TRISA = 0x00;
    TRISB = 0;
    TRISC = 0b11000000;
    TRISE = 0x00;
    TRISF = 0x00;
    TRISG = 0b00000110;
    //
    //------------------------------------------- open timer 0
    //
    OpenTimer0(TIMER_INT_ON &
            T0_8BIT &
            T0_SOURCE_INT &
            T0_PS_1_16);
    //
    //------------------------------------------- interrupt pririty config
    //
    INTCON2bits.TMR0IP = LOW;
    RCONbits.IPEN = TRUE;
    INTCONbits.GIEL = TRUE;
    INTCONbits.GIEH = TRUE;
    //
    //------------------------------------------- init services
    //
    ControlCommInit();
    TerminalCommInit();
    AnalogOutputInit();
    DigitalInputInit();
    DigitalOutputInit();
    RelayOutputInit();
}// End of InitSYS
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="init RAM">

void InitRAM(void) {

    sys_state = SYS_STOP;
    menu_item = PROG_DSP;
    ctrl_comm_address = 0x02;
    skc_comm_address = 0x05;
    program_in_use = I2C_EE_ReadByte(EE_LAST_PGM_IN_USE, EE_FIFTH_PAGE);
    program_select = program_in_use;
    LoadProgram();
    LoadSystemVariable();
    SetSystemInitDisplay();
    SetProgramLoadedDisplay();
    ctrl_comm_address = 0x02;
    skc_comm_address = 0x05;
    vp2_tmr = vp1_tmr;
    vp3_tmr = vp1_tmr;
    Delay10KTCYx(250);
    Delay10KTCYx(250);
}// End of InitRAM
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="main">

void main(void) {

    InitSYS();
    InitRAM();

    while (1) {
        CheckSysTimers();
        CommService();
        IO_Service();
        SysService();
        ProcessService();
        SignalService();
        DisplayService();
    }// End of while(1)
}// End of main
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="check system timers">

void CheckSysTimers(void) {

    if (!SYS_TMR_FLAG) return;
    else SYS_TMR_FLAG = FALSE;

    if (tst_dos1_speed_up_tmr) {
        --tst_dos1_speed_up_tmr;
        TST_DOSER1_SPEED_UP_TEX = FALSE;
    } else TST_DOSER1_SPEED_UP_TEX = TRUE;

    if (tst_dos1_speed_dn_tmr) {
        --tst_dos1_speed_dn_tmr;
        TST_DOSER1_SPEED_DN_TEX = FALSE;
    } else TST_DOSER1_SPEED_DN_TEX = TRUE;

    if (tst_dos2_speed_up_tmr) {
        --tst_dos2_speed_up_tmr;
        TST_DOSER2_SPEED_UP_TEX = FALSE;
    } else TST_DOSER2_SPEED_UP_TEX = TRUE;

    if (tst_dos2_speed_dn_tmr) {
        --tst_dos2_speed_dn_tmr;
        TST_DOSER2_SPEED_DN_TEX = FALSE;
    } else TST_DOSER2_SPEED_DN_TEX = TRUE;

    if (tst_dos3_speed_up_tmr) {
        --tst_dos3_speed_up_tmr;
        TST_DOSER3_SPEED_UP_TEX = FALSE;
    } else TST_DOSER3_SPEED_UP_TEX = TRUE;

    if (tst_dos3_speed_dn_tmr) {
        --tst_dos3_speed_dn_tmr;
        TST_DOSER3_SPEED_DN_TEX = FALSE;
    } else TST_DOSER3_SPEED_DN_TEX = TRUE;

    if (tst_dos4_speed_up_tmr) {
        --tst_dos4_speed_up_tmr;
        TST_DOSER4_SPEED_UP_TEX = FALSE;
    } else TST_DOSER4_SPEED_UP_TEX = TRUE;

    if (tst_dos4_speed_dn_tmr) {
        --tst_dos4_speed_dn_tmr;
        TST_DOSER4_SPEED_DN_TEX = FALSE;
    } else TST_DOSER4_SPEED_DN_TEX = TRUE;

    if (tst_pgm_up_speed_up_tmr) {
        --tst_pgm_up_speed_up_tmr;
        TST_PG_UP_TEX = FALSE;
    } else TST_PG_UP_TEX = TRUE;

    if (tst_pgm_dn_speed_up_tmr) {
        --tst_pgm_dn_speed_up_tmr;
        TST_PG_DN_TEX = FALSE;
    } else TST_PG_DN_TEX = TRUE;

    if (dsp_tmr) {
        --dsp_tmr;
        DSP_TEX = FALSE;
    } else DSP_TEX = TRUE;

    if (sig_tmr) {
        --sig_tmr;
        SIG_TEX = FALSE;
    } else SIG_TEX = TRUE;

    if (err_dsp_tmr) {
        --err_dsp_tmr;
        ERR_DSP_TEX = FALSE;
    } else ERR_DSP_TEX = TRUE;

    if (program_select_tmr) {
        --program_select_tmr;
        PGM_SELECT_TEX = FALSE;
    } else PGM_SELECT_TEX = TRUE;
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="IO service">

void IO_Service(void) {
    //
    //------------------------------------------- service analog/digital io
    //
    ReadDigitalInput();
    WriteAnalogOutput();
    WriteDigitalOutput();
    WriteRelayOutput();
    //
    //------------------------------------------- check taster start
    //
    if (OLD_TST_START != SKC_TST_START) {
        OLD_TST_START = SKC_TST_START;
        if (SKC_TST_START) TST_START_STATE = TRUE;
        else TST_START_STATE = FALSE;
    } else TST_START_STATE = FALSE;
    //
    //------------------------------------------- check taster stop
    //
    if (OLD_TST_STOP != SKC_TST_STOP) {
        OLD_TST_STOP = SKC_TST_STOP;
        if (SKC_TST_STOP) TST_STOP_STATE = TRUE;
        else TST_STOP_STATE = FALSE;
    } else TST_STOP_STATE = FALSE;
    //
    //------------------------------------------- check taster reset
    //
    if (OLD_TST_RESET != SKC_TST_RESET) {
        OLD_TST_RESET = SKC_TST_RESET;
        if (SKC_TST_RESET) TST_RESET_STATE = TRUE;
        else TST_RESET_STATE = FALSE;
    } else TST_RESET_STATE = FALSE;
    //
    //------------------------------------------- check taster program up
    //
    if (!IsMenuVariableActiv()) {
        if (OLD_TST_PG_UP != SKC_TST_PG_UP) {
            OLD_TST_PG_UP = SKC_TST_PG_UP;
            if (SKC_TST_PG_UP) TST_PG_UP_STATE = TRUE;
            else TST_PG_UP_STATE = FALSE;
        } else TST_PG_UP_STATE = FALSE;
    } else {
        if (SKC_TST_PG_UP && TST_PG_UP_TEX) {
            if (tst_pgm_up_speed_up_pcnt < 4) {
                ++tst_pgm_up_speed_up_pcnt;
                tst_pgm_up_speed_up_tmr = 50;
                TST_PG_UP_TEX = FALSE;
            } else if (tst_pgm_up_speed_up_pcnt < 8) {
                ++tst_pgm_up_speed_up_pcnt;
                tst_pgm_up_speed_up_tmr = 20;
                TST_PG_UP_TEX = FALSE;
            } else if (tst_pgm_up_speed_up_pcnt < 12) {
                ++tst_pgm_up_speed_up_pcnt;
                tst_pgm_up_speed_up_tmr = 8;
                TST_PG_UP_TEX = FALSE;
            } else {
                tst_pgm_up_speed_up_tmr = 2;
                TST_PG_UP_TEX = FALSE;
            }// End of else...
            TST_PG_UP_STATE = TRUE;
        } else if (!SKC_TST_PG_UP) {
            TST_PG_UP_STATE = FALSE;
            tst_pgm_up_speed_up_pcnt = 0;
            tst_pgm_up_speed_up_tmr = 0;
            TST_PG_UP_TEX = TRUE;
        } else TST_PG_UP_STATE = FALSE;
    }// End of else...
    //
    //------------------------------------------- check taster program down
    //
    if (!IsMenuVariableActiv()) {
        if (OLD_TST_PG_DN != SKC_TST_PG_DN) {
            OLD_TST_PG_DN = SKC_TST_PG_DN;
            if (SKC_TST_PG_DN) TST_PG_DN_STATE = TRUE;
            else TST_PG_DN_STATE = FALSE;
        } else TST_PG_DN_STATE = FALSE;
    } else {
        if (SKC_TST_PG_DN && TST_PG_DN_TEX) {
            if (tst_pgm_dn_speed_up_pcnt < 4) {
                ++tst_pgm_dn_speed_up_pcnt;
                tst_pgm_dn_speed_up_tmr = 50;
                TST_PG_DN_TEX = FALSE;
            } else if (tst_pgm_dn_speed_up_pcnt < 8) {
                ++tst_pgm_dn_speed_up_pcnt;
                tst_pgm_dn_speed_up_tmr = 20;
                TST_PG_DN_TEX = FALSE;
            } else if (tst_pgm_dn_speed_up_pcnt < 12) {
                ++tst_pgm_dn_speed_up_pcnt;
                tst_pgm_dn_speed_up_tmr = 8;
                TST_PG_DN_TEX = FALSE;
            } else {
                tst_pgm_dn_speed_up_tmr = 2;
                TST_PG_DN_TEX = FALSE;
            }// End of else...
            TST_PG_DN_STATE = TRUE;
        } else if (!SKC_TST_PG_DN) {
            TST_PG_DN_STATE = FALSE;
            tst_pgm_dn_speed_up_pcnt = 0;
            tst_pgm_dn_speed_up_tmr = 0;
            TST_PG_DN_TEX = TRUE;
        } else TST_PG_DN_STATE = FALSE;
    }// End of else...
    //
    //------------------------------------------- check taster program load
    //
    if (OLD_TST_PG_LOAD != SKC_TST_PG_LOAD) {
        OLD_TST_PG_LOAD = SKC_TST_PG_LOAD;
        if (SKC_TST_PG_LOAD) TST_PG_LOAD_STATE = TRUE;
        else TST_PG_LOAD_STATE = FALSE;
    } else TST_PG_LOAD_STATE = FALSE;
    //
    //------------------------------------------- check taster program save
    //
    if (OLD_TST_PG_SAVE != SKC_TST_PG_SAVE) {
        OLD_TST_PG_SAVE = SKC_TST_PG_SAVE;
        if (SKC_TST_PG_SAVE) TST_PG_SAVE_STATE = TRUE;
        else TST_PG_SAVE_STATE = FALSE;
    } else TST_PG_SAVE_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 1 speed up
    //
    if (SKC_TST_DOSER1_SPEED_UP && TST_DOSER1_SPEED_UP_TEX) {
        if (tst_dos1_speed_up_pcnt < 4) {
            ++tst_dos1_speed_up_pcnt;
            tst_dos1_speed_up_tmr = 50;
            TST_DOSER1_SPEED_UP_TEX = FALSE;
        } else if (tst_dos1_speed_up_pcnt < 8) {
            ++tst_dos1_speed_up_pcnt;
            tst_dos1_speed_up_tmr = 20;
            TST_DOSER1_SPEED_UP_TEX = FALSE;
        } else if (tst_dos1_speed_up_pcnt < 12) {
            ++tst_dos1_speed_up_pcnt;
            tst_dos1_speed_up_tmr = 8;
            TST_DOSER1_SPEED_UP_TEX = FALSE;
        } else {
            tst_dos1_speed_up_tmr = 2;
            TST_DOSER1_SPEED_UP_TEX = FALSE;
        }// End of else...
        TST_DOSER1_SPEED_UP_STATE = TRUE;
    } else if (!SKC_TST_DOSER1_SPEED_UP) {
        TST_DOSER1_SPEED_UP_STATE = FALSE;
        tst_dos1_speed_up_pcnt = 0;
        tst_dos1_speed_up_tmr = 0;
        TST_DOSER1_SPEED_UP_TEX = TRUE;
    } else TST_DOSER1_SPEED_UP_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 1 speed down
    //
    if (SKC_TST_DOSER1_SPEED_DN && TST_DOSER1_SPEED_DN_TEX) {
        if (tst_dos1_speed_dn_pcnt < 4) {
            ++tst_dos1_speed_dn_pcnt;
            tst_dos1_speed_dn_tmr = 50;
            TST_DOSER1_SPEED_DN_TEX = FALSE;
        } else if (tst_dos1_speed_dn_pcnt < 8) {
            ++tst_dos1_speed_dn_pcnt;
            tst_dos1_speed_dn_tmr = 20;
            TST_DOSER1_SPEED_DN_TEX = FALSE;
        } else if (tst_dos1_speed_dn_pcnt < 12) {
            ++tst_dos1_speed_dn_pcnt;
            tst_dos1_speed_dn_tmr = 8;
            TST_DOSER1_SPEED_DN_TEX = FALSE;
        } else {
            tst_dos1_speed_dn_tmr = 2;
            TST_DOSER1_SPEED_DN_TEX = FALSE;
        }// End of else...
        TST_DOSER1_SPEED_DN_STATE = TRUE;
    } else if (!SKC_TST_DOSER1_SPEED_DN) {
        TST_DOSER1_SPEED_DN_STATE = FALSE;
        tst_dos1_speed_dn_pcnt = 0;
        tst_dos1_speed_dn_tmr = 0;
        TST_DOSER1_SPEED_DN_TEX = TRUE;
    } else TST_DOSER1_SPEED_DN_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 2 speed up
    //
    if (SKC_TST_DOSER2_SPEED_UP && TST_DOSER2_SPEED_UP_TEX) {
        if (tst_dos2_speed_up_pcnt < 4) {
            ++tst_dos2_speed_up_pcnt;
            tst_dos2_speed_up_tmr = 50;
            TST_DOSER2_SPEED_UP_TEX = FALSE;
        } else if (tst_dos2_speed_up_pcnt < 8) {
            ++tst_dos2_speed_up_pcnt;
            tst_dos2_speed_up_tmr = 20;
            TST_DOSER2_SPEED_UP_TEX = FALSE;
        } else if (tst_dos2_speed_up_pcnt < 12) {
            ++tst_dos2_speed_up_pcnt;
            tst_dos2_speed_up_tmr = 8;
            TST_DOSER2_SPEED_UP_TEX = FALSE;
        } else {
            tst_dos2_speed_up_tmr = 2;
            TST_DOSER2_SPEED_UP_TEX = FALSE;
        }// End of else...
        TST_DOSER2_SPEED_UP_STATE = TRUE;
    } else if (!SKC_TST_DOSER2_SPEED_UP) {
        TST_DOSER2_SPEED_UP_STATE = FALSE;
        tst_dos2_speed_up_pcnt = 0;
        tst_dos2_speed_up_tmr = 0;
        TST_DOSER2_SPEED_UP_TEX = TRUE;
    } else TST_DOSER2_SPEED_UP_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 2 speed down
    //
    if (SKC_TST_DOSER2_SPEED_DN && TST_DOSER2_SPEED_DN_TEX) {
        if (tst_dos2_speed_dn_pcnt < 4) {
            ++tst_dos2_speed_dn_pcnt;
            tst_dos2_speed_dn_tmr = 50;
            TST_DOSER2_SPEED_DN_TEX = FALSE;
        } else if (tst_dos2_speed_dn_pcnt < 8) {
            ++tst_dos2_speed_dn_pcnt;
            tst_dos2_speed_dn_tmr = 20;
            TST_DOSER2_SPEED_DN_TEX = FALSE;
        } else if (tst_dos2_speed_dn_pcnt < 12) {
            ++tst_dos2_speed_dn_pcnt;
            tst_dos2_speed_dn_tmr = 8;
            TST_DOSER2_SPEED_DN_TEX = FALSE;
        } else {
            tst_dos2_speed_dn_tmr = 2;
            TST_DOSER2_SPEED_DN_TEX = FALSE;
        }// End of else...
        TST_DOSER2_SPEED_DN_STATE = TRUE;
    } else if (!SKC_TST_DOSER2_SPEED_DN) {
        TST_DOSER2_SPEED_DN_STATE = FALSE;
        tst_dos2_speed_dn_pcnt = 0;
        tst_dos2_speed_dn_tmr = 0;
        TST_DOSER2_SPEED_DN_TEX = TRUE;
    } else TST_DOSER2_SPEED_DN_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 3 speed up
    //
    if (SKC_TST_DOSER3_SPEED_UP && TST_DOSER3_SPEED_UP_TEX) {
        if (tst_dos3_speed_up_pcnt < 4) {
            ++tst_dos3_speed_up_pcnt;
            tst_dos3_speed_up_tmr = 50;
            TST_DOSER3_SPEED_UP_TEX = FALSE;
        } else if (tst_dos3_speed_up_pcnt < 8) {
            ++tst_dos3_speed_up_pcnt;
            tst_dos3_speed_up_tmr = 20;
            TST_DOSER3_SPEED_UP_TEX = FALSE;
        } else if (tst_dos3_speed_up_pcnt < 12) {
            ++tst_dos3_speed_up_pcnt;
            tst_dos3_speed_up_tmr = 8;
            TST_DOSER3_SPEED_UP_TEX = FALSE;
        } else {
            tst_dos3_speed_up_tmr = 2;
            TST_DOSER3_SPEED_UP_TEX = FALSE;
        }// End of else...
        TST_DOSER3_SPEED_UP_STATE = TRUE;
    } else if (!SKC_TST_DOSER3_SPEED_UP) {
        TST_DOSER3_SPEED_UP_STATE = FALSE;
        tst_dos3_speed_up_pcnt = 0;
        tst_dos3_speed_up_tmr = 0;
        TST_DOSER3_SPEED_UP_TEX = TRUE;
    } else TST_DOSER3_SPEED_UP_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 3 speed down
    //
    if (SKC_TST_DOSER3_SPEED_DN && TST_DOSER3_SPEED_DN_TEX) {
        if (tst_dos3_speed_dn_pcnt < 4) {
            ++tst_dos3_speed_dn_pcnt;
            tst_dos3_speed_dn_tmr = 50;
            TST_DOSER3_SPEED_DN_TEX = FALSE;
        } else if (tst_dos3_speed_dn_pcnt < 8) {
            ++tst_dos3_speed_dn_pcnt;
            tst_dos3_speed_dn_tmr = 20;
            TST_DOSER3_SPEED_DN_TEX = FALSE;
        } else if (tst_dos3_speed_dn_pcnt < 12) {
            ++tst_dos3_speed_dn_pcnt;
            tst_dos3_speed_dn_tmr = 8;
            TST_DOSER3_SPEED_DN_TEX = FALSE;
        } else {
            tst_dos3_speed_dn_tmr = 2;
            TST_DOSER3_SPEED_DN_TEX = FALSE;
        }// End of else...
        TST_DOSER3_SPEED_DN_STATE = TRUE;
    } else if (!SKC_TST_DOSER3_SPEED_DN) {
        TST_DOSER3_SPEED_DN_STATE = FALSE;
        tst_dos3_speed_dn_pcnt = 0;
        tst_dos3_speed_dn_tmr = 0;
        TST_DOSER3_SPEED_DN_TEX = TRUE;
    } else TST_DOSER3_SPEED_DN_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 4 speed up
    //
    if (SKC_TST_DOSER4_SPEED_UP && TST_DOSER4_SPEED_UP_TEX) {
        if (tst_dos4_speed_up_pcnt < 4) {
            ++tst_dos4_speed_up_pcnt;
            tst_dos4_speed_up_tmr = 50;
            TST_DOSER4_SPEED_UP_TEX = FALSE;
        } else if (tst_dos4_speed_up_pcnt < 8) {
            ++tst_dos4_speed_up_pcnt;
            tst_dos4_speed_up_tmr = 20;
            TST_DOSER4_SPEED_UP_TEX = FALSE;
        } else if (tst_dos4_speed_up_pcnt < 12) {
            ++tst_dos4_speed_up_pcnt;
            tst_dos4_speed_up_tmr = 8;
            TST_DOSER4_SPEED_UP_TEX = FALSE;
        } else {
            tst_dos4_speed_up_tmr = 2;
            TST_DOSER4_SPEED_UP_TEX = FALSE;
        }// End of else...
        TST_DOSER4_SPEED_UP_STATE = TRUE;
    } else if (!SKC_TST_DOSER4_SPEED_UP) {
        TST_DOSER4_SPEED_UP_STATE = FALSE;
        tst_dos4_speed_up_pcnt = 0;
        tst_dos4_speed_up_tmr = 0;
        TST_DOSER4_SPEED_UP_TEX = TRUE;
    } else TST_DOSER4_SPEED_UP_STATE = FALSE;
    //
    //------------------------------------------- check taster doser 4 speed down
    //
    if (SKC_TST_DOSER4_SPEED_DN && TST_DOSER4_SPEED_DN_TEX) {
        if (tst_dos4_speed_dn_pcnt < 4) {
            ++tst_dos4_speed_dn_pcnt;
            tst_dos4_speed_dn_tmr = 50;
            TST_DOSER4_SPEED_DN_TEX = FALSE;
        } else if (tst_dos4_speed_dn_pcnt < 8) {
            ++tst_dos4_speed_dn_pcnt;
            tst_dos4_speed_dn_tmr = 20;
            TST_DOSER4_SPEED_DN_TEX = FALSE;
        } else if (tst_dos4_speed_dn_pcnt < 12) {
            ++tst_dos4_speed_dn_pcnt;
            tst_dos4_speed_dn_tmr = 8;
            TST_DOSER4_SPEED_DN_TEX = FALSE;
        } else {
            tst_dos4_speed_dn_tmr = 2;
            TST_DOSER4_SPEED_DN_TEX = FALSE;
        }// End of else...
        TST_DOSER4_SPEED_DN_STATE = TRUE;
    } else if (!SKC_TST_DOSER4_SPEED_DN) {
        TST_DOSER4_SPEED_DN_STATE = FALSE;
        tst_dos4_speed_dn_pcnt = 0;
        tst_dos4_speed_dn_tmr = 0;
        TST_DOSER4_SPEED_DN_TEX = TRUE;
    } else TST_DOSER4_SPEED_DN_STATE = FALSE;
    //
    //------------------------------------------- check taster feeder 1 on/off
    //
    if (OLD_TST_FEEDER1_ON != SKC_TST_FEEDER1_ON) {
        OLD_TST_FEEDER1_ON = SKC_TST_FEEDER1_ON;
        if (SKC_TST_FEEDER1_ON) TST_FEEDER1_ON_STATE = TRUE;
        else TST_FEEDER1_ON_STATE = FALSE;
    } else TST_FEEDER1_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster feeder 2 on/off
    //
    if (OLD_TST_FEEDER2_ON != SKC_TST_FEEDER2_ON) {
        OLD_TST_FEEDER2_ON = SKC_TST_FEEDER2_ON;
        if (SKC_TST_FEEDER2_ON) TST_FEEDER2_ON_STATE = TRUE;
        else TST_FEEDER2_ON_STATE = FALSE;
    } else TST_FEEDER2_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster feeder 3 on/off
    //
    if (OLD_TST_FEEDER3_ON != SKC_TST_FEEDER3_ON) {
        OLD_TST_FEEDER3_ON = SKC_TST_FEEDER3_ON;
        if (SKC_TST_FEEDER3_ON) TST_FEEDER3_ON_STATE = TRUE;
        else TST_FEEDER3_ON_STATE = FALSE;
    } else TST_FEEDER3_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster feeder 4 on/off
    //
    if (OLD_TST_FEEDER4_ON != SKC_TST_FEEDER4_ON) {
        OLD_TST_FEEDER4_ON = SKC_TST_FEEDER4_ON;
        if (SKC_TST_FEEDER4_ON) TST_FEEDER4_ON_STATE = TRUE;
        else TST_FEEDER4_ON_STATE = FALSE;
    } else TST_FEEDER4_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster mixer on/off
    //
    if (OLD_TST_MIKSER_ON != SKC_TST_MIXER_ON) {
        OLD_TST_MIKSER_ON = SKC_TST_MIXER_ON;
        if (SKC_TST_MIXER_ON) TST_MIKSER_ON_STATE = TRUE;
        else TST_MIKSER_ON_STATE = FALSE;
    } else TST_MIKSER_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster vacum pump 1 on/off
    //
    if (OLD_TST_VP1_ON != SKC_TST_VP1_ON) {
        OLD_TST_VP1_ON = SKC_TST_VP1_ON;
        if (SKC_TST_VP1_ON) TST_VP1_ON_STATE = TRUE;
        else TST_VP1_ON_STATE = FALSE;
    } else TST_VP1_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster vacum pump 2 on/off
    //
    if (OLD_TST_VP2_ON != SKC_TST_VP2_ON) {
        OLD_TST_VP2_ON = SKC_TST_VP2_ON;
        if (SKC_TST_VP2_ON) TST_VP2_ON_STATE = TRUE;
        else TST_VP2_ON_STATE = FALSE;
    } else TST_VP2_ON_STATE = FALSE;
    //
    //------------------------------------------- check taster vacum pump 3 on/off
    //
    if (OLD_TST_VP3_ON != SKC_TST_VP3_ON) {
        OLD_TST_VP3_ON = SKC_TST_VP3_ON;
        if (SKC_TST_VP3_ON) TST_VP3_ON_STATE = TRUE;
        else TST_VP3_ON_STATE = FALSE;
    } else TST_VP3_ON_STATE = FALSE;
    //
    //------------------------------------------- check extruder start signal
    //
    if (EXTRUDER_OLD_STATE != EXTRUDER_RUN) {
        EXTRUDER_OLD_STATE = EXTRUDER_RUN;
        if (IsExtruderRunInputActiv()) SetExtruderStartSignal();
        else ResetExtruderStartSignal();
    } else ResetExtruderStartSignal();
}// End of IO_Service
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="comm service">

void CommService(void) {
    //
    //------------------------------------------- select control message type
    //
    if (CLOCK_8_MS && !OLD_COMM_STATE) {
        OLD_COMM_STATE = TRUE;
        if (++comm_svc_pcnt == 4) {
            comm_svc_pcnt = 0;
            ControlCommSendSignalSet();
        } else ControlCommGetSignalSet();
    } else if (!CLOCK_8_MS) OLD_COMM_STATE = FALSE;
    //
    //------------------------------------------- service CuC <=> SKC data transfer
    //
    ControlCommDataSend();
    ControlCommDataReceive();
}// End of CommService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="system service">

void SysService(void) {
    //
    //------------------------------------------- safety first
    //
    if (!IsEmergencySwitchInputActiv()) SetEmergencyAlarm();
    //
    //------------------------------------------- check if both tasters 
    //------------------------------------------- program up and down are pressed
    //------------------------------------------- for opening system setup menu
    //
    if (IsBothPgmUpAndDownTasterPressed()) {
        Delay10TCYx(10);
        if (++mnu_cnt == 5000) menu_item = SETUP_ENTER;
    } else mnu_cnt = 0;
    //
    //------------------------------------------- return to current program in
    //------------------------------------------- use if new program isn't loaded
    //------------------------------------------- nor saved for 20 seconds
    //
    if (IsProgramSelectTimerExpired()) program_select = program_in_use;
    //
    //------------------------------------------- check all SKC tasters
    //
    if (IsTasterProgramLoadPressed()) {//-------- check program load taster
        if (menu_item == PROG_DSP) {
            LoadProgram();
            SetProgramLoadedDisplay();
        } else if (menu_item == SETUP_ENTER) {
            menu_item = VP1_DELAY_TMR;
        } else if (!IsMenuVariableActiv()) {
            SetMenuVariable();
        }// End of else if...        
    } else if (IsTasterProgramSavePressed()) {//- check program save taster
        if (menu_item == PROG_DSP) {
            SaveProgram();
            SetProgramSavedDisplay();
            SetProgramSelectTimer();
        } else if (IsMenuVariableActiv()) {
            ResetMenuVariable();
        } else if (menu_item != PROG_DSP) {
            SaveSystemVariable();
            menu_item = PROG_DSP;
        }// End of else if...   
    } else if (IsTasterProgramUpPressed() && !IsBothPgmUpAndDownTasterPressed()) {//--- check program up taster
        if ((menu_item == PROG_DSP) && (program_select < 50)) {
            SetProgramSelectTimer();
            ++program_select;
        } else if (!IsMenuVariableActiv()) {
            //
            //----------------------------------- if not in variable setup
            //----------------------------------- change menu items
            //
            if (menu_item == VP1_DELAY_TMR) menu_item = MIXER_STOP_DELAY_TMR;
            else if (menu_item == MIXER_STOP_DELAY_TMR) menu_item = MIXER_RAW_MAX_ALARM_DELAY_TMR;
            else if (menu_item == MIXER_RAW_MAX_ALARM_DELAY_TMR) menu_item = MIXER_RAW_MAX_LOAD_DELAY_TMR;
            else if (menu_item == MIXER_RAW_MAX_LOAD_DELAY_TMR) menu_item = VIBRATOR_TMR;
            else if (menu_item == VIBRATOR_TMR) menu_item = VIBRATOR_CYC;
            else if (menu_item == VIBRATOR_CYC) menu_item = FEED1_CYC_TIME;
            else if (menu_item == FEED1_CYC_TIME) menu_item = FEED2_CYC_TIME;
            else if (menu_item == FEED2_CYC_TIME) menu_item = FEED3_CYC_TIME;
            else if (menu_item == FEED3_CYC_TIME) menu_item = FEED4_CYC_TIME;
            else if (menu_item == FEED4_CYC_TIME) menu_item = CYCLON_LEVEL_MAX_ALARM_DELAY_TMR;
            else if (menu_item == CYCLON_LEVEL_MAX_ALARM_DELAY_TMR) menu_item = MILL_STOP_DELAY_TMR;
            else if (menu_item == MILL_STOP_DELAY_TMR) menu_item = CLEANING_AIR_CYC;
            else if (menu_item == CLEANING_AIR_CYC) menu_item = CLEANING_AIR_TMR;
            else if (menu_item == CLEANING_AIR_TMR) menu_item = TERM_COMM_ADDRESS;
            else if (menu_item == TERM_COMM_ADDRESS) menu_item = VP1_DELAY_TMR;
        } else if (IsMenuVariableActiv()) {
            //
            //----------------------------------- if variable setup preselected
            //----------------------------------- change values
            //
            if ((menu_item == VP1_DELAY_TMR) && (vp1_delay_time < 999)) ++vp1_delay_time;
            else if ((menu_item == MIXER_STOP_DELAY_TMR) && (mixer_stop_delay_time < 999)) ++mixer_stop_delay_time;
            else if ((menu_item == MIXER_RAW_MAX_ALARM_DELAY_TMR) && (mix_raw_max_alarm_delay_time < 999)) ++mix_raw_max_alarm_delay_time;
            else if ((menu_item == MIXER_RAW_MAX_LOAD_DELAY_TMR) && (mixer_load_stop_delay_time < 999)) ++mixer_load_stop_delay_time;
            else if ((menu_item == VIBRATOR_TMR) && (vibrator_time < 999)) ++vibrator_time;
            else if ((menu_item == VIBRATOR_CYC) && (vibrator_cyc < 999)) ++vibrator_cyc;
            else if ((menu_item == FEED1_CYC_TIME) && (feed1_cyc_time < 999)) ++feed1_cyc_time;
            else if ((menu_item == FEED2_CYC_TIME) && (feed2_cyc_time < 999)) ++feed2_cyc_time;
            else if ((menu_item == FEED3_CYC_TIME) && (feed3_cyc_time < 999)) ++feed3_cyc_time;
            else if ((menu_item == FEED4_CYC_TIME) && (feed4_cyc_time < 999)) ++feed4_cyc_time;
            else if ((menu_item == CYCLON_LEVEL_MAX_ALARM_DELAY_TMR) && (cyclon_alarm_delay_time < 999)) ++cyclon_alarm_delay_time;
            else if ((menu_item == MILL_STOP_DELAY_TMR) && (mill_stop_delay_time < 999)) ++mill_stop_delay_time;
            else if ((menu_item == CLEANING_AIR_CYC) && (cleaning_air_cyc < 999)) ++cleaning_air_cyc;
            else if ((menu_item == CLEANING_AIR_TMR) && (cleaning_air_time < 999)) ++cleaning_air_time;
            else if ((menu_item == TERM_COMM_ADDRESS) && (term_comm_address < 250)) ++term_comm_address;
        }// End of else if...
    } else if (IsTasterProgramDownPressed() && !IsBothPgmUpAndDownTasterPressed()) {//- check program down taster
        if ((menu_item == PROG_DSP) && (program_select > 1)) {
            SetProgramSelectTimer();
            --program_select;
        } else if (!IsMenuVariableActiv()) {
            //
            //----------------------------------- if not in variable setup
            //----------------------------------- change menu items
            //
            if (menu_item == TERM_COMM_ADDRESS) menu_item = CLEANING_AIR_TMR;
            else if (menu_item == CLEANING_AIR_TMR) menu_item = CLEANING_AIR_CYC;
            else if (menu_item == CLEANING_AIR_CYC) menu_item = MILL_STOP_DELAY_TMR;
            else if (menu_item == MILL_STOP_DELAY_TMR) menu_item = CYCLON_LEVEL_MAX_ALARM_DELAY_TMR;
            else if (menu_item == CYCLON_LEVEL_MAX_ALARM_DELAY_TMR) menu_item = FEED4_CYC_TIME;
            else if (menu_item == FEED4_CYC_TIME) menu_item = FEED3_CYC_TIME;
            else if (menu_item == FEED3_CYC_TIME) menu_item = FEED2_CYC_TIME;
            else if (menu_item == FEED2_CYC_TIME) menu_item = FEED1_CYC_TIME;
            else if (menu_item == FEED1_CYC_TIME) menu_item = VIBRATOR_CYC;
            else if (menu_item == VIBRATOR_CYC) menu_item = VIBRATOR_TMR;
            else if (menu_item == VIBRATOR_TMR) menu_item = MIXER_RAW_MAX_LOAD_DELAY_TMR;
            else if (menu_item == MIXER_RAW_MAX_LOAD_DELAY_TMR) menu_item = MIXER_RAW_MAX_ALARM_DELAY_TMR;
            else if (menu_item == MIXER_RAW_MAX_ALARM_DELAY_TMR) menu_item = MIXER_STOP_DELAY_TMR;
            else if (menu_item == MIXER_STOP_DELAY_TMR) menu_item = VP1_DELAY_TMR;
            else if (menu_item == VP1_DELAY_TMR) menu_item = TERM_COMM_ADDRESS;
        } else if (IsMenuVariableActiv()) {
            //
            //----------------------------------- if variable setup preselected
            //----------------------------------- change values
            //          
            if ((menu_item == VP1_DELAY_TMR) && (vp1_delay_time > 0)) --vp1_delay_time;
            else if ((menu_item == MIXER_STOP_DELAY_TMR) && (mixer_stop_delay_time > 0)) --mixer_stop_delay_time;
            else if ((menu_item == MIXER_RAW_MAX_ALARM_DELAY_TMR) && (mix_raw_max_alarm_delay_time > 0)) --mix_raw_max_alarm_delay_time;
            else if ((menu_item == MIXER_RAW_MAX_LOAD_DELAY_TMR) && (mixer_load_stop_delay_time > 0)) --mixer_load_stop_delay_time;
            else if ((menu_item == VIBRATOR_TMR) && (vibrator_time > 0)) --vibrator_time;
            else if ((menu_item == VIBRATOR_CYC) && (vibrator_cyc > 0)) --vibrator_cyc;
            else if ((menu_item == FEED1_CYC_TIME) && (feed1_cyc_time > 0)) --feed1_cyc_time;
            else if ((menu_item == FEED2_CYC_TIME) && (feed2_cyc_time > 0)) --feed2_cyc_time;
            else if ((menu_item == FEED3_CYC_TIME) && (feed3_cyc_time > 0)) --feed3_cyc_time;
            else if ((menu_item == FEED4_CYC_TIME) && (feed4_cyc_time > 0)) --feed4_cyc_time;
            else if ((menu_item == CYCLON_LEVEL_MAX_ALARM_DELAY_TMR) && (cyclon_alarm_delay_time > 0)) --cyclon_alarm_delay_time;
            else if ((menu_item == MILL_STOP_DELAY_TMR) && (mill_stop_delay_time > 0)) --mill_stop_delay_time;
            else if ((menu_item == CLEANING_AIR_CYC) && (cleaning_air_cyc > 0)) --cleaning_air_cyc;
            else if ((menu_item == CLEANING_AIR_TMR) && (cleaning_air_time > 0)) --cleaning_air_time;
            else if ((menu_item == TERM_COMM_ADDRESS) && (term_comm_address > 1)) --term_comm_address;
        }// End of else if...
    }// End of else if...
    //
    //------------------------------------------- doser 1 manual/auto selector
    //
    if (IsDoser1ManualSelected() && !IsDoser1RunRequestActiv()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in manual position, request
        //--------------------------------------- doser 1 for manual mode
        //
        SetDoser1RunRequest();
        SetDoser1RequestManual();
    } else if (IsDoser1AutoSelected() && !IsDoser1RunRequestActiv()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in automatic position, request
        //--------------------------------------- doser 1 for automatic mode
        //
        SetDoser1RunRequest();
        SetDoser1RequestAuto();
    } else if (!IsDoser1ManualSelected() && !IsDoser1AutoSelected()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in stop position,
        //--------------------------------------- stop doser 1
        //
        ResetDoser1RunRequest();
        ResetDoser1RequestAuto();
        ResetDoser1RequestManual();
    }// End of else...
    //
    //------------------------------------------- set doser 1 dosing reference
    //
    if (IsTasterDoser1SpeedUpPressed() && (dos1_ref < 999)) ++dos1_ref;
    if (IsTasterDoser1SpeedDownPressed() && (dos1_ref > 0)) --dos1_ref;
    //
    //------------------------------------------- doser 2 manual/auto selector
    //
    if (IsDoser2ManualSelected() && !IsDoser2RunRequestActiv()) {
        //
        //--------------------------------------- if selector is switched
        //--------------------------------------- in manual position, request
        //--------------------------------------- doser 2 for manual mode
        //
        SetDoser2RunRequest();
        SetDoser2RequestManual();
    } else if (IsDoser2AutoSelected() && !IsDoser2RunRequestActiv()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in automatic position, request
        //--------------------------------------- doser 2 for automatic mode
        //
        SetDoser2RunRequest();
        SetDoser2RequestAuto();
    } else if (!IsDoser2ManualSelected() && !IsDoser2AutoSelected()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in stop position,
        //--------------------------------------- stop doser 2
        //
        ResetDoser2RunRequest();
        ResetDoser2RequestAuto();
        ResetDoser2RequestManual();
    }// End of else...
    //
    //------------------------------------------- set doser 2 dosing reference
    //
    if (IsTasterDoser2SpeedUpPressed() && (dos2_ref < 999)) ++dos2_ref;
    if (IsTasterDoser2SpeedDownPressed() && (dos2_ref > 0)) --dos2_ref;
    //
    //------------------------------------------- doser 3 manual/auto selector
    //
    if (IsDoser3ManualSelected() && !IsDoser3RunRequestActiv()) {
        //
        //--------------------------------------- if selector is switched
        //--------------------------------------- in manual position, request
        //--------------------------------------- doser 3 for manual mode
        //
        SetDoser3RunRequest();
        SetDoser3RequestManual();
    } else if (IsDoser3AutoSelected() && !IsDoser3RunRequestActiv()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in automatic position, request
        //--------------------------------------- doser 3 for automatic mode
        //
        SetDoser3RunRequest();
        SetDoser3RequestAuto();
    } else if (!IsDoser3ManualSelected() && !IsDoser3AutoSelected()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in stop position,
        //--------------------------------------- stop doser 3
        //
        ResetDoser3RunRequest();
        ResetDoser3RequestAuto();
        ResetDoser3RequestManual();
    }// End of else...
    //
    //------------------------------------------- set doser 3 dosing reference
    //
    if (IsTasterDoser3SpeedUpPressed() && (dos3_ref < 999)) ++dos3_ref;
    if (IsTasterDoser3SpeedDownPressed() && (dos3_ref > 0)) --dos3_ref;
    //
    //------------------------------------------- doser 3 manual/auto selector
    //
    if (IsDoser4ManualSelected() && !IsDoser4RunRequestActiv()) {
        //
        //--------------------------------------- if selector is switched
        //--------------------------------------- in manual position, request
        //--------------------------------------- doser 4 for manual mode
        //
        SetDoser4RunRequest();
        SetDoser4RequestManual();
    } else if (IsDoser4AutoSelected() && !IsDoser4RunRequestActiv()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in automatic position, request
        //--------------------------------------- doser 4 for automatic mode
        //
        SetDoser4RunRequest();
        SetDoser4RequestAuto();
    } else if (!IsDoser4ManualSelected() && !IsDoser4AutoSelected()) {
        //
        //--------------------------------------- if selector switched
        //--------------------------------------- in stop position,
        //--------------------------------------- stop doser 4
        //
        ResetDoser4RunRequest();
        ResetDoser4RequestAuto();
        ResetDoser4RequestManual();
    }// End of else...
    //
    //------------------------------------------- set doser 4 dosing reference
    //
    if (IsTasterDoser4SpeedUpPressed() && (dos4_ref < 999)) ++dos4_ref;
    if (IsTasterDoser4SpeedDownPressed() && (dos4_ref > 0)) --dos4_ref;
    
}// End of SysService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="process service">

void ProcessService(void) {
    CheckControlProcessTimer();
    ControlProcess();
    ProcessFeeder1();
    ProcessFeeder2();
    ProcessFeeder3();
    ProcessFeeder4();
    ProcessDoser1();
    ProcessDoser2();
    ProcessDoser3();
    ProcessDoser4();
    ProcessMixer();
    ProcessVacumPump1();
    //ProcessVacumPump2();
    //ProcessVacumPump3();
}// End of ProcessService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="signal service">

void SignalService(void) {

    switch (sys_state) {

        case SYS_STOP:
            SKC_SIG_STOP = TRUE;
            if(IsControlStartInProgress()) {
               if(CLOCK_1_S) SKC_SIG_START = TRUE;
               else SKC_SIG_START = FALSE;
            } else SKC_SIG_START = FALSE;
            if (CLOCK_2_S && CLOCK_512_MS) STATUS_LED = HIGH;
            else STATUS_LED = LOW;
            break;

        case SYS_RUN:
            SKC_SIG_STOP = FALSE;
            SKC_SIG_START = TRUE;
            if (CLOCK_2_S) STATUS_LED = HIGH;
            else STATUS_LED = LOW;
            break;

        case SYS_ERROR:
            SKC_SIG_STOP = FALSE;
            SKC_SIG_START = FALSE;
            if (CLOCK_64_MS) STATUS_LED = HIGH;
            else STATUS_LED = LOW;
            break;
    }// End of switch...
    //
    //------------------------------------------- extruder run signal
    //
    if (IsExtruderRunInputActiv()) SKC_SIG_EKSTRUDER_RUN = TRUE;
    else SKC_SIG_EKSTRUDER_RUN = FALSE;
    //
    //------------------------------------------- set feeder 1 led sigals
    //
    if (feeder1_state == FEED1_OFF) {
        SKC_SIG_FEEDER1_ON = FALSE;
        SKC_SIG_FEEDER1_FULL = FALSE;
        SKC_SIG_FEEDER1_MIN = FALSE;
        SKC_SIG_FEEDER1_VACUM = FALSE;
        SKC_SIG_FEEDER1_REQUEST = FALSE;
    } else if ((feeder1_state == FEED1_CHARGING) || (feeder1_state == FEED1_DISCHARGING)) {
        if (IsFeeder1VacumValveOn()) SKC_SIG_FEEDER1_VACUM = TRUE;
        else SKC_SIG_FEEDER1_VACUM = FALSE;
        if (IsFeeder1LevelMinInputActiv()) SKC_SIG_FEEDER1_MIN = TRUE;
        else SKC_SIG_FEEDER1_MIN = FALSE;
        if (IsFeeder1RequestInputActiv()) SKC_SIG_FEEDER1_REQUEST = TRUE;
        else SKC_SIG_FEEDER1_REQUEST = FALSE;
        if (IsFeeder1LevelMaxInputActiv()) SKC_SIG_FEEDER1_FULL = TRUE;
        else SKC_SIG_FEEDER1_FULL = FALSE;
        if (feeder1_state == FEED1_CHARGING) {
            if((activ_unit != FEEDER1) || !IsVacumPump1Ready()) {
                if(CLOCK_1_S) SKC_SIG_FEEDER1_ON = TRUE;
                else SKC_SIG_FEEDER1_ON = FALSE;
            } else {
                if(CLOCK_1_S && CLOCK_512_MS) SKC_SIG_FEEDER1_ON = TRUE;
                else SKC_SIG_FEEDER1_ON = FALSE;
            }// End of else...
        } else if (feeder1_state == FEED1_DISCHARGING) SKC_SIG_FEEDER1_ON = TRUE;
    } else if (feeder1_state == FEED1_ERROR) {
        if (CLOCK_128_MS) {//-------------------- error toggle signal period
            SKC_SIG_FEEDER1_ON = TRUE;
            if (IsFeeder1FillingCycleFyilAlarmActiv()) SKC_SIG_FEEDER1_FULL = TRUE;
            if (IsFeeder1RawLevelMinAlarmActiv()) SKC_SIG_FEEDER1_MIN = TRUE;
            if (IsFeeder1SensorAlarmActiv()) {
                SKC_SIG_FEEDER1_FULL = TRUE;
                SKC_SIG_FEEDER1_MIN = TRUE;
                SKC_SIG_FEEDER1_REQUEST = TRUE;
            }// End of if...
        } else {
            SKC_SIG_FEEDER1_ON = FALSE;
            SKC_SIG_FEEDER1_FULL = FALSE;
            SKC_SIG_FEEDER1_MIN = FALSE;
            SKC_SIG_FEEDER1_REQUEST = FALSE;
        }// End of else...
    }// End of feeder 1 signals
    //
    //------------------------------------------- set feeder 2 led sigals
    //
    if (feeder2_state == FEED2_OFF) {
        SKC_SIG_FEEDER2_ON = FALSE;
        SKC_SIG_FEEDER2_FULL = FALSE;
        SKC_SIG_FEEDER2_MIN = FALSE;
        SKC_SIG_FEEDER2_VACUM = FALSE;
        SKC_SIG_FEEDER2_REQUEST = FALSE;
    } else if ((feeder2_state == FEED2_CHARGING) || (feeder2_state == FEED2_DISCHARGING)) {
        if (IsFeeder2VacumValveOn()) SKC_SIG_FEEDER2_VACUM = TRUE;
        else SKC_SIG_FEEDER2_VACUM = FALSE;
        if (IsFeeder2LevelMinInputActiv()) SKC_SIG_FEEDER2_MIN = TRUE;
        else SKC_SIG_FEEDER2_MIN = FALSE;
        if (IsFeeder2RequestInputActiv()) SKC_SIG_FEEDER2_REQUEST = TRUE;
        else SKC_SIG_FEEDER2_REQUEST = FALSE;
        if (IsFeeder2LevelMaxInputActiv()) SKC_SIG_FEEDER2_FULL = TRUE;
        else SKC_SIG_FEEDER2_FULL = FALSE;
        if (feeder2_state == FEED2_CHARGING) {
            if((activ_unit != FEEDER2) || !IsVacumPump1Ready()) {
                if(CLOCK_1_S) SKC_SIG_FEEDER2_ON = TRUE;
                else SKC_SIG_FEEDER2_ON = FALSE;
            } else {
                if(CLOCK_1_S && CLOCK_512_MS) SKC_SIG_FEEDER2_ON = TRUE;
                else SKC_SIG_FEEDER2_ON = FALSE;
            }// End of else...
        } else if (feeder2_state == FEED2_DISCHARGING) SKC_SIG_FEEDER2_ON = TRUE;
    } else if (feeder2_state == FEED2_ERROR) {
        if (CLOCK_128_MS) {
            if (IsFeeder2FillingCycleFyilAlarmActiv()) SKC_SIG_FEEDER2_FULL = TRUE;
            if (IsFeeder2RawLevelMinAlarmActiv()) SKC_SIG_FEEDER2_MIN = TRUE;
            if (IsFeeder2SensorAlarmActiv()) {
                SKC_SIG_FEEDER2_FULL = TRUE;
                SKC_SIG_FEEDER2_MIN = TRUE;
                SKC_SIG_FEEDER2_REQUEST = TRUE;
            }// End of if...
        } else {
            SKC_SIG_FEEDER2_FULL = FALSE;
            SKC_SIG_FEEDER2_MIN = FALSE;
            SKC_SIG_FEEDER2_REQUEST = FALSE;
        }// End of else...
    }// End of feeder 2 signals
    //
    //------------------------------------------- set feeder 3 led sigals
    //
    if (feeder3_state == FEED3_OFF) {
        SKC_SIG_FEEDER3_ON = FALSE;
        SKC_SIG_FEEDER3_FULL = FALSE;
        SKC_SIG_FEEDER3_MIN = FALSE;
        SKC_SIG_FEEDER3_VACUM = FALSE;
        SKC_SIG_FEEDER3_REQUEST = FALSE;
    } else if ((feeder3_state == FEED3_CHARGING) || (feeder3_state == FEED3_DISCHARGING)) {
        if (IsFeeder3VacumValveOn()) SKC_SIG_FEEDER3_VACUM = TRUE;
        else SKC_SIG_FEEDER3_VACUM = FALSE;
        if (IsFeeder3LevelMinInputActiv()) SKC_SIG_FEEDER3_MIN = TRUE;
        else SKC_SIG_FEEDER3_MIN = FALSE;
        if (IsFeeder3RequestInputActiv()) SKC_SIG_FEEDER3_REQUEST = TRUE;
        else SKC_SIG_FEEDER3_REQUEST = FALSE;
        if (IsFeeder3LevelMaxInputActiv()) SKC_SIG_FEEDER3_FULL = TRUE;
        else SKC_SIG_FEEDER3_FULL = FALSE;
        if (feeder3_state == FEED3_CHARGING) {
            if((activ_unit != FEEDER3) || !IsVacumPump1Ready()) {
                if(CLOCK_1_S) SKC_SIG_FEEDER3_ON = TRUE;
                else SKC_SIG_FEEDER3_ON = FALSE;
            } else {
                if(CLOCK_1_S && CLOCK_512_MS) SKC_SIG_FEEDER3_ON = TRUE;
                else SKC_SIG_FEEDER3_ON = FALSE;
            }// End of else...
        } else if (feeder3_state == FEED3_DISCHARGING) SKC_SIG_FEEDER3_ON = TRUE;
    } else if (feeder3_state == FEED3_ERROR) {
        if (CLOCK_128_MS) {
            if (IsFeeder3FillingCycleFyilAlarmActiv()) SKC_SIG_FEEDER3_FULL = TRUE;
            if (IsFeeder3RawLevelMinAlarmActiv()) SKC_SIG_FEEDER3_MIN = TRUE;
            if (IsFeeder3SensorAlarmActiv()) {
                SKC_SIG_FEEDER3_FULL = TRUE;
                SKC_SIG_FEEDER3_MIN = TRUE;
                SKC_SIG_FEEDER3_REQUEST = TRUE;
            }// End of if...
        } else {
            SKC_SIG_FEEDER3_FULL = FALSE;
            SKC_SIG_FEEDER3_MIN = FALSE;
            SKC_SIG_FEEDER3_REQUEST = FALSE;
        }// End of else...
    }// End of feeder 3 signals
    //
    //------------------------------------------- set feeder 4 led sigals
    //
    if (feeder4_state == FEED4_OFF) {
        SKC_SIG_FEEDER4_ON = FALSE;
        SKC_SIG_FEEDER4_FULL = FALSE;
        SKC_SIG_FEEDER4_MIN = FALSE;
        SKC_SIG_FEEDER4_VACUM = FALSE;
        SKC_SIG_FEEDER4_REQUEST = FALSE;
    } else if ((feeder4_state == FEED4_CHARGING) || (feeder4_state == FEED4_DISCHARGING)) {
        if (IsFeeder4VacumValveOn()) SKC_SIG_FEEDER4_VACUM = TRUE;
        else SKC_SIG_FEEDER4_VACUM = FALSE;
        if (IsFeeder4LevelMinInputActiv()) SKC_SIG_FEEDER4_MIN = TRUE;
        else SKC_SIG_FEEDER4_MIN = FALSE;
        if (IsFeeder4RequestInputActiv()) SKC_SIG_FEEDER4_REQUEST = TRUE;
        else SKC_SIG_FEEDER4_REQUEST = FALSE;
        if (IsFeeder4LevelMaxInputActiv()) SKC_SIG_FEEDER4_FULL = TRUE;
        else SKC_SIG_FEEDER4_FULL = FALSE;
        if (feeder4_state == FEED4_CHARGING) {
            if((activ_unit != FEEDER4) || !IsVacumPump1Ready()) {
                if(CLOCK_1_S) SKC_SIG_FEEDER4_ON = TRUE;
                else SKC_SIG_FEEDER4_ON = FALSE;
            } else {
                if(CLOCK_1_S && CLOCK_512_MS) SKC_SIG_FEEDER4_ON = TRUE;
                else SKC_SIG_FEEDER4_ON = FALSE;
            }// End of else...
        } else if (feeder4_state == FEED4_DISCHARGING) SKC_SIG_FEEDER4_ON = TRUE;
    } else if (feeder4_state == FEED4_ERROR) {
        if (CLOCK_128_MS) {
            if (IsFeeder4FillingCycleFyilAlarmActiv()) SKC_SIG_FEEDER4_FULL = TRUE;
            if (IsFeeder4RawLevelMinAlarmActiv()) SKC_SIG_FEEDER4_MIN = TRUE;
            if (IsFeeder4SensorAlarmActiv()) {
                SKC_SIG_FEEDER4_FULL = TRUE;
                SKC_SIG_FEEDER4_MIN = TRUE;
                SKC_SIG_FEEDER4_REQUEST = TRUE;
            }// End of if...
        } else {
            SKC_SIG_FEEDER4_FULL = FALSE;
            SKC_SIG_FEEDER4_MIN = FALSE;
            SKC_SIG_FEEDER4_REQUEST = FALSE;
        }// End of else...
    }// End of feeder 4 signals
    //
    //------------------------------------------- doser 1 led signals
    //
    if (doser1_state == DOS1_STOP) {
        SKC_SIG_DOS1_MANUAL_SEL = FALSE;
        SKC_SIG_DOS1_AUTO_SEL = FALSE;
    } else if (doser1_state == DOS1_RUN_MANUAL) {
        SKC_SIG_DOS1_AUTO_SEL = FALSE;
        if (CLOCK_512_MS)SKC_SIG_DOS1_MANUAL_SEL = TRUE;
        else SKC_SIG_DOS1_MANUAL_SEL = FALSE;
    } else if (doser1_state == DOS1_RUN_AUTO) {
        SKC_SIG_DOS1_MANUAL_SEL = FALSE;
        if (IsDoser1InActivState()) SKC_SIG_DOS1_AUTO_SEL = TRUE;
        else if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_DOS1_AUTO_SEL = TRUE;
        else SKC_SIG_DOS1_AUTO_SEL = FALSE;
    } else if (doser1_state == DOS1_ERROR) {
        SKC_SIG_DOS1_MANUAL_SEL = FALSE;
        SKC_SIG_DOS1_AUTO_SEL = FALSE;
    }// End of else if...
    //
    //------------------------------------------- doser 2 led signals
    //
    if (doser2_state == DOS2_STOP) {
        SKC_SIG_DOS2_MANUAL_SEL = FALSE;
        SKC_SIG_DOS2_AUTO_SEL = FALSE;
    } else if (doser2_state == DOS2_RUN_MANUAL) {
        SKC_SIG_DOS2_AUTO_SEL = FALSE;
        if (CLOCK_512_MS)SKC_SIG_DOS2_MANUAL_SEL = TRUE;
        else SKC_SIG_DOS2_MANUAL_SEL = FALSE;
    } else if (doser2_state == DOS2_RUN_AUTO) {
        SKC_SIG_DOS2_MANUAL_SEL = FALSE;
        if (IsDoser2InActivState()) SKC_SIG_DOS2_AUTO_SEL = TRUE;
        else if (CLOCK_1_S && CLOCK_512_MS)SKC_SIG_DOS2_AUTO_SEL = TRUE;
        else SKC_SIG_DOS2_AUTO_SEL = FALSE;
    } else if (doser2_state == DOS2_ERROR) {
        SKC_SIG_DOS2_MANUAL_SEL = FALSE;
        SKC_SIG_DOS2_AUTO_SEL = FALSE;
    }// End of else if...
    //
    //------------------------------------------- doser 3 led signals
    //
    if (doser3_state == DOS3_STOP) {
        SKC_SIG_DOS3_MANUAL_SEL = FALSE;
        SKC_SIG_DOS3_AUTO_SEL = FALSE;
    } else if (doser3_state == DOS3_RUN_MANUAL) {
        SKC_SIG_DOS3_AUTO_SEL = FALSE;
        if (CLOCK_512_MS)SKC_SIG_DOS3_MANUAL_SEL = TRUE;
        else SKC_SIG_DOS3_MANUAL_SEL = FALSE;
    } else if (doser3_state == DOS3_RUN_AUTO) {
        SKC_SIG_DOS3_MANUAL_SEL = FALSE;
        if (IsDoser3InActivState()) SKC_SIG_DOS3_AUTO_SEL = TRUE;
        else if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_DOS3_AUTO_SEL = TRUE;
        else SKC_SIG_DOS3_AUTO_SEL = FALSE;
    } else if (doser3_state == DOS3_ERROR) {
        SKC_SIG_DOS3_MANUAL_SEL = FALSE;
        SKC_SIG_DOS3_AUTO_SEL = FALSE;
    }// End of else if...
    //
    //------------------------------------------- doser 4 led signals
    //
    if (doser4_state == DOS4_STOP) {
        SKC_SIG_DOS4_MANUAL_SEL = FALSE;
        SKC_SIG_DOS4_AUTO_SEL = FALSE;
    } else if (doser4_state == DOS4_RUN_MANUAL) {
        SKC_SIG_DOS4_AUTO_SEL = FALSE;
        if (CLOCK_512_MS)SKC_SIG_DOS4_MANUAL_SEL = TRUE;
        else SKC_SIG_DOS4_MANUAL_SEL = FALSE;
    } else if (doser4_state == DOS4_RUN_AUTO) {
        SKC_SIG_DOS4_MANUAL_SEL = FALSE;
        if (IsDoser4InActivState()) SKC_SIG_DOS4_AUTO_SEL = TRUE;
        else if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_DOS4_AUTO_SEL = TRUE;
        else SKC_SIG_DOS4_AUTO_SEL = FALSE;
    } else if (doser4_state == DOS4_ERROR) {
        SKC_SIG_DOS4_MANUAL_SEL = FALSE;
        SKC_SIG_DOS4_AUTO_SEL = FALSE;
    }// End of else if...
    //
    //------------------------------------------- vacum pump 1 signaling
    //
    if (vacum_pump1_state == VP1_OFF) {
        SKC_SIG_VACUM_PUMP1_RUN = FALSE;
    } else if (vacum_pump1_state == VP1_RUN) {
        SKC_SIG_VACUM_PUMP1_RUN = TRUE;
    } else if (vacum_pump1_state == VP1_IDLE) {
        if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_VACUM_PUMP1_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP1_RUN = FALSE;
    } else if (vacum_pump1_state == VP1_ERROR) {
        if (CLOCK_128_MS) SKC_SIG_VACUM_PUMP1_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP1_RUN = FALSE;
    }// End of else if...
    //
    //------------------------------------------- vacum pump 2 signaling
    //
    if (vacum_pump2_state == VP2_OFF) {
        SKC_SIG_VACUM_PUMP2_RUN = FALSE;
    } else if (vacum_pump2_state == VP2_RUN) {
        SKC_SIG_VACUM_PUMP2_RUN = TRUE;
    } else if (vacum_pump2_state == VP2_IDLE) {
        if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_VACUM_PUMP2_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP2_RUN = FALSE;
    } else if (vacum_pump2_state == VP2_ERROR) {
        if (CLOCK_128_MS) SKC_SIG_VACUM_PUMP2_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP2_RUN = FALSE;
    }// End of else if...
    //
    //------------------------------------------- vacum pump 3 signaling
    //
    if (vacum_pump3_state == VP3_OFF) {
        SKC_SIG_VACUM_PUMP3_RUN = FALSE;
    } else if (vacum_pump3_state == VP3_RUN) {
        SKC_SIG_VACUM_PUMP3_RUN = TRUE;
    } else if (vacum_pump3_state == VP3_IDLE) {
        if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_VACUM_PUMP3_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP3_RUN = FALSE;
    } else if (vacum_pump3_state == VP3_ERROR) {
        if (CLOCK_128_MS) SKC_SIG_VACUM_PUMP3_RUN = TRUE;
        else SKC_SIG_VACUM_PUMP3_RUN = FALSE;
    }// End of else if...
    //
    //------------------------------------------- mixer signaling
    //
    if (mixer_state == MIX_STOP) {
        SKC_SIG_MIKSER_RUN = FALSE;
        SKC_SIG_MIKSER_MIN = FALSE;
        SKC_SIG_MIKSER_FULL = FALSE;
        SKC_SIG_CYCLON_FULL = FALSE;
    } else if (mixer_state == MIX_RUN) {
        if(IsCyclonLevelMaxInputActiv()) SKC_SIG_CYCLON_FULL = TRUE;
        else SKC_SIG_CYCLON_FULL = FALSE;
        if (IsMixerLevelMaxInputActiv())SKC_SIG_MIKSER_FULL = TRUE;
        else SKC_SIG_MIKSER_FULL = FALSE;
        if (IsMixerLevelMinInputActiv()) SKC_SIG_MIKSER_MIN = TRUE;
        else SKC_SIG_MIKSER_MIN = FALSE;
        if (IsMixerStateOn()) SKC_SIG_MIKSER_RUN = TRUE;
        else if (CLOCK_1_S && CLOCK_512_MS) SKC_SIG_MIKSER_RUN = TRUE;
        else SKC_SIG_MIKSER_RUN = FALSE;
    } else if (mixer_state = MIX_ERROR) {
        if(IsCyclonLevelMaxInputActiv()) SKC_SIG_CYCLON_FULL = TRUE;
        else SKC_SIG_CYCLON_FULL = FALSE;
        if (IsMixerLevelMaxInputActiv())SKC_SIG_MIKSER_FULL = TRUE;
        else SKC_SIG_MIKSER_FULL = FALSE;
        if (IsMixerLevelMinInputActiv()) SKC_SIG_MIKSER_MIN = TRUE;
        else SKC_SIG_MIKSER_MIN = FALSE;
        if (CLOCK_128_MS) {
            SKC_SIG_MIKSER_RUN = TRUE;
            if (IsMixerSensorAlarmActiv()) {
                SKC_SIG_MIKSER_FULL = TRUE;
                SKC_SIG_MIKSER_MIN = TRUE;
            }// End of if...
        } else {
            SKC_SIG_MIKSER_RUN = FALSE;
            if (IsMixerSensorAlarmActiv()) {
                SKC_SIG_MIKSER_FULL = FALSE;
                SKC_SIG_MIKSER_MIN = FALSE;
            }// End of if...
        }// End of else if...
    }// End of else if...

    //
    //------------------------------------------- audio visual alarm signal
    //------------------------------------------- priority selection
    //
    if (IsEmergencyAlarmActiv()) {
        if (CLOCK_16_S && CLOCK_8_S && CLOCK_4_S && CLOCK_2_S && CLOCK_1_S) AlarmOn();
        else AlarmOff();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else if (IsVacumPump1ThermalProtectionAlarmActiv() || IsVacumPump2ThermalProtectionAlarmActiv() \
            || IsVacumPump3ThermalProtectionAlarmActiv() || IsVacumPump1OvercurrentProtectionAlarmActiv() \
            || IsVacumPump2OvercurrentProtectionAlarmActiv() || IsVacumPump3OvercurrentProtectionAlarmActiv() \
            || IsMixerOvercurrentProtectionAlarmActiv() || IsDoser1DriveAlarmActiv() || IsDoser2DriveAlarmActiv() \
            || IsDoser3DriveAlarmActiv() || IsDoser4DriveAlarmActiv() || IsControlCommBusErrorAlarmActiv() ) {

        AlarmOn();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else if (IsFeeder1SensorAlarmActiv() || IsFeeder2SensorAlarmActiv() \
      || IsFeeder3SensorAlarmActiv() || IsFeeder4SensorAlarmActiv() \
      || IsMixerSensorAlarmActiv()) {

        if (CLOCK_8_S && CLOCK_4_S && CLOCK_512_MS) AlarmOn();
        else AlarmOff();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else if (IsFeeder1RawLevelMinAlarmActiv() || IsFeeder2RawLevelMinAlarmActiv() \
      || IsFeeder3RawLevelMinAlarmActiv() || IsFeeder4RawLevelMinAlarmActiv() \
      || IsMixerRawLevelMinimumAlarmActiv() || IsMixerRawLevelMaximumAlarmActiv() \
      || IsMillDisabledAlarmActiv() || IsCyclonMaxLevelAlarmActiv()) {

        if (CLOCK_8_S && CLOCK_4_S && CLOCK_2_S && CLOCK_512_MS) AlarmOn();
        else AlarmOff();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else if (IsFeeder1FillingCycleFyilAlarmActiv() || IsFeeder2FillingCycleFyilAlarmActiv() \
      || IsFeeder3FillingCycleFyilAlarmActiv() || IsFeeder4FillingCycleFyilAlarmActiv()) {

        if (CLOCK_8_S && CLOCK_4_S && CLOCK_2_S && CLOCK_1_S && CLOCK_512_MS) AlarmOn();
        else AlarmOff();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else if (IsVacumPump1VacumAlarmActiv() || IsVacumPump2VacumAlarmActiv() \
      || IsVacumPump3VacumAlarmActiv()) {

        if (CLOCK_16_S  && CLOCK_2_S && CLOCK_512_MS) AlarmOn();
        else AlarmOff();
        if(CLOCK_64_MS) SKC_SIG_ALARM = TRUE;
        else SKC_SIG_ALARM = FALSE;
    } else {
        AlarmOff();
        SKC_SIG_ALARM = FALSE;
    }// End of else...
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="display service">

void DisplayService(void) {

    //------------------------------------------- convert doser 1 reference to
    //------------------------------------------- dispay 1 characters
    IntToChars(dos1_ref);
    dsp1_dig100 = char100;
    dsp1_dig10 = char10;
    dsp1_dig1 = char1;
    //------------------------------------------- convert doser 2 reference to
    //------------------------------------------- dispay 2 characters
    IntToChars(dos2_ref);
    dsp2_dig100 = char100;
    dsp2_dig10 = char10;
    dsp2_dig1 = char1;
    //------------------------------------------- convert doser 3 reference to
    //------------------------------------------- dispay 3 characters
    IntToChars(dos3_ref);
    dsp3_dig100 = char100;
    dsp3_dig10 = char10;
    dsp3_dig1 = char1;
    //------------------------------------------- convert doser 4 reference to
    //------------------------------------------- dispay 4 characters
    IntToChars(dos4_ref);
    dsp4_dig100 = char100;
    dsp4_dig10 = char10;
    dsp4_dig1 = char1;
    //
    //------------------------------------------- switch display item
    //
    if (menu_item == PROG_DSP) {
        //
        //--------------------------------------- program display
        //
        if (IsSystemInitDisplaySet()) {

            if (dsp_svc_pcnt == 0) {
                dsp5_dig100 = 0;
                dsp5_dig10 = 0;
                dsp5_dig1 = 0;
                dsp_char_cnt = 0;
                SetDisplayTimer(20);
                ++dsp_svc_pcnt;
            } else if ((dsp_svc_pcnt == 1) && IsDisplayTimerExpired()) {
                dsp5_dig100 = dsp5_dig10;
                dsp5_dig10 = dsp5_dig1;
                if (dsp_char_cnt == 0) dsp5_dig1 = 'S';
                else if (dsp_char_cnt == 1) dsp5_dig1 = 'Y';
                else if (dsp_char_cnt == 2) dsp5_dig1 = 'S';
                else if (dsp_char_cnt == 3) dsp5_dig1 = 0;
                else if (dsp_char_cnt == 4) dsp5_dig1 = 'i';
                else if (dsp_char_cnt == 5) dsp5_dig1 = 'n';
                else if (dsp_char_cnt == 6) dsp5_dig1 = 'i';
                else if (dsp_char_cnt == 7) dsp5_dig1 = 't';
                else dsp5_dig1 = 0;
                ++dsp_char_cnt;
                if (dsp_char_cnt > 10) {
                    ResetSystemInitDisplay();
                    SetDisplayTimer(200);
                    dsp_svc_pcnt = 0;
                } else SetDisplayTimer(30);
            }// End of else...
        } else if (IsProgramLoadedDisplaySet()) {

            if (dsp_svc_pcnt == 0) {
                dsp5_dig100 = 0;
                dsp5_dig10 = 0;
                dsp5_dig1 = 0;
                dsp_char_cnt = 0;
                if(IsDisplayTimerExpired())SetDisplayTimer(20);
                ++dsp_svc_pcnt;
            } else if ((dsp_svc_pcnt == 1) && IsDisplayTimerExpired()) {
                dsp5_dig100 = dsp5_dig10;
                dsp5_dig10 = dsp5_dig1;
                if (dsp_char_cnt == 0) dsp5_dig1 = 'u';
                else if (dsp_char_cnt == 1) dsp5_dig1 = 'c';
                else if (dsp_char_cnt == 2) dsp5_dig1 = 'i';
                else if (dsp_char_cnt == 3) dsp5_dig1 = 't';
                else if (dsp_char_cnt == 4) dsp5_dig1 = 'A';
                else if (dsp_char_cnt == 5) dsp5_dig1 = 'n';
                else dsp5_dig1 = 0;
                ++dsp_char_cnt;
                if (dsp_char_cnt > 8) {
                    ResetProgramLoadedDisplay();
                    dsp_svc_pcnt = 0;
                }// End of if...
                SetDisplayTimer(30);
            }// End of else...
        } else if (IsProgramSavedDisplaySet()) {

            if (dsp_svc_pcnt == 0) {
                dsp5_dig100 = 0;
                dsp5_dig10 = 0;
                dsp5_dig1 = 0;
                dsp_char_cnt = 0;
                SetDisplayTimer(20);
                ++dsp_svc_pcnt;
            } else if ((dsp_svc_pcnt == 1) && IsDisplayTimerExpired()) {
                dsp5_dig100 = dsp5_dig10;
                dsp5_dig10 = dsp5_dig1;
                if (dsp_char_cnt == 0) dsp5_dig1 = 'S';
                else if (dsp_char_cnt == 1) dsp5_dig1 = 'A';
                else if (dsp_char_cnt == 2) dsp5_dig1 = 'C';
                else if (dsp_char_cnt == 3) dsp5_dig1 = 'U';
                else if (dsp_char_cnt == 4) dsp5_dig1 = 'u';
                else if (dsp_char_cnt == 5) dsp5_dig1 = 'A';
                else if (dsp_char_cnt == 6) dsp5_dig1 = 'n';
                else dsp5_dig1 = 0;
                ++dsp_char_cnt;
                if (dsp_char_cnt > 9) {
                    ResetProgramSavedDisplay();
                    dsp_svc_pcnt = 0;
                }// End of if...
                SetDisplayTimer(30);
            }// End of else...
        } else {
            IntToChars(program_select);
            dsp5_dig100 = 'P';
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
            dsp_svc_pcnt = 0;
        }// End of else...
        //--------------------------------------- switch to display items
    } else if (menu_item == SETUP_ENTER) {
        dsp5_dig100 = 'S';
        dsp5_dig10 = 'E';
        dsp5_dig1 = 't';
    } else if (menu_item == VP1_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(vp1_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '1';
        }// End of else...
    } else if (menu_item == MIXER_STOP_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(mixer_stop_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '2';
        }// End of else...
    } else if (menu_item == MIXER_RAW_MAX_ALARM_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(mix_raw_max_alarm_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '3';
        }// End of else...
    } else if (menu_item == MIXER_RAW_MAX_LOAD_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(mixer_load_stop_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '4';
        }// End of else...
    } else if (menu_item == VIBRATOR_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(vibrator_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '5';
        }// End of else...
    } else if (menu_item == VIBRATOR_CYC) {
        if (IsMenuVariableActiv()) {
            IntToChars(vibrator_cyc);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '6';
        }// End of else...
    } else if (menu_item == FEED1_CYC_TIME) {
        if (IsMenuVariableActiv()) {
            IntToChars(feed1_cyc_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '7';
        }// End of else...
    } else if (menu_item == FEED2_CYC_TIME) {
        if (IsMenuVariableActiv()) {
            IntToChars(feed2_cyc_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '8';
        }// End of else...
    } else if (menu_item == FEED3_CYC_TIME) {
        if (IsMenuVariableActiv()) {
            IntToChars(feed3_cyc_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '0';
            dsp5_dig1 = '9';
        }// End of else...
    } else if (menu_item == FEED4_CYC_TIME) {
        if (IsMenuVariableActiv()) {
            IntToChars(feed4_cyc_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '0';
        }// End of else...
    } else if (menu_item == CYCLON_LEVEL_MAX_ALARM_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(cyclon_alarm_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '1';
        }// End of else...
    } else if (menu_item == MILL_STOP_DELAY_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(mill_stop_delay_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '2';
        }// End of else...
    } else if (menu_item == CLEANING_AIR_CYC) {
        if (IsMenuVariableActiv()) {
            IntToChars(cleaning_air_cyc);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '3';
        }// End of else...
    } else if (menu_item == CLEANING_AIR_TMR) {
        if (IsMenuVariableActiv()) {
            IntToChars(cleaning_air_time);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '4';
        }// End of else...
    } else if (menu_item == TERM_COMM_ADDRESS) {
        if (IsMenuVariableActiv()) {
            IntToChars(term_comm_address);
            dsp5_dig100 = char100;
            dsp5_dig10 = char10;
            dsp5_dig1 = char1;
        } else {
            dsp5_dig100 = 't';
            dsp5_dig10 = '1';
            dsp5_dig1 = '5';
        }// End of else...
    }// End of else if...
    //
    //------------------------------------------- error display
    //
    if (!IsErrorDisplayTimerExpired()) {
        return; //-------------------------------- if timer activ return
    } else skc_error_dsp._byte = ERROR_DISPLAY_CLEAR;
    //
    //------------------------------------------- clear error display register
    //------------------------------------------- and check for activ alarm
    //
    if (err_dsp_cnt == 0) {
        if (IsEmergencyAlarmActiv()) skc_error_dsp._byte = ERROR_EMERGENCY;
    } else if (err_dsp_cnt == 1) {
        if (IsControlCommBusErrorAlarmActiv())skc_error_dsp._byte = ERROR_COMM_BUSS; // nema smisla
    } else if (err_dsp_cnt == 2) {
        if (IsEepromErrorAlarmActiv())skc_error_dsp._byte = ERROR_EEPROM;
    } else if (err_dsp_cnt == 3) {
        if (IsVacumPump1ThermalProtectionAlarmActiv())skc_error_dsp._byte = ERROR_VP1_TH_PROT;
    } else if (err_dsp_cnt == 4) {
        if (IsVacumPump2ThermalProtectionAlarmActiv()) skc_error_dsp._byte = ERROR_VP2_TH_PROT;
    } else if (err_dsp_cnt == 5) {
        if (IsVacumPump3ThermalProtectionAlarmActiv()) skc_error_dsp._byte = ERROR_VP3_TH_PROT;
    } else if (err_dsp_cnt == 6) {
        if (IsVacumPump1OvercurrentProtectionAlarmActiv()) skc_error_dsp._byte = ERROR_VP1_OVRCRNT_PROT;
    } else if (err_dsp_cnt == 7) {
        if (IsVacumPump2OvercurrentProtectionAlarmActiv())skc_error_dsp._byte = ERROR_VP2_OVRCRNT_PROT;
    } else if (err_dsp_cnt == 8) {
        if (IsVacumPump3OvercurrentProtectionAlarmActiv()) skc_error_dsp._byte = ERROR_VP3_OVRCRNT_PROT;
    } else if (err_dsp_cnt == 9) {
        if (IsMixerOvercurrentProtectionAlarmActiv()) skc_error_dsp._byte = ERROR_MIX_OVRCRNT_PROT;
    } else if (err_dsp_cnt == 10) {
        if (IsVacumPump1VacumAlarmActiv()) skc_error_dsp._byte = ERROR_VP1_VACUM_CTRL;
    } else if (err_dsp_cnt == 11) {
        if (IsVacumPump2VacumAlarmActiv()) skc_error_dsp._byte = ERROR_VP2_VACUM_CTRL;
    } else if (err_dsp_cnt == 12) {
        if (IsVacumPump3VacumAlarmActiv())skc_error_dsp._byte = ERROR_VP3_VACUM_CTRL;
    } else if (err_dsp_cnt == 13) {
        if (IsDoser1DriveAlarmActiv()) skc_error_dsp._byte = ERROR_DRIVE1;
    } else if (err_dsp_cnt == 14) {
        if (IsDoser2DriveAlarmActiv()) skc_error_dsp._byte = ERROR_DRIVE2;
    } else if (err_dsp_cnt == 15) {
        if (IsDoser3DriveAlarmActiv()) skc_error_dsp._byte = ERROR_DRIVE3;
    } else if (err_dsp_cnt == 16) {
        if (IsDoser4DriveAlarmActiv()) skc_error_dsp._byte = ERROR_DRIVE4;
    } else if (err_dsp_cnt == 17) {
        if (IsFeeder1SensorAlarmActiv()) skc_error_dsp._byte = ERROR_FEED1_SENSORS;
    } else if (err_dsp_cnt == 18) {
        if (IsFeeder2SensorAlarmActiv()) skc_error_dsp._byte = ERROR_FEED2_SENSORS;
    } else if (err_dsp_cnt == 19) {
        if (IsFeeder3SensorAlarmActiv()) skc_error_dsp._byte = ERROR_FEED3_SENSORS;
    } else if (err_dsp_cnt == 20) {
        if (IsFeeder4SensorAlarmActiv()) skc_error_dsp._byte = ERROR_FEED4_SENSORS;
    } else if (err_dsp_cnt == 21) {
        if (IsMixerSensorAlarmActiv()) skc_error_dsp._byte = ERROR_MIXER_SENSORS;
    } else if (err_dsp_cnt == 22) {
        if (IsFeeder1RawLevelMinAlarmActiv()) skc_error_dsp._byte = ERROR_FEED1_MIN_LEVEL;
    } else if (err_dsp_cnt == 23) {
        if (IsFeeder2RawLevelMinAlarmActiv()) skc_error_dsp._byte = ERROR_FEED2_MIN_LEVEL;
    } else if (err_dsp_cnt == 24) {
        if (IsFeeder3RawLevelMinAlarmActiv()) skc_error_dsp._byte = ERROR_FEED3_MIN_LEVEL;
    } else if (err_dsp_cnt == 25) {
        if (IsFeeder4RawLevelMinAlarmActiv()) skc_error_dsp._byte = ERROR_FEED4_MIN_LEVEL;
    } else if (err_dsp_cnt == 26) {
        if (IsCyclonMaxLevelAlarmActiv()) skc_error_dsp._byte = ERROR_CYCLON_MAX_LEVEL;
    } else if (err_dsp_cnt == 27) {
        if (IsMillDisabledAlarmActiv()) skc_error_dsp._byte = ERROR_CYCLON_MILL_DISABLED;
    } else if (err_dsp_cnt == 28) {
        if (IsMixerRawLevelMinimumAlarmActiv()) skc_error_dsp._byte = ERROR_MIXER_RAW_LEVEL_MIN;
    } else if (err_dsp_cnt == 29) {
        if (IsMixerRawLevelMaximumAlarmActiv()) skc_error_dsp._byte = ERROR_MIXER_RAW_LEVEL_MAX;
    } else if (err_dsp_cnt == 30) {
        if (IsFeeder1FillingCycleFyilAlarmActiv()) skc_error_dsp._byte = ERROR_FEED1_CYC_CNT;
    } else if (err_dsp_cnt == 31) {
        if (IsFeeder2FillingCycleFyilAlarmActiv()) skc_error_dsp._byte = ERROR_FEED2_CYC_CNT;
    } else if (err_dsp_cnt == 32) {
        if (IsFeeder3FillingCycleFyilAlarmActiv()) skc_error_dsp._byte = ERROR_FEED3_CYC_CNT;
    } else if (err_dsp_cnt == 33) {
        if (IsFeeder4FillingCycleFyilAlarmActiv()) skc_error_dsp._byte = ERROR_FEED4_CYC_CNT;
    } // End of else if...
    //
    //------------------------------------------- check if error display register
    //------------------------------------------- is loaded with error code, and
    //------------------------------------------- if so, set timer for error
    //------------------------------------------- code display timme
    //
    if (skc_error_dsp._byte != ERROR_DISPLAY_CLEAR) SetErrorDisplayTimer(300);
    //
    //------------------------------------------- set list counter
    //
    if(++err_dsp_cnt == 34) err_dsp_cnt = 0;
    //
    //------------------------------------------- for every new error display 
    //------------------------------------------- item, counter comparison value
    //------------------------------------------- should be increased (28..29...)
    //
}// End of DisplayService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="load program">

void LoadProgram(void) {
    //
    //------------------------------------------- private variables
    //
    volatile unsigned char ee_page;
    volatile unsigned char ee_program;
    //
    //------------------------------------------- select eeprom page
    //
    if (program_select < 16) {
        ee_page = EE_FIRST_PAGE;
        ee_program = (program_select << 4);
    } else if (program_select < 32) {
        ee_page = EE_SECOND_PAGE;
        ee_program = ((program_select - 16) << 4);
    } else if (program_select < 48) {
        ee_page = EE_THIRD_PAGE;
        ee_program = ((program_select - 32) << 4);
    }else if (program_select < 51) {
        ee_page = EE_FOURTH_PAGE;
        ee_program = ((program_select - 48) << 4);
    }// End of else if...
    //
    //------------------------------------------- load dose speed reference
    //
    dos1_ref = I2C_EE_ReadInt(ee_program, ee_page);
    dos2_ref = I2C_EE_ReadInt((ee_program + 2), ee_page);
    dos3_ref = I2C_EE_ReadInt((ee_program + 4), ee_page);
    dos4_ref = I2C_EE_ReadInt((ee_program + 6), ee_page);
    control1._byte = I2C_EE_ReadInt((ee_program +8), ee_page);
    I2C_EE_WriteByte(EE_LAST_PGM_IN_USE, program_select, EE_FIFTH_PAGE);
    program_in_use = program_select;
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="save program">

void SaveProgram(void) {
    //
    //------------------------------------------- private variables
    //
    volatile unsigned char ee_page;
    volatile unsigned char ee_program;
    //
    //------------------------------------------- select eeprom page
    //
    if (program_select < 16) {
        ee_page = EE_FIRST_PAGE;
        ee_program = (program_select << 4);
    } else if (program_select < 32) {
        ee_page = EE_SECOND_PAGE;
        ee_program = ((program_select - 16) << 4);
    } else if (program_select < 48) {
        ee_page = EE_THIRD_PAGE;
        ee_program = ((program_select - 32) << 4);
    }else if (program_select < 51) {
        ee_page = EE_FOURTH_PAGE;
        ee_program = ((program_select - 48) << 4);
    }// End of else if...
    //
    //------------------------------------------- write doser speed reference
    //
    I2C_EE_WriteInt(ee_program, dos1_ref, ee_page);
    I2C_EE_WriteInt((ee_program + 2), dos2_ref, ee_page);
    I2C_EE_WriteInt((ee_program + 4), dos3_ref, ee_page);
    I2C_EE_WriteInt((ee_program + 6), dos4_ref, ee_page);
    I2C_EE_WriteInt((ee_program + 8), control1._byte, ee_page);
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="load system variable">

void LoadSystemVariable(void) {
    //
    //------------------------------------------- load system variables
    //
    vp1_delay_time = I2C_EE_ReadInt(EE_VP1_DELAY_TMR, EE_FIFTH_PAGE);
    mixer_stop_delay_time = I2C_EE_ReadInt(EE_MIXER_STOP_DELAY_TMR, EE_FIFTH_PAGE);
    mix_raw_max_alarm_delay_time = I2C_EE_ReadInt(EE_MIXER_RAW_MAX_ALARM_DELAY_TMR, EE_FIFTH_PAGE);
    mixer_load_stop_delay_time = I2C_EE_ReadInt(EE_MIXER_RAW_MAX_LOAD_DELAY_TMR, EE_FIFTH_PAGE);
    vibrator_time = I2C_EE_ReadInt(EE_VIBRATOR_TMR, EE_FIFTH_PAGE);
    vibrator_cyc = I2C_EE_ReadInt(EE_VIBRATOR_CYC, EE_FIFTH_PAGE);
    feed1_cyc_time = I2C_EE_ReadInt(EE_FEED1_CYC_TIME, EE_FIFTH_PAGE);
    feed2_cyc_time = I2C_EE_ReadInt(EE_FEED2_CYC_TIME, EE_FIFTH_PAGE);
    feed3_cyc_time = I2C_EE_ReadInt(EE_FEED3_CYC_TIME, EE_FIFTH_PAGE);
    feed4_cyc_time = I2C_EE_ReadInt(EE_FEED4_CYC_TIME, EE_FIFTH_PAGE);
    cyclon_alarm_delay_time = I2C_EE_ReadInt(EE_CYCLON_LEVEL_MAX_ALARM_DELAY_TMR, EE_FIFTH_PAGE);
    mill_stop_delay_time = I2C_EE_ReadInt(EE_MILL_STOP_DELAY_TMR, EE_FIFTH_PAGE);
    cleaning_air_cyc = I2C_EE_ReadInt(EE_CLEANING_AIR_CYC, EE_FIFTH_PAGE);
    cleaning_air_time = I2C_EE_ReadInt(EE_CLEANING_AIR_TMR, EE_FIFTH_PAGE);
    term_comm_address = I2C_EE_ReadByte(EE_TERM_COMM_ADDRESS, EE_FIFTH_PAGE);
    fw_version = I2C_EE_ReadInt(EE_FW_VERSION, EE_FIFTH_PAGE);
    
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="save system variable">

void SaveSystemVariable(void) {
    //
    //------------------------------------------- write system variables
    //
    I2C_EE_WriteInt(EE_VP1_DELAY_TMR, vp1_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_MIXER_STOP_DELAY_TMR, mixer_stop_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_MIXER_RAW_MAX_ALARM_DELAY_TMR, mix_raw_max_alarm_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_MIXER_RAW_MAX_LOAD_DELAY_TMR, mixer_load_stop_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_VIBRATOR_TMR, vibrator_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_VIBRATOR_CYC, vibrator_cyc, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_FEED1_CYC_TIME, feed1_cyc_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_FEED2_CYC_TIME, feed2_cyc_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_FEED3_CYC_TIME, feed3_cyc_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_FEED4_CYC_TIME, feed4_cyc_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_CYCLON_LEVEL_MAX_ALARM_DELAY_TMR, cyclon_alarm_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_MILL_STOP_DELAY_TMR, mill_stop_delay_time, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_CLEANING_AIR_CYC, cleaning_air_cyc, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_CLEANING_AIR_TMR, cleaning_air_time, EE_FIFTH_PAGE);
    I2C_EE_WriteByte(EE_TERM_COMM_ADDRESS, term_comm_address, EE_FIFTH_PAGE);
    I2C_EE_WriteInt(EE_FW_VERSION, fw_version, EE_FIFTH_PAGE);

}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="int to chars">

void IntToChars(unsigned int data) {
    /* reset variable */
    char100 = 0;
    char10 = 0;
    char1 = 0;
    /* size data */
    if (data > 999) data = 999;
    /* get hundreads */
    while (data > 99) {
        data -= 100;
        ++char100;
    }// End of while...
    /* get tens */
    while (data > 9) {
        data -= 10;
        ++char10;
    }// End of while...
    /* get units */
    char1 = data;
    /* convert to ascii */
    char1 += 48;
    char10 += 48;
    char100 += 48;
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="i2c eeprom read byte">

unsigned char I2C_EE_ReadByte(unsigned char address, unsigned char page) {

    volatile unsigned char tmp, ctrl;

    if (page == EE_FIRST_PAGE) ctrl = 0xa0;
    else if (page == EE_SECOND_PAGE) ctrl = 0xa2;
    else if (page == EE_THIRD_PAGE)  ctrl = 0xa4;
    else if (page == EE_FOURTH_PAGE) ctrl = 0xa6;
    else if (page == EE_FIFTH_PAGE)ctrl = 0xa8;
    else if (page == EE_SIXTH_PAGE) ctrl = 0xaa;
    else if (page == EE_SEVENTH_PAGE) ctrl = 0xac;
    else if (page == EE_EIGHT_PAGE)ctrl = 0xae;

    ResetEepromErrorAlarm();

    I2C_Start();
    I2C_WriteByte(ctrl); // control byte
    tmp = I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    I2C_WriteByte(address); // word address
    tmp = I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    I2C_Start();
    I2C_WriteByte(++ctrl); // control byte
    tmp = I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    tmp = I2C_ReadByte();
    I2C_SendNotAck();
    I2C_Stop();
    return (tmp);
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="i2c eeprom read int">

unsigned int I2C_EE_ReadInt(unsigned char address, unsigned char page) {

    volatile unsigned int data;

    data = I2C_EE_ReadByte(address, page);
    data = (data << 8);
    ++address;
    data += I2C_EE_ReadByte(address, page);
    return (data);
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="i2c eeprom write byte">

void I2C_EE_WriteByte(unsigned char address, unsigned char data, unsigned char page) {

    volatile unsigned char tmp, cnt, ctrl_wr;
    
    if (page == EE_FIRST_PAGE) ctrl_wr = 0xa0;
    else if (page == EE_SECOND_PAGE) ctrl_wr = 0xa2;
    else if (page == EE_THIRD_PAGE) ctrl_wr = 0xa4;
    else if (page == EE_FOURTH_PAGE) ctrl_wr = 0xa6;
    else if (page == EE_FIFTH_PAGE) ctrl_wr = 0xa8;
    else if (page == EE_SIXTH_PAGE) ctrl_wr = 0xaa;
    else if (page == EE_SEVENTH_PAGE) ctrl_wr = 0xac;
    else if (page == EE_EIGHT_PAGE) ctrl_wr = 0xae;
    
    ResetEepromErrorAlarm();

    I2C_Start();
    I2C_WriteByte(ctrl_wr); // control byte
    tmp = I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    I2C_WriteByte(address); // word address
    I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    I2C_WriteByte(data); // data
    tmp = I2C_GetAck();
    if(tmp == 0) SetEepromErrorAlarm();
    I2C_Stop();
    //Delay10KTCYx(4);
    cnt = 0;
    tmp = 0;
    while (tmp == 0) {
        I2C_Start();
        I2C_WriteByte(ctrl_wr); // control byte
        tmp = I2C_GetAck();
        if(++cnt == 250) break;
    }// End of while...
    if(tmp == 0) SetEepromErrorAlarm();
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="i2c eeprom write int">

void I2C_EE_WriteInt(unsigned char address, unsigned int data, unsigned char page) {

    I2C_EE_WriteByte(address, (data >> 8), page);
    ++address;
    I2C_EE_WriteByte(address, data, page);
}// </editor-fold>



