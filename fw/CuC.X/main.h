/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Zaglavlje glavnog programa
 *
 *******************************************************************************
 * Ime fajla:       main.h
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

#ifndef MAIN_H
#define	MAIN_H
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
//#pragma udata doser_char_vars
extern unsigned char program_select, program_in_use;
extern unsigned char dsp1_unit, dsp2_unit, dsp3_unit, dsp4_unit;
extern unsigned char sys_svc_pcnt, dsp_svc_pcnt, comm_svc_pcnt;
extern unsigned char sig_svc_pcnt, sys_state_pcnt, timer_10_ms;
extern unsigned char comm_svc_pcnt, comm_cyc_cnt, skc_comm_address;
extern unsigned char tst_dos1_speed_up_tmr, tst_dos1_speed_dn_tmr;
extern unsigned char tst_dos2_speed_up_tmr, tst_dos2_speed_dn_tmr;
extern unsigned char tst_dos3_speed_up_tmr, tst_dos3_speed_dn_tmr;
extern unsigned char tst_dos4_speed_up_tmr, tst_dos4_speed_dn_tmr;
extern unsigned char tst_pgm_up_speed_up_tmr, tst_pgm_dn_speed_up_tmr;
extern unsigned char tst_dos1_speed_up_pcnt, tst_dos1_speed_dn_pcnt;
extern unsigned char tst_dos2_speed_up_pcnt, tst_dos2_speed_dn_pcnt;
extern unsigned char tst_dos3_speed_up_pcnt, tst_dos3_speed_dn_pcnt;
extern unsigned char tst_dos4_speed_up_pcnt, tst_dos4_speed_dn_pcnt;
extern unsigned char tst_pgm_up_speed_up_pcnt, tst_pgm_dn_speed_up_pcnt;
extern unsigned char char1, char10, char100, dsp_char_cnt;
extern unsigned char dsp1_dig100, dsp1_dig10, dsp1_dig1;
extern unsigned char dsp2_dig100, dsp2_dig10, dsp2_dig1;
extern unsigned char dsp3_dig100, dsp3_dig10, dsp3_dig1;
extern unsigned char dsp4_dig100, dsp4_dig10, dsp4_dig1;
extern unsigned char dsp5_dig100, dsp5_dig10, dsp5_dig1;
extern unsigned char err_dsp_cnt;
//
//
//
//#pragma udata feeder_int_vars
extern unsigned int fw_version, program_select_tmr;
extern unsigned int dsp_tmr, sig_tmr, mnu_cnt, err_dsp_tmr;
extern unsigned int dos1_ref, dos2_ref, dos3_ref, dos4_ref;
//
//
//
//#pragama udata doser_int_vars
//
//
//
/*********   F L A G S   ******************************************************/
//
extern BYTE sys_flags1;
#define OLD_TST_START               sys_flags1.b0
#define OLD_TST_STOP                sys_flags1.b1
#define OLD_TST_RESET               sys_flags1.b2
#define OLD_TST_PG_UP               sys_flags1.b3
#define OLD_TST_PG_DN               sys_flags1.b4
#define OLD_TST_PG_LOAD             sys_flags1.b5
#define OLD_TST_PG_SAVE             sys_flags1.b6
#define OLD_COMM_STATE              sys_flags1.b7

extern BYTE sys_flags2;
#define TST_START_STATE             sys_flags2.b0
#define TST_STOP_STATE              sys_flags2.b1
#define TST_RESET_STATE             sys_flags2.b2
#define TST_PG_UP_STATE             sys_flags2.b3
#define TST_PG_DN_STATE             sys_flags2.b4
#define TST_PG_LOAD_STATE           sys_flags2.b5
#define TST_PG_SAVE_STATE           sys_flags2.b6
//
extern BYTE sys_flags3;
#define OLD_TST_DOSER1_SPEED_UP     sys_flags3.b0
#define OLD_TST_DOSER1_SPEED_DN     sys_flags3.b1
#define OLD_TST_FEEDER1_ON          sys_flags3.b2
#define OLD_TST_DOSER2_SPEED_UP     sys_flags3.b3
#define OLD_TST_DOSER2_SPEED_DN     sys_flags3.b4
#define OLD_TST_FEEDER2_ON          sys_flags3.b5
#define OLD_TST_DOSER3_SPEED_UP     sys_flags3.b6
#define OLD_TST_DOSER3_SPEED_DN     sys_flags3.b7

extern BYTE sys_flags4;
#define TST_DOSER1_SPEED_UP_STATE   sys_flags4.b0
#define TST_DOSER1_SPEED_DN_STATE   sys_flags4.b1
#define TST_FEEDER1_ON_STATE        sys_flags4.b2
#define TST_DOSER2_SPEED_UP_STATE   sys_flags4.b3
#define TST_DOSER2_SPEED_DN_STATE   sys_flags4.b4
#define TST_FEEDER2_ON_STATE        sys_flags4.b5
#define TST_DOSER3_SPEED_UP_STATE   sys_flags4.b6
#define TST_DOSER3_SPEED_DN_STATE   sys_flags4.b7

extern BYTE sys_flags5;
#define OLD_TST_FEEDER3_ON          sys_flags5.b0
#define OLD_TST_DOSER4_SPEED_UP     sys_flags5.b1
#define OLD_TST_DOSER4_SPEED_DN     sys_flags5.b2
#define OLD_TST_FEEDER4_ON          sys_flags5.b3
#define OLD_TST_MIKSER_ON           sys_flags5.b4
#define OLD_TST_VP1_ON              sys_flags5.b5
#define OLD_TST_VP2_ON              sys_flags5.b6
#define OLD_TST_VP3_ON              sys_flags5.b7

