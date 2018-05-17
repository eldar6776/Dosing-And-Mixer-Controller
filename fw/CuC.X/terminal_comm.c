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

#include "terminal_comm.h"
#include "control_comm.h"
#include "main.h"
#include <delays.h>
#include <usart.h>
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars2
unsigned char term_rx_lenght, term_tx_lenght, term_rx_tmp;
unsigned char term_tx_cnt, term_comm_address, term_tx_pcnt;
unsigned char term_rx_pcnt, term_rx_buff[10], term_tx_buff[10];
unsigned char term_comm_tmr, term_comm_buff[10];

BYTE term_comm_flags;
//
//
//
//#pragma udata int_vars2
//unsigned int dos1_ref, dos2_ref, dos3_ref, dos4_ref;

#pragma code

void TerminalCommInit(void) {

    TERM_TX_TRIS = OUTPUT_PIN;
    TERM_RX_TRIS = INPUT_PIN;
    TERM_DATA_DIR_TRIS = OUTPUT_PIN;
    TerminalDataDirRX();
    //
    //------------------------------------------- open 1 usart 19200bps
    //
    //Open1USART(USART_TX_INT_OFF &
    //        USART_RX_INT_ON &
    //        USART_ASYNCH_MODE &
    //        USART_NINE_BIT &
    //        USART_ADDEN_ON &
    //        BAUD_8_BIT_RATE &
    //        USART_CONT_RX &
    //        USART_BRGH_LOW,
    //        25);
    TXSTA1bits.TX91 = FALSE;
    TXSTA1bits.SYNC1 = FALSE;
    TXSTA1bits.BRGH1 = LOW;
    TXSTA1bits.TXEN1 = HIGH;
    RCSTA1bits.ADDEN1 = FALSE;
    RCSTA1bits.CREN1 = TRUE;
    RCSTA1bits.RX91 = FALSE;
    RCSTA1bits.SPEN = TRUE;
    BAUDCON1bits.BRG16 = FALSE;
    SPBRGH1 = 0;
    SPBRG1 = 25;
    PIE1bits.RC1IE = HIGH;
    term_rx_lenght = 0;
    term_comm_flags._byte = 0x00;
    //TERM_RX_DATA_RDY  =   FALSE;
    //TERM_TX_DATA_RDY  =   FALSE;
    //TERM_RX_PENDING  =    FALSE;
    //TERM_TX_PENDING  =    FALSE;
    //TERM_MSG_CKSM_ERR  =  FALSE;
    //TERM_COMM_OK  =       FALSE;
}// End of control comm init

void TerminalCommDataSend(void) {

    if (term_tx_pcnt == 0) {
        if (TERM_TX_DATA_RDY && !TERM_RX_PENDING) {
            TerminalCommTx9BitTransmission();
            TerminalCommTx9BitSendAddress();
            TerminalDataDirTX();
            TERM_TX_PENDING = TRUE;
            term_tx_cnt = 0;
            ++term_tx_pcnt;
        } else TerminalDataDirRX();
    } else if (term_tx_pcnt == 1) {
        TerminalCommTxInterruptEnable();
        ++term_tx_pcnt;
    } else if ((term_tx_pcnt == 2) && (!TERM_TX_PENDING)) {
        TerminalDataDirRX();
        term_tx_pcnt = 0;
    }// End of else if...
}// End of terminal comm data send

void TerminalCommDataReceive(void){
    volatile unsigned char tc_tmp1, tc_tmp2;
    //------------------------------------------- check if USART buffer overflow
    if (RCSTA1bits.OERR1 || RCSTA1bits.FERR1) { //------reset USART receiver
        tc_tmp1 = RCREG1;
        tc_tmp1 = RCREG1;
        tc_tmp1 = RCREG1;
        RCSTA1bits.CREN1 = FALSE;
        TerminalCommRx9BitReception() ;
        TerminalCommRxAddressingEnable();
        TERM_RX_PENDING = FALSE;
        TERM_RX_DATA_RDY = FALSE;
        term_rx_lenght = 0;
        RCSTA1bits.CREN1 = TRUE;
        term_rx_pcnt = 0;
    } else if (term_rx_pcnt == 0) {
        if (TERM_RX_PENDING) ++term_rx_pcnt;
        else if (TERM_RX_DATA_RDY) term_rx_pcnt = 2;
    } else if (term_rx_pcnt == 1) {
        if (TERM_RX_DATA_RDY) {
            ++term_rx_pcnt;
        } else if (TERM_COM_TEX) {
            tc_tmp1 = RCREG1;
            tc_tmp1 = RCREG1;
            tc_tmp1 = RCREG1;
            TerminalCommRx9BitReception();
            TerminalCommRxAddressingEnable();
            TERM_RX_PENDING = FALSE;
            TERM_RX_DATA_RDY = FALSE;
            term_rx_lenght = 0;
            term_rx_pcnt = 0;
        }// End of else if...
    } else if (term_rx_pcnt == 2) {
        tc_tmp1 = 0;
        while (tc_tmp1 < 10) {
            term_comm_buff[tc_tmp1] = term_rx_buff[tc_tmp1];
            ++tc_tmp1;
        }// End of while,,,
        TerminalCommRxAddressingEnable();
        TERM_RX_PENDING = FALSE;
        TERM_RX_DATA_RDY = FALSE;
        term_rx_lenght = 0;
        ++term_rx_pcnt;
    } else if (term_rx_pcnt == 3) {
        tc_tmp1 = 0;
        tc_tmp2 = 0;
        while (tc_tmp1 < 9) {
            tc_tmp2 += term_comm_buff[tc_tmp1];
            ++tc_tmp1;
        }// End of while,,,
        if (term_comm_buff[9] == tc_tmp2) ++term_rx_pcnt; 
        else {
            term_tx_buff[0] = TERM_MSG_SENDER_ADD;
            term_tx_buff[1] = term_comm_address;
            term_tx_buff[2] = NAK;
            term_tx_buff[3] = 0x00;
            tc_tmp1 = 0;
            while (tc_tmp1 < 3) {
                term_tx_buff[3] += term_tx_buff[tc_tmp1];
                ++tc_tmp1;
            }// End of while,,,
            TERM_TX_DATA_RDY = TRUE;
            term_rx_pcnt = 0;
        }// End of else...
    } else if (term_rx_pcnt == 4){
        term_tx_buff[0] = TERM_MSG_SENDER_ADD;
            term_tx_buff[1] = term_comm_address;
            term_tx_buff[2] = ACK;
            term_tx_buff[3] = 0x00;
            tc_tmp1 = 0;
            while (tc_tmp1 < 3) {
                term_tx_buff[3] += term_tx_buff[tc_tmp1];
                ++tc_tmp1;
            }// End of while,,,
            TERM_TX_DATA_RDY = TRUE;
            ++term_rx_pcnt;
    }
}// End of terminal comm data receive

//
//----------------------------------------------- end of file
//
