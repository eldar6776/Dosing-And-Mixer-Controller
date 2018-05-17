/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                    Data processing header
 *
 *******************************************************************************
 * Ime fajla:       data_process.h
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

#ifndef PROCESS_CONTROL_H
#define	PROCESS_CONTROL_H
//
//
/** U K L J U C E N I  ********************************************************/
//
//


//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
//#pragma udata feeder_char_vars
extern unsigned char mixer_pcnt, fed1_pcnt, fed2_pcnt, fed3_pcnt, fed4_pcnt;
extern unsigned char vacum_pump1_pcnt, vacum_pump2_pcnt, vacum_pump3_pcnt;
extern unsigned char dos1_pcnt, dos2_pcnt, dos3_pcnt, dos4_pcnt;
extern unsigned char dp_tmp, cp_pcnt, clk_1s, mix_cyc_cnt, cyclon_pcnt;
//
//
//
extern unsigned int vibrator_tmr, vibrator_cyc, mill_stop_delay_tmr, cleaning_air_tmr, cleaning_air_cyc;
extern unsigned int vibrator_time, vibrator_cyc_cnt, mill_stop_delay_time, cleaning_air_time, cleaning_air_cyc_cnt;
extern unsigned int cyclon_alarm_delay_tmr, vp1_tmr, vp2_tmr, vp3_tmr, mixer_load_stop_tmr;
extern unsigned int cyclon_alarm_delay_time, vp1_delay_time, mixer_load_stop_time;
extern unsigned int feed1_cyc_tmr, feed2_cyc_tmr, feed3_cyc_tmr, feed4_cyc_tmr;
extern unsigned int feed1_cyc_time, feed2_cyc_time, feed3_cyc_time, feed4_cyc_time;
extern unsigned int control_tmr, vp2_delay_tmr, vp3_delay_tmr;
extern unsigned int control_time, vp2_delay_time, vp3_delay_time;
extern unsigned int mixer_stop_delay_tmr, mixer_load_stop_delay_tmr, mix_raw_max_alarm_delay_tmr;
extern unsigned int mixer_stop_delay_time, mixer_load_stop_delay_time, mix_raw_max_alarm_delay_time;
//
//
//#pragama udata doser_int_vars
//
//
//
/*********   F L A G S   ******************************************************/
//
//
extern BYTE control_flags1;
#define CONTROL_STATE           control_flags1.b0
#define CONTROL_ON              control_flags1.b1
#define CONTROL_TEX             control_flags1.b2
#define FEED1_CHANGE_TEX        control_flags1.b3
#define FEED2_CHANGE_TEX        control_flags1.b4
#define FEED3_CHANGE_TEX        control_flags1.b5
#define FEED4_CHANGE_TEX        control_flags1.b6

extern BYTE feeder1_flags1;
#define FEED1_TST_ON_STATE      feeder1_flags1.b0
#define FEED1_PREV_STATE        feeder1_flags1.b1
#define FEED1_CYC_TEX           feeder1_flags1.b2
#define FEED1_ALARM_RAW_MIN     feeder1_flags1.b3
#define FEED1_ALARM_CYC_FAIL    feeder1_flags1.b4
#define FEED1_ALARM_SENSOR      feeder1_flags1.b5
#define FEED1_FIRST_CYC         feeder1_flags1.b6


extern BYTE feeder2_flags1;
#define FEED2_TST_ON_STATE      feeder2_flags1.b0
#define FEED2_PREV_STATE        feeder2_flags1.b1
#define FEED2_CYC_TEX           feeder2_flags1.b2
#define FEED2_ALARM_RAW_MIN     feeder2_flags1.b3
#define FEED2_ALARM_CYC_FAIL    feeder2_flags1.b4
#define FEED2_ALARM_SENSOR      feeder2_flags1.b5
#define FEED2_FIRST_CYC         feeder2_flags1.b6

extern BYTE feeder3_flags1;
#define FEED3_TST_ON_STATE      feeder3_flags1.b0
#define FEED3_PREV_STATE        feeder3_flags1.b1
#define FEED3_CYC_TEX           feeder3_flags1.b2
#define FEED3_ALARM_RAW_MIN     feeder3_flags1.b3
#define FEED3_ALARM_CYC_FAIL    feeder3_flags1.b4
#define FEED3_ALARM_SENSOR      feeder3_flags1.b5
#define FEED3_FIRST_CYC         feeder3_flags1.b6