extern BYTE sys_flags6;
#define TST_FEEDER3_ON_STATE        sys_flags6.b0
#define TST_DOSER4_SPEED_UP_STATE   sys_flags6.b1
#define TST_DOSER4_SPEED_DN_STATE   sys_flags6.b2
#define TST_FEEDER4_ON_STATE        sys_flags6.b3
#define TST_MIKSER_ON_STATE         sys_flags6.b4
#define TST_VP1_ON_STATE            sys_flags6.b5
#define TST_VP2_ON_STATE            sys_flags6.b6
#define TST_VP3_ON_STATE            sys_flags6.b7

extern BYTE sys_flags7;
#define TST_DOSER1_SPEED_UP_TEX     sys_flags7.b0
#define TST_DOSER1_SPEED_DN_TEX     sys_flags7.b1
#define TST_DOSER2_SPEED_UP_TEX     sys_flags7.b2
#define TST_DOSER2_SPEED_DN_TEX     sys_flags7.b3
#define TST_DOSER3_SPEED_UP_TEX     sys_flags7.b4
#define TST_DOSER3_SPEED_DN_TEX     sys_flags7.b5
#define TST_DOSER4_SPEED_UP_TEX     sys_flags7.b6
#define TST_DOSER4_SPEED_DN_TEX     sys_flags7.b7

extern BYTE sys_flags8;
#define CLOCK_10_MS                 sys_flags8.b0
#define DSP_PG_LOADED               sys_flags8.b1
#define DSP_PG_SAVED                sys_flags8.b2
#define DSP_TEX                     sys_flags8.b3
#define SIG_TEX                     sys_flags8.b4
#define SIG_SET                     sys_flags8.b5
#define EXTRUDER_OLD_STATE          sys_flags8.b6
#define EXTRUDER_STATE              sys_flags8.b7

extern BYTE sys_flags9;
#define SYS_TMR_FLAG                sys_flags9.b0
#define SETUP_DSP_VALUE             sys_flags9.b1
#define ERR_DSP_TEX                 sys_flags9.b2
#define PGM_SELECT_TEX              sys_flags9.b3
#define TST_PG_UP_TEX               sys_flags9.b4
#define TST_PG_DN_TEX               sys_flags9.b5
#define EMERGENCY_STATE             sys_flags9.b6
#define DSP_SYS_INIT                sys_flags9.b7

extern BYTE sys_flags10;
#define VIBRATOR_DISABLE            sys_flags10.b0
#define CLEANING_AIR_DISABLE        sys_flags10.b1
#define EEPROM_FLAG                 sys_flags10.b2

extern WORD sys_tick;
#define CLOCK_1_MS                  sys_tick.Byte0.b0
#define CLOCK_2_MS                  sys_tick.Byte0.b1
#define CLOCK_4_MS                  sys_tick.Byte0.b2
#define CLOCK_8_MS                  sys_tick.Byte0.b3
#define CLOCK_16_MS                 sys_tick.Byte0.b4
#define CLOCK_32_MS                 sys_tick.Byte0.b5
#define CLOCK_64_MS                 sys_tick.Byte0.b6
#define CLOCK_128_MS                sys_tick.Byte0.b7
#define CLOCK_256_MS                sys_tick.Byte1.b0
#define CLOCK_512_MS                sys_tick.Byte1.b1
#define CLOCK_1_S                   sys_tick.Byte1.b2
#define CLOCK_2_S                   sys_tick.Byte1.b3
#define CLOCK_4_S                   sys_tick.Byte1.b4
#define CLOCK_8_S                   sys_tick.Byte1.b5
#define CLOCK_16_S                  sys_tick.Byte1.b6
#define CLOCK_32_S                  sys_tick.Byte1.b7

extern DEVICE_STATE control1;
//
//
/**  E N U M E R A T O R S   **************************************************/
//
//
//
//----------------------------------------------- setup menu items

extern enum menu_items {
    PROG_DSP, SETUP_ENTER, VP1_DELAY_TMR, MIXER_STOP_DELAY_TMR, MIXER_RAW_MAX_ALARM_DELAY_TMR,
    MIXER_RAW_MAX_LOAD_DELAY_TMR, VIBRATOR_TMR, VIBRATOR_CYC, FEED1_CYC_TIME, FEED2_CYC_TIME, FEED3_CYC_TIME,
    FEED4_CYC_TIME, CYCLON_LEVEL_MAX_ALARM_DELAY_TMR, MILL_STOP_DELAY_TMR, CLEANING_AIR_CYC, CLEANING_AIR_TMR,
    TERM_COMM_ADDRESS
} menu_item;
//
//----------------------------------------------- system state enumerators

extern enum sys_states {
    SYS_STOP, SYS_RUN, SYS_ERROR
} sys_state;

//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void low_isr(void);
void high_isr(void);
void InitRAM(void);
void InitSYS(void);
void CheckSysTimers(void);
void IO_Service(void);
void CommService(void);
void SysService(void);
void ProcessService(void);
void SignalService(void);
void DisplayService(void);
void LoadProgram(void);
void SaveProgram(void);
void LoadSystemVariable(void);
void SaveSystemVariable(void);
void IntToChars(unsigned int data);
unsigned char I2C_EE_ReadByte(unsigned char address, unsigned char page);
unsigned int I2C_EE_ReadInt(unsigned char address, unsigned char page);
void I2C_EE_WriteByte(unsigned char address, unsigned char data, unsigned char page);
void I2C_EE_WriteInt(unsigned char address, unsigned int data, unsigned char page);

#endif	/* MAIN_H */

