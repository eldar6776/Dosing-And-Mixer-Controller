/******************************************************************************
 *
 *        Mikser sirovine ekstrudera - Centralni kontroler
 *
 *                  Zaglavlje softverskog I2C porta
 *
 *******************************************************************************
 * Ime fajla:       i2c.h
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
#ifndef I2C_H
#define	I2C_H

#define I2CLOW  0         //-- Puts pin into output/low mode
#define I2CHIGH 1         //-- Puts pin into Input/high mode


//********* I2C Bus Timing - uS ************
#define I2CSTARTDELAY 50
#define I2CSTOPDELAY  50
#define I2CDATASETTLE 20
#define I2CCLOCKHIGH  100
#define I2CHALFCLOCK  50
#define I2CCLOCKLOW   100
#define I2CACKWAITMIN 50

//********************* FUNCTIONS ************************
unsigned char I2C_WriteByte(unsigned char data);
unsigned char I2C_ReadByte(void);
unsigned char I2C_GetAck(void);
void I2C_SendAck(void);
void I2C_SendNotAck(void);
void I2C_Delay(unsigned char delay);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Clock(void);
unsigned char I2C_ReadBit(void);

#endif	/* I2C_H */