extern BYTE feeder4_flags1;
#define FEED4_TST_ON_STATE      feeder4_flags1.b0
#define FEED4_PREV_STATE        feeder4_flags1.b1
#define FEED4_CYC_TEX           feeder4_flags1.b2
#define FEED4_ALARM_RAW_MIN     feeder4_flags1.b3
#define FEED4_ALARM_CYC_FAIL    feeder4_flags1.b4
#define FEED4_ALARM_SENSOR      feeder4_flags1.b5
#define FEED4_FIRST_CYC         feeder4_flags1.b6

extern BYTE mixer_flags1;
#define MIXER_STATE                         mixer_flags1.b0
#define MIXER_STOP_DELAY_TEX                mixer_flags1.b1
#define MIXER_RAW_LEVEL_MAX_ALARM_DELAY_TEX mixer_flags1.b2
#define MIXER_OVERCURRENT_PROTECTION_ALARM  mixer_flags1.b3
#define MIXER_RAW_LEVEL_MIN_ALARM           mixer_flags1.b4
#define MIXER_SENSOR_ALARM                  mixer_flags1.b5
#define MIXER_RAW_LEVEL_MAX_ALARM           mixer_flags1.b6
#define MIXER_LOAD_REQUEST                  mixer_flags1.b7

extern BYTE mixer_flags2;
#define MIXER_LOADING_STOP_DELAY_TEX        mixer_flags2.b0
#define MIXER_PREV_STATE                    mixer_flags2.b1
#define VIBRATOR_TEX                        mixer_flags2.b2
#define CLEANING_AIR_TEX                    mixer_flags2.b3

extern BYTE cyclon_flags;
#define CYCLON_ALARM                cyclon_flags.b0
#define CYCLON_TEX                  cyclon_flags.b1
#define MILL_TEX                    cyclon_flags.b2
#define MILL_ALARM                  cyclon_flags.b3


extern BYTE process_control_sys_flags;
#define PROCESS_CTRL_TMR_10MS_CLK   process_control_sys_flags.b0

extern BYTE doser1_flags;
#define DOS1_ALARM                  doser1_flags.b0
#define DOS1_ACTIV_STATE            doser1_flags.b1
#define DOS1_RUN_REQUEST            doser1_flags.b2
#define DOS1_REQUEST_AUTO           doser1_flags.b3
#define DOS1_REQUEST_MANUAL         doser1_flags.b4
#define DOS1_PREV_STATE             doser1_flags.b5

extern BYTE doser2_flags;
#define DOS2_ALARM                  doser2_flags.b0
#define DOS2_ACTIV_STATE            doser2_flags.b1
#define DOS2_RUN_REQUEST            doser2_flags.b2
#define DOS2_REQUEST_AUTO           doser2_flags.b3
#define DOS2_REQUEST_MANUAL         doser2_flags.b4
#define DOS2_PREV_STATE             doser2_flags.b5

extern BYTE doser3_flags;
#define DOS3_ALARM                  doser3_flags.b0
#define DOS3_ACTIV_STATE            doser3_flags.b1
#define DOS3_RUN_REQUEST            doser3_flags.b2
#define DOS3_REQUEST_AUTO           doser3_flags.b3
#define DOS3_REQUEST_MANUAL         doser3_flags.b4
#define DOS3_PREV_STATE             doser3_flags.b5

extern BYTE doser4_flags;
#define DOS4_ALARM                  doser4_flags.b0
#define DOS4_ACTIV_STATE            doser4_flags.b1
#define DOS4_RUN_REQUEST            doser4_flags.b2
#define DOS4_REQUEST_AUTO           doser4_flags.b3
#define DOS4_REQUEST_MANUAL         doser4_flags.b4
#define DOS4_PREV_STATE             doser4_flags.b5

extern BYTE vacum_pump1_flags;
#define VP1_OVERCURRENT_PROTECTION_ALARM    vacum_pump1_flags.b0
#define VP1_THERMAL_PROTECTION_ALARM        vacum_pump1_flags.b1
#define VP1_VACUM_ALARM                     vacum_pump1_flags.b2
#define VP1_REQUEST                         vacum_pump1_flags.b3
#define VP1_READY                           vacum_pump1_flags.b4
#define VP1_TEX                             vacum_pump1_flags.b5
#define VP1_PREV_STATE                      vacum_pump1_flags.b6


