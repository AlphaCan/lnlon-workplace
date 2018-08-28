/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 - 2011
 */
//MODULE_LICENSE("GPL");

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include "ethercat.h"
#include <time.h>  
#include <stdlib.h>  
#include <signal.h>  
#include <pthread.h>
#include <unistd.h> 
#include <errno.h>

#define EC_TIMEOUTMON 500

uint8_t IOmap[4096];
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;
timer_t timerid;
#define cycle_time 4000000		//ns
struct itimerspec it;


void period_process(void)
{
//    IOmap[0]++;

    ec_send_processdata();
    wkc = ec_receive_processdata(EC_TIMEOUTRET);
//    printf("1\n");
}

void posix_timer_signal(int sig)
{
	if (SIGUSR1 == sig)
	{
		period_process();
	}
	else
	{
		printf("detele timer\n");
		timer_delete(timerid);
		exit(EXIT_SUCCESS);
	}
}

int SetUp(uint16 slave)
{
    int retval;
    uint8 u8val;
    uint16 u16val;   
    uint32 u32val;
	uint16 SM2_PDO_OUT   = 0x1700;
	uint16 SM3_PDO_INPUT = 0x1B00;
    retval = 0;

    u8val = 0;
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u32val = 0x60400010;//keywords
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u32val = 0x607A0020;//target position
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u32val = 0x60ff0020;//target velocity
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u32val = 0x60710010;//target Torque
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u8val = 4;//
    retval += ec_SDOwrite(slave, SM2_PDO_OUT, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 0;
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u16val = SM2_PDO_OUT;//
    retval += ec_SDOwrite(slave, 0x1c12, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);
    u8val = 1;
    retval += ec_SDOwrite(slave, 0x1c12, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 0;
    retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u32val = 0x60410010;//statu words
    retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x01, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u32val = 0x60640020;//actual position
    retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u32val = 0x606C0020;//actual velocity
    retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x03, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
	u32val = 0x60770010;//actual torque
	retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x04, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
    u8val = 4;
    retval += ec_SDOwrite(slave, SM3_PDO_INPUT, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 0;
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);
    u16val = SM3_PDO_INPUT;//
    retval += ec_SDOwrite(slave, 0x1c13, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);
    u8val = 1;
    retval += ec_SDOwrite(slave, 0x1c13, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u8val = 1;//operation model
    retval += ec_SDOwrite(slave, 0x6060, 0x00, FALSE, sizeof(u8val), &u8val, EC_TIMEOUTRXM);

    u16val = 0x00;//slave-master communication model
    retval += ec_SDOwrite(slave, 0x1c32, 0x01, FALSE, sizeof(u16val), &u16val, EC_TIMEOUTRXM);
	u32val = cycle_time;//slave-master communication cycle
    retval += ec_SDOwrite(slave, 0x1c32, 0x02, FALSE, sizeof(u32val), &u32val, EC_TIMEOUTRXM);
 
    while(EcatError) printf("%s", ec_elist2string());

    printf("slave %d set, retval = %d\n", slave, retval);
    return 1;


}




void simpletest(char *ifname)
{
    int i, j, oloop, iloop, chk,slc;
    
    needlf = FALSE;
    inOP = FALSE;
    
	printf("Starting simple test\n");

   /* initialise SOEM, bind socket to ifname */
   if (ec_init(ifname))
   {
      printf("ec_init on %s succeeded.\n",ifname);
      /* find and auto-config slaves */


       if ( ec_config_init(FALSE) > 0 )
		{
        	printf("%d slaves found and configured.\n",ec_slavecount);
	 		if(ec_slavecount >= 1)
	 		{
	     		for(slc = 1;slc <= ec_slavecount; slc++)
	     		{
					printf("Found %s at position %d\n",ec_slave[slc].name,slc);
					ec_slave[slc].PO2SOconfig = &SetUp;
             	}
	 		}

         	ec_config_map(&IOmap);

         	ec_configdc();

         	printf("Slaves mapped, state to SAFE_OP.\n");
         	/* wait for all slaves to reach SAFE_OP state */
         	ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

         	oloop = ec_slave[0].Obytes;
         	if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
         	if (oloop > 8) oloop = 8;
         	iloop = ec_slave[0].Ibytes;
         	if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
         	if (iloop > 8) iloop = 8;

         	printf("Request operational state for all slaves\n");
         	expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
         	printf("Calculated workcounter %d\n", expectedWKC);
         	ec_slave[0].state = EC_STATE_OPERATIONAL;
         	/* send one valid process data to make outputs in slaves happy*/
         	ec_send_processdata();
         	ec_receive_processdata(EC_TIMEOUTRET);

	        if (timer_settime(timerid, 0, &it, NULL) == -1)  
    		{  
       			printf("fail to timer_settime\n");  
        		exit(-1);  
    		}  
    	
         	/* request OP state for all slaves */
         	ec_writestate(0);
         	chk = 40;
         	/* wait for all slaves to reach OP state */
         	do
         	{
            	ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
         	}
         	while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
         	if (ec_slave[0].state == EC_STATE_OPERATIONAL )
         	{
            	printf("Operational state reached for all slaves.\n");
            	inOP = TRUE;
				IOmap[0] = 6;
				osal_usleep(8000);
				IOmap[0] = 7;
				osal_usleep(8000);
				IOmap[0] = 15;
				osal_usleep(8000);
//              inOP = TRUE;
                /* cyclic loop */
                for(i = 1; i <= 5000; i++)
            	{
                    if(wkc >= expectedWKC)
                    {
                        printf("Processdata cycle %4d, WKC %d , O:", i, wkc);

                        for(j = 0 ; j < oloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].outputs + j));
                        }

                        printf(" I:");
                        for(j = 0 ; j < iloop; j++)
                        {
                            printf(" %2.2x", *(ec_slave[0].inputs + j));
                        }
                        printf(" T:%"PRId64"\r",ec_DCtime);
                        needlf = TRUE;
                    }
                    osal_usleep(5000);
                }
                inOP = FALSE;
            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }
            
            printf("detele timer \n");
            timer_delete(timerid);
            exit(EXIT_SUCCESS);
            printf("\nRequest init state for all slaves\n");
            ec_slave[0].state = EC_STATE_INIT;
            /* request INIT state for all slaves */
            ec_writestate(0);
        }
        else
        {
            printf("No slaves found!\n");
        }
        printf("End simple test, close socket\n");
        /* stop SOEM, close socket */
        ec_close();
    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n",ifname);
    }
}


