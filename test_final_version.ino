#include "logic_esp.h"
#include "web_server.h"

int DutyCycle = 0;
int mq7_Ro1, mq7_Ro2;
uint32_t tmr_main;
uint32_t tmr_value_co;
bool flagMainPeriod = false;
float ppm1;
float ppm2;

int counterCycle = 1; // перенести трохи нижче.
  int counterCycleFromUser = 3;
  int counterSeconds = 0;
  int arr[91]={0};

  bool showSummaryPPM = false;
  bool isMesurment = false;
  bool isReabsorb = false;
  bool isReloadPage = true;

#define period60sec 60*1000L
#define period90sec 90*1000L
#define period1sec 1000L

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello!");

  initESP();
  
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }
  dutyCycleOfPWM();

  ledcWrite(0, 255);
 // delay(period60sec);
   
  ledcWrite(0, DutyCycle);
 // delay(period90sec);
  delay(1000);
  mq7_Ro1 = calib_mq7_Ro(pin_voltageOn_CO1);
  mq7_Ro2 = calib_mq7_Ro(pin_voltageOn_CO2);
 

  
  
  Serial.print("mq7_Ro1: ");
  Serial.print(mq7_Ro1);
  Serial.print("; mq7_Ro2: ");
  Serial.println(mq7_Ro2);

}

void loop() {
  
  if (millis() - tmr_main >= (flagMainPeriod ? period90sec : period60sec)) {
    tmr_main = millis();
  
      if(flagMainPeriod==0)
      {
        ledcWrite(0, 255);
        //isReabsorb = true;
        isMesurment = false;
      }
      else {
        //isMesurment = true;
        //isReabsorb = false;
        }
      
      if(flagMainPeriod==1)
      {
        isMesurment = true;
        ledcWrite(0, DutyCycle);
      }
     flagMainPeriod = !flagMainPeriod;
    }
    
   if (millis() - tmr_value_co >= period1sec)
   {
     startServer( counterSeconds, counterCycle, isMesurment, isReloadPage,isReabsorb,showSummaryPPM);
     counterSeconds++;
     tmr_value_co = millis();
     ppm1 = get_rawValue_mq7(mq7_Ro1, MQ7_REFERENCE_VOLTAGE, pin_voltageOn_CO1);
     ppm2 = get_rawValue_mq7(mq7_Ro2, MQ7_REFERENCE_VOLTAGE, pin_voltageOn_CO2);
     Serial.print("ppm1: ");  
     Serial.print(ppm1 );
     Serial.print("; ppm2: ");  
     Serial.print(ppm2 );
     Serial.print("; deltaPPM: ");  
     Serial.println(ppm2-ppm1 );
     
      if(isMesurment){
      Serial.print("__ isMesurment __ Second : "); 
      Serial.println(counterSeconds); 
     arr[counterSeconds] = ppm1;
    
     if(counterSeconds>=90){
        counterSeconds = 0;
        counterCycle++;
       
        }
      }
      if(!isMesurment){
        Serial.print("__ FalseMesurment __ Second : "); 
      Serial.println(counterSeconds); 
        if(counterSeconds>=60){
        counterSeconds = 0;
         memset(arr,0,90 * sizeof(int));}

        
        }
     
     
   }

  
//------------------------------------------------------------------------------------------------
  
   
// ERROR : it is go in main loop without any delays.
/*
  counterSeconds++; 
  if(counterSeconds>=90){
  counterSeconds = 0;
  counterCycle++;
  }
*/
  
  
//------------------------------------------------------------------------------------------------



}
