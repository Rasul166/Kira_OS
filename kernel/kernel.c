void kira_os_start(void)
{
    daemon_task_id = (kira_task_create(kira_daemon_task, 4) - 1);
    kira_task_create(kira_idle_task, 1);
    __asm volatile("svc 0");
}
