//tapTempo

//A simple code for tap tempo utilisation using FreeRTOS

#include <FreeRTOS.h> //Adds header files for FreeRTOS
#include <task.h>
#include <semphr.h>
#include <queue.h>

//initial variables
int buttonState; //current state of input pin
int lastButtonState=LOW; //previous reading from the input pin
bool waitingForSecondTap=false;

volatile unsigned long newMs=600; //Default Ms, volatile prevents compiler from optimization
unsigned long firstTap=0; //Ms of first tap
unsigned long secondTap=0; //Ms ofs second tap

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 30;    // the debounce time; increase if the output flickers

void taskBlink(void *pvParameters); //function prototype for execution, takes arguments of the pointer pvParameters
void taskTap(void *pvParameters);

//Creating handles to be able to be referenced by other tasks

TaskHandle_t Task_Handle1;

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600); //Creates Serial communication

xTaskCreate(taskBlink,"Task1",128,NULL,1,&Task_Handle1);
xTaskCreate(taskTap,"Task2",128,NULL,1,NULL);

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
  vTaskDelay((newMs/2)/portTICK_PERIOD_MS);
  digitalWrite(9, LOW);    
  vTaskDelay((newMs/2)/portTICK_PERIOD_MS);
  }
};

void taskTap(void *pvParameters)
{
  pinMode(10, INPUT);
  int reading;
  
  while(1)
  {
  reading = digitalRead(10);

  // If the switch changed, due to noise or pressing:
  if (reading == HIGH && lastButtonState==LOW) {
    // reset the debouncing timer
    
    
   if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    lastDebounceTime = millis();

    

    if (!waitingForSecondTap){

    vTaskSuspend(Task_Handle1);
    
    firstTap=millis(); //gets the current time

    waitingForSecondTap=1;

    Serial.println("one");

    }

    else{

    secondTap=millis(); //gets current time of second tap

    newMs=secondTap-firstTap;
    
    waitingForSecondTap=false; //reset

    Serial.println("two");

    vTaskResume(Task_Handle1);

    }
   }
  }

  lastButtonState=reading;

 }

  
vTaskDelay(1 / portTICK_PERIOD_MS);

};