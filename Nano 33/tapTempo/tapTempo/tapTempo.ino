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

  // In the set up function we can create tasks and start the taask scheduler

  // The xTaskCreate() API is called here with the following 6 parameters/arguements

  //xTaskCreate( TaskFunction_t pvTaskCode, const char* const pcName, uint16_t usStackDepth, void *pvParameters, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask);

/*
pvTaskCode: It is simply a pointer to the function that implements the task (in effect, just the name of the function).
pcName: A descriptive name for the task. This is not used by FreeRTOS. It is included purely for debugging purposes.
usStackDepth: Each task has its own unique stack that is allocated by the kernel to the task when the task is created. 
The value specifies the number of words the stack can hold, not the number of bytes. 
For example, if the stack is 32-bits wide and usStackDepth is passed in as 100, then 400 bytes of stack space will be allocated (100 * 4 bytes) in RAM. 
For example Arduino Uno has only 2Kbytes of RAM.
pvParameters: Task input parameter (can be NULL).
uxPriority: Priority of the task ( 0 is the lowest priority).
pxCreatedTask: It can be used to pass out a handle to the task being created. 
This handle can then be used to reference the task in API calls that, for example, change the task priority or delete the task (can be NULL).
*/

//Example task creation:

xTaskCreate(taskBlink,"Task1",128,NULL,1,NULL);
xTaskCreate(taskPrint,"Task2",128,NULL,1,NULL);
xTaskCreate(taskTap,"Task3",128,NULL,1,NULL);

//After creating the task, start the scheduler in the void setup using the

vTaskStartScheduler(); //API

//Void loop() function will remain empty as we don’t want to run any task manually and infinitely. Because task execution is now handled by Scheduler.

//Now, we have to implement task functions and write the logic that you want to execute inside these functions. 
//The function name should be the same as the first argument of xTaskCreate() API

/*The code will need a  delay function to stop a task running, but as RTOS is real time
the Delay() function is not suggested at it stops the CPU.

The suggested kernel API 

vTaskDelay(const TickType_t xTicksToDelay);

delays a task for a number of ticks, the constant portTick_PERIOD_MS can be used to calculate real time

e.g vTaskDelay(200/portTick_PERIOD_MS); can be used for a delay of 200ms

*/
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