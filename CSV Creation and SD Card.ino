	
/*
  Name: Renan Mougenot Breviglieri Silva
  Date: February 9th, 2022
  Title: Storing our data - modified
*/

/*Task 2 is Task 1 modified and improved*/

 
//Include libraries
#include <Arduino_MKRIoTCarrier.h>

//Initialize carrier
MKRIoTCarrier carrier;

//Global parameters
float temperature = 0; //Temperature
float humidity = 0; //Humidity
float pressure = 0; //Pressure
int light = 0; //Light
int r,g,b; //Colors (red,green,blue)
 
//Gyroscope
float Gx, Gy, Gz; //X,Y,Z Coordinates
 
//Accelerometer
float Ax, Ay, Az; //X,Y,Z Coordinates
 
//Counter to count the number of readings
int counter = 0;

//LEDs
uint32_t red = carrier.leds.Color(0, 200, 0); //Int to hold the color red for the LEDs
uint32_t color_gyroscope = carrier.leds.Color(0, 0, 0); //Int to hold the color for the LEDs (green, red, blue)
uint32_t off = carrier.leds.Color(0, 0, 0); //Int to turn off the LEDs

//Acceleration
float threshold = 1.8; //Threshold
float sum_acceleration = 0; //Sum of accelerations 
 
//SD information
File dataFile;
 
void setup() {

  Serial.begin(9600); //Set baud rate

  CARRIER_CASE = true; //Case being used
  carrier.begin(); //Start the carrier
  
  //Initialize the SD card
  if (!SD.begin(SD_CS)) {
    carrier.display.setTextSize(2); //Size of text
    carrier.display.setCursor(35, 70); //Cursor
    carrier.display.print("SD card failed"); //What to display
    carrier.display.setCursor(45, 110); //New cursor
    carrier.display.print("to initialize"); //New message
    while (1); //Loop while there is no SD card
  }
 
  //Initialize the file to be created
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
 
  //Initialize the CSV file and the headers (removed pressure)
  dataFile.println("Temperature,Humidity,Light,Ax,Ay,Az,Gx,Gy,Gz");
 
  //Close the file
  dataFile.close();
  delay(100);

}
 
void measurements(){

  //Initialize the logfile (again)
  dataFile = SD.open("log-0000.csv", FILE_WRITE);
 
  //Read the sensors values
  temperature = carrier.Env.readTemperature();
  humidity = carrier.Env.readHumidity();
  
  //While there is no color
  while(!carrier.Light.colorAvailable()){
    delay(5);
  }
  //Read the color (red, green, blue and hold it in the variable)
  carrier.Light.readColor(r, g, b, light);
  
  //Read the IMU values (Acceleration and Gyroscope)
  carrier.IMUmodule.readAcceleration(Ax, Ay, Az);
  carrier.IMUmodule.readGyroscope(Gx, Gy, Gz);
  
  /*Print each of the sensor values*/

  //Temperature
  dataFile.print(temperature);
  dataFile.print(",");
 
  //Humidity
  dataFile.print(humidity);
  dataFile.print(",");

  //Light
  dataFile.print(light);
  dataFile.print(",");
  
  //Acceleration
  dataFile.print(Ax); //X
  dataFile.print(",");
  dataFile.print(Ay); //Y
  dataFile.print(",");
  dataFile.print(Az); //Z
  dataFile.print(",");
 
  //Gyroscope
  dataFile.print(Gx); //X 
  dataFile.print(",");
  dataFile.print(Gy); //Y
  dataFile.print(",");
  dataFile.print(Gz); //Z
  dataFile.println(",");

  //Close datafile
  dataFile.close();
  

  //Counter of readings
  counter += 1;

}

void security(){

  //Turn On LEDs
  carrier.leds.fill(red);
  carrier.leds.show();

  //Make a sound
  carrier.Buzzer.sound(200);

  //Display message
  carrier.display.fillScreen(ST77XX_RED); //Red background
  carrier.display.setTextColor(ST77XX_WHITE); //White text
  carrier.display.setTextSize(2); //Small sized 
  carrier.display.setCursor(0, 20); //Cursor
  carrier.display.print("They are stealing me"); //Print message

}

void stop_security(){

  carrier.Buzzer.noSound(); //No Sound
  carrier.leds.fill(off); //No light
  carrier.leds.show(); //Show light

}
void loop() {

  carrier.Buttons.update(); //Read buttons

  //If button 0 is pressed, take measurements
  if (carrier.Buttons.onTouchDown(TOUCH0)) {

    stop_security();

    measurements(); //Take measurements

    //Display counter  
    carrier.display.fillScreen(ST77XX_BLACK); //Black background
    carrier.display.setTextColor(ST77XX_WHITE); //White text
    carrier.display.setTextSize(2); //Small sized 
    carrier.display.setCursor(0, 80); //Cursor
    carrier.display.print("Taking measurements"); //Print message

}

//If button 1 is pressed, stop taking measurements
if (carrier.Buttons.onTouchDown(TOUCH1)) {

    stop_security();

    carrier.display.fillScreen(ST77XX_BLACK); //Black background
    carrier.display.setTextColor(ST77XX_WHITE); //White text
    carrier.display.setTextSize(2); //Small sized 
    carrier.display.setCursor(0, 80); //Cursor
    carrier.display.print("Stopped measurements"); //Print message

  }

//If button 2 is pressed, activate the security mode
if (carrier.Buttons.onTouchDown(TOUCH2)) {

  carrier.IMUmodule.readAcceleration(Ax, Ay, Az);

   stop_security();

  carrier.display.fillScreen(ST77XX_RED); //Red background
  carrier.display.setTextColor(ST77XX_WHITE); //White text
  carrier.display.setTextSize(2); //Small sized 
  carrier.display.setCursor(0, 80); //Cursor
  carrier.display.print("Security mode activated"); //Print message

  
  //Check if accelerometer is bigger than threshold
  sum_acceleration = Ax + Ay + Az;
  
  if(sum_acceleration > threshold){
    security(); //Activate security function
  }
}

//If button 3 is pressed, activate gyroscope colors
if (carrier.Buttons.onTouchDown(TOUCH3)) {

  carrier.IMUmodule.readGyroscope(Gx, Gy, Gz);

   stop_security();

  carrier.display.fillScreen(ST77XX_CYAN); //Cyan background
  carrier.display.setTextColor(ST77XX_BLACK); //RED text
  carrier.display.setTextSize(3); //Small sized 
  carrier.display.setCursor(0, 50); //Cursor
  carrier.display.print("Move device"); //Print message

  //Set colors of the LED according to the gyroscope reading (absolute value because I can't have negative colors)
  int abs_gx = abs(Gx);
  int abs_gy = abs(Gy);
  int abs_gz = abs(Gz);

  color_gyroscope = carrier.leds.Color(abs_gx, abs_gy, abs_gz); //Colors of the LED is the balance of (Green, Red, Blue)
  carrier.leds.fill(color_gyroscope); //No light
  carrier.leds.show(); //Show light
  
  }
}
