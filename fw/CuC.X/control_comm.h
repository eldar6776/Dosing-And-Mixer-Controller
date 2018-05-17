/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *         Zaglavlje komunikacijskog modula kontrolnog kruga
 *
 *******************************************************************************
 * Ime fajla:       control_comm.h
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

#ifndef CONTROL_COMM_H
#define CONTROL_COMM_H
//
//
/** U K L J U C E N I  ********************************************************/
//
//
#include "defines.h"
#include "io_cfg.h"
#include "process_control.h"
#include "main.h"
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
//#pragma udata char_vars
extern unsigned char ctrl_rx_lenght, ctrl_tx_lenght, ctrl_rx_tmp;
extern unsigned char ctrl_tx_cnt, ctrl_comm_address, ctrl_pcnt;
extern unsigned char ctrl_rx_buff[8], ctrl_tx_buff[25];
extern unsigned char ctrl_comm_err_cnt, ctrl_comm_timeout_tmr;
extern unsigned char ctrl_comm_rx_tmr;
//
//
//
/*********   F L A G S   ******************************************************/
//
//
//----------------------------------------------- signal 1 expander
//
extern BYTE skc_signal1;
#define SKC_SIG_FEEDER3_ON          skc_signal1.b0
#define SKC_SIG_FEEDER1_ON          skc_signal1.b1
#define SKC_SIG_DOS4_AUTO_SEL       skc_signal1.b2
#define SKC_SIG_DOS3_AUTO_SEL       skc_signal1.b3
#define SKC_SIG_DOS2_AUTO_SEL       skc_signal1.b4
#define SKC_SIG_DOS1_AUTO_SEL       skc_signal1.b5
#define SKC_SIG_CLEANING_MODE       skc_signal1.b6
#define SKC_SIG_START               skc_signal1.b7
//
//----------------------------------------------- signal 2 expander
//
extern BYTE skc_signal2;
#define SKC_SIG_STOP                skc_signal2.b0
#define SKC_SIG_ALARM               skc_signal2.b1
#define SKC_SIG_DOS1_MANUAL_SEL     skc_signal2.b2
#define SKC_SIG_DOS2_MANUAL_SEL     skc_signal2.b3
#define SKC_SIG_DOS3_MANUAL_SEL     skc_signal2.b4
#define SKC_SIG_DOS4_MANUAL_SEL     skc_signal2.b5
#define SKC_SIG_FEEDER2_ON          skc_signal2.b6
#define SKC_SIG_FEEDER4_ON          skc_signal2.b7
//
//----------------------------------------------- signal 3 expander
//
extern BYTE skc_signal3;
#define SKC_SIG_FEEDER1_FULL        skc_signal3.b0
#define SKC_SIG_FEEDER1_MIN         skc_signal3.b1
#define SKC_SIG_FEEDER2_FULL        skc_signal3.b2
#define SKC_SIG_FEEDER2_MIN         skc_signal3.b3
#define SKC_SIG_FEEDER3_FULL        skc_signal3.b4
#define SKC_SIG_FEEDER3_MIN         skc_signal3.b5
#define SKC_SIG_FEEDER4_FULL        skc_signal3.b6
#define SKC_SIG_FEEDER4_MIN         skc_signal3.b7
//
//----------------------------------------------- signal 4 expander
//
extern BYTE skc_signal4;
#define SKC_SIG_MIKSER_FULL         skc_signal4.b0
#define SKC_SIG_EKSTRUDER_RUN       skc_signal4.b1
#define SKC_SIG_VACUM_PUMP2_RUN     skc_signal4.b2
#define SKC_SIG_CYCLON_FULL         skc_signal4.b3
#define SKC_SIG_VACUM_PUMP3_RUN     skc_signal4.b4
#define SKC_SIG_VACUM_PUMP1_RUN     skc_signal4.b5
#define SKC_SIG_MIKSER_MIN          skc_signal4.b6
#define SKC_SIG_MIKSER_RUN          skc_signal4.b7
//
//----------------------------------------------- signal 5 expander
//
extern BYTE skc_signal5;
#define SKC_SIG_FEEDER4_REQUEST     skc_signal5.b0
#define SKC_SIG_FEEDER4_VACUM       skc_signal5.b1
#define SKC_SIG_FEEDER3_REQUEST     skc_signal5.b2
#define SKC_SIG_FEEDER3_VACUM       skc_signal5.b3
#define SKC_SIG_FEEDER2_REQUEST     skc_signal5.b4
#define SKC_SIG_FEEDER2_VACUM       skc_signal5.b5
#define SKC_SIG_FEEDER1_REQUEST     skc_signal5.b6
#define SKC_SIG_FEEDER1_VACUM       skc_signal5.b7
//
//----------------------------------------------- error display expander
//
extern BYTE skc_error_dsp;
#define SKC_ERR_DSP_UNTIS_1         skc_error_dsp.b0
#define SKC_ERR_DSP_UNTIS_2         skc_error_dsp.b1
#define SKC_ERR_DSP_UNTIS_4         skc_error_dsp.b2
#define SKC_ERR_DSP_UNTIS_8         skc_error_dsp.b3
#define SKC_ERR_DSP_TENS_1          skc_error_dsp.b4
#define SKC_ERR_DSP_TENS_2          skc_error_dsp.b5
#define SKC_ERR_DSP_TENS_4          skc_error_dsp.b6
#define SKC_ERR_DSP_TENS_8          skc_error_dsp.b7
//
//----------------------------------------------- inputs 1 to 8
//
extern BYTE skc_inputs_0_7;
#define SKC_TST_START               skc_inputs_0_7.b0
#define SKC_TST_STOP                skc_inputs_0_7.b1
#define SKC_TST_RESET               skc_inputs_0_7.b2
#define SKC_TST_PG_LOAD             skc_inputs_0_7.b3
#define SKC_TST_PG_DN               skc_inputs_0_7.b4
#define SKC_TST_DOSER1_SPEED_UP     skc_inputs_0_7.b5
#define SKC_TST_PG_UP               skc_inputs_0_7.b6
#define SKC_TST_PG_SAVE             skc_inputs_0_7.b7
//
//----------------------------------------------- inputs 9 to 16
//
extern BYTE skc_inputs_8_15;
#define SKC_TST_DOSER1_SPEED_DN     skc_inputs_8_15.b0
#define SKC_TST_FEEDER1_ON          skc_inputs_8_15.b1
#define SKC_TST_DOSER2_SPEED_UP     skc_inputs_8_15.b2
#define SKC_TST_DOSER2_SPEED_DN     skc_inputs_8_15.b3
#define SKC_TST_FEEDER2_ON          skc_inputs_8_15.b4
#define SKC_TST_DOSER3_SPEED_UP     skc_inputs_8_15.b5
#define SKC_TST_DOSER3_SPEED_DN     skc_inputs_8_15.b6
#define SKC_TST_FEEDER3_ON          skc_inputs_8_15.b7
//
//----------------------------------------------- inputs 17 to 24
//
extern BYTE skc_inputs_16_23;
#define SKC_TST_DOSER4_SPEED_UP     skc_inputs_16_23.b0
#define SKC_TST_DOSER4_SPEED_DN     skc_inputs_16_23.b1
#define SKC_TST_FEEDER4_ON          skc_inputs_16_23.b2
#define SKC_TST_MIXER_ON            skc_inputs_16_23.b3
#define SKC_SW_CLEANING_MODE        skc_inputs_16_23.b4
#define SKC_SW_DOSER1_MANUAL        skc_inputs_16_23.b5
#define SKC_SW_DOSER1_AUTO          skc_inputs_16_23.b6
#define SKC_SW_DOSER2_MANUAL        skc_inputs_16_23.b7
//
//----------------------------------------------- inputs 25 to 32
//
extern BYTE skc_inputs_24_31;
#define SKC_SW_DOSER2_AUTO          skc_inputs_24_31.b0
#define SKC_SW_DOSER3_MANUAL        skc_inputs_24_31.b1
#define SKC_SW_DOSER3_AUTO          skc_inputs_24_31.b2
#define SKC_SW_DOSER4_MANUAL        skc_inputs_24_31.b3
#define SKC_SW_DOSER4_AUTO          skc_inputs_24_31.b4
#define SKC_TST_VP1_ON              skc_inputs_24_31.b5
#define SKC_TST_VP2_ON              skc_inputs_24_31.b6
#define SKC_TST_VP3_ON              skc_inputs_24_31.b7
//
//
/********* R S 4 8 5  C O M U N I C A T I O N   D E F I N E S   ***************/
//
//
//----------------------------------------------- comm flags
//
extern BYTE ctrl_comm_flags1;
#define CTRL_RX_DATA_RDY            ctrl_comm_flags1.b7
#define CTRL_TX_DATA_RDY            ctrl_comm_flags1.b6
#define CTRL_RX_PENDING             ctrl_comm_flags1.b5
#define CTRL_TX_PENDING             ctrl_comm_flags1.b4
#define CTRL_RX_ERROR               ctrl_comm_flags1.b3
#define CTRL_COMM_ERROR             ctrl_comm_flags1.b2
#define CTRL_TRANSFER_OK            ctrl_comm_flags1.b0

