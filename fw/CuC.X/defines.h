/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *               Softverske definicije tipova varijabli
 *
 *******************************************************************************
 * Ime fajla:       typedefs.h
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
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char byte; // 8-bit
typedef unsigned int word; // 16-bit
typedef unsigned long dword; // 32-bit

typedef union _BYTE {
    byte _byte;
    struct {
        unsigned b0 : 1;
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
    };
} BYTE;

typedef union _WORD {
    word _word;
    struct {
        byte byte0;
        byte byte1;
    };
    struct {
        BYTE Byte0;
        BYTE Byte1;
    };
    struct {
        BYTE low_byte;
        BYTE high_byte;
    };
    struct {
        unsigned char lsb;
        unsigned char msb;
    };
    struct {
        byte v[2];
    };
} WORD;

#define LSB(a)      ((a).v[0])
#define MSB(a)      ((a).v[1])

typedef union _DWORD {
    dword _dword;
    struct {
        byte byte0;
        byte byte1;
        byte byte2;
        byte byte3;
    };
    struct {
        word word0;
        word word1;
    };
    struct {
        BYTE Byte0;
        BYTE Byte1;
        BYTE Byte2;
        BYTE Byte3;
    };
    struct {
        WORD Word0;
        WORD Word1;
    };
    struct {
        byte v[4];
    }; 
} DWORD;

#define LOWER_LSB(a)    ((a).v[0])
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])

typedef void(*pFunc)(void);

typedef union _POINTER {
    struct {
        byte bLow;
        byte bHigh;
    };
    word _word;
    //pFunc _pFunc;           		// Usage: ptr.pFunc(); Init: ptr.pFunc = &<Function>;
    byte* bRam; // Ram byte pointer: 2 bytes pointer pointing
    // to 1 byte of data
    word* wRam; // Ram word poitner: 2 bytes poitner pointing
    // to 2 bytes of data
    rom byte* bRom; // Size depends on compiler setting
    rom word* wRom;
    //rom near byte* nbRom;        	// Near = 2 bytes pointer
    //rom near word* nwRom;
    //rom far byte* fbRom;         	// Far = 3 bytes pointer
    //rom far word* fwRom;
} POINTER;

typedef union _DEVICE_STATE {
    byte _byte;
    struct {
        unsigned feeder1 : 1;
        unsigned feeder2 : 1;
        unsigned feeder3 : 1;
        unsigned feeder4 : 1;
        unsigned mixer : 1;
        unsigned vacum_pump1 : 1;
        unsigned vacum_pump2 : 1;
        unsigned vacum_pump3 : 1;
    };
} DEVICE_STATE;

