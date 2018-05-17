/******************************************************************************
 *
 *          Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                      Data processing module
 *
 *******************************************************************************
 * Ime fajla:       data_process.c
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
//
//
/** U K L J U C E N I  ********************************************************/
//
//
#include "defines.h"
#include "io_cfg.h"
#include "process_control.h"
#include "digital_output.h"
#include "analog_output.h"
#include "digital_input.h"
#include "control_comm.h"
#include "relay_output.h"
//
//
//** E N U M E R A T O R S ****************************************************/
//
//

enum activ_units {
    FEEDER1, FEEDER2, FEEDER3, FEEDER4
} activ_unit;

enum feeder1_states {
    FED1_OFF, FEED1_CHARGING, FED1_DISCHARGING, FEED1_ERROR
} feeder1_state;

enum feeder2_states {
    FED2_OFF, FED2_CHARGING, FED2_DISCHARGING, FED2_ERROR
} feeder2_state;

enum feeder3_states {
    FED3_OFF, FED3_CHARGING, FED3_DISCHARGING, FEED3_ERROR
} feeder3_state;

enum feeder4_states {
    FED4_OFF, FED4_CHARGING, FEED4_DISCHARGING, FEED4_ERROR
} feeder4_state;

enum doser1_states {
    DOS1_STOP, DOS1_RUN_MANUAL, DOS1_RUN_AUTO, DOS1_ERROR
} doser1_state;

enum doser2_states {
    DOS2_STOP, DOS2_RUN_MANUAL, DOS2_RUN_AUTO, DOS2_ERROR
} doser2_state;

enum doser3_states {
    DOS3_STOP, DOS3_RUN_MANUAL, DOS3_RUN_AUTO, DOS3_ERROR
} doser3_state;

enum doser4_states {
    DOS4_STOP, DOS4_RUN_MANUAL, DOS4_RUN_AUTO, DOS4_ERROR
} doser4_state;

enum mixer_states {
    MIX_STOP, MIX_RUN, MIX_ERROR
} mixer_state;

enum vacum_pump1_states {
    VP1_OFF, VP1_RUN, VP1_IDLE, VP1_ERROR
} vacum_pump1_state;

enum vacum_pump2_states {
    VP2_OFF, VP2_RUN, VP2_IDLE, VP2_ERROR
} vacum_pump2_state;

enum vacum_pump3_states {
    VP3_OFF, VP3_RUN, VP3_IDLE, VP3_ERROR
} vacum_pump3_state;

//
//
//
/** R A M   V A R I J A B L E *************************************************/
//
//
unsigned char mixer_pcnt, fed1_pcnt, fed2_pcnt, fed3_pcnt, fed4_pcnt;
unsigned char vacum_pump1_pcnt, vacum_pump2_pcnt, vacum_pump3_pcnt;
unsigned char dos1_pcnt, dos2_pcnt, dos3_pcnt, dos4_pcnt;
unsigned char feed1_change_tmr, feed2_change_tmr, feed3_change_tmr, feed4_change_tmr;
unsigned char dp_tmp, cp_pcnt, clk_1s, mix_cyc_cnt, cyclon_pcnt;
//
//
//
unsigned int vibrator_tmr, vibrator_cyc, mill_stop_delay_tmr, cleaning_air_tmr, cleaning_air_cyc;
unsigned int vibrator_time, vibrator_cyc_cnt, mill_stop_delay_time, cleaning_air_time, cleaning_air_cyc_cnt;
unsigned int cyclon_alarm_delay_tmr, vp1_tmr, vp2_tmr, vp3_tmr, mixer_load_stop_tmr;
unsigned int cyclon_alarm_delay_time, vp1_delay_time, mixer_load_stop_time;
unsigned int feed1_cyc_tmr, feed2_cyc_tmr, feed3_cyc_tmr, feed4_cyc_tmr;
unsigned int feed1_cyc_time, feed2_cyc_time, feed3_cyc_time, feed4_cyc_time;
unsigned int control_tmr, vp2_delay_tmr, vp3_delay_tmr;
unsigned int control_time, vp2_delay_time, vp3_delay_time;
unsigned int mixer_stop_delay_tmr, mixer_load_stop_delay_tmr, mix_raw_max_alarm_delay_tmr;
unsigned int mixer_stop_delay_time, mixer_load_stop_delay_time, mix_raw_max_alarm_delay_time;
//
//
//
BYTE control_flags1;
BYTE feeder1_flags1;
BYTE feeder1_flags2;
BYTE feeder2_flags1;
BYTE feeder2_flags2;
BYTE feeder3_flags1;
BYTE feeder3_flags2;
BYTE feeder4_flags1;
BYTE feeder4_flags2;
BYTE mixer_flags1;
BYTE mixer_flags2;
BYTE cyclon_flags;
BYTE process_control_sys_flags;
BYTE doser1_flags;
BYTE doser2_flags;
BYTE doser3_flags;
BYTE doser4_flags;
BYTE vacum_pump1_flags;
BYTE vacum_pump2_flags;
BYTE vacum_pump3_flags;
//
//
//
#pragma code
//
//
//
// <editor-fold defaultstate="collapsed" desc="control process">

