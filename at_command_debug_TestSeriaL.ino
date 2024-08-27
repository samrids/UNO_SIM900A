#include <SoftwareSerial.h>

SoftwareSerial SIM900A(2, 3);

unsigned char buffer[64];  // buffer array for data receive over serial port
int count=0;               // counter for buffer array 

void setup()
{
  SIM900A.begin(115200);
  Serial.begin(9600);
}

void loop()
{
  while (SIM900A.available()>0)
  {
    while(SIM900A.available())
    {
      buffer[count++]=SIM900A.read();
      if(count == 64)break;
    }
    Serial.write(buffer,count);
    clearBufferArray();
    count = 0;
  }
  while (Serial.available() > 0) {
    SIM900A.write(Serial.read());
  }     
}

void clearBufferArray() 
{
  for (int i=0; i<count;i++)
  {
    buffer[i]=NULL;
  }
}