OSAL_THREAD_FUNC ecatcheck( void *ptr )
{
    int slave;
    (void)ptr;                  /* Not used */

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
               needlf = FALSE;
               printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state > EC_STATE_NONE)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);
                     }
                  }
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (ec_slave[slave].state == EC_STATE_NONE)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);
                     }
                  }
               }
               if (ec_slave[slave].islost)
               {
                  if(ec_slave[slave].state == EC_STATE_NONE)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        osal_usleep(10000);
    }
}

int main(int argc, char *argv[])
{
    struct sigevent evp;

    signal(SIGUSR1, posix_timer_signal);    /* for timer */
    signal(SIGINT, posix_timer_signal);    /* Ctrl + C */
    memset(&evp, 0, sizeof(struct sigevent));  
    evp.sigev_signo = SIGUSR1;  
    evp.sigev_notify = SIGEV_SIGNAL;
    if (timer_create(CLOCK_MONOTONIC, &evp, &timerid) == -1)  
    {  
       printf("fail to timer_create\n");  
       exit(-1);  
    }  
  	  
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_nsec = cycle_time;  
    it.it_value.tv_sec = 0;  
    it.it_value.tv_nsec =1;
    
	printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");
   	if (argc > 1)   
   	{
      	/* create thread to handle slave error handling in OP */
    	osal_thread_create_rt(&thread1, 128000, &ecatcheck, (void*) &ctime);
      /* start cyclic part */
      	simpletest(argv[1]);
   	}
   	else
   	{
      	printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
   	}
   
   printf("End program\n");
    return (0);
}
