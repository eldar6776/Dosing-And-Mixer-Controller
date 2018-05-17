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

#include "control_comm.h"
#include <delays.h>
#include <usart.h>
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars
unsigned char ctrl_rx_lenght, ctrl_tx_lenght, ctrl_rx_tmp;
unsigned char ctrl_tx_cnt, ctrl_comm_address, ctrl_tx_pcnt;
unsigned char ctrl_rx_pcnt, ctrl_rx_buff[8], ctrl_tx_buff[25];
unsigned char ctrl_comm_err_cnt, ctrl_comm_timeout_tmr;
unsigned char ctrl_comm_rx_tmr;
//
//
//
BYTE skc_signal1;
BYTE skc_signal2;
BYTE skc_signal3;
BYTE skc_signal4;
BYTE skc_signal5;
BYTE skc_error_dsp;
BYTE skc_inputs_0_7;
BYTE skc_inputs_8_15;
BYTE skc_inputs_16_23;
BYTE skc_inputs_24_31;
BYTE ctrl_comm_flags1;
BYTE ctrl_comm_flags2;
//
//
//
//#pragma udata int_vars
//
//
//
#pragma code
//
//
//

void ControlCommInit(void) {
    //
    //------------------------------------------- port init
    //
    CONTROL_RX_TRIS = INPUT_PIN;
    CONTROL_TX_TRIS = INPUT_PIN;
    CTRL_DATA_DIR_TRIS = OUTPUT_PIN;
    ControlCommDataDirRX();
    //
    //------------------------------------------- open 2 usart 19200bps
    //
    TXSTA2bits.TX92 = FALSE;
    TXSTA2bits.SYNC2 = FALSE;
    TXSTA2bits.BRGH2 = LOW;
    TXSTA2bits.TXEN2 = HIGH;
    RCSTA2bits.ADDEN2 = FALSE;
    RCSTA2bits.CREN2 = TRUE;
    RCSTA2bits.RX92 = FALSE;
    RCSTA2bits.SPEN2 = TRUE;
    BAUDCON2bits.BRG16 = FALSE;
    SPBRGH2 = 0;
    SPBRG2 = 12;
    PIE3bits.RC2IE = HIGH;
    //
    //------------------------------------------- flags
    //
    ResetControlCommReceivingInProgress();
    ResetControlCommTransmitInProgress();
    ResetControlCommReceivedDataReady();
    ResetControlCommBusErrorAlarm(); 
    ControlCommSendSignalReset() ;
    ControlCommGetSignalReset();
}// End of control comm init