typedef enum _BOOL {
    FALSE = 0, TRUE
} BOOL;
//
//****** D E F I N I C I J E    O P C E  *************************************/
//
#define	HIGH            1
#define	LOW             0
#define	IN              1
#define	OUT             0
#define	INPUT           1
#define	OUTPUT          0
#define INPUT_PIN       1
#define OUTPUT_PIN      0
#define INPUT_PORT      0xff
#define OUTPUT_PORT     0x00
#define PIN_HIGH        1
#define PIN_LOW         0
#define	YES             1
#define NOT             0
#define	TRUE            1
#define	FALSE           0
//
//
//********   D E F I N I C I J E   R E L A Y  O U T P U T ********************/
//
//
#define VacumPump1On()              (VP1_MOTOR = HIGH)
#define VacumPump1Off()             (VP1_MOTOR = LOW)
#define IsVacumPump1On()            (VP1_MOTOR == HIGH)
#define VacumPump2On()              (VP2_MOTOR = HIGH)
#define VacumPump2Off()             (VP2_MOTOR = LOW)
#define IsVacumPump2On()            (VP2_MOTOR == HIGH)
#define VacumPump3On()              (VP3_MOTOR = HIGH)
#define VacumPump3Off()             (VP3_MOTOR = LOW)
#define IsVacumPump3On()            (VP3_MOTOR == HIGH)
#define Doser4MotorFanOn()          (DOSER4_MOTOR_FAN = HIGH)
#define Doser4MotorFanOff()         (DOSER4_MOTOR_FAN = LOW)
#define IsDoser4MotorFanOn()        (DOSER4_MOTOR_FAN == HIGH)
#define VacumPump1VacumValveOn()    (VP1_VACUM_VALVE = HIGH)
#define VacumPump1VacumValveOff()   (VP1_VACUM_VALVE = LOW)
#define IsVacumPump1VacumValveOn()  (VP1_VACUM_VALVE == HIGH)
#define MixerOn()                   (MIXER_MOTOR = HIGH)
#define MixerOff()                  (MIXER_MOTOR = LOW)
#define IsMixerStateOn()            (MIXER_MOTOR == HIGH)
#define DrivePowerOn()              (DRIVE_POWER_CTRL = HIGH)
#define DrivePowerOff()             (DRIVE_POWER_CTRL = LOW)
#define IsDrivePowerOn()            (DRIVE_POWER_CTRL == HIGH)
#define VibratorOn()                (VIBRATOR = HIGH)
#define VibratorOff()               (VIBRATOR = LOW)
#define IsVibratorOn()              (VIBRATOR == HIGH)
#define Feeder1VacumValveOn()       (FEEDER1_VACUM_VALVE = HIGH)
#define Feeder1VacumValveOff()      (FEEDER1_VACUM_VALVE = LOW)
#define IsFeeder1VacumValveOn()     (FEEDER1_VACUM_VALVE == HIGH)
#define Feeder2VacumValveOn()       (FEEDER2_VACUM_VALVE = HIGH)
#define Feeder2VacumValveOff()      (FEEDER2_VACUM_VALVE = LOW)
#define IsFeeder2VacumValveOn()     (FEEDER2_VACUM_VALVE == HIGH)
#define Feeder3VacumValveOn()       (FEEDER3_VACUM_VALVE = HIGH)
#define Feeder3VacumValveOff()      (FEEDER3_VACUM_VALVE = LOW)
#define IsFeeder3VacumValveOn()     (FEEDER3_VACUM_VALVE == HIGH)
#define Feeder4VacumValveOn()       (FEEDER4_VACUM_VALVE = HIGH)
#define Feeder4VacumValveOff()      (FEEDER4_VACUM_VALVE = LOW)
#define IsFeeder4VacumValveOn()     (FEEDER4_VACUM_VALVE == HIGH)
#define CleaningValveOn()           (CLEANING_VALVE = HIGH)
#define CleaningValveOff()          (CLEANING_VALVE = LOW)
#define IsCleaningValveOn()         (CLEANING_VALVE == HIGH)
#define MillEnable()                (MILL_ENABLE = HIGH)
#define MillDisable()               (MILL_ENABLE = LOW)
#define IsMillEnabled()             (MILL_ENABLE == HIGH)
#define AlarmOn()                   (ALARM = HIGH)
#define AlarmOff()                  (ALARM = LOW)
#define IsAlarmOn()                 (ALARM == HIGH)
#define Output1Clock()              (OUTPUT1_CLK = HIGH, OUTPUT1_CLK = LOW)
#define Output2Clock()              (OUTPUT2_CLK = HIGH, OUTPUT2_CLK = LOW)
//
//
//*********   D E F I N I C I J E   D I G I T A L   O U T P U T   ************/
//
//
#define DigitalOutputClear()        (digital_output_0_7._byte = 0xff)
#define Drive1Run()                 (DRIVE_1_RUN = LOW)
#define Drive1Stop()                (DRIVE_1_RUN = HIGH)
#define IsDrive1Running()           (DRIVE_1_RUN == LOW)
#define Drive2Run()                 (DRIVE_2_RUN = LOW)
#define Drive2Stop()                (DRIVE_2_RUN = HIGH)
#define IsDrive2Running()           (DRIVE_2_RUN == LOW)
#define Drive3Run()                 (DRIVE_3_RUN = LOW)
#define Drive3Stop()                (DRIVE_3_RUN = HIGH)
#define IsDrive3Running()           (DRIVE_3_RUN == LOW)
#define Drive4Run()                 (DRIVE_4_RUN = LOW)
#define Drive4Stop()                (DRIVE_4_RUN = HIGH)
#define IsDrive4Running()           (DRIVE_4_RUN == LOW)
#define DoutAux0Enable()            (DOUT_AUX0 = LOW)
#define DoutAux0Disable()           (DOUT_AUX0 = HIGH)
#define IsDoutAux0Enabled()         (DOUT_AUX0 == LOW)
#define DoutAux1Enable()            (DOUT_AUX1 = LOW)
#define DoutAux1Disable()           (DOUT_AUX1 = HIGH)
#define IsDoutAux1Enabled()         (DOUT_AUX1 == LOW)
#define DoutAux2Enable()            (DOUT_AUX2 = LOW)
#define DoutAux2Disable()           (DOUT_AUX2 = HIGH)
#define IsDoutAux2Enabled()         (DOUT_AUX2 == LOW)
#define DoutAux3Enable()            (DOUT_AUX3 = LOW)
#define DoutAux3Disable()           (DOUT_AUX3 = HIGH)
#define IsDoutAux3Enabled()         (DOUT_AUX3 == LOW)
#define DigitalOutputClock()        (DOUT_CLK = HIGH, DOUT_CLK = LOW)
//
//
//*********   D E F I N I C I J E   D I G I T A L   I N P U T   **************/
//
//
#define IsVacumPump1OvercurrentProtectionInputActiv()   (VP1_CURRENT_PROT == HIGH)
#define IsVacumPump2OvercurrentProtectionInputActiv()   (VP2_CURRENT_PROT == HIGH)
#define IsVacumPump3OvercurrentProtectionInputActiv()   (VP3_CURRENT_PROT == HIGH)
#define IsVacumPump1ThermoProtectionInputActiv()    (VP1_THERMO_PROT == HIGH)
#define IsVacumPump2ThermoProtectionInputActiv()    (VP2_CURRENT_PROT == HIGH)
#define IsVacumPump3ThermoProtectionInputActiv()    (VP3_CURRENT_PROT == HIGH)
#define IsMixerOvercurrentProtectionInputActiv()    (MIKSER_CURRENT_PROT == HIGH)
#define IsEmergencySwitchInputActiv()               (EMERGENCY == HIGH)
#define IsExtruderRunInputActiv()                   (EXTRUDER_RUN == HIGH)
#define IsDrive1OkInputActiv()                      (DRIVE1_OK == HIGH)
#define IsDrive2OkInputActiv()                      (DRIVE2_OK == HIGH)
#define IsDrive3OkInputActiv()                      (DRIVE3_OK == HIGH)
#define IsDrive4OkInputActiv()                      (DRIVE4_OK == HIGH)
#define IsMixerLevelMinInputActiv()                 (MIX_SEN_MIN == HIGH)
#define IsMixerLevelMaxInputActiv()                 (MIX_SEN_MAX == HIGH)
#define IsCyclonLevelMaxInputActiv()                (CYC_SEN_MAX == HIGH)
#define IsFeeder1LevelMaxInputActiv()               (FEEDER1_SEN_MAX == HIGH)
#define IsFeeder1RequestInputActiv()                (FEEDER1_SEN_REQ == HIGH)
#define IsFeeder1LevelMinInputActiv()               (FEEDER1_SEN_MIN == HIGH)
#define IsFeeder2LevelMaxInputActiv()               (FEEDER2_SEN_MAX == HIGH)
#define IsFeeder2RequestInputActiv()                (FEEDER2_SEN_REQ == HIGH)
#define IsFeeder2LevelMinInputActiv()               (FEEDER2_SEN_MIN == HIGH)
#define IsFeeder3LevelMaxInputActiv()               (FEEDER3_SEN_MAX == HIGH)
#define IsFeeder3RequestInputActiv()                (FEEDER3_SEN_REQ == HIGH)
#define IsFeeder3LevelMinInputActiv()               (FEEDER3_SEN_MIN == HIGH)
#define IsFeeder4LevelMaxInputActiv()               (FEEDER4_SEN_MAX == HIGH)
#define IsFeeder4RequestInputActiv()                (FEEDER4_SEN_REQ == HIGH)
#define IsFeeder4LevelMinInputActiv()               (FEEDER4_SEN_MIN == HIGH)
#define IsVP1VacumOkInputActiv()                    (VP1_VACUM_OK == HIGH)
#define IsVP2VacumOkInputActiv()                    (VP2_VACUM_OK == HIGH)
#define IsVP3VacumOkInputActiv()                    (VP3_VACUM_OK == HIGH)
#define ResetMikserCyclusCounter()                  (mix_cyc_cnt = 0)
//
//
//*********   D E F I N I C I J E   P R O C E S   C O N T R O L  *************/
//
//
#define FEEDER1_MIN_LEVEL_CYCLUS_COUNTER        2   // max cyclus time for minimum level to bee filled
#define FEEDER2_MIN_LEVEL_CYCLUS_COUNTER        2   // max cyclus time for minimum level to bee filled
#define FEEDER3_MIN_LEVEL_CYCLUS_COUNTER        2   // max cyclus time for minimum level to bee filled
#define FEEDER4_MIN_LEVEL_CYCLUS_COUNTER        2   // max cyclus time for minimum level to bee filled

#define FEEDER_STATE_CHANGE_TIME                5   // 3 sec min time for on off controll

#define VACUM_PUMP_CONTROL_TIME                 3   // 3 sec min time for on off controll
#define VACUM_PUMP_START_UP_TIME                5   // 5 sec for vacum pump to build vacum
#define VACUM_PUMP_IDLE_TIME                    10  // 10 sec vacum pump idle time before shutdown

#define CONTROL_START_UP_TIME                   3   // 3 sec for delayed mains power on
#define CONTROL_STAB_TIME                       10  // 10 sec for circuits stabilising

