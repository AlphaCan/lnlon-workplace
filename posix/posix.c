#include <stdio.h>  
#include <time.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <string.h>  
#include <unistd.h>  

timer_t timerid;


#define printf_with_time(format, ...)                                        \
{                                                                            \
    struct timespec spec;                                                    \
    clock_gettime(CLOCK_MONOTONIC, &spec);                                    \
    printf("[%lu:%lu]"format"\n", spec.tv_sec, spec.tv_nsec, ##__VA_ARGS__);\
}

void sighandler_cb(int val)
{
    if (SIGUSR1 == val)
    {
        printf_with_time("sighandler");
	
    }
    else
    {
        printf_with_time("delete timer");
        timer_delete(timerid);
        exit(EXIT_SUCCESS);
    }
}

/*void timer_thread(int signum)  
{
	struct timespec ts;
	
	clock_gettime(CLOCK_MONOTONIC, &ts);
	printf("posix test %lu:%lu\n",ts.tv_sec,ts.tv_nsec);
		
}*/

int i;
int main(void)
{
	
	struct sigevent evp;
//	struct sigaction act;
	
	signal(SIGUSR1, sighandler_cb);    /* for timer */
        signal(SIGINT, sighandler_cb);    /* Ctrl + C */

/*	memset(&act, 0, sizeof(act));  
        act.sa_handler = sighandler_cb;  
        act.sa_flags = 0; 
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGUSR1, &act, NULL) == -1)  
        {  
        	printf("fail to sigaction\n");  
        	exit(-1);  
        }  */

	memset(&evp, 0, sizeof(struct sigevent));  
        evp.sigev_signo = SIGUSR1;  
        evp.sigev_notify = SIGEV_SIGNAL;  
	printf_with_time("create timer");
        if (timer_create(CLOCK_MONOTONIC, &evp, &timerid) == -1)  
        {  
        	printf("fail to timer_create\n");  
        	exit(-1);  
        }  
  
   	struct itimerspec it;  
    	it.it_interval.tv_sec = 0;
    	it.it_interval.tv_nsec = 5000000;  
    	it.it_value.tv_sec = 0;  
    	it.it_value.tv_nsec =1;
	printf_with_time("start timer");
    	if (timer_settime(timerid, 0, &it, NULL) == -1)  
    	{  
       		printf("fail to timer_settime\n");  
        	exit(-1);  
    	}  
  
  	for(i = 0 ;i < 5000 ;i++)
	sleep(1); 
  
    	return 0;  






}
