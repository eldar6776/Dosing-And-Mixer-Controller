/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *           Zaglavlje modula za komunikaciju sa terminalom
 *
 *******************************************************************************
 * Ime fajla:       terminal_comm.h
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

#ifndef TERMINAL_COMM_H
#define TERMINAL_COMM_H
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
extern unsigned char term_rx_lenght, term_tx_lenght, term_rx_tmp;
extern unsigned char term_tx_cnt, term_comm_address;
extern unsigned char term_rx_buff[10], term_tx_buff[10], term_comm_buff[10];
extern unsigned char term_comm_tmr, tc_tmp1;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
/********* R S 4 8 5  C O M U N I C A T I O N   D E F I N E S   ***************/
//
//
//----------------------------------------------- comm flags
//
extern BYTE term_comm_flags;
#define TERM_RX_DATA_RDY        term_comm_flags.b7
#define TERM_TX_DATA_RDY        term_comm_flags.b6
#define TERM_RX_PENDING         term_comm_flags.b5
#define TERM_TX_PENDING         term_comm_flags.b4
#define TERM_MSG_CKSM_ERR       term_comm_flags.b3
#define TERM_COMM_OK            term_comm_flags.b2
#define TERM_COM_TEX            term_comm_flags.b1
//
//
//----------------------------------------------- set message format
//
#define TERM_MSG_DEST_ADD       term_comm_buff[0]
#define TERM_MSG_SENDER_ADD     term_comm_buff[1]
#define TERM_MSG_TYPE           term_comm_buff[2]
#define TERM_DATA0              term_comm_buff[3]
#define TERM_DATA1              term_comm_buff[4]
#define TERM_DATA2              term_comm_buff[5]
#define TERM_DATA3              term_comm_buff[6]
#define TERM_DATA4              term_comm_buff[7]
#define TERM_DATA5              term_comm_buff[8]
#define TERM_MSG_CHECKSUM       term_comm_buff[9]
//
//----------------------------------------------- message type
//
//
//#define EOT         0x04        // end of transmission
//#define ACK         0x06        // acknowledge
//#define NAK         0x15        // negative acknowledge
//#define GET_CMD     0x3c        // '<'
//#define SET_CMD     0x3e        // '>'

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
void TerminalCommInit(void);
void TerminalCommDataSend(void);
void TerminalCommDataReceive(void);

#endif	// End of control comm