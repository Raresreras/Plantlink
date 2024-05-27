#include <VirtualWire.h>              
byte message[VW_MAX_MESSAGE_LEN]; //buffer
byte messageLength = VW_MAX_MESSAGE_LEN; //message size
int received_data = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Ready...");
  vw_set_rx_pin(5);
  vw_setup(2000);
  vw_rx_start();
}

void loop()
{
  if (vw_get_message(message, &messageLength)) // non-blocking
  {
    Serial.print("Input: ");
    for (int i = 0; i < messageLength; i++)
    {
      //Serial.print(message[i]);
      received_data = message[i];
    }
    //Serial.println(received_data);
    Serial.write(received_data);
    Serial.println("Poate");
  }
}