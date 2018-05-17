/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Softverski I2C port
 *
 *******************************************************************************
 * Ime fajla:       i2c.c
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

#include "io_cfg.h"
#include "i2c.h"
#include <delays.h>

// <editor-fold defaultstate="collapsed" desc="i2c write byte">

unsigned char I2C_WriteByte(unsigned char data) {

    volatile unsigned char count;

    SDA = I2CLOW;
    SCL = I2CLOW;

    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time

    for (count = 8; count > 0; count--) {//-- Send 8 bits of data
        if ((data & 0x80) == 0) { //-- Get the Bit
            SDA = I2CLOW; //-- Ensure Port pin is low
            SDA_TRIS = I2CLOW; //-- Lower pin if bit = 0
        } else {
            SDA_TRIS = I2CHIGH; //-- Release pin if bit = 1
        }// End of else...
        data = data << 1; //-- Shift next bit into position
        I2C_Clock(); //-- Pulse the clock
    }// End of for loop...
    SDA_TRIS = I2CHIGH; //-- Release data pin for ACK
    return (1U);
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c read byte">

unsigned char I2C_ReadByte(void) {

    volatile unsigned char count;
    volatile unsigned char data;

    SDA = I2CLOW;
    SCL = I2CLOW;

    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time

    for (count = 8; count > 0; count--) {//-- Read 8 bits of data

        data = data << 1;
        SDA_TRIS = I2CHIGH; //-- Release pin so data can be recieved
        if (I2C_ReadBit()) {
            data += 1;
        }// End of if..
    }// End of for loop...
    return (data);
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c start">

void I2C_Start(void) {
    //-- Ensure pins are in high impedance mode --
    SDA_TRIS = I2CHIGH;
    SCL_TRIS = I2CHIGH;
    SCL = I2CLOW;
    SDA = I2CLOW;
    I2C_Delay(I2CSTARTDELAY);
    //-- Generate the start condition
    SDA_TRIS = I2CLOW;
    SDA = I2CLOW;
    I2C_Delay(I2CSTARTDELAY);
    SCL_TRIS = I2CLOW;
    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time

}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c stop">

void I2C_Stop(void) {
    //-- Generate Stop Condition --
    SDA_TRIS = I2CLOW;
    SCL_TRIS = I2CHIGH;
    I2C_Delay(I2CSTOPDELAY);
    SDA_TRIS = I2CHIGH;
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c clock">

void I2C_Clock(void) {
    I2C_Delay(I2CDATASETTLE); //-- Minimum Clock Low Time
    SCL_TRIS = I2CHIGH; //-- Release clock
    I2C_Delay(I2CCLOCKHIGH); //-- Minimum Clock High Time
    SCL_TRIS = I2CLOW; //-- Lower the clock
    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c read bit">

unsigned char I2C_ReadBit(void) {
    unsigned char data = 0;
    I2C_Delay(I2CDATASETTLE); //-- Minimum Clock Low Time
    SCL_TRIS = I2CHIGH; //-- Release clock
    I2C_Delay(I2CHALFCLOCK); //-- 1/2 Minimum Clock High Time
    if (SDA_PIN != 0) data = 1; //-- READ in the data bit
    I2C_Delay(I2CHALFCLOCK); //-- 1/2 Minimum Clock High Time
    SCL_TRIS = I2CLOW; //-- Lower the clock
    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time
    return (data);
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c get acknowledge">

unsigned char I2C_GetAck(void) {

    SDA = I2CLOW;
    SCL = I2CLOW;
    SCL_TRIS = I2CLOW; //-- Ensure clock is low
    SDA_TRIS = I2CHIGH; //-- Release the Data pin so slave can ACK
    SCL_TRIS = I2CHIGH; //-- raise the clock pin
    I2C_Delay(I2CHALFCLOCK); //-- wait for 1/2 the clock pulse
    if (SDA_PIN) { //-- sample the ACK signal
        return (0); //-- No ACK so return BAD
    }// End of if...
    I2C_Delay(I2CHALFCLOCK); //-- Else wait for rest of clock
    SCL_TRIS = I2CLOW; //-- Finish the clock pulse
    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time
    I2C_Delay(I2CCLOCKLOW); //-- Minimum Clock Low Time
    return (1);
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c send acknowledge">

void I2C_SendAck(void) {
    //--- Send Ack to slave except for last time ---
    SDA = 0;
    SDA_TRIS = I2CLOW; //-- Send ACK
    I2C_Delay(I2CDATASETTLE); //-- Give it time to settle
    I2C_Clock(); //-- Pulse the clock
    SDA_TRIS = I2CHIGH; //-- Release ACK
    I2C_Delay(I2CDATASETTLE); //-- Gap between next byte
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c send not acknowledge">

void I2C_SendNotAck(void) {
    //--- Send NotAck to slave except for last time ---
    SDA = 0;
    SDA_TRIS = I2CHIGH; //-- Send ACK
    I2C_Delay(I2CDATASETTLE); //-- Give it time to settle
    I2C_Clock(); //-- Pulse the clock
    SDA_TRIS = I2CHIGH; //-- Release ACK
    I2C_Delay(I2CDATASETTLE); //-- Gap between next byte
}// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="i2c delay">

void I2C_Delay(unsigned char delay) {
    while (delay) {
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        --delay;
    }// End of while
}// </editor-fold>