void ControlCommDataSend(void) {
    //
    //------------------------------------------- preivate variable
    //
    volatile unsigned char tmp1;
    //
    //------------------------------------------- set SKC => CuC data transfer
    //
    if (ctrl_tx_pcnt == 0) {
        if (IsControlCommSendSignalSelected() && !IsControlCommReceivingInProgress()) {
            ControlCommSendSignalReset();
            ControlCommDataDirTX();
            CTRL_MSG_TX_DEST_ADD = skc_comm_address;
            CTRL_MSG_TX_SENDER_ADD = ctrl_comm_address;
            CTRL_MSG_TX_TYPE = SET_CMD;
            CTRL_MSG_TX_SET_SIGNAL1 = skc_signal1._byte;
            CTRL_MSG_TX_SET_SIGNAL2 = skc_signal2._byte;
            CTRL_MSG_TX_SET_SIGNAL3 = skc_signal3._byte;
            CTRL_MSG_TX_SET_SIGNAL4 = skc_signal4._byte;
            CTRL_MSG_TX_SET_SIGNAL5 = skc_signal5._byte;
            CTRL_MSG_TX_SET_ERROR_DSP = skc_error_dsp._byte;
            CTRL_MSG_TX_SET_DSP1_DIG100 = dsp1_dig100;
            CTRL_MSG_TX_SET_DSP1_DIG10 = dsp1_dig10;
            CTRL_MSG_TX_SET_DSP1_DIG1 = dsp1_dig1;
            CTRL_MSG_TX_SET_DSP2_DIG100 = dsp2_dig100;
            CTRL_MSG_TX_SET_DSP2_DIG10 = dsp2_dig10;
            CTRL_MSG_TX_SET_DSP2_DIG1 = dsp2_dig1;
            CTRL_MSG_TX_SET_DSP3_DIG100 = dsp3_dig100;
            CTRL_MSG_TX_SET_DSP3_DIG10 = dsp3_dig10;
            CTRL_MSG_TX_SET_DSP3_DIG1 = dsp3_dig1;
            CTRL_MSG_TX_SET_DSP4_DIG100 = dsp4_dig100;
            CTRL_MSG_TX_SET_DSP4_DIG10 = dsp4_dig10;
            CTRL_MSG_TX_SET_DSP4_DIG1 = dsp4_dig1;
            CTRL_MSG_TX_SET_DSP5_DIG100 = dsp5_dig100;
            CTRL_MSG_TX_SET_DSP5_DIG10 = dsp5_dig10;
            CTRL_MSG_TX_SET_DSP5_DIG1 = dsp5_dig1;
            CTRL_MSG_TX_SET_CHECKSUM = 0;
            tmp1 = 0;
            while (tmp1 < 24) {
                CTRL_MSG_TX_SET_CHECKSUM += ctrl_tx_buff[tmp1];
                ++tmp1;
            }// End of while...
            ctrl_tx_lenght = 24;
            SetControlCommTransmitInProgress();
            ++ctrl_tx_pcnt;
        } else if (IsControlCommGetSignalSelected() && !IsControlCommReceivingInProgress()) {
            ControlCommGetSignalReset();
            ControlCommDataDirTX();
            CTRL_MSG_TX_DEST_ADD = skc_comm_address;
            CTRL_MSG_TX_SENDER_ADD = ctrl_comm_address;
            CTRL_MSG_TX_TYPE = GET_CMD;
            CTRL_MSG_TX_GET_CHECKSUM = 0;
            tmp1 = 0;
            while (tmp1 < 3) {
                CTRL_MSG_TX_GET_CHECKSUM += ctrl_tx_buff[tmp1];
                ++tmp1;
            }// End of while...
            ctrl_tx_lenght = 3;
            SetControlCommTransmitInProgress();
            ++ctrl_tx_pcnt;
        }// End of else if...
    } else if (ctrl_tx_pcnt == 1) {
        ControlCommTxInterruptEnable();
        ++ctrl_tx_pcnt;
    } else if ((ctrl_tx_pcnt == 2) && !IsControlCommTransmitInProgress()) {
        SetControlCommBusControlTimer();
        ctrl_tx_pcnt = 0;
    }// End of else if...
}// End of control comm data send

