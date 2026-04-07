#include <FreeRTOS.h> //Adds header file for FreeRTOS
#include <task.h>
#include <semphr.h>
#include <queue.h>

void taskBlink1(void *pvParameters); 

//function prototype for execution, takes arguments of the pointer pvParameters

void taskBlink2(void *pvParameters); 

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

xTaskCreate(taskBlink1,"Task1",128,NULL,1,NULL);
xTaskCreate(taskBlink2,"Task2",128,NULL,1,NULL);
xTaskCreate(taskPrint,"Task3",128,NULL,1,NULL);

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

void taskBlink1(void *pvParameters)
{
  pinMode(8, OUTPUT);
  while(1)
  {
  digitalWrite(8, HIGH);
  vTaskDelay(200/portTICK_PERIOD_MS);
  digitalWrite(8, LOW);    
  vTaskDelay(200/portTICK_PERIOD_MS);
  }
};

void taskBlink2(void *pvParameters)
{
  pinMode(9, OUTPUT);
  while(1)
  {
  digitalWrite(9, HIGH);
  vTaskDelay(300/portTICK_PERIOD_MS);
  digitalWrite(9, LOW);    
  vTaskDelay(300/portTICK_PERIOD_MS);
  }
}; 

void taskPrint(void *pvParameters)
{
  int counter = 0;
  while(1)
  {
counter++;
Serial.println(counter);
vTaskDelay(500/portTICK_PERIOD_MS); 
  }
};