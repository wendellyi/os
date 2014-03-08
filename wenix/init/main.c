
extern int printk(const char * fmt, ...);
extern void move_to_user_mode(void);

void init_first_task(void)
{
}

void main(void)
{
    init_first_task();
    move_to_user_mode();
}

void delay(int sec)
{
	int i, j;
	for (i = 0; i < 10000 * sec; i++)
		for (j = 0; j < 10000; j++)
			;
}

void task_a(void)
{
    int i = 0;
    while (1)
    {
    	printk("A%d", i++);
    	delay(1);
    }
}

void task_b(void)
{
    int i = 0;
    while (1)
    {
    	printk("B%d", i++);
    	delay(1);
    }
}