void ControlProcess(void) {
    //
    //------------------------------------------- check for active overcurrent
    //------------------------------------------- protection devices
    //
    if (IsEmergencyAlarmActiv() || IsControlCommBusErrorAlarmActiv()) {
        ResetControlStarting();
        ControlOff();
        sys_state = SYS_ERROR;
        cp_pcnt = 0;
    }// End of if...

    switch (sys_state) {

        case SYS_STOP:
            if (!IsVacumPump1OvercurrentProtectionInputActiv()) SetVacumPump1OvercurrentProtectionAlarm();
            if (!IsVacumPump1ThermoProtectionInputActiv())SetVacumPump1ThermalProtectionAlarm();
            if (!IsVacumPump2OvercurrentProtectionInputActiv()) SetVacumPump2OvercurrentProtectionAlarm();
            if (!IsVacumPump2ThermoProtectionInputActiv())SetVacumPump2ThermalProtectionAlarm();
            if (!IsVacumPump3OvercurrentProtectionInputActiv()) SetVacumPump3OvercurrentProtectionAlarm();
            if (!IsVacumPump3ThermoProtectionInputActiv())SetVacumPump3ThermalProtectionAlarm();
            if (!IsMixerOvercurrentProtectionInputActiv()) SetMixerOvercurrentProtectionAlarm();
            if (IsTasterResetPressed()) ControlReset();
            //
            //----------------------------------- check for control on
            //----------------------------------- command
            //
            if ((cp_pcnt == 0) && !IsControlActiv()) {
                AnalogOutputClear();
                DigitalOutputClear();
                VacumPump1Off();
                VacumPump2Off();
                VacumPump3Off();
                VacumPump1VacumValveOff();
                MixerOff();
                DrivePowerOff();
                VibratorOff();
                Doser4MotorFanOff();
                Feeder1VacumValveOff();
                Feeder2VacumValveOff();
                Feeder3VacumValveOff();
                Feeder4VacumValveOff();
                CleaningValveOff();
                if (IsMillDisabledAlarmActiv()) MillDisable();
                else MillEnable();
                ResetControlStarting();
                if (IsTasterStartPressed()) {
                    ControlReset();
                    SetControlTimer(CONTROL_STAB_TIME);
                    DrivePowerOn();
                    SetControlStarting();
                    ++cp_pcnt;
                }// End of if...
            } else if ((cp_pcnt == 1) && IsControlTimerExpired()) {
                //
                //------------------------------- check for drives
                //------------------------------- ready signals
                //
                if (!IsDrive1OkInputActiv() || !IsDrive2OkInputActiv() \
                    || !IsDrive3OkInputActiv() || !IsDrive4OkInputActiv()) {
                    //
                    //--------------------------- check if any drive occure error
                    //
                    DrivePowerOff(); //---------- if so, shut down control
                    ResetControlStarting();
                    ControlOff();
                    sys_state = SYS_ERROR; //---- switch system to error state
                    cp_pcnt = 0;
                } else {
                    //
                    //--------------------------- check for selected vibrator
                    //
                    if ((vibrator_time == 0) || (vibrator_cyc == 0)) DisableVibrator();
                    else {
                        EnableVibrator();
                        SetVibratorCyclusCounter();
                    }// End of else...
                    //
                    //--------------------------- check for selected cleaning
                    //
                    if ((cleaning_air_time == 0) || (cleaning_air_cyc == 0)) DisableCleaningAir();
                    else {
                        EnableCleaningAir();
                        SetCleaningAirCyclusCounter();
                    }// End of else...
                    //
                    //--------------------------- select system  state
                    //
                    if (IsFeeder1PreviousStateOn()) feeder1_state = FEED1_DISCHARGING;
                    if (IsFeeder2PreviousStateOn()) feeder2_state = FEED2_DISCHARGING;
                    if (IsFeeder3PreviousStateOn()) feeder3_state = FEED3_DISCHARGING;
                    if (IsFeeder4PreviousStateOn()) feeder4_state = FEED4_DISCHARGING;
                    if (IsMixerPreviousStateOn()) mixer_state = MIX_RUN;
                    if (IsVacumPump1PreviousStateOn()) vacum_pump1_state = VP1_RUN;
                    //if (IsVacumPump2PreviousStateOn()) vacum_pump2_state = VP2_RUN;
                    //if (IsVacumPump3PreviousStateOn()) vacum_pump3_state = VP3_RUN;
                    SetFeeder1FirstCycle();
                    SetFeeder2FirstCycle();
                    SetFeeder3FirstCycle();
                    SetFeeder4FirstCycle();
                    ControlOn(); //--------- if all drives ok, switch
                    sys_state = SYS_RUN; //------- system to RUN state
                    activ_unit = FEEDER1;
                    cp_pcnt = 0;
                }// End of else...
            }// End of else if...
            break;

        case SYS_RUN:
            if (IsTasterResetPressed()) ControlReset();
            if ((activ_unit == FEEDER1) && (feeder1_state != FEED1_CHARGING)) activ_unit = FEEDER2;
            else if ((activ_unit == FEEDER2) && (feeder2_state != FEED2_CHARGING)) activ_unit = FEEDER3;
            else if ((activ_unit == FEEDER3) && (feeder3_state != FEED3_CHARGING)) activ_unit = FEEDER4;
            else if ((activ_unit == FEEDER4) && (feeder4_state != FEED4_CHARGING)) activ_unit = FEEDER1;
            if ((cp_pcnt == 0) && IsTasterStopPressed()) {
                SetControlTimer(CONTROL_START_UP_TIME);
                ResetControlStarting();
                DrivePowerOff();
                ControlOff();
                sys_state = SYS_STOP;
                cp_pcnt = 0;
            }// End of if...
            break;

        case SYS_ERROR:
            //
            //----------------------------------- check for any new alarm
            //
            if (IsEmergencyAlarmActiv())SetEmergencyAlarm();
            if (IsControlCommBusErrorAlarmActiv())SetControlCommBusErrorAlarm();
            if (!IsVacumPump1OvercurrentProtectionInputActiv()) SetVacumPump1OvercurrentProtectionAlarm();
            if (!IsVacumPump1ThermoProtectionInputActiv())SetVacumPump1ThermalProtectionAlarm();
            if (!IsVacumPump2OvercurrentProtectionInputActiv()) SetVacumPump2OvercurrentProtectionAlarm();
            if (!IsVacumPump2ThermoProtectionInputActiv())SetVacumPump2ThermalProtectionAlarm();
            if (!IsVacumPump3OvercurrentProtectionInputActiv()) SetVacumPump3OvercurrentProtectionAlarm();
            if (!IsVacumPump3ThermoProtectionInputActiv())SetVacumPump3ThermalProtectionAlarm();
            if (!IsMixerOvercurrentProtectionInputActiv()) SetMixerOvercurrentProtectionAlarm();
            if (!IsDrive1OkInputActiv()) SetDoser1DriveAlarm();
            if (!IsDrive2OkInputActiv()) SetDoser2DriveAlarm();
            if (!IsDrive3OkInputActiv()) SetDoser3DriveAlarm();
            if (!IsDrive4OkInputActiv()) SetDoser4DriveAlarm();
            //
            //----------------------------------- clear output registers
            //
            AnalogOutputClear();
            DigitalOutputClear();
            if (IsAlarmOn()) {
                RelayOutputClear();
                AlarmOn();
            } else RelayOutputClear();
            if (IsMillDisabledAlarmActiv()) MillDisable();
            ResetControlStarting();
            //
            //----------------------------------- check for reset condition
            //
            if (IsTasterResetPressed()) {
                ControlReset();
                sys_state = SYS_STOP;
                cp_pcnt = 0;
            }// End of if...
            break;
    }// End of switch...
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="control reset">

void ControlReset(void) {
    //
    //------------------------------------------- reset emergency alarm
    //
    ResetEmergencyAlarm();
    //
    //------------------------------------------- reset feeder 1 alarms
    //
    ResetFeeder1RawLevelMinAlarm();
    ResetFeeder1FillingCycleFailAlarm();
    ResetFeeder1SensorAlarm();
    ResetFeeder1Signals();
    //
    //------------------------------------------- reset feeder 2 alarms
    //
    ResetFeeder2RawLevelMinAlarm();
    ResetFeeder2FillingCycleFailAlarm();
    ResetFeeder2SensorAlarm();
    ResetFeeder2Signals();
    //
    //------------------------------------------- reset feeder 3 alarms
    //
    ResetFeeder3RawLevelMinAlarm();
    ResetFeeder3FillingCycleFailAlarm();
    ResetFeeder3SensorAlarm();
    ResetFeeder3Signals();
    //
    //------------------------------------------- reset feeder 4 alarms
    //
    ResetFeeder4RawLevelMinAlarm();
    ResetFeeder4FillingCycleFailAlarm();
    ResetFeeder4SensorAlarm();
    ResetFeeder4Signals();
    //
    //------------------------------------------- reset vacum pump 1 alarms
    //
    ResetVacumPump1VacumAlarm();
    ResetVacumPump1ThermalProtectionAlarm();
    ResetVacumPump1OvercurrentProtectionAlarm();
    //
    //------------------------------------------- reset vacum pump 2 alarms
    //
    ResetVacumPump2VacumAlarm();
    ResetVacumPump2ThermalProtectionAlarm();
    ResetVacumPump2OvercurrentProtectionAlarm();
    //
    //------------------------------------------- reset vacum pump 3 alarms
    //
    ResetVacumPump3VacumAlarm();
    ResetVacumPump3ThermalProtectionAlarm();
    ResetVacumPump3OvercurrentProtectionAlarm();
    //
    //------------------------------------------- reset cyclon alarms
    //
    ResetCyclonMaxLevelAlarm();
    if (!IsCyclonLevelMaxInputActiv()) {
        ResetMillDisabledAlarm();
        MillEnable();
    }// End of else if...
    //
    //------------------------------------------- reset mixer alarms
    //
    ResetMixerOvercurrentProtectionAlarm();
    ResetMixerRawLevelMinimumAlarm();
    ResetMixerRawLevelMaximumAlarm();
    ResetMixerSensorAlarm();
    //
    //------------------------------------------- reset doser 1 alarms
    //
    ResetDoser1DriveAlarm();
    //
    //------------------------------------------- reset doser 2 alarms
    //
    ResetDoser2DriveAlarm();
    //
    //------------------------------------------- reset doser 3 alarms
    //
    ResetDoser3DriveAlarm();
    //
    //------------------------------------------- reset doser 4 alarms
    //
    ResetDoser4DriveAlarm();
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process feeder 1">

void ProcessFeeder1(void) {

    if (!IsControlActiv()) {
        feeder1_state = FEED1_OFF;
    }// End of if...

    switch (feeder1_state) {

        case FEED1_OFF:
            if (IsTasterFeeder1OnOffPressed()) {
                feeder1_state = FEED1_DISCHARGING;
                SetFeeder1ControlState();
            }// End of if...
            Feeder1VacumValveOff();
            ResetFeeder1RawLevelMinAlarm();
            ResetFeeder1FillingCycleFailAlarm();
            ResetFeeder1Signals();
            fed1_pcnt = 0;
            break;

        case FEED1_CHARGING:
            if (IsTasterFeeder1OnOffPressed()) {
                feeder1_state = FEED1_OFF;
                ResetFeeder1PreviousState();
                break;
            }// End of if...
            if (IsCleaningModeActiv() || IsFeeder1LevelMaxInputActiv()) {
                ResetFeeder1FillingCycleFailAlarm();
                Feeder1VacumValveOff();
                SetFeeder1ChangeTimer();
                feeder1_state = FEED1_DISCHARGING;
                fed1_pcnt = 0;
            } else if (!IsFeeder1LevelMinInputActiv() && !IsFeeder1FirstCycle()) SetFeeder1RawLevelMinAlarm();

            if ((fed1_pcnt == 0) && (activ_unit == FEEDER1)) {
                SetVacumPump1Request();
                ++fed1_pcnt; /*goto next feeding process task*/
            } else if ((fed1_pcnt == 1) && (activ_unit == FEEDER1)) {
                if (IsVacumPump1Ready()) {
                    SetFeeder1CyclusTimer();
                    Feeder1VacumValveOn();
                    ++fed1_pcnt;
                } else SetVacumPump1Request();
            } else if ((fed1_pcnt == 2) && IsFeeder1CyclusTimeExpired()) {
                SetFeeder1FillingCycleFailAlarm();
                Feeder1VacumValveOff();
                SetFeeder1ChangeTimer();
                feeder1_state = FEED1_DISCHARGING;
                fed1_pcnt = 0;
            }// End of else if...
            break;

        case FEED1_DISCHARGING:
            if (IsTasterFeeder1OnOffPressed()) {
                feeder1_state = FEED1_OFF;
                ResetFeeder1PreviousState();
                break;
            }// End of if...

            if (IsFeeder1LevelMinInputActiv()) ResetFeeder1FirstCycle();
            else if (!IsCleaningModeActiv() && !IsFeeder1FirstCycle()) {
                if (!IsFeeder1RequestInputActiv()) {
                    SetFeeder1SensorAlarm();
                    feeder1_state = FEED1_ERROR;
                }// End of if...
                else SetFeeder1RawLevelMinAlarm();
            }// End of else if...

            if (IsFeeder1RequestInputActiv() && IsFeeder1ChangeTimerExpired()) {
                if (IsFeeder1LevelMaxInputActiv()) {
                    SetFeeder1SensorAlarm();
                    feeder1_state = FEED1_ERROR;
                } else feeder1_state = FEED1_CHARGING;
            }// End of if...
            break;

        case FEED1_ERROR:
            if (IsTasterFeeder1OnOffPressed()) {
                feeder1_state = FEED1_OFF;
                ResetFeeder1PreviousState();
            }// End of if..

            if (IsTasterResetPressed()) {
                feeder1_state = FEED1_DISCHARGING;
                ResetFeeder1Signals();
                fed1_pcnt = 0;
            }// End of if..

            Feeder1VacumValveOff();
            break;
    }// End of switch   
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process feeder 2">

void ProcessFeeder2(void) {

    if (!IsControlActiv()) {
        feeder2_state = FEED2_OFF;
    }// End of if...

    switch (feeder2_state) {

        case FEED2_OFF:
            if (IsTasterFeeder2OnOffPressed()) {
                feeder2_state = FEED2_DISCHARGING;
                SetFeeder2PreviousState();
            }// End of if...
            Feeder2VacumValveOff();
            ResetFeeder2RawLevelMinAlarm();
            ResetFeeder2FillingCycleFailAlarm();
            ResetFeeder2Signals();
            fed2_pcnt = 0;
            break;

        case FEED2_CHARGING:
            if (IsTasterFeeder2OnOffPressed()) {
                feeder2_state = FEED2_OFF;
                ResetFeeder2PreviousState();
                break;
            }// End of if...
            if (IsCleaningModeActiv() || IsFeeder2LevelMaxInputActiv()) {
                ResetFeeder2FillingCycleFailAlarm();
                Feeder2VacumValveOff();
                SetFeeder2ChangeTimer();
                feeder2_state = FEED2_DISCHARGING;
                fed2_pcnt = 0;
            } else if (!IsFeeder2LevelMinInputActiv() && !IsFeeder2FirstCycle()) SetFeeder2RawLevelMinAlarm();
            if ((fed2_pcnt == 0) && (activ_unit == FEEDER2)) {
                SetVacumPump1Request();
                ++fed2_pcnt; /*goto next feeding process task*/
            } else if ((fed2_pcnt == 1) && (activ_unit == FEEDER2)) {
                if (IsVacumPump1Ready()) {
                    SetFeeder2CyclusTimer();
                    Feeder2VacumValveOn();
                    ++fed2_pcnt;
                } else SetVacumPump1Request();
            } else if ((fed2_pcnt == 2) && IsFeeder2CyclusTimeExpired()) {
                SetFeeder2FillingCycleFailAlarm();
                Feeder2VacumValveOff();
                SetFeeder2ChangeTimer();
                feeder2_state = FEED2_DISCHARGING;
                fed2_pcnt = 0;
            }// End of else if...
            break;

        case FEED2_DISCHARGING:
            if (IsTasterFeeder2OnOffPressed()) {
                feeder2_state = FEED2_OFF;
                ResetFeeder2PreviousState();
                break;
            }// End of if...

            if (IsFeeder2LevelMinInputActiv()) ResetFeeder2FirstCycle();
            else if (!IsCleaningModeActiv() && !IsFeeder2FirstCycle()) {
                if (!IsFeeder2RequestInputActiv()) {
                    SetFeeder2SensorAlarm();
                    feeder2_state = FEED2_ERROR;
                }// End of if...
                else SetFeeder2RawLevelMinAlarm();
            }// End of else if...

            if (IsFeeder2RequestInputActiv() && IsFeeder2ChangeTimerExpired()) {
                if (IsFeeder2LevelMaxInputActiv()) {
                    SetFeeder2SensorAlarm();
                    feeder2_state = FEED2_ERROR;
                } else feeder2_state = FEED2_CHARGING;
            }// End of if...
            break;

        case FEED2_ERROR:
            if (IsTasterFeeder2OnOffPressed()) {
                feeder2_state = FEED2_OFF;
                ResetFeeder2PreviousState();
            }// End of if..

            if (IsTasterResetPressed()) {
                feeder2_state = FEED2_DISCHARGING;
                ResetFeeder2Signals();
                fed2_pcnt = 0;
            }// End of if..

            Feeder2VacumValveOff();
            break;
    }// End of switch
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process feeder 3">

void ProcessFeeder3(void) {

    if (!IsControlActiv()) {
        feeder3_state = FEED3_OFF;
    }// End of if...

    switch (feeder3_state) {

        case FEED3_OFF:
            if (IsTasterFeeder3OnOffPressed()) {
                feeder3_state = FEED3_DISCHARGING;
                SetFeeder3PreviousState();
            }// End of if...
            Feeder3VacumValveOff();
            ResetFeeder3RawLevelMinAlarm();
            ResetFeeder3FillingCycleFailAlarm();
            ResetFeeder3Signals();
            fed3_pcnt = 0;
            break;

        case FEED3_CHARGING:
            if (IsTasterFeeder3OnOffPressed()) {
                feeder3_state = FEED3_OFF;
                ResetFeeder3PreviousState();
                break;
            }// End of if...
            if (IsCleaningModeActiv() || IsFeeder3LevelMaxInputActiv()) {
                ResetFeeder3FillingCycleFailAlarm();
                Feeder3VacumValveOff();
                SetFeeder3ChangeTimer();
                feeder3_state = FEED3_DISCHARGING;
                fed3_pcnt = 0;
            } else if (!IsFeeder3LevelMinInputActiv() && !IsFeeder3FirstCycle()) SetFeeder3RawLevelMinAlarm();
            if ((fed3_pcnt == 0) && (activ_unit == FEEDER3)) {
                SetVacumPump1Request();
                ++fed3_pcnt; /*goto next feeding process task*/
            } else if ((fed3_pcnt == 1) && (activ_unit == FEEDER3)) {
                if (IsVacumPump1Ready()) {
                    SetFeeder3CyclusTimer();
                    Feeder3VacumValveOn();
                    ++fed3_pcnt;
                } else SetVacumPump1Request();
            } else if ((fed3_pcnt == 2) && IsFeeder3CyclusTimeExpired()) {
                SetFeeder3FillingCycleFailAlarm();
                Feeder3VacumValveOff();
                SetFeeder3ChangeTimer();
                feeder3_state = FEED3_DISCHARGING;
                fed3_pcnt = 0;
            }// End of else if...
            break;

        case FEED3_DISCHARGING:
            if (IsTasterFeeder3OnOffPressed()) {
                feeder3_state = FEED3_OFF;
                ResetFeeder3PreviousState();
                break;
            }// End of if...

            if (IsFeeder3LevelMinInputActiv()) ResetFeeder3FirstCycle();
            else if (!IsCleaningModeActiv() && !IsFeeder3FirstCycle()) {
                if (!IsFeeder3RequestInputActiv()) {
                    SetFeeder3SensorAlarm();
                    feeder3_state = FEED3_ERROR;
                }// End of if...
                else SetFeeder3RawLevelMinAlarm();
            }// End of else if...

            if (IsFeeder3RequestInputActiv() && IsFeeder3ChangeTimerExpired()) {
                if (IsFeeder3LevelMaxInputActiv()) {
                    SetFeeder3SensorAlarm();
                    feeder3_state = FEED3_ERROR;
                } else feeder3_state = FEED3_CHARGING;
            }// End of if...
            break;

        case FEED3_ERROR:
            if (IsTasterFeeder3OnOffPressed()) {
                feeder3_state = FEED3_OFF;
                ResetFeeder3PreviousState();
            }// End of if...

            if (IsTasterResetPressed()) {
                feeder3_state = FEED3_DISCHARGING;
                ResetFeeder3Signals();
                fed3_pcnt = 0;
            }// End of if..

            Feeder3VacumValveOff();
            break;
    }// End of switch
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process feeder 4">

void ProcessFeeder4(void) {

    if (!IsControlActiv()) {
        feeder4_state = FEED4_OFF;
    }// End of if...

    switch (feeder4_state) {

        case FEED4_OFF:
            if (IsTasterFeeder4OnOffPressed()) {
                feeder4_state = FEED4_DISCHARGING;
                SetFeeder4PreviousState();
            }// End of if...
            Feeder4VacumValveOff();
            ResetFeeder4RawLevelMinAlarm();
            ResetFeeder4FillingCycleFailAlarm();
            ResetFeeder4Signals();
            fed4_pcnt = 0;
            break;

        case FEED4_CHARGING:
            if (IsTasterFeeder4OnOffPressed()) {
                feeder4_state = FEED4_OFF;
                ResetFeeder4PreviousState();
                break;
            }// End of if...
            if (IsCleaningModeActiv() || IsFeeder4LevelMaxInputActiv()) {
                ResetFeeder4FillingCycleFailAlarm();
                Feeder4VacumValveOff();
                SetFeeder4ChangeTimer();
                feeder4_state = FEED4_DISCHARGING;
                fed4_pcnt = 0;
            } else if (!IsFeeder4LevelMinInputActiv() && !IsFeeder4FirstCycle()) SetFeeder4RawLevelMinAlarm();
            if ((fed4_pcnt == 0) && (activ_unit == FEEDER4)) {
                SetVacumPump1Request();
                ++fed4_pcnt; /*goto next feeding process task*/
            } else if ((fed4_pcnt == 1) && (activ_unit == FEEDER4)) {
                if (IsVacumPump1Ready()) {
                    SetFeeder4CyclusTimer();
                    Feeder4VacumValveOn();
                    ++fed4_pcnt;
                } else SetVacumPump1Request();
            } else if ((fed4_pcnt == 2) && IsFeeder4CyclusTimeExpired()) {
                SetFeeder4FillingCycleFailAlarm();
                Feeder4VacumValveOff();
                SetFeeder4ChangeTimer();
                feeder4_state = FEED4_DISCHARGING;
                fed4_pcnt = 0;
            }// End of else if...
            break;

        case FEED4_DISCHARGING:
            if (IsTasterFeeder4OnOffPressed()) {
                feeder4_state = FEED4_OFF;
                ResetFeeder4PreviousState();
                break;
            }// End of if...

            if (IsFeeder4LevelMinInputActiv()) ResetFeeder4FirstCycle();
            else if (!IsCleaningModeActiv() && !IsFeeder4FirstCycle()) {
                if (!IsFeeder4RequestInputActiv()) {
                    SetFeeder4SensorAlarm();
                    feeder4_state = FEED4_ERROR;
                }// End of if...
                else SetFeeder4RawLevelMinAlarm();
            }// End of else if...

            if (IsFeeder4RequestInputActiv() && IsFeeder4ChangeTimerExpired()) {
                if (IsFeeder4LevelMaxInputActiv()) {
                    SetFeeder4SensorAlarm();
                    feeder4_state = FEED4_ERROR;
                } else feeder4_state = FEED4_CHARGING;
            }// End of if...
            break;

        case FEED4_ERROR:
            if (IsTasterFeeder4OnOffPressed()) {
                feeder4_state = FEED4_OFF;
                ResetFeeder4PreviousState();
            }// End of if...

            if (IsTasterResetPressed()) {
                feeder4_state = FEED4_DISCHARGING;
                ResetFeeder4Signals();
                fed4_pcnt = 0;
            }// End of if..

            Feeder4VacumValveOff();
            break;
    }// End of switch
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process doser 1">

void ProcessDoser1(void) {
    //
    //------------------------------------------- check if control is switched on
    //
    if (!IsControlActiv()) {
        ResetDoser1RunRequest();
        ResetDoser1RequestManual();
        ResetDoser1RequestAuto();
        doser1_state = DOS1_STOP;
        return;
    } else if (!IsDrive1OkInputActiv()) {//------ check for drive 1 error occured
        SetDoser1DriveAlarm();
        doser1_state = DOS1_ERROR;
    }// End of if...
    //
    //------------------------------------------- select current doser 1 state
    //
    switch (doser1_state) {
            //
            //----------------------------------- if stop wait for start condition
            //
        case DOS1_STOP:
            Drive1Stop();
            dos1_set_value = 0U;
            dos1_pcnt = 0U;
            if (IsDoser1RunRequestActiv() && IsDoser1RequestManual()) {
                ResetDoser1RequestManual();
                SetDoser1PreviousState();
                doser1_state = DOS1_RUN_MANUAL;
            } else if (IsDoser1RunRequestActiv() && IsDoser1RequestAuto()) {
                ResetDoser1RequestAuto();
                SetDoser1PreviousState();
                doser1_state = DOS1_RUN_AUTO;
            }// End of else if...
            break;
            //
            //----------------------------------- operate doser 1 manualy
            //
        case DOS1_RUN_MANUAL:
            if (!IsDoser1RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser1RunRequestActiv()) ResetDoser1PreviousState();
                doser1_state = DOS1_STOP;
            } else {
                Drive1Run();
                dos1_set_value = dos1_ref;
            }// End of else...            
            break;
            //
            //----------------------------------- operate doser 1 with mixer
            //
        case DOS1_RUN_AUTO:
            if (!IsDoser1RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser1RunRequestActiv()) ResetDoser1PreviousState();
                doser1_state = DOS1_STOP;
            } else if (!IsMixerLoadRequestActiv()) {
                ResetDoser1ActivState();
                Drive1Stop();
                dos1_set_value = 0U;
            } else if (IsMixerLoadRequestActiv()) {
                SetDoser1ActivState();
                Drive1Run();
                dos1_set_value = dos1_ref;
            }// End of else if...
            break;
            //
            //----------------------------------- doser 1 drive error
            //
        case DOS1_ERROR:
            if (IsTasterResetPressed() && !IsDoser1DriveAlarmActiv()) {
                if (IsDoser1PreviousStateOn() && IsDoser1ManualSelected()) {
                    doser1_state = DOS1_RUN_MANUAL;
                } else if (IsDoser1PreviousStateOn() && IsDoser1AutoSelected()) {
                    doser1_state = DOS1_RUN_AUTO;
                } else doser1_state = DOS1_STOP;
            }// End of else...
            Drive1Stop();
            dos1_set_value = 0U;
            dos1_pcnt = 0;
            break;
    }// End of switch
}// End of process doser 1
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process doser 2">

void ProcessDoser2(void) {
    //
    //------------------------------------------- check if control is switched on
    //
    if (!IsControlActiv()) {
        ResetDoser2RunRequest();
        ResetDoser2RequestManual();
        ResetDoser2RequestAuto();
        doser2_state = DOS2_STOP;
        return;
    } else if (!IsDrive2OkInputActiv()) {//------ check for drive 2 error
        SetDoser2DriveAlarm();
        doser2_state = DOS2_ERROR;
    }// End of if...
    //
    //------------------------------------------- select current doser 2 state
    //
    switch (doser2_state) {
            //
            //------------------------------------- if stop wait for start condition
            //
        case DOS2_STOP:
            Drive2Stop();
            dos2_set_value = 0U;
            dos2_pcnt = 0U;
            if (IsDoser2RunRequestActiv() && IsDoser2RequestManual()) {
                ResetDoser2RequestManual();
                SetDoser2PreviousState();
                doser2_state = DOS2_RUN_MANUAL;
            } else if (IsDoser2RunRequestActiv() && IsDoser2RequestAuto()) {
                ResetDoser2RequestAuto();
                SetDoser2PreviousState();
                doser2_state = DOS2_RUN_AUTO;
            }// End of else if...
            break;
            //
            //------------------------------------- operate doser 2 manualy
            //
        case DOS2_RUN_MANUAL:
            if (!IsDoser2RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser2RunRequestActiv()) ResetDoser2PreviousState();
                doser2_state = DOS2_STOP;
            } else {
                Drive2Run();
                dos2_set_value = dos2_ref;
            }// End of else...
            break;
            //
            //------------------------------------- operate doser 2 with mixer
            //
        case DOS2_RUN_AUTO:
            if (!IsDoser2RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser2RunRequestActiv()) ResetDoser2PreviousState();
                doser2_state = DOS2_STOP;
            } else if (!IsMixerLoadRequestActiv()) {
                ResetDoser2ActivState();
                Drive2Stop();
                dos2_set_value = 0U;
            } else if (IsMixerLoadRequestActiv()) {
                SetDoser2ActivState();
                Drive2Run();
                dos2_set_value = dos2_ref;
            }// End of else if...
            break;
            //
            //------------------------------------- doser 2 drive error
            //
        case DOS2_ERROR:
            if (IsTasterResetPressed() && !IsDoser2DriveAlarmActiv()) {
                if (IsDoser2PreviousStateOn() && IsDoser2ManualSelected()) {
                    doser2_state = DOS2_RUN_MANUAL;
                } else if (IsDoser2PreviousStateOn() && IsDoser2AutoSelected()) {
                    doser2_state = DOS2_RUN_AUTO;
                } else doser2_state = DOS2_STOP;
            }// End of else...
            Drive2Stop();
            dos2_set_value = 0U;
            dos2_pcnt = 0;
            break;
    }// End of switch
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process doser 3">

void ProcessDoser3(void) {
    //
    //------------------------------------------- check if control is switched on
    //
    if (!IsControlActiv()) {
        ResetDoser3RunRequest();
        ResetDoser3RequestManual();
        ResetDoser3RequestAuto();
        doser3_state = DOS3_STOP;
        return;
    } else if (!IsDrive3OkInputActiv()) {//------ check for drive 3 error
        SetDoser3DriveAlarm();
        doser3_state = DOS3_ERROR;
    }// End of if...
    //
    //------------------------------------------- select current doser 3 state
    //
    switch (doser3_state) {
            //
            //------------------------------------- if stop wait for start condition
            //
        case DOS3_STOP:
            Drive3Stop();
            dos3_set_value = 0U;
            dos3_pcnt = 0U;
            if (IsDoser3RunRequestActiv() && IsDoser3RequestManual()) {
                ResetDoser3RequestManual();
                SetDoser3PreviousState();
                doser3_state = DOS3_RUN_MANUAL;
            } else if (IsDoser3RunRequestActiv() && IsDoser3RequestAuto()) {
                ResetDoser3RequestAuto();
                SetDoser3PreviousState();
                doser3_state = DOS3_RUN_AUTO;
            }// End of else if...
            break;
            //
            //------------------------------------- operate doser 3 manualy
            //
        case DOS3_RUN_MANUAL:
            if (!IsDoser3RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser3RunRequestActiv()) ResetDoser3PreviousState();
                doser3_state = DOS3_STOP;
            } else {
                Drive3Run();
                dos3_set_value = dos3_ref;
            }// End of else...
            break;
            //
            //------------------------------------- operate doser 3 with mixer
            //
        case DOS3_RUN_AUTO:
            if (!IsDoser3RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser3RunRequestActiv()) ResetDoser3PreviousState();
                doser3_state = DOS3_STOP;
            } else if (!IsMixerLoadRequestActiv()) {
                ResetDoser3ActivState();
                Drive3Stop();
                dos3_set_value = 0U;
            } else if (IsMixerLoadRequestActiv()) {
                SetDoser3ActivState();
                Drive3Run();
                dos3_set_value = dos3_ref;
            }// End of else if...
            break;
            //
            //------------------------------------- doser 3 drive error
            //
        case DOS3_ERROR:
            if (IsTasterResetPressed() && !IsDoser3DriveAlarmActiv()) {
                if (IsDoser3PreviousStateOn() && IsDoser3ManualSelected()) {
                    doser3_state = DOS3_RUN_MANUAL;
                } else if (IsDoser3PreviousStateOn() && IsDoser3AutoSelected()) {
                    doser3_state = DOS3_RUN_AUTO;
                } else doser3_state = DOS3_STOP;
            }// End of else...
            Drive3Stop();
            dos3_set_value = 0U;
            dos3_pcnt = 0;
            break;
    }// End of switch
}// End of process doser 3
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process doser 4">

void ProcessDoser4(void) {
    //
    //------------------------------------------- check if control is switched on
    //
    if (!IsControlActiv()) {
        ResetDoser4RunRequest();
        ResetDoser4RequestManual();
        ResetDoser4RequestAuto();
        doser4_state = DOS4_STOP;
        return;
    } else if (!IsDrive4OkInputActiv()) {//------ check for drive 4 error occured
        SetDoser4DriveAlarm();
        doser4_state = DOS4_ERROR;
    }// End of if...
    //
    //------------------------------------------- select current doser 4 state
    //
    switch (doser4_state) {
            //
            //----------------------------------- if stop wait for start condition
            //
        case DOS4_STOP:
            Drive4Stop();
            Doser4MotorFanOff();
            dos4_set_value = 0U;
            dos4_pcnt = 0U;
            if (IsDoser4RunRequestActiv() && IsDoser4RequestManual()) {
                ResetDoser4RequestManual();
                SetDoser4PreviousState();
                doser4_state = DOS4_RUN_MANUAL;
            } else if (IsDoser4RunRequestActiv() && IsDoser4RequestAuto()) {
                ResetDoser4RequestAuto();
                SetDoser4PreviousState();
                doser4_state = DOS4_RUN_AUTO;
            }// End of else if...
            break;
            //
            //----------------------------------- operate doser 4 manualy
            //
        case DOS4_RUN_MANUAL:
            Doser4MotorFanOn();
            if (!IsDoser4RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser4RunRequestActiv()) ResetDoser4PreviousState();
                doser4_state = DOS4_STOP;
            } else {
                Drive4Run();
                dos4_set_value = dos4_ref;
            }// End of else...
            break;
            //
            //----------------------------------- operate doser 4 with mixer
            //
        case DOS4_RUN_AUTO:
            Doser4MotorFanOn();
            if (!IsDoser4RunRequestActiv() || IsMixerRawLevelMaximumAlarmActiv()) {
                if (!IsDoser4RunRequestActiv()) ResetDoser4PreviousState();
                doser4_state = DOS4_STOP;
            } else if (!IsMixerLoadRequestActiv()) {
                ResetDoser4ActivState();
                Drive4Stop();
                dos4_set_value = 0U;
            } else if (IsMixerLoadRequestActiv()) {
                SetDoser4ActivState();
                Drive4Run();
                dos4_set_value = dos4_ref;
            }// End of else if...
            break;
            //
            //----------------------------------- doser 4 drive error
            //
        case DOS4_ERROR:
            if (IsTasterResetPressed() && !IsDoser4DriveAlarmActiv()) {
                if (IsDoser4PreviousStateOn() && IsDoser4ManualSelected()) {
                    doser4_state = DOS4_RUN_MANUAL;
                } else if (IsDoser4PreviousStateOn() && IsDoser4AutoSelected()) {
                    doser4_state = DOS4_RUN_AUTO;
                } else doser4_state = DOS4_STOP;
            }// End of else...
            Doser4MotorFanOff();
            Drive4Stop();
            dos4_set_value = 0U;
            dos4_pcnt = 0;
            break;
    }// End of switch
}// End of process doser 4
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process mixer">

