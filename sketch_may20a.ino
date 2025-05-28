// Load Wi-Fi library
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h> 
#include <AsyncFsWebServer.h> 

#include <DHT.h>
#include <FS.h>
#include <LittleFS.h>
#include <SPIFFS.h>

#include "PwmOutput.h"
#include "PwmInput.h"
//#define FILESYSTEM SPIFFS
#define FILESYSTEM LittleFS

#define DHTPIN 23
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid = "Melody";
const char* password = "claeclae";

AsyncFsWebServer server(80, FILESYSTEM, "myServer");


// Variable to store the HTTP request
String header;


// Auxiliar variables to store the current output state
String output2State = "off";

// Assign output variables to GPIO pins
const int output2 = 2;

// Buffer circulaire pour les périodes
const int bufferSize = 5;
unsigned long intervalBuffer[bufferSize];
int bufferIndex = 0;
bool bufferFull = false;

unsigned long startTime = 0;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

const char* PARAM_ACTION = "action";
const char* PARAM_IDX = "idx";
const char* PARAM_VALUE = "value";

TaskHandle_t xHandle;


PwmOutput *pwmOutput[5];
PwmInput *pwmInput[3];

int rpm1,rpm2,rpm3 = 0;

static void IRAM_ATTR intr_handler(void *args) {
  int idx = (int)args;
  unsigned long now = micros();
   if (now - pwmInput[idx]->lastPulseTime > 2500) {  // Ignore fronts trop proches (<500 µs)
    pwmInput[idx]->pulseCount++;
    pwmInput[idx]->lastPulseTime = now;
  }
}


void countTask(void *) {
  while (1) {
    unsigned long prev = micros();
    vTaskDelay(1000/portTICK_PERIOD_MS);
    unsigned long now = micros();

    unsigned long currentTime = millis();
    if (currentTime>startTime+10000 && startTime!=-1) {
      pwmOutput[0]->dutyCycle =255;
      pwmOutput[1]->dutyCycle =22;

      int correctedDutyCycle1 = correctedPWM(pwmOutput[0]->dutyCycle);
      int correctedDutyCycle2 = correctedPWM(pwmOutput[1]->dutyCycle);
       
      ledcWrite(pwmOutput[0]->pin, correctedDutyCycle1);
      ledcWrite(pwmOutput[1]->pin, correctedDutyCycle2);

      startTime=-1;
    }
    
    float currentInterval1 = pwmInput[0]->pulseCount;
    float currentInterval2 = pwmInput[1]->pulseCount;
    float currentInterval3 = pwmInput[2]->pulseCount;
    
    currentInterval1 = (float)currentInterval1 / (float)((now-prev)) * 1000000.00;
    currentInterval2 = (float)currentInterval2 / (float)((now-prev)) * 1000000.00;
    currentInterval3 = (float)currentInterval3 / (float)((now-prev)) * 1000000.00;

    if (currentInterval1 > 0.00) {
      rpm1 = (int)(currentInterval1 / 2.0 * 60.0); // 2 impulsions par tour
      pwmInput[0]->pulseCount = 0;
    }

    if (currentInterval2 > 0.00) {
      rpm2 = (int)(currentInterval2 / 2.0 * 60.0); // 2 impulsions par tour
      pwmInput[1]->pulseCount = 0;
    }

    if (currentInterval3 > 0.00) {
      rpm3 = (int)(currentInterval3 / 2.0 * 60.0); // 2 impulsions par tour
      pwmInput[2]->pulseCount = 0;
    }

    Serial.println();
    
  }
}





  

uint8_t correctedPWM(uint8_t input) {
  // input entre 0 et 255
  float min = 0.0;
  float scale = 255.0;
  float in1 = (input/255.00*scale);
  Serial.println("in1:" + String(in1));
  
  float x = in1 / scale;
  Serial.println("x:" + String(x));
  float corrected = pow(x, 1.2); // courbe exponentielle douce
  Serial.println("corrected:" + String(corrected));
  return (uint8_t)(corrected * scale)+min;
}


String makeBtn(String action, String idx, String val) {
    String html="";
    html+="<div class=\"btn\">";
    html+="<a href=\"serverIndex.htm?action=" + action + "&idx=" + idx + "&value=" + val + "\">";
    html+="<p>" + val + "</p>";
    html+="</a>";
    html+="</div>";
    return html;
}

