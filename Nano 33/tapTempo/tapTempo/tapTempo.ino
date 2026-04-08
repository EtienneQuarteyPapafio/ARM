//tapTempo

//A simple code for tap tempo utilisation using FreeRTOS

#include <FreeRTOS.h> //Adds header files for FreeRTOS
#include <task.h>
#include <semphr.h>
#include <queue.h>

//initial variables
int buttonState; //current state of input pin
int lastButtonState=LOW; //previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void taskBlink(void *pvParameters); //function prototype for execution, takes arguments of the pointer pvParameters
void taskTap(void *pvParameters);
void taskPrint(void *pvParameters);

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); //Creates Serial communication

xTaskCreate(taskBlink,"Task1",128,NULL,1,NULL);
xTaskCreate(taskPrint,"Task2",128,NULL,1,NULL);
xTaskCreate(taskTap,"Task3",128,NULL,1,NULL);

//After creating the task, start the scheduler in the void setup using the

vTaskStartScheduler(); //API

}

void loop() {
  // put your main code here, to run repeatedly:
}

void taskBlink(void *pvParameters)
{
  pinMode(9, OUTPUT);
  while(1)
  {
  digitalWrite(9, HIGH);
  vTaskDelay(800/portTICK_PERIOD_MS);
  digitalWrite(9, LOW);    
  vTaskDelay(800/portTICK_PERIOD_MS);
  }
};

void taskTap(void *pvParameters)
{
  pinMode(10, INPUT);
  int reading;
  while(1)
  {
  int reading = digitalRead(10);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      
    }
  }

  lastButtonState = reading;
}
};

void taskPrint(void *pvParameters){
  while(1)
  {
   Serial.println("Tap");
  vTaskDelay(1600/portTICK_PERIOD_MS); 
  }
};