void ProcessMixer(void) {
    //
    //------------------------------------------- check if control is switched on
    //
    if (!IsControlActiv()) {
        mixer_state = MIX_STOP;
        return;
    }// End of if...
    //
    //------------------------------------------- check cyclon raw level
    //
    if (IsCyclonLevelMaxInputActiv()) {
        if (cyclon_pcnt == 0) {
            SetCyclonAlarmDelayTimer();
            SetMillStopDelayTimer();
            ++cyclon_pcnt;
        } else if (cyclon_pcnt == 1) {
            if (IsCyclonAlarmDelayTimerExpired()) SetCyclonMaxLevelAlarm();
            if (IsMillStopDelayTimerExpired()) {
                SetMillDisabledAlarm();
                MillDisable();
            }// End of if...
        }// End of else if....
    } else {
        ResetMillDisabledAlarm();
        ResetCyclonMaxLevelAlarm();
        MillEnable();
        cyclon_pcnt = 0;
    }// End of else...
    //
    //------------------------------------------- check current mixer state
    //
    switch (mixer_state) {
            //
            //----------------------------------- if in STOP state, wait for
            //----------------------------------- switch on command
            //
        case MIX_STOP:
            if (IsTasterMixerOnOffPressed()) {
                mixer_state = MIX_RUN;
                SetMixerPreviousState();
                SetMixerStopDelayTimer();
                SetMixerLoadRequest();
                MixerOn();
            }// End of if...
            ResetMixerOvercurrentProtectionAlarm();
            ResetMixerRawLevelMaximumAlarm();
            ResetMixerRawLevelMinimumAlarm();
            ResetMixerSensorAlarm();
            ResetMixerLoadRequest();
            mixer_pcnt = 0;
            break;
            //
            //----------------------------------- mixer in RUN state
            //
        case MIX_RUN:
            if (IsTasterMixerOnOffPressed()) {//--- check  off command requested
                mixer_state = MIX_STOP;
                ResetMixerPreviousState();
                MixerOff();
                break;
            }// End of if...
            //
            //----------------------------------- check motor protection device
            //
            if (!IsMixerOvercurrentProtectionInputActiv()) {
                SetMixerOvercurrentProtectionAlarm();
                mixer_state = MIX_ERROR;
                break;
            }// End of if...
            //
            //----------------------------------- check is raw level minimum 
            //----------------------------------- sensor activ, if not check
            //----------------------------------- if cleaning mode activ, if not
            //----------------------------------- alarm raw level minimum
            //
            if (!IsMixerLevelMinInputActiv() && !IsCleaningModeActiv()) SetMixerRawLevelMinimumAlarm();
            //
            //----------------------------------- check for impossible sensors
            //----------------------------------- logic state and alarm error
            //
            if (!IsMixerLevelMinInputActiv() && IsMixerLevelMaxInputActiv()) {
                SetMixerSensorAlarm();
                mixer_state = MIX_ERROR;
                break;
            }// End of if...
            //
            //----------------------------------- if all ok till now, check for
            //----------------------------------- mixer start on and off conditions
            //----------------------------------- with regard to extruder state
            //
            if (IsExtruderStarted() && !IsMixerStateOn()) {
                SetMixerStopDelayTimer();
                SetMixerLoadRequest();
                MixerOn();
                mixer_pcnt = 0;
            } else if (!IsExtruderRunInputActiv() && IsMixerStopDelayTimerExpired()) {
                SetMixerRawLewelMaxAlarmDelayTimer();
                ResetMixerLoadRequest();
                MixerOff();
            } else if (IsExtruderRunInputActiv()) {
                SetMixerStopDelayTimer();
                MixerOn();
            }// End of else if...
            //
            //
            //
            if (IsMixerRawLevelMinimumAlarmActiv()) mixer_pcnt = 0;
            //
            //----------------------------------- automatic load control
            //----------------------------------- with regard to raw maximum 
            //----------------------------------- level sensor
            //
            if ((mixer_pcnt == 0) && IsMixerStateOn()) {
                if (IsMixerLevelMaxInputActiv()) {
                    SetMixerLoadingStopDelayTimer();
                    SetMixerRawLewelMaxAlarmDelayTimer();
                    ++mixer_pcnt;
                } else if (!IsMixerRawLevelMinimumAlarmActiv() && !IsMixerLevelMaxInputActiv() && IsMixerStateOn()) {
                    SetMixerLoadRequest();
                }// End of else...
            } else if (mixer_pcnt == 1) {
                if (IsMixerLoadingStopDelayTimerExpired()) ResetMixerLoadRequest();
                if (!IsMixerLevelMaxInputActiv()) {
                    SetMixerLoadRequest();
                    if (cleaning_air_cyc_cnt) --cleaning_air_cyc_cnt;
                    if (vibrator_cyc_cnt) --vibrator_cyc_cnt;
                    mixer_pcnt = 0;
                } else if (IsMixerRawLewelMaxAlarmDelayTimerExpired()) {
                    SetMixerRawLevelMaximumAlarm();
                    ResetMixerLoadRequest();
                    mixer_pcnt = 0;
                    //mixer_state = MIX_ERROR;
                }// End of else if...
            }// End of else if...
            if (IsCleaningAirCyclusCounterEmpty() && !IsCleaningAirDisabled()) {
                SetCleaningAirCyclusCounter();
                SetCleaningAirTimer();
                CleaningValveOn();
            } else if (IsCleaningValveOn() && IsCleaningAirTimerExpired()) {
                CleaningValveOff();
            }// End of else if...

            if (IsVibratorCyclusCounterEmpty() && !IsVibratorDisabled()) {
                SetVibratorCyclusCounter();
                SetVibratorTimer();
                VibratorOn();
            } else if (IsVibratorOn() && IsVibratorTimerExpired()) {
                VibratorOff();
            }// End of else if...
            break;
            //
            //----------------------------------- mixer in ERROR state
            //----------------------------------- wait for RESET or OFF command.
            //----------------------------------- for RESET mixer control is 
            //----------------------------------- restored to previous state
            //
        case MIX_ERROR:
            if (IsTasterMixerOnOffPressed()) {
                mixer_state = MIX_STOP;
            } else if (IsTasterResetPressed()) {
                if (IsMixerPreviousStateOn()) {
                    //
                    //--------------------------- disable loading after every
                    //--------------------------- error reset if previous
                    //--------------------------- mixer state was on
                    //
                    if (IsMixerLevelMaxInputActiv()) mixer_pcnt = 1;
                    else mixer_pcnt = 0;
                    mixer_state = MIX_RUN;
                    SetMixerRawLewelMaxAlarmDelayTimer();
                    MixerOn();
                } else mixer_state = MIX_STOP;
            }// End of else...            
            MixerOff();
            break;
    }// End of switch...
}//End of process mixer
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process vacum pump 1">

