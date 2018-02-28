#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Process.h>

static const int RXPin = 9, TXPin = 4;
static const uint32_t GPSBaud = 4800;

// Setup process for upload data
Process process;

// The TinyGPS++ object
TinyGPSPlus garmin;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin, true);

// Custom for $SDDBT sentence
TinyGPSCustom depth(garmin, "SDDBT", 3);

struct Data{
  float X;
  float Y;
  float Z;  
};
struct Data G;  //Data struct uses to receive data in void loop

void setup() 
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Bridge.begin();                // Start connection between Arduino and Linux
                                 // !!! Forget this will result to unable connect to internet
}

void loop() 
{
  
  while (ss.available() > 0){
    if (garmin.encode(ss.read())){
//      displayInfo();
        G = collectData();
        // Multiply latitude and longtitude by 10^7 so cURL won't round up decimal point
        G.X = G.X*10000000;
        G.Y = G.Y*10000000;
        GoogleSheet(G.X,G.Y,G.Z);
        Serial.println("-------------------------------------------");
        Serial.print("X = ");
        Serial.println(G.X,7);
        Serial.print("Y = ");
        Serial.println(G.Y,7);
        Serial.print("Z = ");
        Serial.println(G.Z);
        Serial.println("-------------------------------------------");
        Serial.println("");
    }
  }
        
}


struct Data collectData()
{
  float X,Y,Z;
  //Latitude, Longtitude
//  Serial.print(F("Location: ")); 
  if (garmin.location.isValid())
  {
    X = garmin.location.lat();
    Y = garmin.location.lng();
//    Serial.print(garmin.location.lat(), 6);
//    Serial.print(F(","));
//    Serial.println(garmin.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  //Depth
  String Depth = depth.value();
  Z = Depth.toFloat();
  Serial.print("Depth: ");
//  Serial.println(depth.value()); 
  Serial.println(Z); 

  Serial.println();

  //Create data struct use to return result
  struct Data result;
  result.X = X;
  result.Y = Y;
  result.Z = Z;
  return result;
}


//Uploadt to Google Sheet
void GoogleSheet(float X,float Y,float Z){
  //FOR REAL-TIME VERSION
  String cURLcommand = "curl -k \"";
  cURLcommand +="https://script.google.com/macros/s/AKfycby6aY1ArEzTQxON8gi87EvYes2JJCU5pE-VP7VVU_Nakf2jFHY/exec?";
  cURLcommand +="XData=";
  cURLcommand +=X;
  cURLcommand +="&YData=";
  cURLcommand +=Y;
  cURLcommand +="&DepthData=";
  cURLcommand +=Z;
  cURLcommand +="\"";
  process.runShellCommand(cURLcommand);     //Write to Google Sheet
  Serial.println(cURLcommand);
}
