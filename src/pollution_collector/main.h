/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.9 01/08/18
**********************************************/


//#include<DSM501.h>

// Calibration resistance at atmospheric CO2 level
float rzero = 76.63;

#define DSM501_PM25 6
#define DHTPIN 5
#define PIN_MQ135 A4
#define PIN_MQ7 A0

#define DHTTYPE DHT22

///sdcard
bool sdcard_status = false;
#define FILE_NAME "valores.csv"
File sensorFile;

bool bluetooh_status = false;
//#define BLUETOOTH_NAME "Dobrador  de AR"
//#define BLUETOOTH_PASSWORD 0666

//SoftwareSerial bluetooth(6, 7); // Emulate port (Rx, Tx) to use with hc-06

const int pin = 3 ;// the input pin connect with  the Vout2 of Dust Sensor
float duration;
long unsigned int init_time;
long unsigned int sampletime_ms = 30000;
float lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

#define TIME_MAX  30
float pm25val = 0.05;
float pm25coef = 0.00207916725464941;//The coefficient of pm2.5
float pm25[TIME_MAX+1] ;//collect data of pm2.5 after a period of time

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor = MQ135(PIN_MQ135);
MQ7 mq7(PIN_MQ7,5.0);

// led and button config
const int buttonPinBlue = 2;
const int buttonPinSD = 7;
const int ledPinBlue =  8;
const int ledPinSD =  9;