#define INPUT_DEBOUNCE_TIME                    100  // 100 ms input state change time for input change sevice
//
//----------------------------------------------- feeder 1 macros
//
#define SetFeeder1CyclusTimer()                 (feed1_cyc_tmr = feed1_cyc_time, FEED1_CYC_TEX = FALSE)
#define IsFeeder1CyclusTimeExpired()            (FEED1_CYC_TEX == TRUE)
#define SetFeeder1ControlState()                (control1.feeder1 = TRUE)
#define ResetFeeder1PreviousState()             (control1.feeder1 = FALSE)
#define IsFeeder1PreviousStateOn()              (control1.feeder1 == TRUE)
#define SetFeeder1RawLevelMinAlarm()            (FEED1_ALARM_RAW_MIN = TRUE)
#define ResetFeeder1RawLevelMinAlarm()          (FEED1_ALARM_RAW_MIN = FALSE)
#define IsFeeder1RawLevelMinAlarmActiv()        (FEED1_ALARM_RAW_MIN == TRUE)
#define SetFeeder1FillingCycleFailAlarm()       (FEED1_ALARM_CYC_FAIL = TRUE)
#define ResetFeeder1FillingCycleFailAlarm()     (FEED1_ALARM_CYC_FAIL = FALSE)
#define IsFeeder1FillingCycleFyilAlarmActiv()   (FEED1_ALARM_CYC_FAIL == TRUE)
#define SetFeeder1SensorAlarm()                 (FEED1_ALARM_SENSOR = TRUE)
#define ResetFeeder1SensorAlarm()               (FEED1_ALARM_SENSOR = FALSE)
#define IsFeeder1SensorAlarmActiv()             (FEED1_ALARM_SENSOR == TRUE)
#define ResetFeeder1Signals()                   (SKC_SIG_FEEDER1_ON = FALSE, \
                                                 SKC_SIG_FEEDER1_FULL = FALSE, \
                                                 SKC_SIG_FEEDER1_MIN = FALSE, \
                                                 SKC_SIG_FEEDER1_REQUEST = FALSE)
#define SetFeeder1ChangeTimer()                 (feed1_change_tmr = FEEDER_STATE_CHANGE_TIME, FEED1_CHANGE_TEX = FALSE)
#define IsFeeder1ChangeTimerExpired()           (FEED1_CHANGE_TEX == TRUE)
#define SetFeeder1FirstCycle()                  (FEED1_FIRST_CYC = TRUE)
#define ResetFeeder1FirstCycle()                (FEED1_FIRST_CYC = FALSE)
#define IsFeeder1FirstCycle()                   (FEED1_FIRST_CYC == TRUE)
//
//----------------------------------------------- feeder 2 macros
//
#define SetFeeder2CyclusTimer()                 (feed2_cyc_tmr = feed2_cyc_time, FEED2_CYC_TEX = FALSE)
#define IsFeeder2CyclusTimeExpired()            (FEED2_CYC_TEX == TRUE)
#define SetFeeder2PreviousState()               (control1.feeder2 = TRUE)
#define ResetFeeder2PreviousState()             (control1.feeder2 = FALSE)
#define IsFeeder2PreviousStateOn()              (control1.feeder2 == TRUE)
#define SetFeeder2RawLevelMinAlarm()            (FEED2_ALARM_RAW_MIN = TRUE)
#define ResetFeeder2RawLevelMinAlarm()          (FEED2_ALARM_RAW_MIN = FALSE)
#define IsFeeder2RawLevelMinAlarmActiv()        (FEED2_ALARM_RAW_MIN == TRUE)
#define SetFeeder2FillingCycleFailAlarm()       (FEED2_ALARM_CYC_FAIL = TRUE)
#define ResetFeeder2FillingCycleFailAlarm()     (FEED2_ALARM_CYC_FAIL = FALSE)
#define IsFeeder2FillingCycleFyilAlarmActiv()   (FEED2_ALARM_CYC_FAIL == TRUE)
#define SetFeeder2SensorAlarm()                 (FEED2_ALARM_SENSOR = TRUE)
#define ResetFeeder2SensorAlarm()               (FEED2_ALARM_SENSOR = FALSE)
#define IsFeeder2SensorAlarmActiv()             (FEED2_ALARM_SENSOR == TRUE)
#define ResetFeeder2Signals()                   (SKC_SIG_FEEDER2_ON = FALSE, \
                                                 SKC_SIG_FEEDER2_FULL = FALSE, \
                                                 SKC_SIG_FEEDER2_MIN = FALSE, \
                                                 SKC_SIG_FEEDER2_REQUEST = FALSE)
#define SetFeeder2ChangeTimer()                 (feed2_change_tmr = FEEDER_STATE_CHANGE_TIME, FEED2_CHANGE_TEX = FALSE)
#define IsFeeder2ChangeTimerExpired()           (FEED2_CHANGE_TEX == TRUE)
#define SetFeeder2FirstCycle()                  (FEED2_FIRST_CYC = TRUE)
#define ResetFeeder2FirstCycle()                (FEED2_FIRST_CYC = FALSE)
#define IsFeeder2FirstCycle()                   (FEED2_FIRST_CYC == TRUE)
//
//----------------------------------------------- feeder 3 macros
//
#define SetFeeder3CyclusTimer()                  (feed3_cyc_tmr = feed3_cyc_time, FEED3_CYC_TEX = FALSE)
#define IsFeeder3CyclusTimeExpired()            (FEED3_CYC_TEX == TRUE)
#define SetFeeder3PreviousState()               (control1.feeder3 = TRUE)
#define ResetFeeder3PreviousState()             (control1.feeder3 = FALSE)
#define IsFeeder3PreviousStateOn()              (control1.feeder3 == TRUE)
#define SetFeeder3RawLevelMinAlarm()            (FEED3_ALARM_RAW_MIN = TRUE)
#define ResetFeeder3RawLevelMinAlarm()          (FEED3_ALARM_RAW_MIN = FALSE)
#define IsFeeder3RawLevelMinAlarmActiv()        (FEED3_ALARM_RAW_MIN == TRUE)
#define SetFeeder3FillingCycleFailAlarm()       (FEED3_ALARM_CYC_FAIL = TRUE)
#define ResetFeeder3FillingCycleFailAlarm()     (FEED3_ALARM_CYC_FAIL = FALSE)
#define IsFeeder3FillingCycleFyilAlarmActiv()   (FEED3_ALARM_CYC_FAIL == TRUE)
#define SetFeeder3SensorAlarm()                 (FEED3_ALARM_SENSOR = TRUE)
#define ResetFeeder3SensorAlarm()               (FEED3_ALARM_SENSOR = FALSE)
#define IsFeeder3SensorAlarmActiv()             (FEED3_ALARM_SENSOR == TRUE)
#define ResetFeeder3Signals()                   (SKC_SIG_FEEDER3_ON = FALSE, \
                                                 SKC_SIG_FEEDER3_FULL = FALSE, \
                                                 SKC_SIG_FEEDER3_MIN = FALSE, \
                                                 SKC_SIG_FEEDER3_REQUEST = FALSE)
