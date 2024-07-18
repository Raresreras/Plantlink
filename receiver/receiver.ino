#include <VirtualWire.h>

#define pump1 8 //
#define pump2 7 //
#define pump3 9 //

#define hum_sen_1 A1 //fertilizer level
#define hum_sen_2 A2 //main water level

//communication
byte message[VW_MAX_MESSAGE_LEN]; //buffer
byte messageLength = VW_MAX_MESSAGE_LEN; //message size
int received_data = 0;

//humidity sensor maximum and minimum values
int hum_min1 = 150;
int hum_max1 = 500; 
int hum_min2 = 150;
int hum_max2 = 500; 


int pump_activ = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  vw_set_rx_pin(5);
  vw_setup(2000);
  vw_rx_start();
}

void loop() {
  Serial.println("WL1_" + String(100 - get_level1())); //Water level 1
  Serial.println("WL2_" + String(100 - get_level2())); //Water level 2
  Serial.println(get_data_transmitter());
  if (Serial.available() > 0) pump_activ = read_serial();
  switch(pump_activ){
    case 1:
      digitalWrite(pump1, HIGH);
      delay(50);
      digitalWrite(pump1, LOW);
      break;
    case 2:
      digitalWrite(pump2, HIGH);
      delay(50);
      digitalWrite(pump2, LOW);
      break;
    case 3:
      digitalWrite(pump3, HIGH);
      delay(50);
      digitalWrite(pump3, LOW);
      break;
  }
}

int read_serial(){
  byte input = 0;
  if (Serial.available() > 0) {
    input = Serial.read();
    return input - 48; //48 is the ascii value for 0 
  }
}

int get_data_transmitter(){
  if (vw_get_message(message, &messageLength)){
    for (int i = 0; i < messageLength; i++){
      received_data = message[i];
    }
    return received_data;
  }
}

int get_level1(){
  int value = analogRead(hum_sen_1);
  if (value > hum_max1) hum_max1 = value;
  if (value < hum_min1) hum_min1 = value;
  return (float) (value - hum_min1)/(hum_max1 - hum_min1) * 100; 
}

int get_level2(){
  int value = analogRead(hum_sen_2);
  if (value > hum_max2) hum_max2 = value;
  if (value < hum_min2) hum_min2 = value;
  return (float) (value - hum_min2)/(hum_max2 - hum_min2) * 100; 
}