String processor(const String& var)
{
  if (var=="TITLE")
    return F("ICX-7250 Fan Controller");

  if (var=="RPM1")
    return String(rpm1);
  if (var=="Freq0")
    return String(pwmOutput[0]->freq);
  if (var=="Duty0")
    return String(pwmOutput[0]->dutyCycle);

  if (var=="RPM2")
    return String(rpm2);
  if (var=="Freq1")
    return String(pwmOutput[1]->freq);
  if (var=="Duty1")
    return String(pwmOutput[1]->dutyCycle);

  if (var=="RPM3")
    return String(rpm3);
  if (var=="Freq2")
    return String(pwmOutput[2]->freq);
  if (var=="Duty2")
    return String(pwmOutput[2]->dutyCycle);

  if (var=="RPM3")
    return F("0");
  if (var=="Freq3")
    return String(pwmOutput[3]->freq);
  if (var=="Duty3")
    return String(pwmOutput[3]->dutyCycle);


  if (var=="RPM4")
    return F("0");
  if (var=="Freq4")
    return String(pwmOutput[4]->freq);
  if (var=="Duty4")
    return String(pwmOutput[4]->dutyCycle);


  if (var=="TEMP") {
      float temp = dht.readTemperature();
      if (isnan(temp)) {
        Serial.println(F("Echec reception"));
        return String("NAN");
      }
      return String(temp);
  }
  if (var=="HUMIDITE") {
      float humidite = dht.readHumidity();
      if (isnan(humidite)) {
        Serial.println(F("Echec reception"));
        return String("NAN");
      }
      return String(humidite);
  }

  if (var.indexOf("BTNFREQ")>=0) {
    Serial.print("p1");
      String idx = String(var[7]);
      String html="";
      html+= makeBtn("modFreq", idx, "-10000");
      html+= makeBtn("modFreq", idx, "-1000");
      html+= makeBtn("modFreq", idx, "-100");
      html+= makeBtn("setFreq", idx, "0");
      html+= makeBtn("setFreq", idx, "1000");
      html+= makeBtn("setFreq", idx, "2000");
      html+= makeBtn("setFreq", idx, "5000");
      html+= makeBtn("setFreq", idx, "10000");
      html+= makeBtn("setFreq", idx, "20000");
      html+= makeBtn("setFreq", idx, "25000");
      html+= makeBtn("modFreq", idx, "+100");
      html+= makeBtn("modFreq", idx, "+1000");
      html+= makeBtn("modFreq", idx, "+10000");
      return html;
  }
  if (var.indexOf("BTNDUTY")>=0) {
    Serial.print("p1");
      String idx = String(var[7]);
      String html="";
      html+= makeBtn("modDuty", idx, "-100");
      html+= makeBtn("modDuty", idx, "-10");
      html+= makeBtn("modDuty", idx, "-1");
      html+= makeBtn("setDuty", idx, "0");
      html+= makeBtn("setDuty", idx, "50");
      html+= makeBtn("setDuty", idx, "100");
      html+= makeBtn("setDuty", idx, "150");
      html+= makeBtn("setDuty", idx, "200");
      html+= makeBtn("setDuty", idx, "255");
      html+= makeBtn("modDuty", idx, "+1");
      html+= makeBtn("modDuty", idx, "+10");
      html+= makeBtn("modDuty", idx, "+100");
      return html;
  }
  
  if (var=="GPIO2")
    return String(output2State);
}



          



void handleMod(AsyncWebServerRequest *request) {
  const AsyncWebParameter* pActionParam = request->getParam(PARAM_ACTION);
  const AsyncWebParameter* pIdxParam = request->getParam(PARAM_IDX);
  const AsyncWebParameter* pValueParam = request->getParam(PARAM_VALUE);
  
  String pAction = "";
  String pIdx = "";
  String pValue = "";

  if (pActionParam!=NULL && pActionParam->value()!=NULL) {
      pAction = pActionParam->value();
  }

  if (pIdxParam!=NULL && pIdxParam->value()!=NULL) {
      pIdx = pIdxParam->value();
  }

  if (pValueParam!=NULL && pValueParam->value()!=NULL) {
      pValue = pValueParam->value();
  }

  
  if (pAction.indexOf("modGPIO2")>=0) {
      if (pValue.indexOf("ON")>=0) {
              Serial.println("GPIO 2 on");
              output2State = "on";
              digitalWrite(output2, HIGH);

      }
      else if (pValue.indexOf("OFF")>=0) {
              Serial.println("GPIO 2 off");
              output2State = "off";
              digitalWrite(output2, LOW);
      }
  }

  if (pAction.indexOf("modDuty")>=0) {
      int idx = String(pIdx).toInt();
      int value = String(pValue).toInt();
      pwmOutput[idx]->dutyCycle += value;

      int correctedDutyCycle= correctedPWM(pwmOutput[idx]->dutyCycle);
      ledcWrite(pwmOutput[idx]->pin, correctedDutyCycle);
  }
  else if (pAction.indexOf("setDuty")>=0) {
      int idx = String(pIdx).toInt();
      int value = String(pValue).toInt();
      pwmOutput[idx]->dutyCycle = value;

      int correctedDutyCycle= correctedPWM(pwmOutput[idx]->dutyCycle);
      ledcWrite(pwmOutput[idx]->pin, correctedDutyCycle);
  }

  if (pAction.indexOf("modFreq")>=0) {
      int idx = String(pIdx).toInt();
      int value = String(pValue).toInt();
      pwmOutput[idx]->freq += value;

      ledcDetach(pwmOutput[idx]->pin);
      ledcAttach(pwmOutput[idx]->pin, pwmOutput[idx]->freq, pwmOutput[idx]->resolution);

  }
  else if (pAction.indexOf("setFreq")>=0) {
      int idx = String(pIdx).toInt();
      int value = String(pValue).toInt();
      pwmOutput[idx]->freq = value;

      ledcDetach(pwmOutput[idx]->pin);
      ledcAttach(pwmOutput[idx]->pin, pwmOutput[idx]->freq, pwmOutput[idx]->resolution);
  }


}

