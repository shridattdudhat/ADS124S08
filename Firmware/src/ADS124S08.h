/*
ADS124S08.h - Library for ADS124S08 
Released into the public domain.
*/

#ifndef ADS124S08_h
#define ADS124S08_h

// Includes
#include "Arduino.h"
#include <SPI.h>

#define CONFIG_SPI_MASTER_DUMMY   0x00
#define VREF 2.5
#define RES (double)((double)VREF/pow(2,23))

// Register Read Commands
#define ADS_RREG   0x20		//Read n nnnn registers starting at address r rrrr
                              //first byte 001r rrrr (2xh)(2) - second byte 000n nnnn(2)
#define ADS_WREG    0x40		//Write n nnnn registers starting at address r rrrr
                              //first byte 010r rrrr (2xh)(2) - second byte 000n nnnn(2)
#define ADS_WAKEUP  0x02
#define ADS_RESET		0x06
#define ADS_START		0x08		//Start/restart (synchronize) conversions
#define ADS_STOP		0x0A		//Stop conversion
#define ADS_RDATA		0x12		//Read data by command; supports multiple read back.

#define	ADS_INPMUX		0x02
#define	ADS_REF		    0x05

//Pin declartion the other you need are controlled by the SPI library
const int ADS124S08_DRDY_PIN = 10;
const int ADS124S08_CS_PIN = 11;
const int ADS124S08_START_PIN = 12;
const int ADS124S08_PWDN_PIN = 13;

class ADS124S08
{
public:
  ADS124S08();
  void ADS124S08_Init(void);
  void ADS124S08_Reset(void);
  void ADS124S08_Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA);
  void ADS124S08_Reg_Read (unsigned char READ_WRITE_ADDRESS);
  void ADS124S08_SPI_Command_Data(unsigned char data_in);
  void ADS124S08_Disable_Start(void);
  void ADS124S08_Enable_Start(void);
  void ADS124S08_Hard_Stop (void);
  void ADS124S08_Start_Data_Conv_Command (void);
  void ADS124S08_Soft_Stop (void);
  void ADS124S08_Start_Read_Data_Continuous (void);
  void ADS124S08_Wakeup_Data_Conv_Command(void);
  void ADS124S08_Stop_Read_Data_Continuous (void);
  void ADS124S08_Reset_Data_Conv_Command(void);
  float ADS124S08_Read_Data(void);
  uint32_t ADS124S08_Read();
  uint8_t readRegister(uint8_t address);
  void setChannel(uint8_t channel);
};

#endif
