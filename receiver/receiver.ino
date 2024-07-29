#include <VirtualWire.h>

#define pump1 8 //fertilizer pump
#define pump2 7 //water source pump
#define pump3 9 //main pump

#define hum_sen_1 A1 //main water level
#define hum_sen_2 A2 //fertilizer level

//communication
byte message[VW_MAX_MESSAGE_LEN]; //buffer
byte messageLength = VW_MAX_MESSAGE_LEN; //message size
int received_data = 0;

//humidity sensor maximum and minimum values - calibration values
int hum_min1 = 220;
int hum_max1 = 410; 
int hum_min2 = 200; 
int hum_max2 = 480; 


int pump_activ = 0;

void setup() {
  Serial.begin(57600);
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
  while (Serial.available() > 0) {
    pump_activ = Serial.readStringUntil("/n").toInt();
      if (pump_activ > 0 && pump_activ < 5){
        delay(50);

        if (pump_activ == 1){
          digitalWrite(pump1, HIGH);
          delay(2000);
          digitalWrite(pump1, LOW);
        }
        if (pump_activ == 2){
          digitalWrite(pump2, HIGH);
          delay(2000);
          digitalWrite(pump2, LOW);
        }
        if (pump_activ == 3){
          digitalWrite(pump3, HIGH);
          delay(2000);
          digitalWrite(pump3, LOW);
          pump_activ = 0;
        }
      }
    
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
