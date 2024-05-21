#include <DHT11.h>
#include <VirtualWire.h>

//pin initialization
DHT11 dht11(2);

//soil sensor
#define soil_input A0
int soil_max = 500;
int soil_min = 250;

// virtual wire
#define size 1
byte TX_buffer[size]={0};
byte i;

int board_code = 101; //board version + board number

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);


  // virtual wire
  vw_set_tx_pin(5); // pin
  vw_setup(2000); // bps
  for(i=0;i<size;i++)
  {
     TX_buffer[i]=i;
  }
}

void loop() {
  /*Serial.print("Temp: ");
  Serial.println(get_temp());
  Serial.print("Hum: ");
  Serial.println(get_hum());
  Serial.print("Soil hum:");
  Serial.println(100 - get_soil());*/
  transmit(byte('V'));
  transsmit(board_code);
  transmit(byte('T'));
  trasnmit(get_Temp());
  transmit(byte('H'));
  transmit(get_hum());
  transmit(byte('S'));
  transmit(100 - get_soil());
}

//Sensors

int get_temp(){
  int temperature = dht11.readTemperature();
  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT)
    return temperature;
}

int get_hum(){
  int humidity = dht11.readHumidity();
  if (humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT)
    return humidity;
}

float get_soil(){
  int value = analogRead(soil_input);
  if (value > soil_max) soil_max = value;
  if (value < soil_min) soil_min = value;
  return (float) (value - soil_min)/(soil_max - soil_min) * 100; 
}

void transmit(int val){
  TX_buffer[0] = val;  
  vw_send(TX_buffer, size); 
  vw_wait_tx(); 
  delay(10);  
}
