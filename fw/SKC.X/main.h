/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Signal and Keys Controller
 *
 *                      Zaglavlje glavnog programa
 *
 *******************************************************************************
 * Ime fajla:       main.h
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
//
#ifndef MAIN_H
#define	MAIN_H
//
//
//
#include "typedefs.h"
//
//
/** K O N F I G U R A C I J A  M I K R O K O N T R O L E R A  *****************/
//
//
#pragma config FOSC	= INTIO67 // interni oscilator ukljucen
#pragma config FCMEN	= OFF	// iskljucen fail safe clock monitor
#pragma config IESO     = OFF   // iskljucen preklopnik oscilatora
#pragma config PWRT 	= ON	// iskljucen timer zadrske po ukljucenju
#pragma config BOREN	= ON	// iskljucen monitoring napona napajanja
#pragma config WDTEN	= OFF	// iskljucen watch dog timer
#pragma config CCP2MX   = PORTC // CCP2 on PORTC 1
#pragma config PBADEN   = OFF   // PORTB 4:0 digital io on reset
#pragma config LPT1OSC  = OFF   // TIMER1 config for higher power operation
#pragma config HFOFST   = OFF   // wait until oscilator is stable
#pragma config MCLRE    = ON    // MCLR pin enabled
#pragma config LVP	= OFF	// onemoguceno programiranje ICSP bez povisenog napona
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
#pragma udata char_vars
unsigned char pg_in_use, rx_lenght, tx_lenght, rx_hold_tmr, err_dsp_tmr1;
unsigned char comm_data_cnt, net_address, err_cnt1, new_mask, err_dsp_tmr2;
unsigned char comm_buff[25], rx_buff[25], tx_buff[8];
unsigned char comm_temp, temp_cnt, checksum, tx_cnt, comm_err_cnt;
unsigned char dsp_tmr, sig_pcnt, msg_transfer_tmr;
unsigned char display_mode, display_name, status_led_cnt;
unsigned char dsp1_dig100, dsp1_dig10, dsp1_dig1;
unsigned char dsp2_dig100, dsp2_dig10, dsp2_dig1;
unsigned char dsp3_dig100, dsp3_dig10, dsp3_dig1;
unsigned char dsp4_dig100, dsp4_dig10, dsp4_dig1;
unsigned char dsp5_dig100, dsp5_dig10, dsp5_dig1;
unsigned char error_code1, error_code10, err_pcnt;
const rom unsigned char error_msg1[] = "Error rS 485 Port...";
//
//
//
#pragma udata int_vars
unsigned int dos1_ref, dos2_ref, dos3_ref, dos4_ref;
unsigned int dsp_pg, dsp_dos1, dsp_dos2, dsp_dos3, dsp_dos4;
//
//
/**  E N U M E R A T O R S   **************************************************/
//
//
//
//
//
enum _display_digit_counter{
    DIGIT_100, DIGIT_10, DIGIT_1
} dsp_digit_cnt;
//
//
/** E E P R O M    A S S I G N E D    A D R E S S E S  ************************/
//
//
#define EE_VERSION_INFO_B0  0x00
#define EE_VERSION_INFO_B1  0x01
#define EE_VERSION_INFO_B2  0x02
#define EE_DEV_ID_B0        0x03
#define EE_DEV_ID_B1        0x04
#define EE_DEV_ID_B2        0x05
#define EE_RS485_ADDRESS    0x07
//
//
/*********   F L A G S   ******************************************************/
//
//----------------------------------------------- define error code
//
#define ERROR_DISPLAY_CLEAR         0xff
//
#define ERROR_SYS_COMM              0x01
//
#define ERROR_EMERGENCY             0x10
#define ERROR_VP1_TH_PROT           0x11
#define ERROR_VP2_TH_PROT           0x12
#define ERROR_VP3_TH_PROT           0x13
#define ERROR_VP1_OVRCRNT_PROT      0x14
#define ERROR_VP2_OVRCRNT_PROT      0x15
#define ERROR_VP3_OVRCRNT_PROT      0x16
#define ERROR_MIX_OVRCRNT_PROT      0x17
//
#define ERROR_VP1_VACUM_CTRL        0x21
#define ERROR_VP2_VACUM_CTRL        0x22
#define ERROR_VP3_VACUM_CTRL        0x23
//
#define ERROR_DRIVE1                0x31
#define ERROR_DRIVE2                0x32
#define ERROR_DRIVE3                0x33
#define ERROR_DRIVE4                0x34
//
#define ERROR_FED1_SENSOR_CTRL      0x41
#define ERROR_FED2_SENSOR_CTRL      0x42
#define ERROR_FED3_SENSOR_CTRL      0x43
#define ERROR_FED4_SENSOR_CTRL      0x44
//
#define ERROR_FEED1_MIN_LEVEL       0x51
#define ERROR_FEED2_MIN_LEVEL       0x52
#define ERROR_FEED3_MIN_LEVEL       0x53
#define ERROR_FEED4_MIN_LEVEL       0x54
#define ERROR_CYCLON_MAX_LEVEL      0x55
//
#define ERROR_FEED1_CYC_CNT         0x61
#define ERROR_FEED2_CYC_CNT         0x62
#define ERROR_FEED3_CYC_CNT         0x63
#define ERROR_FEED4_CYC_CNT         0x64
//
//----------------------------------------------- system flags
//
BYTE system_flags;
#define DSP_UPDATE                  system_flags.b0
#define ERR_DSP_UPDATE              system_flags.b1
#define SIG_UPDATE                  system_flags.b2
//
//----------------------------------------------- timers flags
//
BYTE timer_flags1;
#define SIG_SRV_TEX                 timer_flags1.b0
#define TX_HOLD_TEX                 timer_flags1.b1
#define RX_HOLD_TEX                 timer_flags1.b2
#define CLOCK_TICK                  timer_flags1.b3
#define DSP_UPD_TEX                 timer_flags1.b4
#define ERR_DSP_TEX                 timer_flags1.b5
#define MSG_TRANSFER_TEX            timer_flags1.b6
//
//----------------------------------------------- signal 1 expander
//
BYTE signal1;
#define SIG_FEEDER3_ON              signal1.b0
#define SIG_FEEDER1_ON              signal1.b1
#define SIG_DOS4_RUN_AUTO           signal1.b2
#define SIG_DOS3_RUN_AUTO           signal1.b3
#define SIG_DOS2_RUN_AUTO           signal1.b4
#define SIG_DOS1_RUN_AUTO           signal1.b5
#define SIG_CLEANING_MODE           signal1.b6
#define SIG_START                   signal1.b7
//
//----------------------------------------------- signal 2 expander
//
BYTE signal2;
#define SIG_STOP                    signal2.b0
#define SIG_ALARM                   signal2.b1
#define SIG_DOS1_RUN_MANUAL         signal2.b2
#define SIG_DOS2_RUN_MANUAL         signal2.b3
#define SIG_DOS3_RUN_MANUAL         signal2.b4
#define SIG_DOS4_RUN_MANUAL         signal2.b5
#define SIG_FEEDER2_ON              signal2.b6
#define SIG_FEEDER4_ON              signal2.b7
//
//----------------------------------------------- signal 3 expander
//
BYTE signal3;
#define SIG_FEEDER1_FULL            signal3.b0
#define SIG_FEEDER1_MIN             signal3.b1
#define SIG_FEEDER2_FULL            signal3.b2
#define SIG_FEEDER2_MIN             signal3.b3
#define SIG_FEEDER3_FULL            signal3.b4
#define SIG_FEEDER3_MIN             signal3.b5
#define SIG_FEEDER4_FULL            signal3.b6
#define SIG_FEEDER4_MIN             signal3.b7
//
//----------------------------------------------- signal 4 expander
//
BYTE signal4;
#define SIG_MIKSER_FULL             signal4.b0
#define SIG_EKSTRUDER_RUN           signal4.b1
#define SIG_VACUM_PUMP2_RUN         signal4.b2
#define SIG_CYCLON_FULL             signal4.b3
#define SIG_VACUM_PUMP3_RUN         signal4.b4
#define SIG_VACUM_PUMP1_RUN         signal4.b5
#define SIG_MIKSER_MIN              signal4.b6
#define SIG_MIKSER_RUN              signal4.b7
//
//----------------------------------------------- signal 5 expander
//
BYTE signal5;
#define SIG_FEEDER4_REQUEST         signal5.b0
#define SIG_FEEDER4_VACUM           signal5.b1
#define SIG_FEEDER3_REQUEST         signal5.b2
#define SIG_FEEDER3_VACUM           signal5.b3
#define SIG_FEEDER2_REQUEST         signal5.b4
#define SIG_FEEDER2_VACUM           signal5.b5
#define SIG_FEEDER1_REQUEST         signal5.b6
#define SIG_FEEDER1_VACUM           signal5.b7
//
//----------------------------------------------- error display expander
//
BYTE error_dsp;
#define ERR_DSP_UNTIS_1             error_dsp.b0
#define ERR_DSP_UNTIS_2             error_dsp.b1
#define ERR_DSP_UNTIS_4             error_dsp.b2
#define ERR_DSP_UNTIS_8             error_dsp.b3
#define ERR_DSP_TENS_1              error_dsp.b4
#define ERR_DSP_TENS_2              error_dsp.b5
#define ERR_DSP_TENS_4              error_dsp.b6
#define ERR_DSP_TENS_8              error_dsp.b7
//
//----------------------------------------------- inputs 1 to 8
//
BYTE inputs_0_7;
#define TST_START                   inputs_0_7.b0
#define TST_STOP                    inputs_0_7.b1
#define TST_RESET                   inputs_0_7.b2
#define TST_PG_LOAD                 inputs_0_7.b3
#define TST_PG_DN                   inputs_0_7.b4
#define TST_DOSER1_SPEED_UP         inputs_0_7.b5
#define TST_PG_UP                   inputs_0_7.b6
#define TST_PG_SAVE                 inputs_0_7.b7
//
//
//
BYTE inputs_0_7_flags1;
#define OLD_TST_START               inputs_0_7_flags1.b0
#define OLD_TST_STOP                inputs_0_7_flags1.b1
#define OLD_TST_RESET               inputs_0_7_flags1.b2
#define OLD_TST_PG_LOAD             inputs_0_7_flags1.b3
#define OLD_TST_PG_DN               inputs_0_7_flags1.b4
#define OLD_TST_DOSER1_SPEED_UP     inputs_0_7_flags1.b5
#define OLD_TST_PG_UP               inputs_0_7_flags1.b6
#define OLD_TST_PG_SAVE             inputs_0_7_flags1.b7
//
//
//
BYTE inputs_0_7_flags2;
#define TST_START_STATE             inputs_0_7_flags2.b0
#define TST_STOP_STATE              inputs_0_7_flags2.b1
#define TST_RESET_STATE             inputs_0_7_flags2.b2
#define TST_PG_LOAD_STATE           inputs_0_7_flags2.b3
#define TST_PG_DN_STATE             inputs_0_7_flags2.b4
#define TST_DOSER1_SPEED_UP_STATE   inputs_0_7_flags2.b5
#define TST_PG_UP_STATE             inputs_0_7_flags2.b6
#define TST_PG_SAVE_STATE           inputs_0_7_flags2.b7
//
//----------------------------------------------- inputs 9 to 16
//
BYTE inputs_8_15;
#define TST_DOSER1_SPEED_DN         inputs_8_15.b0
#define TST_FEEDER1_ON              inputs_8_15.b1
#define TST_DOSER2_SPEED_UP         inputs_8_15.b2
#define TST_DOSER2_SPEED_DN         inputs_8_15.b3
#define TST_FEEDER2_ON              inputs_8_15.b4
#define TST_DOSER3_SPEED_UP         inputs_8_15.b5
#define TST_DOSER3_SPEED_DN         inputs_8_15.b6
#define TST_FEEDER3_ON              inputs_8_15.b7
//
//
//
BYTE inputs_8_15_flags1;
#define OLD_TST_DOSER1_SPEED_DN     inputs_8_15_flags1.b0
#define OLD_TST_FEEDER1_ON          inputs_8_15_flags1.b1
#define OLD_TST_DOSER2_SPEED_UP     inputs_8_15_flags1.b2
#define OLD_TST_DOSER2_SPEED_DN     inputs_8_15_flags1.b3
#define OLD_TST_FEEDER2_ON          inputs_8_15_flags1.b4
#define OLD_TST_DOSER3_SPEED_UP     inputs_8_15_flags1.b5
#define OLD_TST_DOSER3_SPEED_DN     inputs_8_15_flags1.b6
#define OLD_TST_FEEDER3_ON          inputs_8_15_flags1.b7
//
//
//
BYTE inputs_8_15_flags2;
#define TST_DOSER1_SPEED_DN_STATE   inputs_8_15_flags2.b0
#define TST_FEEDER1_ON_STATE        inputs_8_15_flags2.b1
#define TST_DOSER2_SPEED_UP_STATE   inputs_8_15_flags2.b2
#define TST_DOSER2_SPEED_DN_STATE   inputs_8_15_flags2.b3
#define TST_FEEDER2_ON_STATE        inputs_8_15_flags2.b4
#define TST_DOSER3_SPEED_UP_STATE   inputs_8_15_flags2.b5
#define TST_DOSER3_SPEED_DN_STATE   inputs_8_15_flags2.b6
#define TST_FEEDER3_ON_STATE        inputs_8_15_flags2.b7
//
//----------------------------------------------- inputs 17 to 24
//
BYTE inputs_16_23;
#define TST_DOSER4_SPEED_UP         inputs_16_23.b0
#define TST_DOSER4_SPEED_DN         inputs_16_23.b1
#define TST_FEEDER4_ON              inputs_16_23.b2
#define TST_MIKSER_RUN              inputs_16_23.b3
#define SW_CLEANING_MODE            inputs_16_23.b4
#define SW_DOSER1_MANUAL            inputs_16_23.b5
#define SW_DOSER1_AUTO              inputs_16_23.b6
#define SW_DOSER2_MANUAL            inputs_16_23.b7
//
//
//
BYTE inputs_16_23_flags1;
#define OLD_TST_DOSER4_SPEED_UP     inputs_16_23_flags1.b0
#define OLD_TST_DOSER4_SPEED_DN     inputs_16_23_flags1.b1
#define OLD_TST_FEEDER4_ON          inputs_16_23_flags1.b2
#define OLD_TST_MIKSER_RUN          inputs_16_23_flags1.b3
//
//
//
BYTE inputs_16_23_flags2;
#define TST_DOSER4_SPEED_UP_STATE   inputs_16_23_flags2.b0
#define TST_DOSER4_SPEED_DN_STATE   inputs_16_23_flags2.b1
#define TST_FEEDER4_ON_STATE        inputs_16_23_flags2.b2
#define TST_MIKSER_RUN_STATE        inputs_16_23_flags2.b3
#define SW_CLEANING_MODE_STATE      inputs_16_23_flags2.b4
#define SW_DOSER1_MANUAL_STATE      inputs_16_23_flags2.b5
#define SW_DOSER1_AUTO_STATE        inputs_16_23_flags2.b6
#define SW_DOSER2_MANUAL_STATE      inputs_16_23_flags2.b7
//
//----------------------------------------------- inputs 25 to 32
//
BYTE inputs_24_31;
#define SW_DOSER2_AUTO              inputs_24_31.b0
#define SW_DOSER3_MANUAL            inputs_24_31.b1
#define SW_DOSER3_AUTO              inputs_24_31.b2
#define SW_DOSER4_MANUAL            inputs_24_31.b3
#define SW_DOSER4_AUTO              inputs_24_31.b4
#define TST_VACUM_PUMP1_ON          inputs_24_31.b5
#define TST_VACUM_PUMP2_ON          inputs_24_31.b6
#define TST_VACUM_PUMP3_ON          inputs_24_31.b7
//
//
//
BYTE inputs_24_31_flags1;
#define OLD_TST_VACUM_PUMP1_ON      inputs_24_31_flags1.b5
#define OLD_TST_VACUM_PUMP2_ON      inputs_24_31_flags1.b6
#define OLD_TST_VACUM_PUMP3_ON      inputs_24_31_flags1.b7
//
//
//
BYTE inputs_24_31_flags2;
#define SW_DOSER2_AUTO_STATE        inputs_24_31_flags2.b0
#define SW_DOSER3_MANUAL_STATE      inputs_24_31_flags2.b1
#define SW_DOSER3_AUTO_STATE        inputs_24_31_flags2.b2
#define SW_DOSER4_MANUAL_STATE      inputs_24_31_flags2.b3
#define SW_DOSER4_AUTO_STATE        inputs_24_31_flags2.b4
#define TST_VACUM_PUMP1_ON_STATE    inputs_24_31_flags2.b5
#define TST_VACUM_PUMP2_ON_STATE    inputs_24_31_flags2.b6
#define TST_VACUM_PUMP3_ON_STATE    inputs_24_31_flags2.b7
//
//----------------------------------------------- shift 1 register byte selector
//
BYTE shift1;
#define SIG1_LATCH                  shift1.b0
#define SIG2_LATCH                  shift1.b1
#define SIG3_LATCH                  shift1.b2
#define SIG4_LATCH                  shift1.b3
#define SIG5_LATCH                  shift1.b4
#define ERR1_LATCH                  shift1.b5
#define SKC_STATUS_LED              shift1.b6
//
//----------------------------------------------- shift 2 register byte selector
//
BYTE shift2;
#define INPUT_1_8                   shift2.b0
#define INPUT_9_16                  shift2.b1
#define INPUT_17_24                 shift2.b2
#define INPUT_25_32                 shift2.b3
//
//
/********* R S 4 8 5  C O M U N I C A T I O N   D E F I N E S   ***************/
//
//
//----------------------------------------------- comm flags
//
BYTE comm_flags;
#define RX_DATA_RDY                 comm_flags.b7
#define TX_DATA_RDY                 comm_flags.b6
#define RX_PENDING                  comm_flags.b5
#define TX_PENDING                  comm_flags.b4
#define MSG_CKSM_ERR                comm_flags.b3
#define MSG_TRANSFER_OK             comm_flags.b2
//
//
//
#define DataDirRX()         (COMM_DATA_DIR = PIN_LOW, TRISCbits.TRISC2 = OUTPUT_PIN)
#define DataDirTX()         (COMM_DATA_DIR = PIN_HIGH, TRISCbits.TRISC2 = OUTPUT_PIN)
//
//
//
#define NUL     0x00        // null character
#define EOT     0x04        // end of transmission
#define ACK     0x06        // acknowledge
#define NAK     0x15        // negative acknowledge
#define LF      0x0a        // line feed
#define CR      0x0d        // carriage return
#define ESC     0x1b        // escape
#define GET_CMD 0x3c        // '<'
#define SET_CMD 0x3e        // '>'
//
//----------------------------------------------- set message format
//
#define RX_DESTINATION_ADDRESS      rx_buff[0]
#define RX_SENDER_ADDRESS           rx_buff[1]
#define RX_MESSAGE_TYPE             rx_buff[2]
#define RX_GET_MSG_CHECKSUM         rx_buff[3]
#define RX_SET_SIGNAL1              rx_buff[3]
#define RX_SET_SIGNAL2              rx_buff[4]
#define RX_SET_SIGNAL3              rx_buff[5]
#define RX_SET_SIGNAL4              rx_buff[6]
#define RX_SET_SIGNAL5              rx_buff[7]
#define RX_SET_ERROR_DSP            rx_buff[8]
#define RX_SET_DSP1_DIG100          rx_buff[9]
#define RX_SET_DSP1_DIG10           rx_buff[10]
#define RX_SET_DSP1_DIG1            rx_buff[11]
#define RX_SET_DSP2_DIG100          rx_buff[12]
#define RX_SET_DSP2_DIG10           rx_buff[13]
#define RX_SET_DSP2_DIG1            rx_buff[14]
#define RX_SET_DSP3_DIG100          rx_buff[15]
#define RX_SET_DSP3_DIG10           rx_buff[16]
#define RX_SET_DSP3_DIG1            rx_buff[17]
#define RX_SET_DSP4_DIG100          rx_buff[18]
#define RX_SET_DSP4_DIG10           rx_buff[19]
#define RX_SET_DSP4_DIG1            rx_buff[20]
#define RX_SET_DSP5_DIG100          rx_buff[21]
#define RX_SET_DSP5_DIG10           rx_buff[22]
#define RX_SET_DSP5_DIG1            rx_buff[23]
#define RX_SET_MSG_CHECKSUM         rx_buff[24]
//#define RX_SET_DSP_DECIMAL1       rx_buff[24]
//#define RX_SET_DSP_DECIMAL2       rx_buff[25]
//#define RX_SET_MSG_CHECKSUM       rx_buff[26]
//
//----------------------------------------------- get message format
//
#define TX_DESTINATION_ADDRESS      tx_buff[0]
#define TX_SENDER_ADDRESS           tx_buff[1]
#define TX_MESSAGE_TYPE             tx_buff[2]
#define TX_INPUT_0_7_STATE          tx_buff[3]
#define TX_NAK_MSG_CHKSUM           tx_buff[3]
#define TX_ACK_MSG_CHKSUM           tx_buff[3]
#define TX_INPUT_8_15_STATE         tx_buff[4]
#define TX_INPUT_16_23_STATE        tx_buff[5]
#define TX_INPUT_24_31_STATE        tx_buff[6]
#define TX_GET_MSG_CHKSUM           tx_buff[7]
//
//----------------------------------------------- message specifics
//
#define DOSER1              0x01
#define DOSER2              0x02
#define DOSER3              0x03
#define DOSER4              0x04
//
//
/********* M A C R O S   ******************************************************/
//
//
#define SetShift1Latch()        (SH1_LATCH = HIGH)
#define ResetShift1Latch()      (SH1_LATCH = LOW)
#define SetShift1OutputEnable() (SH1_OE = LOW)
#define ResetShift1OutputEnable()   (SH1_OE = HIGH)
#define SetShift2latch()        (SH2_LATCH = HIGH)
#define ResetShift2Latch()      (SH2_LATCH = LOW)
#define ClearSignal1()          (shift1._byte = 0x00)
#define SelectSignal1()         (shift1._byte |= 0x01)
#define DeselectSignal1()       (shift1._byte &= 0b11111110)
#define SelectSignal2()         (shift1._byte |= 0x02)
#define DeselectSignal2()       (shift1._byte &= 0b11111101)
#define SelectSignal3()         (shift1._byte |= 0x04)
#define DeselectSignal3()       (shift1._byte &= 0b11111011)
#define SelectSignal4()         (shift1._byte |= 0x08)
#define DeselectSignal4()       (shift1._byte &= 0b11110111)
#define SelectSignal5()         (shift1._byte |= 0x10)
#define DeselectSignal5()       (shift1._byte &= 0b11101111)
#define SelectSignalError1()    (shift1._byte |= 0x20)
#define DeselectSignalError1()  (shift1._byte &= 0b11011111)
#define SelectStatusLED()       (shift1._byte &= 0b10111111)
#define DeselectStatusLED()     (shift1._byte |= 0x40)
#define SetDisplay1Latch()      (DISP1_LATCH = HIGH)
#define ResetDisplay1Latch()    (DISP1_LATCH = LOW)
#define SetDisplay2Latch()      (DISP2_LATCH = HIGH)
#define ResetDisplay2Latch()    (DISP2_LATCH = LOW)
#define SetDisplay3Latch()      (DISP3_LATCH = HIGH)
#define ResetDisplay3Latch()    (DISP3_LATCH = LOW)
#define SetDisplay4Latch()      (DISP4_LATCH = HIGH)
#define ResetDisplay4Latch()    (DISP4_LATCH = LOW)
#define SetDisplay5Latch()      (DISP5_LATCH = HIGH)
#define ResetDisplay5Latch()    (DISP5_LATCH = LOW)
#define DisplayDigit100()       (LATA &= 0b00011111, LATA |= 0b00100000)
#define DisplayDigit10()        (LATA &= 0b00011111, LATA |= 0b01000000)
#define DisplayDigit1()         (LATA &= 0b00011111, LATA |= 0b10000000)
#define DisplayOff()            (LATA &= 0b00011111)
#define ReadInputs_1_8()        (ResetShift2Latch(), shift2._byte = 0b11111110, WriteSPI(shift2._byte), SetShift2latch(), inputs_1_8._byte = DATA_PORT)
#define Readinputs_8_15()       (ResetShift2Latch(), shift2._byte = 0b11111101, WriteSPI(shift2._byte), SetShift2latch(), inputs_8_15._byte = DATA_PORT)
#define Readinputs_16_23()      (ResetShift2Latch(), shift2._byte = 0b11111011, WriteSPI(shift2._byte), SetShift2latch(), inputs_16_23._byte = DATA_PORT)
#define Readinputs_24_31()      (ResetShift2Latch(), shift2._byte = 0b11110111, WriteSPI(shift2._byte), SetShift2latch(), inputs_24_31._byte = DATA_PORT)
#define DisplayUnits_0()        (error_dsp._byte &= 0xf0)
#define DisplayUnits_1()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x01)
#define DisplayUnits_2()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x02)
#define DisplayUnits_3()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x03)
#define DisplayUnits_4()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x04)
#define DisplayUnits_5()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x05)
#define DisplayUnits_6()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x06)
#define DisplayUnits_7()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x07)
#define DisplayUnits_8()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x08)
#define DisplayUnits_9()        (error_dsp._byte &= 0xf0, error_dsp._byte |= 0x09)
#define DisplayTens_0()         (error_dsp._byte &= 0x0f)
#define DisplayTens_1()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x10)
#define DisplayTens_2()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x20)
#define DisplayTens_3()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x30)
#define DisplayTens_4()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x40)
#define DisplayTens_5()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x50)
#define DisplayTens_6()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x60)
#define DisplayTens_7()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x70)
#define DisplayTens_8()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x80)
#define DisplayTens_9()         (error_dsp._byte &= 0x0f, error_dsp._byte |= 0x90)
#define SetTasterStartPressed()             (TST_START_STATE = TRUE)
#define ResetTasterStartPressed()           (TST_START_STATE = FALSE)
#define IsTasterStartPressed()              (TST_START_STATE == TRUE)
#define SetTasterStopPressed()              (TST_STOP_STATE = TRUE)
#define ResetTasterStopPressed()            (TST_STOP_STATE = FALSE)
#define IsTasterStopPressed()               (TST_STOP_STATE == TRUE)
#define SetTasterResetPressed()             (TST_RESET_STATE = TRUE)
#define ResetTasterResetPressed()           (TST_RESET_STATE = FALSE)
#define IsTasterResetPressed()              (TST_RESET_STATE == TRUE)
#define SetTasterProgramLoadPressed()       (TST_PG_LOAD_STATE = TRUE)
#define ResetTasterProgramLoadPressed()     (TST_PG_LOAD_STATE = FALSE)
#define IsTasterProgramLoadPressed()        (TST_PG_LOAD_STATE == TRUE)
#define SetTasterProgramSavePressed()       (TST_PG_SAVE_STATE = TRUE)
#define ResetTasterProgramSavePressed()     (TST_PG_SAVE_STATE = FALSE)
#define IsTasterProgramSavePressed()        (TST_PG_SAVE_STATE == TRUE)
#define SetTasterProgramUpPressed()         (TST_PG_UP_STATE = TRUE)
#define ResetTasterProgramUpPressed()       (TST_PG_UP_STATE = FALSE)
#define IsTasterProgramUpPressed()          (TST_PG_UP_STATE == TRUE)
#define SetTasterProgramDownPressed()       (TST_PG_DN_STATE = TRUE)
#define ResetTasterProgramDownPressed()     (TST_PG_DN_STATE = FALSE)
#define IsTasterProgramDownPressed()        (TST_PG_DN_STATE == TRUE)
#define SetTasterDoser1SpeedUpPressed()     (TST_DOSER1_SPEED_UP_STATE = TRUE)
#define ResetTasterDoser1SpeedUpPressed()   (TST_DOSER1_SPEED_UP_STATE = FALSE)
#define IsTasterDoser1SpeedUpPressed()      (TST_DOSER1_SPEED_UP_STATE == TRUE)
#define SetTasterDoser1SpeedDownPressed()   (TST_DOSER1_SPEED_DN_STATE = TRUE)
#define ResetTasterDoser1SpeedDownPressed() (TST_DOSER1_SPEED_DN_STATE = FALSE)
#define IsTasterDoser1SpeedDownPressed()    (TST_DOSER1_SPEED_DN_STATE == TRUE)
#define SetTasterFeeder1OnPressed()         (TST_FEEDER1_ON_STATE = TRUE)
#define ResetTasterFeeder1OnPressed()       (TST_FEEDER1_ON_STATE = FALSE)
#define IsTasterFeeder1OnPressed()          (TST_FEEDER1_ON_STATE = TRUE)
#define SetTasterDoser2SpeedUpPressed()     (TST_DOSER2_SPEED_UP_STATE = TRUE)
#define ResetTasterDoser2SpeedUpPressed()   (TST_DOSER2_SPEED_UP_STATE = FALSE)
#define IsTasterDoser2SpeedUpPressed()      (TST_DOSER2_SPEED_UP_STATE == TRUE)
#define SetTasterDoser2SpeedDownPressed()   (TST_DOSER2_SPEED_DN_STATE = TRUE)
#define ResetTasterDoser2SpeedDownPressed() (TST_DOSER2_SPEED_DN_STATE = FALSE)
#define IsTasterDoser2SpeedDownPressed()    (TST_DOSER2_SPEED_DN_STATE == TRUE)
#define SetTasterFeeder2OnPressed()         (TST_FEEDER2_ON_STATE = TRUE)
#define ResetTasterFeeder2OnPressed()       (TST_FEEDER2_ON_STATE = FALSE)
#define IsTasterFeeder2OnPressed()          (TST_FEEDER2_ON_STATE = TRUE)
#define SetTasterDoser3SpeedUpPressed()     (TST_DOSER3_SPEED_UP_STATE = TRUE)
#define ResetTasterDoser3SpeedUpPressed()   (TST_DOSER3_SPEED_UP_STATE = FALSE)
#define IsTasterDoser3SpeedUpPressed()      (TST_DOSER3_SPEED_UP_STATE == TRUE)
#define SetTasterDoser3SpeedDownPressed()   (TST_DOSER3_SPEED_DN_STATE = TRUE)
#define ResetTasterDoser3SpeedDownPressed() (TST_DOSER3_SPEED_DN_STATE = FALSE)
#define IsTasterDoser3SpeedDownPressed()    (TST_DOSER3_SPEED_DN_STATE == TRUE)
#define SetTasterFeeder3OnPressed()         (TST_FEEDER3_ON_STATE = TRUE)
#define ResetTasterFeeder3OnPressed()       (TST_FEEDER3_ON_STATE = FALSE)
#define IsTasterFeeder3OnPressed()          (TST_FEEDER3_ON_STATE = TRUE)
#define SetTasterDoser4SpeedUpPressed()     (TST_DOSER4_SPEED_UP_STATE = TRUE)
#define ResetTasterDoser4SpeedUpPressed()   (TST_DOSER4_SPEED_UP_STATE = FALSE)
#define IsTasterDoser4SpeedUpPressed()      (TST_DOSER4_SPEED_UP_STATE == TRUE)
#define SetTasterDoser4SpeedDownPressed()   (TST_DOSER4_SPEED_DN_STATE = TRUE)
#define ResetTasterDoser4SpeedDownPressed() (TST_DOSER4_SPEED_DN_STATE = FALSE)
#define IsTasterDoser4SpeedDownPressed()    (TST_DOSER4_SPEED_DN_STATE = TRUE)
#define SetTasterFeeder4OnPressed()         (TST_FEEDER4_ON_STATE = TRUE)
#define ResetTasterFeeder4OnPressed()       (TST_FEEDER4_ON_STATE = FALSE)
#define IsTasterFeeder4OnPressed()          (TST_FEEDER4_ON_STATE == TRUE)
#define SetTasterMikserRunPressed()         (TST_MIKSER_RUN_STATE = TRUE)
#define ResetTasterMikserRunPressed()       (TST_MIKSER_RUN_STATE = FALSE)
#define IsTasterMikserRunPressed()          (TST_MIKSER_RUN_STATE == TRUE)
#define IsSwCleaningModeOn()                (SW_CLEANING_MODE == LOW)
#define IsSwDoser1ManualOn()                ((SW_DOSER1_MANUAL == LOW) && (SW_DOSER1_AUTO == HIGH))
#define IsSwDoser1AutoOn()                  ((SW_DOSER1_AUTO == LOW) && (SW_DOSER1_MANUAL == HIGH))
#define IsSwDoser2ManualOn()                ((SW_DOSER2_MANUAL == LOW) && (SW_DOSER2_AUTO == HIGH))
#define IsSwDoser2AutoOn()                  ((SW_DOSER2_AUTO == LOW) && (SW_DOSER2_MANUAL == HIGH))
#define IsSwDoser3ManualOn()                ((SW_DOSER3_MANUAL == LOW) && (SW_DOSER2_AUTO == HIGH))
#define IsSwDoser3AutoOn()                  ((SW_DOSER3_AUTO == LOW) && (SW_DOSER3_MANUAL == HIGH))
#define IsSwDoser4ManualOn()                ((SW_DOSER4_MANUAL == LOW) && (SW_DOSER4_AUTO == HIGH))
#define IsSwDoser4AutoOn()                  ((SW_DOSER4_AUTO == LOW) && (SW_DOSER4_MANUAL == HIGH))
#define SetSignalFeeder3On()                (SIG_FEEDER3_ON = HIGH)
#define ResetSignalFeeder3On()              (SIG_FEEDER3_ON = LOW)
#define IsSignalFeeder3OnActiv()            (SIG_FEEDER3_ON == HIGH)
#define SetSignalFeeder1On()                (SIG_FEEDER1_ON = HIGH)
#define ResetSignalFeeder1On()              (SIG_FEEDER1_ON = LOW)
#define IsSignalFeeder1OnActiv()            (SIG_FEEDER1_ON == HIGH)
#define SetSignalDoser4RunAuto()            (SIG_DOS4_RUN_AUTO = HIGH)
#define ResetSignalDoser4RunAuto()          (SIG_DOS4_RUN_AUTO = LOW)
#define IsSignalDoser4RunAutoActiv()        (SIG_DOS4_RUN_AUTO == HIGH)
#define SetSignalDoser3RunAuto()            (SIG_DOS3_RUN_AUTO = HIGH)
#define ResetSignalDoser3RunAuto()          (SIG_DOS3_RUN_AUTO = LOW)
#define IsSignalDoser3RunAutoActiv()        (SIG_DOS3_RUN_AUTO == HIGH)
#define SetSignalDoser2RunAuto()            (SIG_DOS2_RUN_AUTO = HIGH)
#define ResetSignalDoser2RunAuto()          (SIG_DOS2_RUN_AUTO = LOW)
#define IsSignalDoser2RunAutoActiv()        (SIG_DOS2_RUN_AUTO == HIGH)
#define SetSignalDoser1RunAuto()            (SIG_DOS1_RUN_AUTO = HIGH)
#define ResetSignalDoser1RunAuto()          (SIG_DOS1_RUN_AUTO = LOW)
#define IsSignalDoser1RunAutoActiv()        (SIG_DOS1_RUN_AUTO == HIGH)
#define SetSignalCleaningMode()             (SIG_CLEANING_MODE = HIGH)
#define ResetSignalCleaningMode()           (SIG_CLEANING_MODE = LOW)
#define IsSignalCleaningModeActiv()         (SIG_CLEANING_MODE == HIGH)
#define SetSignaStart()                     (SIG_FEEDER3_ON = HIGH)
#define ResetSignalStart()                  (SIG_FEEDER3_ON = LOW)
#define IsSignalStartActiv()                (SIG_FEEDER3_ON == HIGH)
#define SetSignalStop()                     (SIG_STOP = HIGH)
#define ResetSignalStop()                   (SIG_STOP = LOW)
#define IsSignalStopActiv()                 (SIG_STOP == HIGH)
#define SetSignalAlarm()                    (SIG_ALARM = HIGH)
#define ResetSignalAlarm()                  (SIG_ALARM = LOW)
#define IsSignalAlarmAciv()                 (SIG_ALARM == HIGH)
#define SetSignalDoser1RunManual()          (SIG_DOS1_RUN_MANUAL = HIGH)
#define ResetSignalDoser1RunManual()        (SIG_DOS1_RUN_MANUAL = LOW)
#define IsSignalDoser1RunManualActiv()      (SIG_DOS1_RUN_MANUAL == HIGH)
#define SetSignalDoser2RunManual()          (SIG_DOS2_RUN_MANUAL = HIGH)
#define ResetSignalDoser2RunManual()        (SIG_DOS2_RUN_MANUAL = LOW)
#define IsSignalDoser2RunManualActiv()      (SIG_DOS2_RUN_MANUAL == HIGH)
#define SetSignalDoser3RunManual()          (SIG_DOS3_RUN_MANUAL = HIGH)
#define ResetSignalDoser3RunManual()        (SIG_DOS3_RUN_MANUAL = LOW)
#define IsSignalDoser3RunManualActiv()      (SIG_DOS3_RUN_MANUAL == HIGH)
#define SetSignalDoser4RunManual()          (SIG_DOS4_RUN_MANUAL = HIGH)
#define ResetSignalDoser4RunManual()        (SIG_DOS4_RUN_MANUAL = LOW)
#define IsSignalDoser4RunManualActiv()      (SIG_DOS4_RUN_MANUAL == HIGH)
#define SetSignalFeeder2On()                (SIG_FEEDER2_ON = HIGH)
#define ResetSignalFeeder2On()              (SIG_FEEDER2_ON = LOW)
#define IsSignalFeeder2OnActiv()            (SIG_FEEDER2_ON == HIGH)
#define SetSignalFeeder4On()                (SIG_FEEDER4_ON = HIGH)
#define ResetSignalFeeder4On()              (SIG_FEEDER4_ON = LOW)
#define IsSignalFeeder4OnActiv()            (SIG_FEEDER4_ON == HIGH)
#define SetSignalFeeder1Full()              (SIG_FEEDER1_FULL = HIGH)
#define ResetSignalFeeder1Full()            (SIG_FEEDER1_FULL = LOW)
#define IsSignalFeeder1FullActiv()          (SIG_FEEDER1_FULL == HIGH)
#define SetSignalFeeder1Min()               (SIG_FEEDER1_MIN = HIGH)
#define ResetSignalFeeder1Min()             (SIG_FEEDER1_MIN = LOW)
#define IsSignalFeeder1MinActiv()           (SIG_FEEDER1_MIN == HIGH)
#define SetSignalFeeder2Full()              (SIG_FEEDER2_FULL = HIGH)
#define ResetSignalFeeder2Full()            (SIG_FEEDER2_FULL = LOW)
#define IsSignalFeeder2FullActiv()          (SIG_FEEDER2_FULL == HIGH)
#define SetSignalFeeder2Min()               (SIG_FEEDER2_MIN = HIGH)
#define ResetSignalFeeder2Min()             (SIG_FEEDER2_MIN = LOW)
#define IsSignalFeeder2MinActiv()           (SIG_FEEDER2_MIN == HIGH)
#define SetSignalFeeder3Full()              (SIG_FEEDER3_FULL = HIGH)
#define ResetSignalFeeder3Full()            (SIG_FEEDER3_FULL = LOW)
#define IsSignalFeeder3FullActiv()          (SIG_FEEDER3_FULL == HIGH)
#define SetSignalFeeder3Min()               (SIG_FEEDER3_MIN = HIGH)
#define ResetSignalFeeder3Min()             (SIG_FEEDER3_MIN = LOW)
#define IsSignalFeeder3MinActiv()           (SIG_FEEDER3_MIN == HIGH)
#define SetSignalFeeder4Full()              (SIG_FEEDER4_FULL = HIGH)
#define ResetSignalFeeder4Full()            (SIG_FEEDER4_FULL = LOW)
#define IsSignalFeeder4FullActiv()          (SIG_FEEDER4_FULL == HIGH)
#define SetSignalFeeder4Min()               (SIG_FEEDER4_MIN = HIGH)
#define ResetSignalFeeder4Min()             (SIG_FEEDER4_MIN = LOW)
#define IsSignalFeeder4MinActiv()           (SIG_FEEDER4_MIN == HIGH)
#define SetSignalMikserFull()               (SIG_MIKSER_FULL = HIGH)
#define ResetSignalMikserFull()             (SIG_MIKSER_FULL = LOW)
#define IsSignalMikserFullActiv()           (SIG_MIKSER_FULL == HIGH)
#define SetSignalEkstruderRun()             (SIG_EKSTRUDER_RUN = HIGH)
#define ResetSignalEkstruderRun()           (SIG_EKSTRUDER_RUN = LOW)
#define IsSignalEkstruderRunActiv()         (SIG_EKSTRUDER_RUN == HIGH)
#define SetSignalVacumPump2Run()            (SIG_VACUM_PUMP2_RUN = HIGH)
#define ResetSignalvacumPump2Run()          (SIG_VACUM_PUMP2_RUN = LOW)
#define IsSignalvacumPump2RunActiv()        (SIG_VACUM_PUMP2_RUN == HIGH)
#define SetSignalVacumPump4Run()            (SIG_VACUM_PUMP4_RUN = HIGH)
#define ResetSignalvacumPump4Run()          (SIG_VACUM_PUMP4_RUN = LOW)
#define IsSignalvacumPump4RunActiv()        (SIG_VACUM_PUMP4_RUN == HIGH)
#define SetSignalVacumPump3Run()            (SIG_VACUM_PUMP3_RUN = HIGH)
#define ResetSignalvacumPump3Run()          (SIG_VACUM_PUMP3_RUN = LOW)
#define IsSignalvacumPump3RunActiv()        (SIG_VACUM_PUMP3_RUN == HIGH)
#define SetSignalVacumPump1Run()            (SIG_VACUM_PUMP1_RUN = HIGH)
#define ResetSignalvacumPump1Run()          (SIG_VACUM_PUMP1_RUN = LOW)
#define IsSignalvacumPump1RunActiv()        (SIG_VACUM_PUMP1_RUN == HIGH)
#define SetSignalMikserMin()                (SIG_MIKSER_MIN = HIGH)
#define ResetSignalMikserMin()              (SIG_MIKSER_MIN = LOW)
#define IsSignalMikserMinActiv()            (SIG_MIKSER_MIN == HIGH)
#define SetSignalMikserRun()                (SIG_MIKSER_RUN = HIGH)
#define ResetSignalMikserRun()              (SIG_MIKSER_RUN = LOW)
#define IsSignalMikserRunActiv()            (SIG_MIKSER_RUN == HIGH)
#define SignalFeeder4Request()              (SIG_FEEDER4_REQUEST = HIGH)
#define ResetFeeder4Request()               (SIG_FEEDER4_REQUEST = LOW)
#define IsFeeder4RequestActiv()             (SIG_FEEDER4_REQUEST == HIGH)
#define SetSignalFeeder4Vacum()             (SIG_FEEDER4_VACUM = HIGH)
#define ResetSignalFeeder4Vacum()           (SIG_FEEDER4_VACUM = LOW)
#define IsSignalFeeder4VacumActiv()         (SIG_FEEDER4_VACUM == HIGH)
#define SignalFeeder3Request()              (SIG_FEEDER3_REQUEST = HIGH)
#define ResetFeeder3Request()               (SIG_FEEDER3_REQUEST = LOW)
#define IsFeeder3RequestActiv()             (SIG_FEEDER3_REQUEST == HIGH)
#define SetSignalFeeder3Vacum()             (SIG_FEEDER3_VACUM = HIGH)
#define ResetSignalFeeder3Vacum()           (SIG_FEEDER3_VACUM = LOW)
#define IsSignalFeeder3VacumActiv()         (SIG_FEEDER3_VACUM == HIGH)
#define SignalFeeder2Request()              (SIG_FEEDER2_REQUEST = HIGH)
#define ResetFeeder2Request()               (SIG_FEEDER2_REQUEST = LOW)
#define IsFeeder2RequestActiv()             (SIG_FEEDER2_REQUEST == HIGH)
#define SetSignalFeeder2Vacum()             (SIG_FEEDER2_VACUM = HIGH)
#define ResetSignalFeeder2Vacum()           (SIG_FEEDER2_VACUM = LOW)
#define IsSignalFeeder2VacumActiv()         (SIG_FEEDER2_VACUM == HIGH)
#define SignalFeeder1Request()              (SIG_FEEDER1_REQUEST = HIGH)
#define ResetFeeder1Request()               (SIG_FEEDER1_REQUEST = LOW)
#define IsFeeder1RequestActiv()             (SIG_FEEDER1_REQUEST == HIGH)
#define SetSignalFeeder1Vacum()             (SIG_FEEDER1_VACUM = HIGH)
#define ResetSignalFeeder1Vacum()           (SIG_FEEDER1_VACUM = LOW)
#define IsSignalFeeder1VacumActiv()         (SIG_FEEDER1_VACUM == HIGH)
#define SetVacumPump1OnTasterPressed()      (TST_VACUM_PUMP1_ON_STATE = TRUE)
#define ResetVacumPump1OnTasterPressed()    (TST_VACUM_PUMP1_ON_STATE = FALSE)
#define IsVacumPump1OnTasterPressed()       (TST_VACUM_PUMP1_ON_STATE == TRUE)
#define SetVacumPump2OnTasterPressed()      (TST_VACUM_PUMP2_ON_STATE = TRUE)
#define ResetVacumPump2OnTasterPressed()    (TST_VACUM_PUMP2_ON_STATE = FALSE)
#define IsVacumPump2OnTasterPressed()       (TST_VACUM_PUMP2_ON_STATE == TRUE)
#define SetVacumPump3OnTasterPressed()      (TST_VACUM_PUMP3_ON_STATE = TRUE)
#define ResetVacumPump3OnTasterPressed()    (TST_VACUM_PUMP3_ON_STATE = FALSE)
#define IsVacumPump3OnTasterPressed()       (TST_VACUM_PUMP3_ON_STATE == TRUE)
//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void low_isr(void);
void high_isr(void);
void InitRAM(void);
void InitSYS(void);
void IO_Service(void);
void CommService(void);
void SignalService(void);
void ErrorService(void);
void DisplayService(void);
unsigned char CharTo7seg(unsigned char data);
unsigned char EEPROM_ReadByte(unsigned char address);
unsigned int EEPROM_ReadInt(unsigned char address);
void EEPROM_WriteByte(unsigned char address, unsigned char data);
void EEPROM_WriteInt(unsigned char address, unsigned int data);
//
//----------------------------------------------- end of file
//
#endif	/* MAIN_H */
 