/************************************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (STAT) will blink, when jumper LED_E is closed.
*
* Running Release code will set ReadOutProtection (see down) via function FLASH_ReadOutProtection_Enable().
* Do not run Release code until you know how to set back ReadOutProtection!
************************************************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_flash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void FLASH_ReadOutProtection_Enable(void);
void DelayByDiv(void);
void BlinkLedOff ( void );
void BlinkLedOn ( void );

/* The queue used by both tasks. */
static QueueHandle_t xQueueOn = NULL;
static QueueHandle_t xQueueOff = NULL;

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c, heap_2.c or heap_4.c are used, then the size
	of the heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE
	in FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook	function is
	called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void BlinkLedOff ( void )
{

	volatile unsigned int * pioc_addr = ( unsigned int *)0x40011010;
	unsigned long ulReceivedValue;
	const unsigned long ulValueToSend = 100UL;

	while(1)
	{

		xQueueReceive( xQueueOff, &ulReceivedValue, portMAX_DELAY );
		*pioc_addr = * pioc_addr |  0x20000000;
		DelayByDiv(); // delay --> not much compiler optimizer settings dependent
		xQueueSend( xQueueOn, &ulValueToSend, 0 );

	}

}

void BlinkLedOn ( void )
{

	volatile unsigned int * pioc_addr = ( unsigned int *)0x40011010;
	const unsigned long ulValueToSend = 100UL;
	unsigned long ulReceivedValue;

	while(1)
	{
		*pioc_addr = * pioc_addr | 0x2000; 
		DelayByDiv(); // delay --> not much compiler optimizer settings dependent
		xQueueSend( xQueueOff, &ulValueToSend, 0 );
		xQueueReceive( xQueueOn, &ulReceivedValue, portMAX_DELAY );



	}

}



int main(int argc, char *argv[])
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// GPIOC Periph clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// Configure PC12 to mode: slow rise-time, pushpull output
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; // GPIO No. 12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // GPIO No. 13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // slow rise time
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOC, &GPIO_InitStructure); // GPIOC init

	//FLASH_ReadOutProtection_Enable(); // enable ReadOutProtection when running Release code


	/* Create the queue. */
	xQueueOn = xQueueCreate( 2, sizeof( unsigned long ) );
	xQueueOff = xQueueCreate( 2, sizeof( unsigned long ) );


	/* Start the two tasks as described in the comments at the top of this
	file. */
	xTaskCreate( (TaskFunction_t)BlinkLedOff, "BlkLed", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), NULL );
	xTaskCreate( (TaskFunction_t)BlinkLedOn, "BlkLed", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 2), NULL );

#if 0
	/* Create the software timer that is responsible for turning off the LED
	if the button is not pushed within 5000ms, as described at the top of
	this file. */
	xLEDTimer = xTimerCreate( 	"LEDTimer", 				/* A text name, purely to help debugging. */
								( 5000 / portTICK_PERIOD_MS ),/* The timer period, in this case 5000ms (5s). */
								pdFALSE,					/* This is a one shot timer, so xAutoReload is set to pdFALSE. */
								( void * ) 0,				/* The ID is not used, so can be set to anything. */
								vLEDTimerCallback			/* The callback function that switches the LED off. */
							);
#endif
	/* Start the tasks and timer running. */
	vTaskStartScheduler();


	for (;;);
}

void FLASH_ReadOutProtection_Enable(void)
// If FLASH readout protection not already set, enable protection and reset device
// NOTES: The user area of the Flash memory can be protected against read by untrusted code.
//Protection is enabled only for firmware compiled with flag RELEASE_PUBLIC set (see makefile).
//When readout protection is set debugging via JTAG is not possible any more.
//If the read protection is set while the debugger is still connected through JTAG/SWD, apply a
//POR (power-on reset) instead of a system reset (without debugger connection).
{
	if (FLASH_GetReadOutProtectionStatus() != SET)
	{
#ifdef RELEASE_PUBLIC // HINT: define is done via makefile
	FLASH_Unlock();
	if (FLASH_ReadOutProtection(ENABLE) != FLASH_COMPLETE) // set readout protection
	{
	// ERROR: could not program read protection
	}
	else
	NVIC_SystemReset(); // protection set --> reset device to enable protection
#else
	// output warning message
#endif
	}
}

void DelayByDiv(void)
// delay implemented by floating division
// not much compiler optimizer settings dependent
{
	float x=50.0f;
	while (x > 0.0001f)
	x = x/1.0001f; // delay loop
}

