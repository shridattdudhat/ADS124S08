#include "ADS124S08.h"
#include "math.h"

ADS124S08::ADS124S08()
{
  pinMode(ADS124S08_DRDY_PIN, INPUT);                  //data ready input line
  pinMode(ADS124S08_START_PIN, OUTPUT);               // start
  pinMode(ADS124S08_PWDN_PIN, OUTPUT);                // Power down output
}

// return mV value
float ADS124S08::ADS124S08_Read_Data()
{
  float value = -1;
  if(digitalRead(ADS124S08_DRDY_PIN) == LOW)               // monitor Data ready(DRDY pin)
  {
    while(digitalRead(ADS124S08_DRDY_PIN) == LOW);

    uint32_t iData = 0;
    digitalWrite(ADS124S08_CS_PIN, LOW);
    delay(5);
    iData = SPI.transfer(CONFIG_SPI_MASTER_DUMMY);
    iData = (iData<<8) + SPI.transfer(CONFIG_SPI_MASTER_DUMMY);
    iData = (iData<<8) + SPI.transfer(CONFIG_SPI_MASTER_DUMMY);
    delay(5);
    digitalWrite(ADS124S08_CS_PIN, HIGH);

    value = (RES) * (float)iData * 1000;
  	return value;
  }
  else
  {
    return value;
  }
}

uint32_t ADS124S08::ADS124S08_Read()
{
  uint32_t iData = 0;
  uint8_t rxBuffer[3] = {0};

  digitalWrite(ADS124S08_CS_PIN,LOW);
  delay(5);
  SPI.transfer(ADS_RDATA);
  //SPI.transfer(0x00);
  rxBuffer[0] = SPI.transfer(0x00);
	rxBuffer[1] = SPI.transfer(0x00);
  rxBuffer[2] = SPI.transfer(0x00);
  //SPI.transfer(0x00);
  delay(5);
  digitalWrite(ADS124S08_CS_PIN,HIGH);

  Serial.println(rxBuffer[0]);
  Serial.println(rxBuffer[1]);
  Serial.println(rxBuffer[2]);

  iData = (rxBuffer[0] << 16) + (rxBuffer[1] << 8) + rxBuffer[2];
	return iData;
}

uint8_t ADS124S08::readRegister(uint8_t address)
{
  uint8_t data;
  uint8_t data1;
  digitalWrite(ADS124S08_CS_PIN,LOW);
  delay(5);
  SPI.transfer(ADS_RREG|(address & 0x1f));
  SPI.transfer(0x00);
  data = SPI.transfer(CONFIG_SPI_MASTER_DUMMY);
  delay(5);
  digitalWrite(ADS124S08_CS_PIN,HIGH);
  return data;
}

void ADS124S08::ADS124S08_Init()
{
  // Log.info("ADS Init");
  delay(10);
  ADS124S08_Reset();
  delay(20);
  ADS124S08_Reset_Data_Conv_Command();
  delay(1000);

  ADS124S08_Reg_Write(ADS_INPMUX, 0x0C);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);
  ADS124S08_Reg_Write(ADS_REF, 0x1A);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);

  ADS124S08_Enable_Start();
  delay(10);
  ADS124S08_Disable_Start();
  delay(50);

  ADS124S08_Start_Data_Conv_Command();
  delay(50);
}

void ADS124S08::setChannel(uint8_t channel)
{
  channel = (channel << 4) + 0x0C;
  ADS124S08_Reg_Write(ADS_INPMUX, channel);	//Ch 1 enabled, gain 6, connected to electrode in
  delay(10);
}

void ADS124S08::ADS124S08_Reset()
{
  digitalWrite(ADS124S08_PWDN_PIN, HIGH);
  delay(100);					// Wait 100 mSec
  digitalWrite(ADS124S08_PWDN_PIN, LOW);
  delay(100);
  digitalWrite(ADS124S08_PWDN_PIN, HIGH);
  delay(100);
}

void ADS124S08::ADS124S08_Disable_Start()
{
  digitalWrite(ADS124S08_START_PIN, LOW);
  delay(20);
}

void ADS124S08::ADS124S08_Enable_Start()
{
  digitalWrite(ADS124S08_START_PIN, HIGH);
  delay(20);
}

void ADS124S08::ADS124S08_Hard_Stop (void)
{
  digitalWrite(ADS124S08_START_PIN, LOW);
  delay(100);
}

void ADS124S08::ADS124S08_Reset_Data_Conv_Command (void)
{
  ADS124S08_SPI_Command_Data(ADS_RESET);					// Send 0x08 to the ADS1x9x
}


void ADS124S08::ADS124S08_Start_Data_Conv_Command (void)
{
  ADS124S08_SPI_Command_Data(ADS_START);					// Send 0x08 to the ADS1x9x
}

void ADS124S08::ADS124S08_Wakeup_Data_Conv_Command (void)
{
  ADS124S08_SPI_Command_Data(ADS_WAKEUP);					// Send 0x08 to the ADS1x9x
}

void ADS124S08::ADS124S08_Soft_Stop (void)
{
  ADS124S08_SPI_Command_Data(ADS_STOP);                   // Send 0x0A to the ADS1x9x
}

void ADS124S08::ADS124S08_Start_Read_Data_Continuous (void)
{

}

void ADS124S08::ADS124S08_Stop_Read_Data_Continuous (void)
{

}

void ADS124S08::ADS124S08_SPI_Command_Data(unsigned char data_in)
{
  digitalWrite(ADS124S08_CS_PIN, LOW);
  delay(2);
  SPI.transfer(data_in);
  delay(2);
  digitalWrite(ADS124S08_CS_PIN, HIGH);
}

//Sends a write command to SCP1000
void ADS124S08::ADS124S08_Reg_Write (unsigned char READ_WRITE_ADDRESS, unsigned char DATA)
{
  // now combine the register address and the command into one byte:
  byte dataToSend = READ_WRITE_ADDRESS + ADS_WREG;
  // take the chip select low to select the device:
  digitalWrite(ADS124S08_CS_PIN, LOW);
  delay(2);
  SPI.transfer(dataToSend); //Send register location
  SPI.transfer(0x00);		//number of register to wr
  SPI.transfer(DATA);		//Send value to record into register
  delay(2);
  // take the chip select high to de-select:
  digitalWrite(ADS124S08_CS_PIN, HIGH);
}