/*
* Getting FS info (total and free bytes) is strictly related to
* filesystem library used (LittleFS, FFat, SPIFFS etc etc) and ESP framework
*/
#ifdef ESP32
void getFsInfo(fsInfo_t* fsInfo) {
	fsInfo->fsName = "LittleFS";
	fsInfo->totalBytes = LittleFS.totalBytes();
	fsInfo->usedBytes = LittleFS.usedBytes();  
}
#endif

void setup() {
  Serial.begin(115200);

  startTime = millis();
  Serial.println("Start");
  Serial.println("FS Begin");
  if(!FILESYSTEM.begin(true)){
    Serial.println("An Error has occurred while mounting FILESYSTEM");
    return;
  }

  
  Serial.println("FS End");
  
  pwmOutput[0] = new PwmOutput("Fan1", 14, 25000, 8);
  pwmOutput[1] = new PwmOutput("Fan2", 12, 25000, 8);

  pwmOutput[2] = new PwmOutput("SocketFan1", 32, 300, 8);
  pwmOutput[3] = new PwmOutput("SocketFan2", 33, 300, 8);
  pwmOutput[4] = new PwmOutput("SocketFan3", 25, 300, 8);

  pwmOutput[2]->dutyCycle = 100;
  pwmOutput[3]->dutyCycle = 100;
  pwmOutput[4]->dutyCycle = 100;

  pinMode(2, OUTPUT);

  pwmInput[0] = new PwmInput("Ventilo1", 19);
  pwmInput[1] = new PwmInput("Ventilo2", 18);
  pwmInput[2] = new PwmInput("Ventilo3", 5);

  for (int idx=0;idx<5;idx++) {
    Serial.println("Initialize pwmOutput # " + String(idx));
    
    pinMode(pwmOutput[idx]->pin, OUTPUT);
    ledcAttach(pwmOutput[idx]->pin, pwmOutput[idx]->freq, pwmOutput[idx]->resolution);
    ledcWrite(pwmOutput[idx]->pin, 100);
  }

  // Full speed at start
  ledcWrite(pwmOutput[0]->pin, 255);
  ledcWrite(pwmOutput[1]->pin, 255);

  gpio_install_isr_service(0);
  for (int idx=0;idx<3;idx++) {
    Serial.println("Initialize pwmInput # " + String(idx));
    pinMode(pwmInput[idx]->pin, INPUT_PULLUP);
    gpio_set_intr_type((gpio_num_t)pwmInput[idx]->pin, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add((gpio_num_t)pwmInput[idx]->pin, intr_handler,(void*)idx);
  }

  xTaskCreate(countTask, "counter", 2000, NULL, 20, &xHandle);
    
  dht.begin();

  if (!server.startWiFi(10000)) {
      Serial.println("\nWiFi not connected! Starting AP mode...");
      server.startCaptivePortal("ESP_AP", "123456789", "/setup");
  }    

  server.enableFsCodeEditor();
  server.setAuthentication("admin", "admin");
  #ifdef ESP32
  server.setFsInfoCallback(getFsInfo);
  #endif


  server.on("/serverIndex.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    // Handling function implementation
    handleMod(request);
    request->send(FILESYSTEM, "/serverIndex.htm", "text/html", false, processor);
  });

  if (server.init()) {
    Serial.print(F("\n\nWeb Server started on IP Address: "));
    Serial.println(server.getServerIP());
  }

  server.serveStatic("/", FILESYSTEM, "/").setTemplateProcessor(processor).setDefaultFile("index.htm");
}



void loop() {
    delay(1);
}