extern BYTE ctrl_comm_flags2;
#define CTRL_COMM_TRANSFER_TEX      ctrl_comm_flags2.b0
#define CTRL_COMM_TIMEOUT_TEX       ctrl_comm_flags2.b1
#define CTRL_COMM_RX_TEX            ctrl_comm_flags2.b2
#define CTRL_COMM_SIGNAL_SET        ctrl_comm_flags2.b3
#define CTRL_COMM_INPUTS_GET        ctrl_comm_flags2.b4
//
//
//
//----------------------------------------------- set SKC message format
//
#define CTRL_MSG_TX_DEST_ADD            ctrl_tx_buff[0]
#define CTRL_MSG_TX_SENDER_ADD          ctrl_tx_buff[1]
#define CTRL_MSG_TX_TYPE                ctrl_tx_buff[2]
#define CTRL_MSG_TX_GET_CHECKSUM        ctrl_tx_buff[3]
#define CTRL_MSG_TX_SET_SIGNAL1         ctrl_tx_buff[3]
#define CTRL_MSG_TX_SET_SIGNAL2         ctrl_tx_buff[4]
#define CTRL_MSG_TX_SET_SIGNAL3         ctrl_tx_buff[5]
#define CTRL_MSG_TX_SET_SIGNAL4         ctrl_tx_buff[6]
#define CTRL_MSG_TX_SET_SIGNAL5         ctrl_tx_buff[7]
#define CTRL_MSG_TX_SET_ERROR_DSP       ctrl_tx_buff[8]
#define CTRL_MSG_TX_SET_DSP1_DIG100     ctrl_tx_buff[9]
#define CTRL_MSG_TX_SET_DSP1_DIG10      ctrl_tx_buff[10]
#define CTRL_MSG_TX_SET_DSP1_DIG1       ctrl_tx_buff[11]
#define CTRL_MSG_TX_SET_DSP2_DIG100     ctrl_tx_buff[12]
#define CTRL_MSG_TX_SET_DSP2_DIG10      ctrl_tx_buff[13]
#define CTRL_MSG_TX_SET_DSP2_DIG1       ctrl_tx_buff[14]
#define CTRL_MSG_TX_SET_DSP3_DIG100     ctrl_tx_buff[15]
#define CTRL_MSG_TX_SET_DSP3_DIG10      ctrl_tx_buff[16]
#define CTRL_MSG_TX_SET_DSP3_DIG1       ctrl_tx_buff[17]
#define CTRL_MSG_TX_SET_DSP4_DIG100     ctrl_tx_buff[18]
#define CTRL_MSG_TX_SET_DSP4_DIG10      ctrl_tx_buff[19]
#define CTRL_MSG_TX_SET_DSP4_DIG1       ctrl_tx_buff[20]
#define CTRL_MSG_TX_SET_DSP5_DIG100     ctrl_tx_buff[21]
#define CTRL_MSG_TX_SET_DSP5_DIG10      ctrl_tx_buff[22]
#define CTRL_MSG_TX_SET_DSP5_DIG1       ctrl_tx_buff[23]
#define CTRL_MSG_TX_SET_CHECKSUM        ctrl_tx_buff[24]
//#define CTRL_MSG_TX_SET_DSP_DECIMAL1    ctrl_tx_buff[24]
//#define CTRL_MSG_TX_SET_DSP_DECIMAL2    ctrl_tx_buff[25]
//#define CTRL_MSG_TX_SET_CHECKSUM        ctrl_tx_buff[26]
//
//----------------------------------------------- set (not/)acknowledge  format
//
//#define CTRL_MSG_DEST_ADD           ctrl_rx_buff[0]
//#define CTRL_MSG_SENDER_ADD         ctrl_rx_buff[1]
//#define CTRL_MSG_TYPE               ctrl_rx_buff[2]
//#define CTRL_MSG_ACK_CHECKSUM       ctrl_rx_buff[3]
//
//----------------------------------------------- get message format
//
#define CTRL_MSG_RX_DEST_ADD            ctrl_rx_buff[0]
#define CTRL_MSG_RX_SENDER_ADD          ctrl_rx_buff[1]
#define CTRL_MSG_RX_MSG_TYPE            ctrl_rx_buff[2]
#define CTRL_MSG_RX_ACK_CHECKSUM        ctrl_rx_buff[3]
#define CTRL_MSG_RX_NAK_CHECKSUM        ctrl_rx_buff[3]
#define CTRL_MSG_RX_GET_IN_0_7          ctrl_rx_buff[3]
#define CTRL_MSG_RX_GET_IN_8_15         ctrl_rx_buff[4]
#define CTRL_MSG_RX_GET_IN_16_23        ctrl_rx_buff[5]
#define CTRL_MSG_RX_GET_IN_24_31        ctrl_rx_buff[6]
#define CTRL_MSG_RX_GET_CHKSUM          ctrl_rx_buff[7]
//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void ControlCommInit(void);
void ControlCommDataSend(void);
void ControlCommDataReceive(void);

#endif	// End of control comm