#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
SoftwareSerial link(D7, D8); // Rx, Tx
String dataIn;
String dt[10];
int i;
boolean parsing = false;
String Skel;

String apiKey = "5Y47YB79XRKQQ5DS";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Yuki Kato Kato";     // replace with your wifi ssid and wpa2 key
const char *pass =  "wifineerror";
const char* server = "api.thingspeak.com";
WiFiClient client;
void setup()
{
  Serial.begin(115200);
  link.begin(9600);
  dataIn = "";

//========wifi=================

Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

//end
  
}

void loop()
{

  ambilData();

}



void ambilData()
{
  if (link.available() > 0)
  {
    char inChar = (char)link.read();
    dataIn += inChar;
    if (inChar == '\n') {
      parsing = true;
    }
  }
  if (parsing)
  {
    parsingData();
    parsing = false;
    dataIn = "";
  }
}

void parsingData()
{
  int j = 0;
  Serial.print("data masuk : ");
  Serial.print(dataIn);
  Serial.print("\n");
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 1; i < dataIn.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[i] == '#') || (dataIn[i] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[i];
    }
  }
  //kirim data hasil parsing
  Serial.print("kelembapan tanah : ");
  Serial.print(dt[0].toInt());
  Serial.print("\n");

Skel=dt[0];

 if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(Skel);
//                             postStr +="&field2=";
//                             postStr += String(h);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("KELEMBAPAN :");
                             Serial.print(Skel);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(1000);
}