extern BYTE vacum_pump2_flags;
#define VP2_OVERCURRENT_PROTECTION_ALARM    vacum_pump2_flags.b0
#define VP2_THERMAL_PROTECTION_ALARM        vacum_pump2_flags.b1
#define VP2_VACUM_ALARM                     vacum_pump2_flags.b2
#define VP2_REQUEST                         vacum_pump2_flags.b3
#define VP2_READY                           vacum_pump2_flags.b4
#define VP2_TEX                             vacum_pump2_flags.b5
#define VP2_PREV_STATE                      vacum_pump2_flags.b6

extern BYTE vacum_pump3_flags;
#define VP3_OVERCURRENT_PROTECTION_ALARM    vacum_pump3_flags.b0
#define VP3_THERMAL_PROTECTION_ALARM        vacum_pump3_flags.b1
#define VP3_VACUM_ALARM                     vacum_pump3_flags.b2
#define VP3_REQUEST                         vacum_pump3_flags.b3
#define VP3_READY                           vacum_pump3_flags.b4
#define VP3_TEX                             vacum_pump3_flags.b5
#define VP3_PREV_STATE                      vacum_pump3_flags.b6
//
//
/********* E N U M E R A T O R S   ********************************************/
//
//
extern enum activ_units {
    FEEDER1, FEEDER2, FEEDER3, FEEDER4
} activ_unit;
//

extern enum feeder1_states {
    FEED1_OFF, FEED1_CHARGING, FEED1_DISCHARGING, FEED1_ERROR
} feeder1_state;
//

extern enum feeder2_states {
    FEED2_OFF, FEED2_CHARGING, FEED2_DISCHARGING, FEED2_ERROR
} feeder2_state;
//

extern enum feeder3_states {
    FEED3_OFF, FEED3_CHARGING, FEED3_DISCHARGING, FEED3_ERROR
} feeder3_state;
//

extern enum feeder4_states {
    FEED4_OFF, FEED4_CHARGING, FEED4_DISCHARGING, FEED4_ERROR
} feeder4_state;
//

extern enum doser1_states {
    DOS1_STOP, DOS1_RUN_MANUAL, DOS1_RUN_AUTO, DOS1_ERROR
} doser1_state;
//

extern enum doser2_states {
    DOS2_STOP, DOS2_RUN_MANUAL, DOS2_RUN_AUTO, DOS2_ERROR
} doser2_state;
//

extern enum doser3_states {
    DOS3_STOP, DOS3_RUN_MANUAL, DOS3_RUN_AUTO, DOS3_ERROR
} doser3_state;
//

extern enum doser4_states {
    DOS4_STOP, DOS4_RUN_MANUAL, DOS4_RUN_AUTO, DOS4_ERROR
} doser4_state;
//

extern enum mixer_states {
    MIX_STOP, MIX_RUN, MIX_ERROR
} mixer_state;
//

extern enum vacum_pump1_states {
    VP1_OFF, VP1_RUN, VP1_IDLE, VP1_ERROR
} vacum_pump1_state;
//

extern enum vacum_pump2_states {
    VP2_OFF, VP2_RUN, VP2_IDLE, VP2_ERROR
} vacum_pump2_state;
//

extern enum vacum_pump3_states {
    VP3_OFF, VP3_RUN, VP3_IDLE, VP3_ERROR
} vacum_pump3_state;
//
//
/** F U N C T I O N S   P R O T O T Y P E S   *********************************/
//
//
void ControlProcess(void);
void ControlReset(void);
void ProcessFeeder1(void);
void ProcessFeeder2(void);
void ProcessFeeder3(void);
void ProcessFeeder4(void);
void ProcessDoser1(void);
void ProcessDoser2(void);
void ProcessDoser3(void);
void ProcessDoser4(void);
void ProcessMixer(void);
void ProcessVacumPump1(void);
void ProcessVacumPump2(void);
void ProcessVacumPump3(void);
void CheckControlProcessTimer(void);

#endif	/* DATA_PROCESS_H */

