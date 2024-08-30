#include <SoftwareSerial.h>  // serial software library for interfacing gsm module
SoftwareSerial SIM900A(2, 3); // RX, TX  // connect gsm Tx at D2 and Rx at D3

String pvtKey="Write_API_Key";     // private key for posting data to thingspeak
String url="";
String ip="\"api.thingspeak.com\"";        // thingspeak server ip or url
int port=80;                              // thingspeak server port

String SendCmd="AT+CIPSEND=";           // sending number of byte command
String Start="AT+CIPSTART=\"TCP\"";     // TCPIP start command 

String instr="";
long randNumber;

String msg="";

void setup() 
{
  SIM900A.begin(115200);    // init SIM900A for GSM
  Serial.begin(9600);
  delay(2000);
  
  Serial.println("Initializing GSM");
  initGSM();      // init GSM module

  Serial.println("Initializing GPRS");
  initGPRS();     // init GPRS in GSM Module
  
  Serial.println("System Ready");
  delay(2000);
}

void loop()
{
        
      Serial.println("Sending Data to server...");
      url="GET /update";
      url+="?api_key=";
      url+=pvtKey;
      url+="&field1=";
      randNumber = random(60, 200);
      msg = String(randNumber);
      url+=msg;
      url+=" HTTP/1.0\r\n\r\n";
      String svr=Start+","+ip+","+port;
      //Serial.println(svr);
      delay(1000);
      connectGSM(svr,"CONNECT");
      delay(1000);
      int len = url.length();
      String str="";
      str=SendCmd+len;
      sendToServer(str);
      SIM900A.print(url);
      delay(1000);
      SIM900A.write(0x1A);
      Serial.println("Waiting...30 sec.");
      delay(30000);
    
      msg="";   
}

void sendToServer(String str)
{
  SIM900A.println(str);
  delay(1000);
}

void initGSM()
{
  connectGSM("AT","OK");
  connectGSM("ATE1","OK");
  connectGSM("AT+CPIN?","READY");
}
void initGPRS()
{
  connectGSM("AT+CIPSHUT","OK");
  connectGSM("AT+CGATT=1","OK");
  connectGSM("AT+CSTT=\"internet\",\"\",\"\"","OK");
  connectGSM("AT+CIICR","OK");
  delay(1000);
  SIM900A.println("AT+CIFSR");
  delay(1000);
}
void connectGSM (String cmd, char *res)
{
  while(1)
  {
    Serial.println("Command :==>" + cmd);
    SIM900A.println(cmd);
    delay(500);
    while(SIM900A.available()>0)
    {
      if(SIM900A.find(res))
      {
        delay(1000);
        Serial.println(" |_Response : "+cmd +" ==>"+res);
        return;
      }
    }
    delay(1000);
   }
 }
