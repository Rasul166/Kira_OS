#include <stdio.h>
#include <kira_uart.h>
#include <kira_delay.h>
#include <kira_task.h>
#include <kira_ds.h>
#include <kira_timer.h>
// Reset and Clock Control
#define rcc_apb2enr *((volatile unsigned int *)0x40021018)

// GPIO Port C (LED Blinking)
#define gpio_crh *((volatile unsigned int *)0x40011004)
#define gpio_odr *((volatile unsigned int *)0x4001100C)
Mutex_t uart_mutex;
Semaphore_t rx_sem;
Kira_Queue_t kira_queue;
void Task1(void)
{
	while (1)
	{
		kira_mutex_take(&uart_mutex);
		kira_print_string("1111111111111111 end ");
		kira_mutex_give(&uart_mutex);
		kira_task_sleep(2);
		delayms(1);
	}
}

void Task2(void)
{
	while (1)
	{
		kira_mutex_take(&uart_mutex);
		kira_print_string("222222222222222222 end ");
		kira_mutex_give(&uart_mutex);
		kira_task_sleep(1);
		delayms(1);
	}
}

void Task3(void)
{
	while (1)
	{
		kira_semaphore_wait(&rx_sem);
		__disable_irq();
		char as = ring_buffer_pop();
		if (as != '_')
		{
			kira_print_char(as);
		}
		kira_scheduler();
		__enable_irq();
	}
}
void Task4(void)
{
	int i = 0;
	float j = 0;

	while (1)
	{
		Custom_data cstm;
		cstm.sensor_id = i;
		cstm.temperature = j;
		i++;
		j = j + 2;
		kira_queue_send(cstm,&kira_queue);
	}
}
void Task5(void)
{
	while (1)
	{
		Custom_data cd = kira_queue_receive(&kira_queue);
		int a = cd.sensor_id;
		int b = cd.temperature;
		kira_mutex_take(&uart_mutex);
		kira_print_string("Sensor Id:");
		kira_print_int(a);
		kira_print_string("Temperature:");
		kira_print_int(b);
		kira_mutex_give(&uart_mutex);
	}
}
void Task6(void)
{
	while (1)
	{ int arr[3000];
		for(int i=0;i<3000;i++){
			arr[i]=1;
		}
		
	}
}


int main()
{

	// 1. Initialize SysTick (1ms heartbeat at 72MHz / 8)
	kira_uart_init();
	kira_task_create(Task1, 2);
	kira_task_create(Task2, 2);
	kira_task_create(Task3, 3);
	kira_task_create(Task4, 1);
	kira_task_create(Task5, 1);

	kira_systick_init();
	kira_mutex_init(&uart_mutex);
	kira_semaphore_init(&rx_sem);
	kira_os_start();
	return 0;
}