#define SetFeeder3ChangeTimer()                 (feed3_change_tmr = FEEDER_STATE_CHANGE_TIME, FEED3_CHANGE_TEX = FALSE)
#define IsFeeder3ChangeTimerExpired()           (FEED3_CHANGE_TEX == TRUE)
#define SetFeeder3FirstCycle()                  (FEED3_FIRST_CYC = TRUE)
#define ResetFeeder3FirstCycle()                (FEED3_FIRST_CYC = FALSE)
#define IsFeeder3FirstCycle()                   (FEED3_FIRST_CYC == TRUE)
//
//----------------------------------------------- feeder 4 macros
//
#define SetFeeder4CyclusTimer()                 (feed4_cyc_tmr = feed4_cyc_time, FEED4_CYC_TEX = FALSE)
#define IsFeeder4CyclusTimeExpired()            (FEED4_CYC_TEX == TRUE)
#define SetFeeder4PreviousState()               (control1.feeder4 = TRUE)
#define ResetFeeder4PreviousState()             (control1.feeder4 = FALSE)
#define IsFeeder4PreviousStateOn()              (control1.feeder4 == TRUE)
#define SetFeeder4RawLevelMinAlarm()            (FEED4_ALARM_RAW_MIN = TRUE)
#define ResetFeeder4RawLevelMinAlarm()          (FEED4_ALARM_RAW_MIN = FALSE)
#define IsFeeder4RawLevelMinAlarmActiv()        (FEED4_ALARM_RAW_MIN == TRUE)
#define SetFeeder4FillingCycleFailAlarm()       (FEED4_ALARM_CYC_FAIL = TRUE)
#define ResetFeeder4FillingCycleFailAlarm()     (FEED4_ALARM_CYC_FAIL = FALSE)
#define IsFeeder4FillingCycleFyilAlarmActiv()   (FEED4_ALARM_CYC_FAIL == TRUE)
#define SetFeeder4SensorAlarm()                 (FEED4_ALARM_SENSOR = TRUE)
#define ResetFeeder4SensorAlarm()               (FEED4_ALARM_SENSOR = FALSE)
#define IsFeeder4SensorAlarmActiv()             (FEED4_ALARM_SENSOR == TRUE)
#define ResetFeeder4Signals()                   (SKC_SIG_FEEDER4_ON = FALSE, \
                                                 SKC_SIG_FEEDER4_FULL = FALSE, \
                                                 SKC_SIG_FEEDER4_MIN = FALSE, \
                                                 SKC_SIG_FEEDER4_REQUEST = FALSE)
