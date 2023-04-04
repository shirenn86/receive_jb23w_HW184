
//7seg CLK  ->  D7
//7seg DIO  ->  D6
//beep      ->  D3

#include <TM1637Display.h>
#define CLK 7
#define DIO 6
#define BEEP 3
TM1637Display display(CLK, DIO);

#include <mcp_can.h>          //can
#include <SPI.h>              //can
#define CS_PIN  (10)
#define INT_PIN (8)

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

unsigned char temp[8] = {2, 1, 0x05, 0, 0, 0, 0, 0};    // temp  0x05

MCP_CAN CAN0(CS_PIN);      
 
void setup()
{
  Serial.begin(115200);
  CAN0.begin(CAN_500KBPS, MCP_8MHz);    //     (, MCP_8MHz)is 
  pinMode(INT_PIN, INPUT);              //        HW-184 optional 
  display.setBrightness(7);
//  display.clear();
  
  display.showNumberDec(-1, false, 3, 0);

    pinMode(BEEP, OUTPUT);              //   beep
}
 
void loop()
{
  CAN0.sendMsgBuf(0x7df, 0, 8, temp);      //to ECU send mess
  
    if(!digitalRead(INT_PIN)){
      CAN0.readMsgBuf(&len, rxBuf);              
      rxId = CAN0.getCanId();                    

        if (rxId == 0x7e8) {              //if from ecu
        if (rxBuf[2] == 0x05) {           //if water temp

        display.showNumberDec(rxBuf[3] - 40);      //7seg
          if ((rxBuf[3] - 40)>100){digitalWrite(BEEP,HIGH);}     //temp over 100
        }}}
delay(1000);
digitalWrite(BEEP,LOW);
}