void ProcessVacumPump1(void) {
    //
    //
    //
    if (!IsControlActiv()) {
        vacum_pump1_state = VP1_OFF;
    }// End of if...
    //
    //------------------------------------------- select current vacum pump 1 state
    //
    switch (vacum_pump1_state) {
            //
            //-----------------------------------
            //
        case VP1_OFF:
            if (IsTasterVacumPump1OnOffPressed() && IsVacumPump1TimerExpired() && !IsCleaningModeActiv()) {
                SetVacumPump1PreviousState();
                vacum_pump1_state = VP1_RUN;
                //SetVacumPump1Timer(VACUM_PUMP_CONTROL_TIME);
            }// End of if...
            VacumPump1Off();
            VacumPump1VacumValveOff();
            ResetVacumPump1Ready();
            vacum_pump1_pcnt = 0;
            break;
            //
            //-----------------------------------
            //
        case VP1_IDLE:

            if (!IsVacumPump1OvercurrentProtectionInputActiv() || !IsVacumPump1ThermoProtectionInputActiv()) {
                if (!IsVacumPump1OvercurrentProtectionInputActiv()) SetVacumPump1OvercurrentProtectionAlarm();
                if (!IsVacumPump1ThermoProtectionInputActiv()) SetVacumPump1ThermalProtectionAlarm();
                vacum_pump1_state = VP1_ERROR;
                break;
            }// End of if....

            if (IsTasterVacumPump1OnOffPressed() || IsCleaningModeActiv()) {
                ResetVacumPump1PreviousState();
                vacum_pump1_state = VP1_OFF; // check control switch for start
                //SetVacumPump1Timer(VACUM_PUMP_CONTROL_TIME);
                vacum_pump1_pcnt = 0;
            }// End of if...
            if (vacum_pump1_pcnt == 0) {
                ResetVacumPump1Ready();
                VacumPump1VacumValveOff();
                VacumPump1Off();
                ++vacum_pump1_pcnt;
            } else if (vacum_pump1_pcnt == 1) {
                if (IsVacumPump1Requested()) {
                    vacum_pump1_state = VP1_RUN;
                    vacum_pump1_pcnt = 0;
                }// End of if...
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP1_RUN:

            if (!IsVacumPump1OvercurrentProtectionInputActiv() || !IsVacumPump1ThermoProtectionInputActiv()) {
                if (!IsVacumPump1OvercurrentProtectionInputActiv()) SetVacumPump1OvercurrentProtectionAlarm();
                if (!IsVacumPump1ThermoProtectionInputActiv()) SetVacumPump1ThermalProtectionAlarm();
                vacum_pump1_state = VP1_ERROR;
                break;
            }// End of if....

            if (IsTasterVacumPump1OnOffPressed() || IsCleaningModeActiv()) {
                ResetVacumPump1PreviousState();
                vacum_pump1_state = VP1_OFF; // check control switch for start
                SetVacumPump1Timer(VACUM_PUMP_CONTROL_TIME);
                break;
            }// End of if...
            if (vacum_pump1_pcnt == 0) {
                SetVacumPump1Timer(VACUM_PUMP_START_UP_TIME);
                VacumPump1VacumValveOff();
                VacumPump1On();
                ++vacum_pump1_pcnt;
            } else if ((vacum_pump1_pcnt == 1) && IsVacumPump1TimerExpired()) {
                SetVacumPump1Timer(VACUM_PUMP_CONTROL_TIME);
                VacumPump1VacumValveOn();
                ++vacum_pump1_pcnt;
            } else if ((vacum_pump1_pcnt == 2) && IsVacumPump1TimerExpired()) {
                if (!IsVP1VacumOkInputActiv()) SetVacumPump1VacumAlarm();
                ++vacum_pump1_pcnt;
            } else if (vacum_pump1_pcnt == 3) {
                if (!IsVacumPump1On()) {
                    vacum_pump1_pcnt = 0;
                } else {
                    VacumPump1VacumValveOn();
                    SetVacumPump1Timer(vp1_delay_time);
                    ResetVacumPump1Request();
                    SetVacumPump1Ready();
                    ++vacum_pump1_pcnt;
                }// End of else....
            } else if (vacum_pump1_pcnt == 4) {
                if ((feeder1_state == FEED1_CHARGING) \
                        || (feeder2_state == FEED2_CHARGING) \
                        || (feeder3_state == FEED3_CHARGING) \
                        || (feeder4_state == FEED4_CHARGING)) {
                    --vacum_pump1_pcnt;
                } else if (IsVacumPump1TimerExpired()) {
                    vacum_pump1_state = VP1_IDLE;
                    vacum_pump1_pcnt = 0;
                }// else VacumPump1VacumValveOff();
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP1_ERROR:
            if (IsTasterVacumPump1OnOffPressed()) {
                ResetVacumPump1PreviousState();
                vacum_pump1_state = VP1_OFF; // check control switch for start
            }// End of else...
            
            if (IsTasterResetPressed()) {
                vacum_pump1_state = VP1_RUN;
            }// End of if..
            
            vacum_pump1_pcnt = 0;
            VacumPump1Off();
            VacumPump1VacumValveOff();
            ResetVacumPump1Ready();
            break;
    }// End of switch...
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process vacum pump 2">

void ProcessVacumPump2(void) {
    //
    //
    //
    if (!IsControlActiv()) {
        vacum_pump2_state = VP2_OFF;
    }// End of if...
    //------------------------------------------- select current vacum pump 2 state
    switch (vacum_pump2_state) {
            //
            //-----------------------------------
            //
        case VP2_OFF:
            if (IsTasterVacumPump2OnOffPressed() && IsVacumPump2TimerExpired() && !IsCleaningModeActiv()) {
                SetVacumPump2PreviousState();
                vacum_pump2_state = VP2_RUN;
                //SetVacumPump2Timer(VACUM_PUMP_CONTROL_TIME);
            }// End of if...
            VacumPump2Off();
            //VacumPump2VacumValveOff();
            ResetVacumPump2Ready();
            vacum_pump2_pcnt = 0;
            break;
            //
            //-----------------------------------
            //
        case VP2_IDLE:
            if (IsTasterVacumPump2OnOffPressed() || IsCleaningModeActiv()) {
                ResetVacumPump2PreviousState();
                vacum_pump2_state = VP2_OFF; // check control switch for start
                SetVacumPump2Timer(VACUM_PUMP_CONTROL_TIME);
                vacum_pump2_pcnt = 0;
            }// End of if...
            if (vacum_pump2_pcnt == 0) {
                ResetVacumPump2Ready();
                VacumPump2Off();
                ++vacum_pump2_pcnt;
            } else if (vacum_pump2_pcnt == 1) {
                if (IsVacumPump2Requested()) {
                    vacum_pump2_state = VP2_RUN;
                    vacum_pump2_pcnt = 0;
                }// End of if...
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP2_RUN:
            if ((IsTasterVacumPump2OnOffPressed() && IsVacumPump2TimerExpired()) || IsCleaningModeActiv()) {
                ResetVacumPump2PreviousState();
                vacum_pump2_state = VP2_OFF; // check control switch for start
                SetVacumPump2Timer(VACUM_PUMP_CONTROL_TIME);
                break;
            }// End of if...
            if (vacum_pump2_pcnt == 0) {
                SetVacumPump2Timer(VACUM_PUMP_START_UP_TIME);
                VacumPump2On();
                ++vacum_pump2_pcnt;
            } else if ((vacum_pump2_pcnt == 1) && IsVacumPump2TimerExpired()) {
                if (!IsVP2VacumOkInputActiv()) SetVacumPump2VacumAlarm();
                ++vacum_pump2_pcnt;
            } else if (vacum_pump2_pcnt == 2) {
                SetVacumPump2Timer(vp2_delay_tmr);
                ResetVacumPump2Request();
                SetVacumPump2Ready();
                ++vacum_pump2_pcnt;
            } else if (vacum_pump2_pcnt == 3) {
                if ((feeder1_state == FEED1_CHARGING) \
                        || (feeder2_state == FEED2_CHARGING) \
                        || (feeder3_state == FEED3_CHARGING) \
                        || (feeder4_state == FEED4_CHARGING)) {
                    --vacum_pump2_pcnt;
                } else if (IsVacumPump2TimerExpired()) {
                    vacum_pump2_state = VP2_IDLE;
                    vacum_pump2_pcnt = 0;
                }// End of else if...
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP2_ERROR:
            if (IsTasterVacumPump2OnOffPressed() && IsVacumPump2TimerExpired()) {
                ResetVacumPump2PreviousState();
                vacum_pump2_state = VP2_OFF; // check control switch for start
            }// End of else...
            
            if (IsTasterResetPressed()) {
                vacum_pump2_state = VP2_RUN;
            }// End of if..
            
            vacum_pump2_pcnt = 0;
            VacumPump2Off();
            //VacumPump2VacumValveOff();
            ResetVacumPump2Ready();
            break;
    }// End of switch...
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="process vacum pump 3">

void ProcessVacumPump3(void) {//
    //
    //
    if (!IsControlActiv()) {
        vacum_pump3_state = VP3_OFF;
    }// End of if...

    //------------------------------------------- select current vacum pump 3 state
    switch (vacum_pump3_state) {
            //
            //-----------------------------------
            //
        case VP3_OFF:
            if (IsTasterVacumPump3OnOffPressed() && IsVacumPump3TimerExpired() && !IsCleaningModeActiv()) {
                SetVacumPump3PreviousState();
                vacum_pump3_state = VP3_RUN;
                //SetVacumPump3Timer(VACUM_PUMP_CONTROL_TIME);
            }// End of if...
            VacumPump3Off();
            //VacumPump3VacumValveOff();
            ResetVacumPump3Ready();
            vacum_pump3_pcnt = 0;
            break;
            //
            //-----------------------------------
            //
        case VP3_IDLE:
            if (IsTasterVacumPump3OnOffPressed() || IsCleaningModeActiv()) {
                ResetVacumPump3PreviousState();
                vacum_pump3_state = VP3_OFF; // check control switch for start
                SetVacumPump3Timer(VACUM_PUMP_CONTROL_TIME);
                vacum_pump3_pcnt = 0;
            }// End of if...
            if (vacum_pump3_pcnt == 0) {
                ResetVacumPump3Ready();
                VacumPump3Off();
                ++vacum_pump3_pcnt;
            } else if (vacum_pump3_pcnt == 1) {
                if (IsVacumPump3Requested()) {
                    vacum_pump3_state = VP3_RUN;
                    vacum_pump3_pcnt = 0;
                }// End of if...
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP3_RUN:
            if ((IsTasterVacumPump3OnOffPressed() && IsVacumPump3TimerExpired()) || IsCleaningModeActiv()) {
                ResetVacumPump3PreviousState();
                vacum_pump3_state = VP3_OFF; // check control switch for start
                SetVacumPump3Timer(VACUM_PUMP_CONTROL_TIME);
                break;
            }// End of if...
            if (vacum_pump3_pcnt == 0) {
                SetVacumPump3Timer(VACUM_PUMP_START_UP_TIME);
                VacumPump3On();
                ++vacum_pump3_pcnt;
            } else if ((vacum_pump3_pcnt == 1) && IsVacumPump3TimerExpired()) {
                if (!IsVP3VacumOkInputActiv()) SetVacumPump3VacumAlarm();
                ++vacum_pump3_pcnt;
            } else if (vacum_pump3_pcnt == 2) {
                SetVacumPump3Timer(vp3_delay_tmr);
                ResetVacumPump3Request();
                SetVacumPump3Ready();
                ++vacum_pump3_pcnt;
            } else if (vacum_pump3_pcnt == 3) {
                if ((feeder1_state == FEED1_CHARGING) \
                        || (feeder2_state == FEED2_CHARGING) \
                        || (feeder3_state == FEED3_CHARGING) \
                        || (feeder4_state == FEED4_CHARGING)) {
                    --vacum_pump3_pcnt;
                } else if (IsVacumPump3TimerExpired()) {
                    vacum_pump3_state = VP3_IDLE;
                    vacum_pump3_pcnt = 0;
                }// End of else if...
            }// End of else if...
            break;
            //
            //-----------------------------------
            //
        case VP3_ERROR:
            if (IsTasterVacumPump3OnOffPressed() && IsVacumPump3TimerExpired()) {
                ResetVacumPump3PreviousState();
                vacum_pump3_state = VP3_OFF; // check control switch for start
            }// End of else...
            
            if (IsTasterResetPressed()) {
                vacum_pump3_state = VP3_RUN;
            }// End of if..
            
            vacum_pump3_pcnt = 0;
            VacumPump3Off();
            //VacumPump3VacumValveOff();
            ResetVacumPump3Ready();
            break;
    }// End of switch...
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="check data process timer">

void CheckControlProcessTimer(void) {
    //
    //------------------------------------------- wait for 10ms system clock
    //
    if (!PROCESS_CTRL_TMR_10MS_CLK) return; /*wait for system clock*/
    else {
        PROCESS_CTRL_TMR_10MS_CLK = FALSE; /*clear system clock flag*/
        if (++clk_1s < 100) return; /* check for 1 second period*/
        else clk_1s = 0;
    }// End of else...
    //
    //------------------------------------------- vacum pump 1 timer
    //
    if (vp1_tmr) {
        --vp1_tmr;
        VP1_TEX = FALSE;
    } else VP1_TEX = TRUE;
    //
    //------------------------------------------- vacum pump 2 timer
    //
    if (vp2_tmr) {
        --vp2_tmr;
        VP2_TEX = FALSE;
    } else VP2_TEX = TRUE;
    //
    //------------------------------------------- vacum pump 3 timer
    //
    if (vp3_tmr) {
        --vp3_tmr;
        VP3_TEX = FALSE;
    } else VP3_TEX = TRUE;
    //
    //------------------------------------------- feeder 1 charging cyclus timer
    //
    if (feed1_cyc_tmr) {
        --feed1_cyc_tmr;
        FEED1_CYC_TEX = FALSE;
    } else FEED1_CYC_TEX = TRUE;
    //
    //------------------------------------------- feeder 2 charging cyclus timer
    //
    if (feed2_cyc_tmr) {
        --feed2_cyc_tmr;
        FEED2_CYC_TEX = FALSE;
    } else FEED2_CYC_TEX = TRUE;
    //
    //------------------------------------------- feeder 3 charging cyclus timer
    //
    if (feed3_cyc_tmr) {
        --feed3_cyc_tmr;
        FEED3_CYC_TEX = FALSE;
    } else FEED3_CYC_TEX = TRUE;
    //
    //------------------------------------------- feeder 4 charging cyclus timer
    //
    if (feed4_cyc_tmr) {
        --feed4_cyc_tmr;
        FEED4_CYC_TEX = FALSE;
    } else FEED4_CYC_TEX = TRUE;
    //
    //------------------------------------------- power control timer
    //
    if (control_tmr) {
        --control_tmr;
        CONTROL_TEX = FALSE;
    } else CONTROL_TEX = TRUE;
    //
    //------------------------------------------- mixer on time after extruder
    //------------------------------------------- stop signal
    //
    if (mixer_stop_delay_tmr) {
        --mixer_stop_delay_tmr;
        MIXER_STOP_DELAY_TEX = FALSE;
    } else MIXER_STOP_DELAY_TEX = TRUE;
    //
    //------------------------------------------- mixer alarm timer after
    //------------------------------------------- maximum raw level reached
    //
    if (mix_raw_max_alarm_delay_tmr) {
        --mix_raw_max_alarm_delay_tmr;
        MIXER_RAW_LEVEL_MAX_ALARM_DELAY_TEX = FALSE;
    } else MIXER_RAW_LEVEL_MAX_ALARM_DELAY_TEX = TRUE;
    //
    //------------------------------------------- mixer load stop delay timer
    //
    if (mixer_load_stop_delay_tmr) {
        --mixer_load_stop_delay_tmr;
        MIXER_LOADING_STOP_DELAY_TEX = FALSE;
    } else MIXER_LOADING_STOP_DELAY_TEX = TRUE;
    //
    //------------------------------------------- vibrator working cyclus timer
    //
    if (vibrator_tmr) {
        --vibrator_tmr;
        VIBRATOR_TEX = FALSE;
    } else VIBRATOR_TEX = TRUE;
    //
    //------------------------------------------- cleaning air valve timer
    //
    if (cleaning_air_tmr) {
        --cleaning_air_tmr;
        CLEANING_AIR_TEX = FALSE;
    } else CLEANING_AIR_TEX = TRUE;
    //
    //------------------------------------------- cyclon raw level maximum alarm
    //
    if (cyclon_alarm_delay_tmr) {
        --cyclon_alarm_delay_tmr;
        CYCLON_TEX = FALSE;
    } else CYCLON_TEX = TRUE;
    //
    //------------------------------------------- mill disable delay timer
    //
    if (mill_stop_delay_tmr) {
        --mill_stop_delay_tmr;
        MILL_TEX = FALSE;
    } else MILL_TEX = TRUE;
    //
    //------------------------------------------- feeder 1 state change timer
    //
    if (feed1_change_tmr) {
        --feed1_change_tmr;
        FEED1_CHANGE_TEX = FALSE;
    } else FEED1_CHANGE_TEX = TRUE;
    //
    //------------------------------------------- feeder 2 state change timer
    //
    if (feed2_change_tmr) {
        --feed2_change_tmr;
        FEED2_CHANGE_TEX = FALSE;
    } else FEED2_CHANGE_TEX = TRUE;
    //
    //------------------------------------------- feeder 3 state change timer
    //
    if (feed3_change_tmr) {
        --feed3_change_tmr;
        FEED3_CHANGE_TEX = FALSE;
    } else FEED3_CHANGE_TEX = TRUE;
    //
    //------------------------------------------- feeder 4 state change timer
    //
    if (feed4_change_tmr) {
        --feed4_change_tmr;
        FEED4_CHANGE_TEX = FALSE;
    } else FEED4_CHANGE_TEX = TRUE;

}// </editor-fold>

//
//----------------------------------------------- end of file
//

