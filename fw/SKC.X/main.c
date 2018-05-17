/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Signal and Keys Controller
 *
 *                          Glavni program
 *
 *******************************************************************************
 * Ime fajla:       main.c
 *
 * Procesor:        PIC18F23K20
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
#include <spi.h>
#include "typedefs.h"
#include "io_cfg.h"
#include "main.h"
//
#pragma code high_vector = 0x08
//
// <editor-fold defaultstate="collapsed" desc="asm high vector">

void interrupt_at_high_vector(void) {
    _asm goto high_isr _endasm
}// End of interrupt_at_high_vector
// </editor-fold>
//
#pragma code low_vector = 0x18
//
// <editor-fold defaultstate="collapsed" desc="asm low vector">

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
    //------------------------------------------- check interrupt source
    //
    if (PIE1bits.RCIE && PIR1bits.RCIF) {
        //PIR1bits.RCIF = FALSE; //clear interrupt flag
        comm_temp = getcUSART();
        if (!RX_PENDING && (net_address == comm_temp)) {
            RX_PENDING = TRUE; //set receiver busy flag
            rx_hold_tmr = 20;
            RX_HOLD_TEX = FALSE;
            comm_buff[0] = comm_temp;
        } else if (!RX_DATA_RDY && RX_PENDING) {
            ++comm_data_cnt;
            rx_hold_tmr = 20;
            RX_HOLD_TEX = FALSE;
            comm_buff[comm_data_cnt] = comm_temp;
            if ((comm_data_cnt == 3) && (comm_buff[2] == GET_CMD)) {
                rx_lenght = comm_data_cnt;
                RX_DATA_RDY = TRUE;
            } else if (comm_data_cnt == 24){
              if(comm_buff[2] == SET_CMD){
                rx_lenght = comm_data_cnt;  
                RX_DATA_RDY = TRUE;                
              }else {
                rx_lenght = 0;
                RX_PENDING = FALSE;
                RX_DATA_RDY = FALSE;
                comm_data_cnt = 0;
              }// End of else...
            }// End of else if...
        }// End of else if...
    }// End of USART RX INTERRUPT

    if (PIE1bits.TXIE && PIR1bits.TXIF) {
        if (tx_cnt <= tx_lenght){
            putcUSART(tx_buff[tx_cnt]);
            tx_cnt++;
        } else if (TXSTAbits.TRMT){
            DataDirRX();
            tx_lenght = 0;
            tx_cnt = 0;
            PIE1bits.TXIE = FALSE;
            TX_DATA_RDY = FALSE;
        }// End of else if...
    }// End of USART TX INTERRUPT

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
    //
    //------------------------------------------- check interrupt source
    //
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) {
        INTCONbits.TMR0IF = FALSE; //clear interrupt flag
        //TMR0H = 0xc1; // timer0 IRQ 1ms
        TMR0L = 0x06; //
        //
        //--------------------------------------- 3 x 7 - segment display update timer
        //
        if (dsp_tmr) {
            --dsp_tmr;
            DSP_UPDATE = FALSE;
        } else DSP_UPDATE = TRUE;
        //
        //--------------------------------------- error display update timer
        //
        if (err_dsp_tmr1) {
            --err_dsp_tmr1;
            ERR_DSP_TEX = FALSE;
        } else ERR_DSP_TEX = TRUE;
        //
        //---------------------------------------  message transfer timeout timer
        //
        if(msg_transfer_tmr){
            --msg_transfer_tmr;
            MSG_TRANSFER_TEX = FALSE;
        } else MSG_TRANSFER_TEX = TRUE;
        //
        //--------------------------------------- receiving message timeout timer
        //
        if (rx_hold_tmr) {
            --rx_hold_tmr;
            RX_HOLD_TEX = FALSE;
        } else RX_HOLD_TEX = TRUE;
        //
        //------------------------------------------- skc status led
        //
        //ClearSignal1();
        if (status_led_cnt) {
            --status_led_cnt;
        } else DeselectStatusLED();
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
    //------------------------------------------- config oscilator source
    //
    OSCCONbits.IRCF2 = 1; // int.osc. freq. select 16 MHz
    OSCCONbits.IRCF1 = 1; // int.osc. freq. select 16 MHz
    OSCCONbits.IRCF0 = 1; // int.osc. freq. select 16 MHz
    OSCCONbits.SCS1 = 0; //
    OSCCONbits.SCS0 = 0; //

    OSCTUNEbits.PLLEN = 1; // PLL enabled
    while (!OSCCONbits.IOFS) continue;
    //
    //------------------------------------------- config oscilator frequency
    //
    ANSEL = 0b00000000;
    ANSELH = 0b00000000;
    TRISA = 0x00;
    TRISB = 0xff;
    TRISC = 0b11000000;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    //
    //------------------------------------------- open timer 0
    //------------------------------------------- interrupt time base for
    //------------------------------------------- display driver
    //
    OpenTimer0(TIMER_INT_ON &
            T0_8BIT &
            T0_SOURCE_INT &
            T0_PS_1_64);
    //
    //------------------------------------------- open usart 19200bps
    //
    TXSTAbits.TX9 = FALSE;
    TXSTAbits.SYNC = FALSE;
    TXSTAbits.BRGH = LOW;
    TXSTAbits.TXEN = HIGH;
    RCSTAbits.ADDEN = FALSE;
    RCSTAbits.CREN = TRUE;
    RCSTAbits.RX9 = FALSE;
    RCSTAbits.SPEN = TRUE;
    BAUDCONbits.BRG16 = FALSE;
    SPBRGH = 0;
    SPBRG = 51;
    PIE1bits.RCIE = HIGH;
    //PIE1bits.TXIE = HIGH;
    //
    //------------------------------------------- open spi interface
    //
    OpenSPI(SPI_FOSC_16, MODE_00, SMPEND);
    SH1_OE = HIGH;
    SH1_OE_TRIS = OUTPUT_PIN;    
    //
    //------------------------------------------- interrupt pririty config
    //
    DataDirRX();
    INTCON2bits.TMR0IP = LOW;
    IPR1bits.RCIP = HIGH;
    IPR1bits.TXIP = HIGH;
    RCONbits.IPEN = TRUE;
    INTCONbits.GIEL = TRUE;
    INTCONbits.GIEH = TRUE;
    //
    //------------------------------------------- interrupts enabled
    //
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="init RAM">

void InitRAM(void) {
    IO_Service();
    ResetTasterStartPressed();
    ResetTasterStopPressed();
    ResetTasterResetPressed();
    ResetTasterProgramLoadPressed();
    ResetTasterProgramSavePressed();
    ResetTasterProgramUpPressed();
    ResetTasterProgramDownPressed();
    ResetTasterDoser1SpeedUpPressed();
    ResetTasterDoser1SpeedDownPressed();
    ResetTasterFeeder1OnPressed();
    ResetTasterDoser2SpeedUpPressed();
    ResetTasterDoser2SpeedDownPressed();
    ResetTasterFeeder2OnPressed();
    ResetTasterDoser3SpeedUpPressed();
    ResetTasterDoser3SpeedDownPressed();
    ResetTasterFeeder3OnPressed();
    ResetTasterDoser4SpeedUpPressed();
    ResetTasterDoser4SpeedDownPressed();
    ResetTasterFeeder4OnPressed();
    ResetTasterMikserRunPressed();
    ResetVacumPump1OnTasterPressed();
    ResetVacumPump2OnTasterPressed();
    ResetVacumPump3OnTasterPressed();
    net_address = 0x05;
    dsp_digit_cnt = DIGIT_1;
    err_dsp_tmr1 = 200;
    err_pcnt = 0;
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    Delay10KTCYx(250);
    comm_err_cnt = 0;
    msg_transfer_tmr = 200;
    MSG_TRANSFER_TEX = FALSE;
    MSG_TRANSFER_OK = TRUE;
    RX_PENDING = FALSE;
    RX_DATA_RDY = FALSE;
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="main">

void main(void) {

    InitSYS();
    InitRAM();

    while (1) {
        IO_Service();
        CommService();
        SignalService();
        ErrorService();
        DisplayService();
    }// End of while(1)
}// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="IO service">

void IO_Service(void) {
    //
    //------------------------------------------- private variable
    //
    volatile unsigned char spi_response;
    //
    //------------------------------------------- Read inputs_1_8();
    //
    ResetShift2Latch();
    shift2._byte = 0b11111110;
    spi_response = WriteSPI(shift2._byte);
    SetShift2latch();
    inputs_0_7._byte = DATA_PORT;
    //
    //------------------------------------------- Read inputs_9_16();
    //
    ResetShift2Latch();
    shift2._byte = 0b11111101;
    spi_response = WriteSPI(shift2._byte);
    SetShift2latch();
    inputs_8_15._byte = DATA_PORT;
    //
    //------------------------------------------- Read inputs_17_24();
    //
    ResetShift2Latch();
    shift2._byte = 0b11111011;
    spi_response = WriteSPI(shift2._byte);
    SetShift2latch();
    inputs_16_23._byte = DATA_PORT;
    //
    //------------------------------------------- Read inputs_24_31();
    //
    ResetShift2Latch();
    shift2._byte = 0b11110111;
    spi_response = WriteSPI(shift2._byte);
    SetShift2latch();
    inputs_24_31._byte = DATA_PORT;
    //
    //------------------------------------------- Disable input buffer;
    //
    ResetShift2Latch();
    shift2._byte = 0b11111111;
    spi_response = WriteSPI(shift2._byte);
    SetShift2latch();
    ResetShift2Latch();
    //
    //------------------------------------------- check taster start state
    //

    if (TST_START != OLD_TST_START) {
        OLD_TST_START = TST_START;
        if (!TST_START) SetTasterStartPressed();
        else ResetTasterStartPressed();
    }// End of if...
    //
    //------------------------------------------- check taster stop state
    //
    if (TST_STOP != OLD_TST_STOP) {
        OLD_TST_STOP = TST_STOP;
        if (!TST_STOP) SetTasterStopPressed();
        else ResetTasterStopPressed();
    }// End of if...
    //
    //------------------------------------------- check taster reset state
    //
    if (TST_RESET != OLD_TST_RESET) {
        OLD_TST_RESET = TST_RESET;
        if (!TST_RESET) SetTasterResetPressed();
        else ResetTasterResetPressed();
    }// End of if...
    //
    //------------------------------------------- check taster pg_load state
    //
    if (TST_PG_LOAD != OLD_TST_PG_LOAD) {
        OLD_TST_PG_LOAD = TST_PG_LOAD;
        if (!TST_PG_LOAD) SetTasterProgramLoadPressed();
        else ResetTasterProgramLoadPressed();
    }// End of if...
    //
    //------------------------------------------- check taster pg_save state
    //
    if (TST_PG_SAVE != OLD_TST_PG_SAVE) {
        OLD_TST_PG_SAVE = TST_PG_SAVE;
        if (!TST_PG_SAVE) SetTasterProgramSavePressed();
        else ResetTasterProgramSavePressed();
    }// End of if...
    //
    //------------------------------------------- check taster pg_up state
    //
    if (TST_PG_UP != OLD_TST_PG_UP) {
        OLD_TST_PG_UP = TST_PG_UP;
        if (!TST_PG_UP) SetTasterProgramUpPressed();
        else ResetTasterProgramUpPressed();
    }// End of if...
    //
    //------------------------------------------- check taster pg_dn state
    //
    if (TST_PG_DN != OLD_TST_PG_DN) {
        OLD_TST_PG_DN = TST_PG_DN;
        if (!TST_PG_DN) SetTasterProgramDownPressed();
        else ResetTasterProgramDownPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser1_speed_up state
    //
    if (TST_DOSER1_SPEED_UP != OLD_TST_DOSER1_SPEED_UP) {
        OLD_TST_DOSER1_SPEED_UP = TST_DOSER1_SPEED_UP;
        if (!TST_DOSER1_SPEED_UP)SetTasterDoser1SpeedUpPressed();
        else ResetTasterDoser1SpeedUpPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser1_speed_dn state
    //
    if (TST_DOSER1_SPEED_DN != OLD_TST_DOSER1_SPEED_DN) {
        OLD_TST_DOSER1_SPEED_DN = TST_DOSER1_SPEED_DN;
        if (!TST_DOSER1_SPEED_DN) SetTasterDoser1SpeedDownPressed();
        else ResetTasterDoser1SpeedDownPressed();
    }// End of if...
    //
    //------------------------------------------- check taster feeder1_on state
    //
    if (TST_FEEDER1_ON != OLD_TST_FEEDER1_ON) {
        OLD_TST_FEEDER1_ON = TST_FEEDER1_ON;
        if (!TST_FEEDER1_ON) SetTasterFeeder1OnPressed();
        else ResetTasterFeeder1OnPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser2_speed_up state
    //
    if (TST_DOSER2_SPEED_UP != OLD_TST_DOSER2_SPEED_UP) {
        OLD_TST_DOSER2_SPEED_UP = TST_DOSER2_SPEED_UP;
        if (!TST_DOSER2_SPEED_UP)SetTasterDoser2SpeedUpPressed();
        else ResetTasterDoser2SpeedUpPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser2_speed_dn state
    //
    if (TST_DOSER2_SPEED_DN != OLD_TST_DOSER2_SPEED_DN) {
        OLD_TST_DOSER2_SPEED_DN = TST_DOSER2_SPEED_DN;
        if (!TST_DOSER2_SPEED_DN) SetTasterDoser2SpeedDownPressed();
        else ResetTasterDoser2SpeedDownPressed();
    }// End of if...
    //
    //------------------------------------------- check taster feeder2_on state
    //
    if (TST_FEEDER2_ON != OLD_TST_FEEDER2_ON) {
        OLD_TST_FEEDER2_ON = TST_FEEDER2_ON;
        if (!TST_FEEDER2_ON)SetTasterFeeder2OnPressed();
        else ResetTasterFeeder2OnPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser3_speed_up state
    //
    if (TST_DOSER3_SPEED_UP != OLD_TST_DOSER3_SPEED_UP) {
        OLD_TST_DOSER3_SPEED_UP = TST_DOSER3_SPEED_UP;
        if (!TST_DOSER3_SPEED_UP)SetTasterDoser3SpeedUpPressed();
        else ResetTasterDoser3SpeedUpPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser3_speed_dn state
    //
    if (TST_DOSER3_SPEED_DN != OLD_TST_DOSER3_SPEED_DN) {
        OLD_TST_DOSER3_SPEED_DN = TST_DOSER3_SPEED_DN;
        if (!TST_DOSER3_SPEED_DN)SetTasterDoser3SpeedDownPressed();
        else ResetTasterDoser3SpeedDownPressed();
    }// End of if...
    //
    //------------------------------------------- check taster feeder3_on state
    //
    if (TST_FEEDER3_ON != OLD_TST_FEEDER3_ON) {
        OLD_TST_FEEDER3_ON = TST_FEEDER3_ON;
        if (!TST_FEEDER3_ON)SetTasterFeeder3OnPressed();
        else ResetTasterFeeder3OnPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser4_speed_up state
    //
    if (TST_DOSER4_SPEED_UP != OLD_TST_DOSER4_SPEED_UP) {
        OLD_TST_DOSER4_SPEED_UP = TST_DOSER4_SPEED_UP;
        if (!TST_DOSER4_SPEED_UP) SetTasterDoser4SpeedUpPressed();
        else ResetTasterDoser4SpeedUpPressed();
    }// End of if...
    //
    //------------------------------------------- check taster doser4_speed_dn state
    //
    if (TST_DOSER4_SPEED_DN != OLD_TST_DOSER4_SPEED_DN) {
        OLD_TST_DOSER4_SPEED_DN = TST_DOSER4_SPEED_DN;
        if (!TST_DOSER4_SPEED_DN) SetTasterDoser4SpeedDownPressed();
        else ResetTasterDoser4SpeedDownPressed();
    }// End of if...
    //
    //------------------------------------------- check taster feeder4_on state
    //
    if (TST_FEEDER4_ON != OLD_TST_FEEDER4_ON) {
        OLD_TST_FEEDER4_ON = TST_FEEDER4_ON;
        if (!TST_FEEDER4_ON) SetTasterFeeder4OnPressed();
        else ResetTasterFeeder4OnPressed();
    }// End of if...
    //
    //------------------------------------------- check taster mixer on/off state
    //
    if (TST_MIKSER_RUN != OLD_TST_MIKSER_RUN) {
        OLD_TST_MIKSER_RUN = TST_MIKSER_RUN;
        if (!TST_MIKSER_RUN)SetTasterMikserRunPressed();
        else ResetTasterMikserRunPressed();
    }// End of if...
    //
    //------------------------------------------- check taster vp1 on/off state
    //
    if (TST_VACUM_PUMP1_ON != OLD_TST_VACUM_PUMP1_ON) {
        OLD_TST_VACUM_PUMP1_ON = TST_VACUM_PUMP1_ON;
        if (!TST_VACUM_PUMP1_ON) SetVacumPump1OnTasterPressed();
        else ResetVacumPump1OnTasterPressed();
    }// End of if...
    //
    //------------------------------------------- check taster vp2 on/off state
    //
    if (TST_VACUM_PUMP2_ON != OLD_TST_VACUM_PUMP2_ON) {
        OLD_TST_VACUM_PUMP2_ON = TST_VACUM_PUMP2_ON;
        if (!TST_VACUM_PUMP2_ON) SetVacumPump2OnTasterPressed();
        else ResetVacumPump2OnTasterPressed();
    }// End of if...
    //
    //------------------------------------------- check taster vp3 on/off state
    //
    if (TST_VACUM_PUMP3_ON != OLD_TST_VACUM_PUMP3_ON) {
        OLD_TST_VACUM_PUMP3_ON = TST_VACUM_PUMP3_ON;
        if (!TST_VACUM_PUMP3_ON) SetVacumPump3OnTasterPressed();
        else ResetVacumPump3OnTasterPressed();
    }// End of if...
    //
    SW_CLEANING_MODE_STATE = SW_CLEANING_MODE;
    SW_DOSER1_MANUAL_STATE = SW_DOSER1_MANUAL;
    SW_DOSER1_AUTO_STATE = SW_DOSER1_AUTO;
    SW_DOSER2_MANUAL_STATE = SW_DOSER2_MANUAL;
    SW_DOSER2_AUTO_STATE = SW_DOSER2_AUTO;
    SW_DOSER3_MANUAL_STATE = SW_DOSER3_MANUAL;
    SW_DOSER3_AUTO_STATE = SW_DOSER3_AUTO;
    SW_DOSER4_MANUAL_STATE  = SW_DOSER4_MANUAL;
    SW_DOSER4_AUTO_STATE = SW_DOSER4_AUTO;

    
}// End of IO_Service
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="comm service">

void CommService(void) {

    volatile unsigned char tmp1, tmp2;
    
    //------------------------------------------- check if USART buffer overflow
    if (RCSTAbits.OERR) { //--------------------- reset USART and go ahead
        RCSTAbits.CREN = FALSE;
        TXSTAbits.TXEN = FALSE;
        Delay100TCYx(10);
        RCSTAbits.CREN = TRUE;
        TXSTAbits.TXEN = TRUE;
        tmp1 = RCREG;
        tmp1 = RCREG;
        tmp1 = RCREG;
        RX_PENDING = FALSE;
        RX_DATA_RDY = FALSE;
        DataDirRX();
        comm_data_cnt = 0;
        tx_lenght = 0;
        tx_cnt = 0;
        PIE1bits.TXIE = FALSE;
    } // End of if..
    //
    //------------------------------------------- check if is command received
    //
    if(RX_PENDING && RX_HOLD_TEX && !RX_DATA_RDY){
        tmp1 = RCREG;
        tmp1 = RCREG;
        tmp1 = RCREG;
        RX_PENDING = FALSE;
        RX_DATA_RDY = FALSE;
        DataDirRX();
        if(++comm_err_cnt == 100) MSG_TRANSFER_OK = FALSE;
        comm_data_cnt = 0;
        tx_lenght = 0;
        tx_cnt = 0;
        PIE1bits.TXIE = FALSE;
    } else if (RX_DATA_RDY) {
        msg_transfer_tmr = 200;
        MSG_TRANSFER_TEX = FALSE;
        DataDirTX();
        SelectStatusLED();
        status_led_cnt = 7;
        //--------------------------------------- copy mesage in working buffer
        tmp1 = 0;
        while (tmp1 < 25) {
            rx_buff[tmp1] = comm_buff[tmp1];
            comm_buff[tmp1] = 0;
            tmp1++;
        } // End of while,,,
        //--------------------------------------- signal ready for next message
        comm_data_cnt = 0;
        RX_DATA_RDY = FALSE;
        RX_PENDING = FALSE;
        rx_lenght = 0;
        //--------------------------------------- enable address detection
        //RCSTAbits.ADDEN = TRUE;
        //--------------------------------------- decode message
        switch (RX_MESSAGE_TYPE) {

            case GET_CMD:
                tmp1 = 0;
                tmp2 = 0;
                while (tmp1 < 3 ){
                    tmp2 += rx_buff[tmp1];
                    ++tmp1;
                }// End of while...
                if (RX_GET_MSG_CHECKSUM != tmp2) {
                    if(++comm_err_cnt == 100) MSG_TRANSFER_OK = FALSE;
                    TX_DESTINATION_ADDRESS = RX_SENDER_ADDRESS;
                    TX_SENDER_ADDRESS = net_address;
                    TX_MESSAGE_TYPE = NAK;
                    TX_NAK_MSG_CHKSUM = 0;
                    tmp1 = 0;
                    while (tmp1 < 3) {
                        TX_NAK_MSG_CHKSUM += tx_buff[tmp1];
                        ++tmp1;
                    }// End of while...
                    tx_lenght = 3;                    
                } else {
                    comm_err_cnt = 0;
                    MSG_TRANSFER_OK = TRUE;
                    TX_DESTINATION_ADDRESS = RX_SENDER_ADDRESS;
                    TX_SENDER_ADDRESS = net_address;
                    TX_MESSAGE_TYPE = GET_CMD;
                    TX_INPUT_0_7_STATE = inputs_0_7_flags2._byte;
                    TX_INPUT_8_15_STATE = inputs_8_15_flags2._byte;
                    TX_INPUT_16_23_STATE = inputs_16_23_flags2._byte;
                    TX_INPUT_24_31_STATE = inputs_24_31_flags2._byte;
                    TX_GET_MSG_CHKSUM = 0;
                    tmp1 = 0;
                    while (tmp1 < 7) {
                        TX_GET_MSG_CHKSUM += tx_buff[tmp1];
                        ++tmp1;
                    }// End of while
                    tx_lenght = 7;
                }// End of else....
                tx_cnt = 0;
                TX_DATA_RDY = TRUE;
                PIE1bits.TXIE = TRUE;
                break;

            case SET_CMD:
                tmp1 = 0;
                tmp2 = 0;
                while (tmp1 < 24) {
                    tmp2 += rx_buff[tmp1];
                    ++tmp1;
                }// End of while...
                if (RX_SET_MSG_CHECKSUM != tmp2) {
                    if(++comm_err_cnt == 100) MSG_TRANSFER_OK = FALSE;
                    TX_DESTINATION_ADDRESS = RX_SENDER_ADDRESS;
                    TX_SENDER_ADDRESS = net_address;
                    TX_MESSAGE_TYPE = NAK;
                    TX_NAK_MSG_CHKSUM = 0;
                    tmp1 = 0;
                    while (tmp1 < 3) {
                        TX_NAK_MSG_CHKSUM += tx_buff[tmp1];
                        ++tmp1;
                    }// End of while...
                    tx_lenght = 3;
                } else {
                    comm_err_cnt = 0;
                    MSG_TRANSFER_OK = TRUE;
                    signal1._byte = RX_SET_SIGNAL1;
                    signal2._byte = RX_SET_SIGNAL2;
                    signal3._byte = RX_SET_SIGNAL3;
                    signal4._byte = RX_SET_SIGNAL4;
                    signal5._byte = RX_SET_SIGNAL5;
                    error_dsp._byte = RX_SET_ERROR_DSP;
                    dsp1_dig100 = RX_SET_DSP1_DIG100;
                    dsp1_dig10 = RX_SET_DSP1_DIG10;
                    dsp1_dig1 = RX_SET_DSP1_DIG1;
                    dsp2_dig100 = RX_SET_DSP2_DIG100;
                    dsp2_dig10 = RX_SET_DSP2_DIG10;
                    dsp2_dig1 = RX_SET_DSP2_DIG1;
                    dsp3_dig100 = RX_SET_DSP3_DIG100;
                    dsp3_dig10 = RX_SET_DSP3_DIG10;
                    dsp3_dig1 = RX_SET_DSP3_DIG1;
                    dsp4_dig100 = RX_SET_DSP4_DIG100;
                    dsp4_dig10 = RX_SET_DSP4_DIG10;
                    dsp4_dig1 = RX_SET_DSP4_DIG1;
                    dsp5_dig100 = RX_SET_DSP5_DIG100;
                    dsp5_dig10 = RX_SET_DSP5_DIG10;
                    dsp5_dig1 = RX_SET_DSP5_DIG1;
                    TX_DESTINATION_ADDRESS = RX_SENDER_ADDRESS;
                    TX_SENDER_ADDRESS = net_address;
                    TX_MESSAGE_TYPE = ACK;
                    TX_ACK_MSG_CHKSUM = 0;
                    tmp1 = 0;
                    while (tmp1 < 3) {
                        TX_ACK_MSG_CHKSUM += tx_buff[tmp1];
                        ++tmp1;
                    }// End of while...
                    tx_lenght = 3;
                }// End of else if....                 
                tx_cnt = 0;
                TX_DATA_RDY = TRUE;
                PIE1bits.TXIE = TRUE;
                break;
        }// End of switch
    }// End of Rx
}// End of CommService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="signal service">

void SignalService(void) {
    //
    //------------------------------------------- private variable
    //
    volatile unsigned char spi_response;
    //
    //

    ResetShift1Latch();
    ResetShift1OutputEnable();
    SelectSignal1();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignal1();
    spi_response = WriteSPI(signal1._byte);
    SetShift1OutputEnable();
    ResetShift1OutputEnable();
    SelectSignal2();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignal2();
    spi_response = WriteSPI(signal2._byte);
    SetShift1OutputEnable();
    ResetShift1OutputEnable();
    SelectSignal3();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignal3();
    spi_response = WriteSPI(signal3._byte);
    SetShift1OutputEnable();
    ResetShift1OutputEnable();
    SelectSignal4();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignal4();
    spi_response = WriteSPI(signal4._byte);
    SetShift1OutputEnable();
    ResetShift1OutputEnable();
    SelectSignal5();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignal5();
    spi_response = WriteSPI(signal5._byte);
    SetShift1OutputEnable();
    ResetShift1OutputEnable();
}// End of SignalService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="error service">

void ErrorService(void) {
    //
    //------------------------------------------- private variable
    //
    volatile unsigned char spi_response;
    //
    //
    //
    //
    //------------------------------------------- communication error
    //
    if ((MSG_TRANSFER_TEX || !MSG_TRANSFER_OK ) && ERR_DSP_TEX) {
        //
        //--------------------------------------- try USART reset
        //
        RCSTAbits.CREN = FALSE;
        TXSTAbits.TXEN = FALSE;
        Delay100TCYx(10);
        RCSTAbits.CREN = TRUE;
        TXSTAbits.TXEN = TRUE;
        spi_response = RCREG;
        spi_response = RCREG;
        spi_response = RCREG;
        RX_PENDING = FALSE;
        RX_DATA_RDY = FALSE;
        DataDirRX();
        comm_data_cnt = 0;
        tx_lenght = 0;
        tx_cnt = 0;
        PIE1bits.TXIE = FALSE;
        //
        //--------------------------------------- clear all signals
        //
        signal1._byte = 0;
        signal2._byte = 0;
        signal3._byte = 0;
        signal4._byte = 0;
        signal5._byte = 0;
        //--------------------------------------- display comm error
        error_dsp._byte = ERROR_SYS_COMM;
        //--------------------------------------- set refresh rate
        err_dsp_tmr1 = 250;
        ERR_DSP_TEX = FALSE;
        if (err_pcnt == 0) {
            dsp5_dig100 = 0;
            dsp5_dig10 = 0;
            dsp5_dig1 = 0;
            dsp4_dig100 = 0;
            dsp4_dig10 = 0;
            dsp4_dig1 = 0;
            dsp3_dig100 = 0;
            dsp3_dig10 = 0;
            dsp3_dig1 = 0;
            dsp2_dig100 = 0;
            dsp2_dig10 = 0;
            dsp2_dig1 = 0;
            dsp1_dig100 = 0;
            dsp1_dig10 = 0;
            dsp1_dig1 = 0;
            err_pcnt++;
        } else if (err_pcnt == 1) {
            dsp1_dig100 = dsp1_dig10;
            dsp1_dig10 = dsp1_dig1;
            dsp1_dig1 = dsp2_dig100;
            dsp2_dig100 = dsp2_dig10;
            dsp2_dig10 = dsp2_dig1;
            dsp2_dig1 = dsp3_dig100;
            dsp3_dig100 = dsp3_dig10;
            dsp3_dig10 = dsp3_dig1;
            dsp3_dig1 = dsp4_dig100;
            dsp4_dig100 = dsp4_dig10;
            dsp4_dig10 = dsp4_dig1;
            if (err_cnt1 < 20) dsp4_dig1 = error_msg1[err_cnt1];
            else dsp4_dig1 = 0;
            if (++err_cnt1 > 40){
                err_cnt1 = 0;
                err_pcnt = 0;
            }// End of if...
        }// End of else if...
    }// End of if...

    //
    //------------------------------------------- refresh error display
    //
    ResetShift1Latch();
    ResetShift1OutputEnable();
    SelectSignalError1();
    spi_response = WriteSPI(shift1._byte);
    SetShift1Latch();
    ResetShift1Latch();
    DeselectSignalError1();
    spi_response = WriteSPI(error_dsp._byte);
    SetShift1OutputEnable();
}// End of ErrorService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="display service">

void DisplayService(void) {
    //
    //------------------------------------------- private variable
    //
    volatile unsigned char spi_response;
    volatile unsigned char dsp1_tmp;
    volatile unsigned char dsp2_tmp;
    volatile unsigned char dsp3_tmp;
    volatile unsigned char dsp4_tmp;
    volatile unsigned char dsp5_tmp;
    //
    //------------------------------------------- system clock check
    //
    if (!DSP_UPDATE) return;
    dsp_tmr = 5;
    DSP_UPDATE = FALSE;
    //
    //
    //
    switch (dsp_digit_cnt) {

        case (DIGIT_100):
            dsp1_tmp = CharTo7seg(dsp1_dig100);
            dsp2_tmp = CharTo7seg(dsp2_dig100);
            dsp3_tmp = CharTo7seg(dsp3_dig100);
            dsp4_tmp = CharTo7seg(dsp4_dig100);
            dsp5_tmp = CharTo7seg(dsp5_dig100);
            break;

        case (DIGIT_10):
            dsp1_tmp = CharTo7seg(dsp1_dig10);
            dsp2_tmp = CharTo7seg(dsp2_dig10);
            dsp3_tmp = CharTo7seg(dsp3_dig10);
            dsp4_tmp = CharTo7seg(dsp4_dig10);
            dsp5_tmp = CharTo7seg(dsp5_dig10);
            break;

        case (DIGIT_1):
            dsp1_tmp = CharTo7seg(dsp1_dig1);
            dsp2_tmp = CharTo7seg(dsp2_dig1);
            dsp3_tmp = CharTo7seg(dsp3_dig1);
            dsp4_tmp = CharTo7seg(dsp4_dig1);
            dsp5_tmp = CharTo7seg(dsp5_dig1);
            break;
    }// End of switch display digit counter

    DisplayOff();

    spi_response = WriteSPI(dsp1_tmp);
    SetDisplay1Latch();
    ResetDisplay1Latch();
    spi_response = WriteSPI(dsp2_tmp);
    SetDisplay2Latch();
    ResetDisplay2Latch();
    spi_response = WriteSPI(dsp3_tmp);
    SetDisplay3Latch();
    ResetDisplay3Latch();
    spi_response = WriteSPI(dsp4_tmp);
    SetDisplay4Latch();
    ResetDisplay4Latch();
    spi_response = WriteSPI(dsp5_tmp);
    SetDisplay5Latch();
    ResetDisplay5Latch();

    switch (dsp_digit_cnt) {

        case (DIGIT_100):
            DisplayDigit100();
            dsp_digit_cnt = DIGIT_10;
            break;

        case (DIGIT_10):
            DisplayDigit10();
            dsp_digit_cnt = DIGIT_1;
            break;

        case (DIGIT_1):
            DisplayDigit1();
            dsp_digit_cnt = DIGIT_100;
            break;
    }// End of switch display digit counter    
}// End of DisplayService
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="char to 7 segment display">

unsigned char CharTo7seg(unsigned char data) {
    if (data == '0') return 0b00111111;
    else if (data == '1') return 0b00000110;
    else if (data == '2') return 0b01011011;
    else if (data == '3') return 0b01001111;
    else if (data == '4') return 0b01100110;
    else if (data == '5') return 0b01101101;
    else if (data == '6') return 0b01111101;
    else if (data == '7') return 0b00000111;
    else if (data == '8') return 0b01111111;
    else if (data == '9') return 0b01101111;
    else if (data == 'A') return 0b01110111;
    else if (data == 'B') return 0b01111111;
    else if (data == 'b') return 0b01111100;
    else if (data == 'C') return 0b00111001;
    else if (data == 'c') return 0b01011000;
    else if (data == 'd') return 0b01011110;
    else if (data == 'E') return 0b01111001;
    else if (data == 'F') return 0b01110001;
    else if (data == 'G') return 0b01111101;
    else if (data == 'H') return 0b01110110;
    else if (data == 'h') return 0b01110100;
    else if (data == 'I') return 0b00000110;
    else if (data == 'i') return 0b00000100;
    else if (data == 'J') return 0b00001110;
    else if (data == 'k') return 0b01111000;
    else if (data == 'L') return 0b00111000;
    else if (data == 'l') return 0b00110000;
    else if (data == 'M') return 0b00110111;
    else if (data == 'n') return 0b01010100;
    else if (data == 'O') return 0b00111111;
    else if (data == 'o') return 0b01011100;
    else if (data == 'P') return 0b01110011;
    else if (data == 'r') return 0b01010000;
    else if (data == 'S') return 0b01101101;
    else if (data == 't') return 0b01111000;
    else if (data == 'U') return 0b00111110;
    else if (data == 'u') return 0b00011100;
    else if (data == 'Y') return 0b01101110;
    else if (data == '.') return 0b10000000;
    else if (data == ' ') return 0b00000000;
    else if (data == 0x20) return 0b00000000;
    else if (data == 0x00) return 0b00000000;
    else if (data == '-') return 0b01000000;
    else if (data == '_') return 0b00001000;
}// End of CharTo7seg
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="eeprom read byte">

unsigned char EEPROM_ReadByte(unsigned char address) {
    EECON1 = 0;
    EEADR = address;
    EECON1bits.RD = 1;
    return (EEDATA);
}// End of EEPROM_ReadByte
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="eeprom read int">

unsigned int EEPROM_ReadInt(unsigned char address) {
    volatile unsigned int data;
    data = EEPROM_ReadByte(address);
    data = (data << 8);
    data += EEPROM_ReadByte(address + 0x01);
    return data;
}// End of eeprom read int
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="eeprom write byte">

void EEPROM_WriteByte(unsigned char address, unsigned char data) {
    volatile unsigned char tmp_int;
    //    SEGMENT_WR = 0x00;
    tmp_int = INTCON;
    INTCONbits.GIEH = FALSE;
    INTCONbits.GIEL = FALSE;
    EECON1 = 0; //ensure CFGS=0 and EEPGD=0
    EECON1bits.WREN = 1; //enable write to EEPROM
    EEADR = address; //setup Address
    EEDATA = data; //and data
    EECON2 = 0x55; //required sequence #1
    EECON2 = 0xaa; //#2
    EECON1bits.WR = 1; //#3 = actual write
    while (!PIR2bits.EEIF); //wait until finished
    EECON1bits.WREN = 0; //disable write to EEPROM
    INTCON = tmp_int;
    Delay1KTCYx(50);
}// End of EEPROM_WriteByte
// </editor-fold>
//
//
//
// <editor-fold defaultstate="collapsed" desc="eeprom write int">

void EEPROM_WriteInt(unsigned char address, unsigned int data) {
    EEPROM_WriteByte(address, (data >> 8));
    EEPROM_WriteByte((address + 0x01), data);
}// </editor-fold>
