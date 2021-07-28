#include "ADS124S08.h"
#include <SPI.h>
#define VREF 2.5                   // Internal reference of 2.048V

ADS124S08 PC_ADS124S08;            // class

float volt_V = 0;
float volt_mV = 0;

double resolution;

#if defined(ARDUINO_ARCH_AVR)
  #define SerialDEBUG Serial
#elif defined(ARDUINO_ARCH_SAM)
  #define SerialDEBUG SerialUSB
#else
  #define SerialDEBUG Serial
#endif


void setup()
{
  // initalize the  data ready and chip select pins:
  pinMode(ADS124S08_DRDY_PIN, INPUT);                  //data ready input line
  pinMode(ADS124S08_CS_PIN, OUTPUT);                   //chip enable output line
  pinMode(ADS124S08_START_PIN, OUTPUT);               // start
  pinMode(ADS124S08_PWDN_PIN, OUTPUT);                // Power down output

  digitalWrite(ADS124S08_CS_PIN, HIGH);

  SerialDEBUG.begin(9600);
  delay(1000);
  // start the SPI library:
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  //CPOL = 0, CPHA = 1
  SPI.setDataMode(SPI_MODE1);
  // Selecting 1Mhz clock for SPI
  SPI.setClockDivider(SPI_CLOCK_DIV8); // DIV16
  //initalize ADS1292 slave
  PC_ADS124S08.ADS124S08_Init();                      // initialise ADS124S08
  SerialDEBUG.println("ADS124S08 Initialised successfully....");
 }

void loop()
{
  uint8_t channel = 0;
  SerialDEBUG.println("SPI Scanner Started");
  for(int channel = 0 ;channel < 12; channel++)
  {
    SerialDEBUG.print("Channel : ");
    SerialDEBUG.print(channel);
    PC_ADS124S08.setChannel(channel);
    delay(50);
    if(digitalRead(ADS124S08_DRDY_PIN) == LOW)               // monitor Data ready(DRDY pin)
    {
      uint32_t data = PC_ADS124S08.ADS124S08_Read_Data();      // read 6 bytes conversion register
      SerialDEBUG.print(" , Value : ");
      SerialDEBUG.println(data);
    }
    delay(500);
  }
  delay(1000);
}