#define SetFeeder4ChangeTimer()                 (feed4_change_tmr = FEEDER_STATE_CHANGE_TIME, FEED4_CHANGE_TEX = FALSE)
#define IsFeeder4ChangeTimerExpired()           (FEED4_CHANGE_TEX == TRUE)
#define SetFeeder4FirstCycle()                  (FEED4_FIRST_CYC = TRUE)
#define ResetFeeder4FirstCycle()                (FEED4_FIRST_CYC = FALSE)
#define IsFeeder4FirstCycle()                   (FEED4_FIRST_CYC == TRUE)
//
//----------------------------------------------- vacum pump 1 macros
//
#define SetVacumPump1Request()                          (VP1_REQUEST = TRUE)
#define ResetVacumPump1Request()                        (VP1_REQUEST = FALSE)
#define IsVacumPump1Requested()                         (VP1_REQUEST == TRUE)
#define SetVacumPump1PreviousState()                    (control1.vacum_pump1 = TRUE)
#define ResetVacumPump1PreviousState()                  (control1.vacum_pump1 = FALSE)
#define IsVacumPump1PreviousStateOn()                   (control1.vacum_pump1 == TRUE)
#define SetVacumPump1Ready()                            (VP1_READY = TRUE)
#define ResetVacumPump1Ready()                          (VP1_READY = FALSE)
#define IsVacumPump1Ready()                             (VP1_READY == TRUE)
#define SetVacumPump1VacumAlarm()                       (VP1_VACUM_ALARM = TRUE)
#define ResetVacumPump1VacumAlarm()                     (VP1_VACUM_ALARM = FALSE)
#define IsVacumPump1VacumAlarmActiv()                   (VP1_VACUM_ALARM == TRUE)
#define IsVacumPump1TimerExpired()                      (VP1_TEX == TRUE)
#define SetVacumPump1ThermalProtectionAlarm()           (VP1_THERMAL_PROTECTION_ALARM = HIGH)
#define ResetVacumPump1ThermalProtectionAlarm()         (VP1_THERMAL_PROTECTION_ALARM = LOW)
#define IsVacumPump1ThermalProtectionAlarmActiv()       (VP1_THERMAL_PROTECTION_ALARM == HIGH)
#define SetVacumPump1OvercurrentProtectionAlarm()       (VP1_OVERCURRENT_PROTECTION_ALARM = HIGH)
#define ResetVacumPump1OvercurrentProtectionAlarm()     (VP1_OVERCURRENT_PROTECTION_ALARM = LOW)
#define IsVacumPump1OvercurrentProtectionAlarmActiv()   (VP1_OVERCURRENT_PROTECTION_ALARM == HIGH)
#define SetVacumPump1Timer(time)                        (vp1_tmr = time, VP1_TEX = FALSE)
#define IsVacumPump1TimerExpired()                      (VP1_TEX == TRUE)
//
//----------------------------------------------- vacum pump 2 macros
//
#define SetVacumPump2Request()                          (VP2_REQUEST = TRUE)
#define ResetVacumPump2Request()                        (VP2_REQUEST = FALSE)
#define IsVacumPump2Requested()                         (VP2_REQUEST == TRUE)
#define SetVacumPump2PreviousState()                    (control1.vacum_pump2 = TRUE)
#define ResetVacumPump2PreviousState()                  (control1.vacum_pump2 = FALSE)
#define IsVacumPump2PreviousStateOn()                   (control1.vacum_pump2 == TRUE)
#define SetVacumPump2Ready()                            (VP2_READY = TRUE)
#define ResetVacumPump2Ready()                          (VP2_READY = FALSE)
#define IsVacumPump2Ready()                             (VP2_READY == TRUE)
#define SetVacumPump2VacumAlarm()                       (VP2_VACUM_ALARM = TRUE)
#define ResetVacumPump2VacumAlarm()                     (VP2_VACUM_ALARM = FALSE)
#define IsVacumPump2VacumAlarmActiv()                   (VP2_VACUM_ALARM == TRUE)
#define IsVacumPump2TimerExpired()                      (VP2_TEX == TRUE)
#define SetVacumPump2ThermalProtectionAlarm()           (VP2_THERMAL_PROTECTION_ALARM = HIGH)
#define ResetVacumPump2ThermalProtectionAlarm()         (VP2_THERMAL_PROTECTION_ALARM = LOW)
#define IsVacumPump2ThermalProtectionAlarmActiv()       (VP2_THERMAL_PROTECTION_ALARM == HIGH)
#define SetVacumPump2OvercurrentProtectionAlarm()       (VP2_OVERCURRENT_PROTECTION_ALARM = HIGH)
#define ResetVacumPump2OvercurrentProtectionAlarm()     (VP2_OVERCURRENT_PROTECTION_ALARM = LOW)
#define IsVacumPump2OvercurrentProtectionAlarmActiv()   (VP2_OVERCURRENT_PROTECTION_ALARM == HIGH)
#define SetVacumPump2Timer(time)                        (vp2_tmr = time, VP2_TEX = FALSE)
#define IsVacumPump2TimerExpired()                      (VP2_TEX == TRUE)
//
//----------------------------------------------- vacum pump 3 macros
//
#define SetVacumPump3Request()                          (VP3_REQUEST = TRUE)
#define ResetVacumPump3Request()                        (VP3_REQUEST = FALSE)
#define IsVacumPump3Requested()                         (VP3_REQUEST == TRUE)
#define SetVacumPump3PreviousState()                    (control1.vacum_pump3 = TRUE)
#define ResetVacumPump3PreviousState()                  (control1.vacum_pump3 = FALSE)
#define IsVacumPump3PreviousStateOn()                   (control1.vacum_pump3 == TRUE)
#define SetVacumPump3Ready()                            (VP3_READY = TRUE)
#define ResetVacumPump3Ready()                          (VP3_READY = FALSE)
#define IsVacumPump3Ready()                             (VP3_READY == TRUE)
#define SetVacumPump3VacumAlarm()                       (VP3_VACUM_ALARM = TRUE)
#define ResetVacumPump3VacumAlarm()                     (VP3_VACUM_ALARM = FALSE)
#define IsVacumPump3VacumAlarmActiv()                   (VP3_VACUM_ALARM == TRUE)
#define IsVacumPump3TimerExpired()                      (VP3_TEX == TRUE)
#define SetVacumPump3ThermalProtectionAlarm()           (VP3_THERMAL_PROTECTION_ALARM = HIGH)
#define ResetVacumPump3ThermalProtectionAlarm()         (VP3_THERMAL_PROTECTION_ALARM = LOW)
#define IsVacumPump3ThermalProtectionAlarmActiv()       (VP3_THERMAL_PROTECTION_ALARM == HIGH)
#define SetVacumPump3OvercurrentProtectionAlarm()       (VP3_OVERCURRENT_PROTECTION_ALARM = HIGH)
#define ResetVacumPump3OvercurrentProtectionAlarm()     (VP3_OVERCURRENT_PROTECTION_ALARM = LOW)
#define IsVacumPump3OvercurrentProtectionAlarmActiv()   (VP3_OVERCURRENT_PROTECTION_ALARM == HIGH)
#define SetVacumPump3Timer(time)                        (vp3_tmr = time, VP3_TEX = FALSE)
#define IsVacumPump3TimerExpired()                      (VP3_TEX == TRUE)
//
//----------------------------------------------- cyclon macros
//
#define SetCyclonMaxLevelAlarm()                (CYCLON_ALARM = TRUE)
#define ResetCyclonMaxLevelAlarm()              (CYCLON_ALARM = FALSE)
#define IsCyclonMaxLevelAlarmActiv()            (CYCLON_ALARM == TRUE)
#define SetCyclonAlarmDelayTimer()              (cyclon_alarm_delay_tmr = cyclon_alarm_delay_time, CYCLON_TEX = FALSE)
#define IsCyclonAlarmDelayTimerExpired()        (CYCLON_TEX == TRUE)
#define SetMillStopDelayTimer()                 (mill_stop_delay_tmr = mill_stop_delay_time, MILL_TEX = FALSE)
#define IsMillStopDelayTimerExpired()           (MILL_TEX == TRUE)
#define SetMillDisabledAlarm()                  (MILL_ALARM = TRUE)
#define ResetMillDisabledAlarm()                (MILL_ALARM = FALSE)
#define IsMillDisabledAlarmActiv()              (MILL_ALARM == TRUE)
//
//----------------------------------------------- controler macros
//
#define ControlOn()                             (CONTROL_ON = TRUE)
#define ControlOff()                            (CONTROL_ON = FALSE)
#define IsControlActiv()                        (CONTROL_ON == TRUE)
#define SetControlStarting()                    (CONTROL_STATE = TRUE)
#define ResetControlStarting()                  (CONTROL_STATE = FALSE)
#define IsControlStartInProgress()              (CONTROL_STATE == TRUE)
#define SetControlTimer(time)                   (control_tmr = time, CONTROL_TEX = FALSE)
#define IsControlTimerExpired()                 (CONTROL_TEX == TRUE)
//
//----------------------------------------------- mixer macros
//
#define IsCleaningModeActiv()                   (SKC_SW_CLEANING_MODE == TRUE)
#define SetMixerRawLevelMinimumAlarm()          (MIXER_RAW_LEVEL_MIN_ALARM = TRUE)
#define ResetMixerRawLevelMinimumAlarm()        (MIXER_RAW_LEVEL_MIN_ALARM = FALSE)
#define IsMixerRawLevelMinimumAlarmActiv()      (MIXER_RAW_LEVEL_MIN_ALARM == TRUE)
#define SetMixerRawLevelMaximumAlarm()          (MIXER_RAW_LEVEL_MAX_ALARM = TRUE)
#define ResetMixerRawLevelMaximumAlarm()        (MIXER_RAW_LEVEL_MAX_ALARM = FALSE)
#define IsMixerRawLevelMaximumAlarmActiv()      (MIXER_RAW_LEVEL_MAX_ALARM == TRUE)
#define IsMixerStopDelayTimerExpired()          (MIXER_STOP_DELAY_TEX == TRUE)
#define SetMixerStopDelayTimer()                (mixer_stop_delay_tmr = mixer_stop_delay_time, MIXER_STOP_DELAY_TEX = FALSE)
#define IsMixerRawLewelMaxAlarmDelayTimerExpired()  (MIXER_RAW_LEVEL_MAX_ALARM_DELAY_TEX == TRUE)
#define SetMixerRawLewelMaxAlarmDelayTimer()    (mix_raw_max_alarm_delay_tmr = mix_raw_max_alarm_delay_time, MIXER_RAW_LEVEL_MAX_ALARM_DELAY_TEX = FALSE)
#define IsMixerLoadingStopDelayTimerExpired()   (MIXER_LOADING_STOP_DELAY_TEX == TRUE)
#define SetMixerLoadingStopDelayTimer()         (mixer_load_stop_delay_tmr = mixer_load_stop_delay_time, MIXER_LOADING_STOP_DELAY_TEX = FALSE)
#define SetMixerSensorAlarm()                   (MIXER_SENSOR_ALARM = TRUE)
#define ResetMixerSensorAlarm()                 (MIXER_SENSOR_ALARM = FALSE)
#define IsMixerSensorAlarmActiv()               (MIXER_SENSOR_ALARM == TRUE)
#define SetMixerOvercurrentProtectionAlarm()    (MIXER_OVERCURRENT_PROTECTION_ALARM = TRUE)
#define ResetMixerOvercurrentProtectionAlarm()  (MIXER_OVERCURRENT_PROTECTION_ALARM = FALSE)
#define IsMixerOvercurrentProtectionAlarmActiv()(MIXER_OVERCURRENT_PROTECTION_ALARM == TRUE)
#define SetMixerLoadRequest()                   (MIXER_LOAD_REQUEST = TRUE)
#define ResetMixerLoadRequest()                 (MIXER_LOAD_REQUEST = FALSE)
#define IsMixerLoadRequestActiv()               (MIXER_LOAD_REQUEST == TRUE)
#define SetMixerPreviousState()                 (control1.mixer = TRUE)
#define ResetMixerPreviousState()               (control1.mixer = FALSE)
#define IsMixerPreviousStateOn()                (control1.mixer == TRUE)
#define SetCleaningAirTimer()                   (cleaning_air_tmr = cleaning_air_time, CLEANING_AIR_TEX = FALSE)
#define IsCleaningAirTimerExpired()             (CLEANING_AIR_TEX == TRUE)
#define SetCleaningAirCyclusCounter()           (cleaning_air_cyc_cnt = cleaning_air_cyc)
#define IsCleaningAirCyclusCounterEmpty()       (cleaning_air_cyc_cnt == 0)
#define EnableCleaningAir()                     (CLEANING_AIR_DISABLE = FALSE)
#define DisableCleaningAir()                    (CLEANING_AIR_DISABLE = TRUE)
#define IsCleaningAirDisabled()                 (CLEANING_AIR_DISABLE == TRUE)
#define SetVibratorTimer()                      (vibrator_tmr = vibrator_time, VIBRATOR_TEX = FALSE)
#define IsVibratorTimerExpired()                (VIBRATOR_TEX == TRUE)
#define SetVibratorCyclusCounter()              (vibrator_cyc_cnt = vibrator_cyc)
#define IsVibratorCyclusCounterEmpty()          (vibrator_cyc_cnt == 0)
#define EnableVibrator()                        (VIBRATOR_DISABLE = FALSE)
#define DisableVibrator()                       (VIBRATOR_DISABLE = TRUE)
#define IsVibratorDisabled()                    (VIBRATOR_DISABLE == TRUE)
//
//----------------------------------------------- doser unit 1 macros
//
#define IsDoser1InActivState()                  (DOS1_ACTIV_STATE == HIGH)
#define SetDoser1ActivState()                   (DOS1_ACTIV_STATE = HIGH)
#define ResetDoser1ActivState()                 (DOS1_ACTIV_STATE = LOW)
#define SetDoser1PreviousState()                (DOS1_PREV_STATE = TRUE)
#define ResetDoser1PreviousState()              (DOS1_PREV_STATE = FALSE)
#define IsDoser1PreviousStateOn()               (DOS1_PREV_STATE == TRUE)
#define IsDoser1RunRequestActiv()               (DOS1_RUN_REQUEST == HIGH)
#define SetDoser1RunRequest()                   (DOS1_RUN_REQUEST = HIGH)
#define ResetDoser1RunRequest()                 (DOS1_RUN_REQUEST = LOW)
#define SetDoser1RequestManual()                (DOS1_REQUEST_MANUAL = TRUE)
#define ResetDoser1RequestManual()              (DOS1_REQUEST_MANUAL = FALSE)
#define IsDoser1RequestManual()                 (DOS1_REQUEST_MANUAL == TRUE)
#define SetDoser1RequestAuto()                  (DOS1_REQUEST_AUTO = TRUE)
#define ResetDoser1RequestAuto()                (DOS1_REQUEST_AUTO = FALSE)
#define IsDoser1RequestAuto()                   (DOS1_REQUEST_AUTO == TRUE)
#define SetDoser1DriveAlarm()                   (DOS1_ALARM = TRUE)
#define ResetDoser1DriveAlarm()                 (DOS1_ALARM = FALSE)
#define IsDoser1DriveAlarmActiv()               (DOS1_ALARM == TRUE)
//
//----------------------------------------------- doser unit 2 macros
//
#define IsDoser2InActivState()                  (DOS2_ACTIV_STATE == HIGH)
#define SetDoser2ActivState()                   (DOS2_ACTIV_STATE = HIGH)
#define ResetDoser2ActivState()                 (DOS2_ACTIV_STATE = LOW)
#define SetDoser2PreviousState()                (DOS2_PREV_STATE = TRUE)
#define ResetDoser2PreviousState()              (DOS2_PREV_STATE = FALSE)
#define IsDoser2PreviousStateOn()               (DOS2_PREV_STATE == TRUE)
#define IsDoser2RunRequestActiv()               (DOS2_RUN_REQUEST == HIGH)
#define SetDoser2RunRequest()                   (DOS2_RUN_REQUEST = HIGH)
#define ResetDoser2RunRequest()                 (DOS2_RUN_REQUEST = LOW)
#define SetDoser2RequestManual()                (DOS2_REQUEST_MANUAL = TRUE)
#define ResetDoser2RequestManual()              (DOS2_REQUEST_MANUAL = FALSE)
#define IsDoser2RequestManual()                 (DOS2_REQUEST_MANUAL == TRUE)
#define SetDoser2RequestAuto()                  (DOS2_REQUEST_AUTO = TRUE)
#define ResetDoser2RequestAuto()                (DOS2_REQUEST_AUTO = FALSE)
#define IsDoser2RequestAuto()                   (DOS2_REQUEST_AUTO == TRUE)
#define SetDoser2DriveAlarm()                   (DOS2_ALARM = TRUE)
#define ResetDoser2DriveAlarm()                 (DOS2_ALARM = FALSE)
#define IsDoser2DriveAlarmActiv()               (DOS2_ALARM == TRUE)
//
//----------------------------------------------- doser unit 3 macros
//
#define IsDoser3InActivState()                  (DOS3_ACTIV_STATE == HIGH)
#define SetDoser3ActivState()                   (DOS3_ACTIV_STATE = HIGH)
#define ResetDoser3ActivState()                 (DOS3_ACTIV_STATE = LOW)
#define SetDoser3PreviousState()                (DOS3_PREV_STATE = TRUE)
#define ResetDoser3PreviousState()              (DOS3_PREV_STATE = FALSE)
#define IsDoser3PreviousStateOn()               (DOS3_PREV_STATE == TRUE)
#define IsDoser3RunRequestActiv()               (DOS3_RUN_REQUEST == HIGH)
#define SetDoser3RunRequest()                   (DOS3_RUN_REQUEST = HIGH)
#define ResetDoser3RunRequest()                 (DOS3_RUN_REQUEST = LOW)
#define SetDoser3RequestManual()                (DOS3_REQUEST_MANUAL = TRUE)
#define ResetDoser3RequestManual()              (DOS3_REQUEST_MANUAL = FALSE)
#define IsDoser3RequestManual()                 (DOS3_REQUEST_MANUAL == TRUE)
#define SetDoser3RequestAuto()                  (DOS3_REQUEST_AUTO = TRUE)
#define ResetDoser3RequestAuto()                (DOS3_REQUEST_AUTO = FALSE)
#define IsDoser3RequestAuto()                   (DOS3_REQUEST_AUTO == TRUE)
#define SetDoser3DriveAlarm()                   (DOS3_ALARM = TRUE)
#define ResetDoser3DriveAlarm()                 (DOS3_ALARM = FALSE)
#define IsDoser3DriveAlarmActiv()               (DOS3_ALARM == TRUE)
//
//----------------------------------------------- doser unit 4 macros
//
#define IsDoser4InActivState()                  (DOS4_ACTIV_STATE == HIGH)
#define SetDoser4ActivState()                   (DOS4_ACTIV_STATE = HIGH)
#define ResetDoser4ActivState()                 (DOS4_ACTIV_STATE = LOW)
#define SetDoser4PreviousState()                (DOS4_PREV_STATE = TRUE)
#define ResetDoser4PreviousState()              (DOS4_PREV_STATE = FALSE)
#define IsDoser4PreviousStateOn()               (DOS4_PREV_STATE == TRUE)
#define IsDoser4RunRequestActiv()               (DOS4_RUN_REQUEST == HIGH)
#define SetDoser4RunRequest()                   (DOS4_RUN_REQUEST = HIGH)
#define ResetDoser4RunRequest()                 (DOS4_RUN_REQUEST = LOW)
#define SetDoser4RequestManual()                (DOS4_REQUEST_MANUAL = TRUE)
#define ResetDoser4RequestManual()              (DOS4_REQUEST_MANUAL = FALSE)
#define IsDoser4RequestManual()                 (DOS4_REQUEST_MANUAL == TRUE)
#define SetDoser4RequestAuto()                  (DOS4_REQUEST_AUTO = TRUE)
#define ResetDoser4RequestAuto()                (DOS4_REQUEST_AUTO = FALSE)
#define IsDoser4RequestAuto()                   (DOS4_REQUEST_AUTO == TRUE)
#define SetDoser4DriveAlarm()                   (DOS4_ALARM = TRUE)
#define ResetDoser4DriveAlarm()                 (DOS4_ALARM = FALSE)
#define IsDoser4DriveAlarmActiv()               (DOS4_ALARM == TRUE)
//
/*********  M A C R O S    M A I N _ H    ************************************/
//
//
#define SetEmergencyAlarm()                 (EMERGENCY_STATE = TRUE)
#define ResetEmergencyAlarm()               (EMERGENCY_STATE = FALSE)
#define IsEmergencyAlarmActiv()             (EMERGENCY_STATE == TRUE)
#define IsTasterStartPressed()              (TST_START_STATE == TRUE)
#define IsTasterStopPressed()               (TST_STOP_STATE == TRUE)
#define IsTasterResetPressed()              (TST_RESET_STATE == TRUE)
#define IsTasterProgramLoadPressed()        (TST_PG_LOAD_STATE == TRUE)
#define IsTasterProgramSavePressed()        (TST_PG_SAVE_STATE == TRUE)
#define IsTasterProgramUpPressed()          (TST_PG_UP_STATE == TRUE)
#define IsTasterProgramDownPressed()        (TST_PG_DN_STATE == TRUE)
#define IsBothPgmUpAndDownTasterPressed()   ((SKC_TST_PG_UP == TRUE) && (SKC_TST_PG_DN == TRUE))
#define IsTasterDoser1SpeedUpPressed()      (TST_DOSER1_SPEED_UP_STATE == TRUE)
#define IsTasterDoser1SpeedDownPressed()    (TST_DOSER1_SPEED_DN_STATE == TRUE)
#define IsTasterFeeder1OnOffPressed()       (TST_FEEDER1_ON_STATE == TRUE)
#define IsTasterDoser2SpeedUpPressed()      (TST_DOSER2_SPEED_UP_STATE == TRUE)
#define IsTasterDoser2SpeedDownPressed()    (TST_DOSER2_SPEED_DN_STATE == TRUE)
#define IsTasterFeeder2OnOffPressed()       (TST_FEEDER2_ON_STATE == TRUE)
#define IsTasterDoser3SpeedUpPressed()      (TST_DOSER3_SPEED_UP_STATE == TRUE)
#define IsTasterDoser3SpeedDownPressed()    (TST_DOSER3_SPEED_DN_STATE == TRUE)
#define IsTasterFeeder3OnOffPressed()       (TST_FEEDER3_ON_STATE == TRUE)
#define IsTasterDoser4SpeedUpPressed()      (TST_DOSER4_SPEED_UP_STATE == TRUE)
#define IsTasterDoser4SpeedDownPressed()    (TST_DOSER4_SPEED_DN_STATE == TRUE)
#define IsTasterFeeder4OnOffPressed()       (TST_FEEDER4_ON_STATE == TRUE)
#define IsTasterMixerOnOffPressed()         (TST_MIKSER_ON_STATE == TRUE)
#define IsDoser1ManualSelected()            (SKC_SW_DOSER1_MANUAL == LOW)
#define IsDoser1AutoSelected()              (SKC_SW_DOSER1_AUTO == LOW)
#define IsDoser2ManualSelected()            (SKC_SW_DOSER2_MANUAL == LOW)
#define IsDoser2AutoSelected()              (SKC_SW_DOSER2_AUTO == LOW)
#define IsDoser3ManualSelected()            (SKC_SW_DOSER3_MANUAL == LOW)
#define IsDoser3AutoSelected()              (SKC_SW_DOSER3_AUTO == LOW)
#define IsDoser4ManualSelected()            (SKC_SW_DOSER4_MANUAL == LOW)
#define IsDoser4AutoSelected()              (SKC_SW_DOSER4_AUTO == LOW)
#define IsTasterVacumPump1OnOffPressed()    (TST_VP1_ON_STATE == TRUE)
#define IsTasterVacumPump2OnOffPressed()    (TST_VP2_ON_STATE == TRUE)
#define IsTasterVacumPump3OnOffPressed()    (TST_VP3_ON_STATE == TRUE)
#define IsExtruderStarted()                 (EXTRUDER_STATE == TRUE)
#define SetExtruderStartSignal()            (EXTRUDER_STATE = TRUE)
#define ResetExtruderStartSignal()          (EXTRUDER_STATE = FALSE)
#define SetMenuVariable()                   (SETUP_DSP_VALUE = TRUE)
#define ResetMenuVariable()                 (SETUP_DSP_VALUE = FALSE)
#define IsMenuVariableActiv()               (SETUP_DSP_VALUE == TRUE)
#define SetProgramLoadedDisplay()           (DSP_PG_LOADED = TRUE)
#define ResetProgramLoadedDisplay()         (DSP_PG_LOADED = FALSE)
#define IsProgramLoadedDisplaySet()         (DSP_PG_LOADED == TRUE)
#define SetProgramSavedDisplay()            (DSP_PG_SAVED = TRUE)
#define ResetProgramSavedDisplay()          (DSP_PG_SAVED = FALSE)
#define IsProgramSavedDisplaySet()          (DSP_PG_SAVED == TRUE)
#define SetSystemInitDisplay()              (DSP_SYS_INIT = TRUE)
#define ResetSystemInitDisplay()            (DSP_SYS_INIT = FALSE)
#define IsSystemInitDisplaySet()            (DSP_SYS_INIT == TRUE)
#define SetDisplayTimer(time)               (dsp_tmr = time, DSP_TEX = FALSE)
#define IsDisplayTimerExpired()             (DSP_TEX == TRUE)
#define SetErrorDisplayTimer(time)          (err_dsp_tmr = time, ERR_DSP_TEX = FALSE)
#define IsErrorDisplayTimerExpired()        (ERR_DSP_TEX == TRUE)
#define SetProgramSelectTimer()             (program_select_tmr = 2000, PGM_SELECT_TEX = FALSE)
#define IsProgramSelectTimerExpired()       (PGM_SELECT_TEX == TRUE)
#define SetEepromErrorAlarm()               (EEPROM_FLAG = TRUE)
#define ResetEepromErrorAlarm()             (EEPROM_FLAG = FALSE)
#define IsEepromErrorAlarmActiv()           (EEPROM_FLAG == TRUE)
//
//
//*********   D E F I N I C I J E  C O N T R O L  C O M M   ******************/
//
//
#define NUL         0x00        // null character
#define EOT         0x04        // end of transmission
#define ACK         0x06        // acknowledge

