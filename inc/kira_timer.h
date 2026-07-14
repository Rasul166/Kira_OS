 #include<kira_ds.h>
 #include<stdint.h>
 
  typedef struct 
   {
    int Timer_id;
    int period;
    char mode;
    int Time_remaining;
    void (*func_ptr)(void);
    int state;                 
   }Kira_timer_t;

   
 typedef struct
{
    int timer_id;
    int operation;
}kira_timer_command;

   extern  Kira_timer_t LOST[15];
   extern volatile int timer_count;
   extern  int exp_timers[15];
   extern volatile int daemon_task_id;
   

   KIRA_BUFFER_DEFINE(kira_timer_command,command_queue,20);
   extern command_queue cmd_q;

   void kira_timer_create(int Timer_id,int period,char mode,void (*fu_ptr)(void));
   void kira_timer_command_send(int timerid,int op);
   kira_timer_command  kira_timer_command_receive(void);
   void __disable_irq();
   void __enable_irq();