void ControlCommDataReceive(void) {
    //
    //------------------------------------------- private variable
    //
    volatile unsigned char tc_tmp1, tc_tmp2;
    //
    //------------------------------------------- check if USART buffer overflow
    //
    if (RCSTA2bits.OERR2 || RCSTA2bits.FERR2) { //------reset USART receiver
        ResetControlCommReceivingInProgress();
        ResetControlCommReceivedDataReady();
        ControlCommReceiverDisable();
        Delay100TCYx(100);
        ControlCommReceiverEnable();
        tc_tmp1 = RCREG2;
        tc_tmp1 = RCREG2;
        tc_tmp1 = RCREG2;
        ctrl_rx_lenght = 0;
        ctrl_rx_pcnt = 0;
    } else if (ctrl_rx_pcnt == 0) {
        if (IsControlCommReceivingInProgress()) ++ctrl_rx_pcnt;
        else if (IsControlCommReceivedDataReady()) ctrl_rx_pcnt = 2;
        else if (!IsControlCommBusControlTimerExpired()) ++ctrl_rx_pcnt;
    } else if (ctrl_rx_pcnt == 1) {
        if (IsControlCommReceivedDataReady()) {
            ++ctrl_rx_pcnt;
        } else if (IsControlCommReceivingTimerExpired() || IsControlCommBusControlTimerExpired()) {
            tc_tmp1 = RCREG2;
            tc_tmp1 = RCREG2;
            tc_tmp1 = RCREG2;
            ResetControlCommReceivingInProgress();
            ResetControlCommReceivedDataReady();
            if (++ctrl_comm_err_cnt > 200) SetControlCommBusErrorAlarm();
            ctrl_rx_lenght = 0;
            ctrl_rx_pcnt = 0;
        }// End of else if...
    } else if (ctrl_rx_pcnt == 2) {
        //
        //--------------------------------------- check received message
        //
        switch (CTRL_MSG_RX_MSG_TYPE) {
              //
             //---------------------------------- SKC acknowladge data
            //
            case ACK:
                tc_tmp1 = 0;
                tc_tmp2 = 0;
                while (tc_tmp1 < 3) {
                    tc_tmp2 += ctrl_rx_buff[tc_tmp1];
                    ++tc_tmp1;
                }// End of while...
                if (CTRL_MSG_RX_ACK_CHECKSUM == tc_tmp2) {
                    ctrl_comm_err_cnt = 0;
                    ResetControlCommBusErrorAlarm();
                } else if (++ctrl_comm_err_cnt > 200) SetControlCommBusErrorAlarm();
                ++ctrl_rx_pcnt;
                break;
               //
              //--------------------------------- SKC not-acknowladge data
             //---------------------------------- message checksum failed
            //
            case NAK:
                tc_tmp1 = 0;
                tc_tmp2 = 0;
                while (tc_tmp1 < 3) {
                    tc_tmp2 += ctrl_rx_buff[tc_tmp1];
                    ++tc_tmp1;
                }// End of while...
                if (CTRL_MSG_RX_NAK_CHECKSUM == tc_tmp2) {
                    ctrl_comm_err_cnt = 0;
                    ResetControlCommBusErrorAlarm();
                } else if (++ctrl_comm_err_cnt > 200) SetControlCommBusErrorAlarm();
                ++ctrl_rx_pcnt;
                break;
               //
              //--------------------------------- SKC send inputs state data
             //
            case GET_CMD:
                tc_tmp1 = 0;
                tc_tmp2 = 0;
                while (tc_tmp1 < 7) {
                    tc_tmp2 += ctrl_rx_buff[tc_tmp1];
                    ++tc_tmp1;
                }// End of while...
                if (CTRL_MSG_RX_GET_CHKSUM == tc_tmp2) {
                    skc_inputs_0_7._byte = CTRL_MSG_RX_GET_IN_0_7;
                    skc_inputs_8_15._byte = CTRL_MSG_RX_GET_IN_8_15;
                    skc_inputs_16_23._byte = CTRL_MSG_RX_GET_IN_16_23;
                    skc_inputs_24_31._byte = CTRL_MSG_RX_GET_IN_24_31;
                    ctrl_comm_err_cnt = 0;
                    ResetControlCommBusErrorAlarm();
                } else if (++ctrl_comm_err_cnt > 200) SetControlCommBusErrorAlarm();
                ++ctrl_rx_pcnt;
                break;
        }// End of switch
    } else if (ctrl_rx_pcnt == 3) {
        ResetControlCommReceivingInProgress();
        ResetControlCommReceivedDataReady();
        tc_tmp1 = RCREG2;
        tc_tmp1 = RCREG2;
        tc_tmp1 = RCREG2;
        ctrl_rx_lenght = 0;
        ctrl_rx_pcnt = 0;
    }// End of else if...
}// End of control comm data receive
//
//----------------------------------------------- end of file
//