#define NAK         0x15        // negative acknowledge
#define LF          0x0a        // line feed
#define CR          0x0d        // carriage return
#define ESC         0x1b        // escape
#define GET_CMD     0x3c        // '<'
#define SET_CMD     0x3e        // '>'
//
//----------------------------------------------- define error code
//
#define ERROR_DISPLAY_CLEAR         0xff
//
#define ERROR_COMM_BUSS             0x01
#define ERROR_EEPROM                0x02
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
#define ERROR_FEED1_SENSORS         0x41
#define ERROR_FEED2_SENSORS         0x42
#define ERROR_FEED3_SENSORS         0x43
#define ERROR_FEED4_SENSORS         0x44
#define ERROR_MIXER_SENSORS         0x45
//
#define ERROR_FEED1_MIN_LEVEL       0x51
#define ERROR_FEED2_MIN_LEVEL       0x52
#define ERROR_FEED3_MIN_LEVEL       0x53
#define ERROR_FEED4_MIN_LEVEL       0x54
#define ERROR_CYCLON_MAX_LEVEL      0x55
#define ERROR_CYCLON_MILL_DISABLED  0x56
#define ERROR_MIXER_RAW_LEVEL_MIN   0x57
#define ERROR_MIXER_RAW_LEVEL_MAX   0x58
//
#define ERROR_FEED1_CYC_CNT         0x61
#define ERROR_FEED2_CYC_CNT         0x62
#define ERROR_FEED3_CYC_CNT         0x63
#define ERROR_FEED4_CYC_CNT         0x64
//
//
/*********  M A C R O S   ******************************************************/
//
//
#define ControlCommReceiverEnable()         (RCSTA2bits.CREN2 = TRUE)
#define ControlCommReceiverDisable()        (RCSTA2bits.CREN2 = FALSE)
#define ControlCommRxAddressingEnable()     (RCSTA2bits.ADDEN2 = TRUE)
#define ControlCommRxAddressingDisable()    (RCSTA2bits.ADDEN2 = FALSE)
#define ControlCommRx9BitReception()        (RCSTA2bits.RX92 = TRUE)
#define ControlCommRx8BitReception()        (RCSTA2bits.RX92 = FALSE)
#define ControlCommTx9BitTransmission()     (TXSTA2bits.TX92 = TRUE)
#define ControlCommTx8BitTransmission()     (TXSTA2bits.TX92 = FALSE)
#define ControlCommTx9BitSendAddress()      (TXSTA2bits.TX9D2 = TRUE)
#define ControlCommTx9BitSendData()         (TXSTA2bits.TX9D2 = FALSE)
#define ControlCommTxInterruptEnable()      (PIE3bits.TX2IE = TRUE)
#define ControlCommTxInterruptDisable()     (PIE3bits.TX2IE = FALSE)
#define ControlCommSendSignalSet()          (CTRL_COMM_SIGNAL_SET = TRUE)
#define ControlCommSendSignalReset()        (CTRL_COMM_SIGNAL_SET = FALSE)
#define IsControlCommSendSignalSelected()   (CTRL_COMM_SIGNAL_SET == TRUE)
#define ControlCommGetSignalSet()           (CTRL_COMM_INPUTS_GET = TRUE)
#define ControlCommGetSignalReset()         (CTRL_COMM_INPUTS_GET = FALSE)
#define IsControlCommGetSignalSelected()    (CTRL_COMM_INPUTS_GET == TRUE)
#define SetControlCommReceivingInProgress() (CTRL_RX_PENDING = TRUE)
#define ResetControlCommReceivingInProgress()(CTRL_RX_PENDING = FALSE)
#define IsControlCommReceivingInProgress()  (CTRL_RX_PENDING == TRUE)
#define SetControlCommTransmitInProgress()  (CTRL_TX_PENDING = TRUE)
#define ResetControlCommTransmitInProgress()(CTRL_TX_PENDING = FALSE)
#define IsControlCommTransmitInProgress()   (CTRL_TX_PENDING == TRUE)
#define SetControlCommReceivedDataReady()   (CTRL_RX_DATA_RDY = TRUE)
#define ResetControlCommReceivedDataReady() (CTRL_RX_DATA_RDY = FALSE)
#define IsControlCommReceivedDataReady()    (CTRL_RX_DATA_RDY == TRUE)
#define SetControlCommReceivingTimer()      (ctrl_comm_rx_tmr = 40, CTRL_COMM_RX_TEX = FALSE)
#define IsControlCommReceivingTimerExpired()(CTRL_COMM_RX_TEX == TRUE)
#define SetControlCommBusControlTimer()     (ctrl_comm_timeout_tmr = 100, CTRL_COMM_TIMEOUT_TEX = FALSE)
#define IsControlCommBusControlTimerExpired()(CTRL_COMM_TIMEOUT_TEX == TRUE)
#define SetControlCommBusErrorAlarm()       (CTRL_COMM_ERROR = TRUE)
#define ResetControlCommBusErrorAlarm()     (CTRL_COMM_ERROR = FALSE, ctrl_comm_err_cnt = 0)
#define IsControlCommBusErrorAlarmActiv()   (CTRL_COMM_ERROR == TRUE)
//
//
//********   D E F I N I C I J E   T E R M I N A L   C O M M  ****************/
//
//
#define TerminalCommRxAddressingEnable()    (RCSTA1bits.ADDEN1 = TRUE)
#define TerminalCommRxAddressingDisable()   (RCSTA1bits.ADDEN1 = FALSE)
#define TerminalCommRx9BitReception()       (RCSTA1bits.RX9 = TRUE)
#define TerminalCommRx8BitReception()       (RCSTA1bits.RX9 = FALSE)
#define TerminalCommTx9BitTransmission()    (TXSTA1bits.TX9 = TRUE)
#define TerminalCommTx8BitTransmission()    (TXSTA1bits.TX9 = FALSE)
#define TerminalCommTx9BitSendAddress()     (TXSTA1bits.TX9D = TRUE)
#define TerminalCommTx9BitSendData()        (TXSTA1bits.TX9D = FALSE)
#define TerminalCommTxInterruptEnable()     (PIE1bits.TX1IE = TRUE)
#define TerminalCommTxInterruptDisable()    (PIE1bits.TX1IE = FALSE)

#endif //TYPEDEFS_H
