/*
  Name: Renan Mougenot Breviglieri Silva
  Date: January 13th, 2022
  Title: Get to know your kit "improved"
*/


/*THIS CODE WAS MODIFIED BY ME TO DO EXTRA THINGS*/


//Include libraries
#include <Arduino_MKRIoTCarrier.h>

//Global parameters
MKRIoTCarrier carrier; //Initialize carrier
float temperature = 0; //Float to hold the temperature
float humidity = 0; //Float to hold the humidity
uint32_t cold = carrier.leds.Color(0, 0, 200); //Int to hold the color blue for the LEDs
uint32_t hot = carrier.leds.Color(0, 200, 0); //Int to hold the color red for the LEDs
uint32_t color_humidity = carrier.leds.Color(80, 80, 90); //Int to hold the color white-ish for the LEDs (green, red, blue)
uint32_t off = carrier.leds.Color(0, 0, 0); //Int to turn off the LEDs


void setup() {

  //Set baud rate
  Serial.begin(9600);

  //Enclosure
  CARRIER_CASE = true;

  //Initialize the IoTSK carrier
  carrier.begin();

}

void loop() {

  //Read temperature sensor and put it in temperature
  temperature = carrier.Env.readTemperature();

  //Delay
  delay(200);

  //Read humidity sensor and put it in humidity
  humidity = carrier.Env.readHumidity();

  //Update buttons
  carrier.Buttons.update();

  //Print temperature in the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  /* HANDLE BUTTONS */

  //If button 0 is touched
  if (carrier.Buttons.onTouchDown(TOUCH0)) {

    //Turn Off LEDs
    carrier.leds.fill(off);
    carrier.leds.show();

    //Call function to print temperature
    printTemperature();

 }

 //If button 1 is touched
 if (carrier.Buttons.onTouchDown(TOUCH1)) {

   //Turn Off LEDs
   carrier.leds.fill(off);
   carrier.leds.show();

   //Call function to print humidity
   printHumidity();

 }
}

//Function to print temperature
void printTemperature() {

 //If temperature is less than 22 celsius
 if(temperature < 22){

     //Messages to print in the display
     char *message = "Muito Frio!";
     char *message_2 = "Ta Louco";

     //Information of the screen
     carrier.display.fillScreen(ST77XX_WHITE); //BACKGROUND: white
     carrier.display.setTextColor(0x7FF); //TEXT: cyan
     carrier.display.setTextSize(3); //SIZE: medium

     carrier.display.setCursor(10, 20); //Set cursor (y, x)

     //Print information
     carrier.display.print("Temperature: "); //Print text
     carrier.display.setTextColor(ST77XX_RED); //Set new color to the text (TEXT: red)
     carrier.display.setCursor(60, 60); //New cursor for the temperature (y, x)
     carrier.display.print(temperature); //Print temperature
     carrier.display.println(" C"); //Print C from Celsius

     //Set the LEDs
     carrier.leds.fill(cold); //LED: blue
     carrier.leds.setBrightness(temperature); //Set brightness according to temperature
     carrier.leds.show(); //Start LEDs

     //Print extra messages
     carrier.display.setCursor(35, 120); //Set cursor for first message (y, x)
     carrier.display.println(message); //Print first custom message

     carrier.display.setCursor(60, 150); //Set cursor for second message (y, x)
     carrier.display.println(message_2); //Print second custom message


 }

//If temperature is more than 22 celsius
else{

     //Messages to print in the display
     char *message = "Ta Quente";
     char *message_2 = "Bora!";

     //Information of the screen
     carrier.display.fillScreen(0x0000); //BACKGROUND: black
     carrier.display.setTextColor(ST77XX_RED); //TEXT: red
     carrier.display.setTextSize(3); //SIZE: medium

     carrier.display.setCursor(10, 20); //Set cursor (y, x)

     //Print information
     carrier.display.print("Temperature: "); //Print text
     carrier.display.setTextColor(0xBDF7); //Set new color to the text (TEXT: light gray)
     carrier.display.setCursor(60, 60); //New cursor for the temperature (y, x)
     carrier.display.print(temperature); //Print temperature
     carrier.display.println(" C"); //Print C from Celsius

     //Set the LEDs
     carrier.leds.fill(hot); //LED: red
     carrier.leds.setBrightness(temperature); //Set brightness according to temperature
     carrier.leds.show(); //Start LEDs

     //Print extra messages
     carrier.display.setCursor(35, 120); //Set cursor for first message (y, x)
     carrier.display.println(message); //Print first custom message

     carrier.display.setCursor(60, 150); //Set cursor for second message (y, x)
     carrier.display.println(message_2); //Print second custom message

  }
}


//Function to print humidity
void printHumidity() {

//Configuring display
 carrier.display.fillScreen(0x86B4); //BACKGROUND: green-ish
 carrier.display.setTextColor(0x0000); //TEXT: black
 carrier.display.setTextSize(3); //SIZE: medium

 carrier.display.setCursor(50, 20); //Set cursor (y, x)

 //Print information
 carrier.display.print("Humidity: "); //Print text
 carrier.display.setTextColor(0x4AFB); //Set new color to the text (TEXT: blue-ish)
 carrier.display.setCursor(60, 60); //New cursor for the humidity (y, x)
 carrier.display.print(humidity); //Print humidity
 carrier.display.println(" %");//Print percent sign
 carrier.display.fillCircle(120, 150, humidity, 0x675D); //Print a circle based on the humidity (y,x,size,color)

 //Set the LEDs
 carrier.leds.fill(color_humidity); //LED: custom (white-ish)
 carrier.leds.setBrightness(humidity); //Set brightness according to humidity
 carrier.leds.show(); //Start LEDs

}
