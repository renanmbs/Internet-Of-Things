/*
  Name: Renan Mougenot Breviglieri Silva
  Date: January 13th, 2022
  Title: Handle LED according to the Serial Line input
*/


//Run one time
void setup() {
  
  //Baud Rate
  Serial.begin(9600);

  delay(1000);
  
  //Print Options
  Serial.println("Welcome, those are your options:");
  Serial.println("Send H/h to turn LED On");
  Serial.println("Send L/l to turn LED Off");

  //Attach and start LED
  pinMode(LED_BUILTIN, OUTPUT); 

  delay(500);
}

//Loop forever
void loop() {

  //Parameters
  char command; //Hold the command input

  //Wait for input
  while(Serial.available() == 0){}

  //Read the command from the Serial
  command = Serial.read();

  //If it reads an H or h
  if(command == 'H' || command == 'h'){
    
    Serial.println("LED On"); //Send message of led on
    digitalWrite(LED_BUILTIN, HIGH);   //Turn the LED on
  
  }
  
  //If it reads an L or l
  else if(command == 'L' || command == 'l'){
    
    Serial.println("LED Off"); //Send message of led off
    digitalWrite(LED_BUILTIN, LOW); //Turn the LED off
    
  }

}
