/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.9 01/08/18
**********************************************/

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include "MQ135.h"
#include "MQ7.h"
#include "DHT.h"
#include "Streaming.h"
#include "main.h"

void setup(){
  digitalWrite(ledPinBlue, LOW);
  digitalWrite(ledPinSD, LOW);
  pinMode(ledPinSD, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  pinMode(buttonPinBlue, INPUT);
  pinMode(buttonPinSD, INPUT);

  Serial.begin(9600);
  dht.begin();

  //Serial.println("INICIO"); // deve ser removido no futuro

  // for (int i = 1; i <= 60; i++)
  // {
  //   digitalWrite(ledPinSD, HIGH);
  //   digitalWrite(ledPinBlue, HIGH);
  //   delay(500);
  //   digitalWrite(ledPinSD, LOW);
  //   digitalWrite(ledPinBlue, LOW);
  //   delay(500);
  // }

  while(true){
    if (digitalRead(buttonPinBlue) == HIGH){
      digitalWrite(ledPinBlue, HIGH);
      bluetooh_status = true;
      break;
    }
    else if (digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, HIGH);
      sdcard_status = true;
      break;
    }
    bluetooh_status = true;
    break;
  }
  if(sdcard_status){
    if (!SD.begin(4)) {
      Serial.println("Erro to Start SDCARD");
      while(1){
        digitalWrite(ledPinSD, HIGH);
        delay(200);
        digitalWrite(ledPinSD, LOW);
        delay(200);
      }
    }
  }
  init_time = millis();

  for(int j=TIME_MAX;j>0;j--)
    pm25[j]=0.05;
    pm25[0]=1.5;
}


bool has_pass_fifthen_seconds(long unsigned int &init_time, long unsigned int &range_collect_time){
  if ((millis()-init_time) > range_collect_time){
    Serial << "PASSOU 15 SEC";
    return true;
  }
  Serial << "FALSE";
  return false;
}

void loop()
{
  int init_time_in_loop = millis();
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if (has_pass_fifthen_seconds(init_time, sampletime_ms))
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    pm25val = pm25coef * concentration * 10;

    pm25[0] = pm25val;

    lowpulseoccupancy = 0;
    init_time = millis();
  }
  if(digitalRead(buttonPinBlue) == HIGH){
    rzero = mq135_sensor.getRZero();

    if(digitalRead(ledPinBlue) == HIGH){
      digitalWrite(ledPinBlue, LOW);
      delay(200);
      digitalWrite(ledPinBlue, HIGH);
    }
    else{
      digitalWrite(ledPinBlue, HIGH);
      delay(200);
      digitalWrite(ledPinBlue, LOW);
    }
  }
  // Get temp and humidity values with DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float ppmCO = mq7.getPPM();

  // Get sensor CO2 Resistence (RS/R0) to convert values in PPM
  //float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  //float resistance = mq135_sensor.getResistance();
  float ppmco2 = mq135_sensor.getPPM(rzero);
  //float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity, rzero);

  if(sdcard_status){
    sensorFile = SD.open(FILE_NAME, FILE_WRITE);
    sensorFile << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", " <<  pm25val  << "\n";
    sensorFile.close();
    if(digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, LOW);
      while(1);
    }
  }
    Serial << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", " << pm25val  << "\n";
  if (init_time_in_loop < 1000){
    delay(1000-init_time_in_loop);
  }
}