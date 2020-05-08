#include <stdio.h> /*use for printf()*/
#include <signal.h> /*use for signal()*/
#include <stdlib.h> /*use for const EXIT SUCCESS AND FAILURE*/
#include <unistd.h> /*use for fork() and sleep()*/
#include <sys/types.h> /*use for type pid_t*/
#include <sys/wait.h> /*use for wait()*/

/*Part 1 : Creation of the Handler */

/*Handler for signal() in child process*/
void c_handler(int num)
{
    printf("Child [%d]: received SIGTERM signal. Terminate \n", getpid());
    kill(getpid(), num); // this will kill the child process
    exit(EXIT_SUCCESS);
}

/*Handler for signal(SIGNINT) in parent process*/
void i_handler(int num)
{
    printf("\nYou can't kill this process with CTRL+C.\n");
}

/*Handler for signal() in parent process*/
void p_handler(int num)
{
    printf("Parent [%d]: received signal n°%d. Ignored.\n", getpid(), num); //Ignore all signals
}

/*Part 2 : Main*/

int main()
{
    /* Ask user the number of child process he want*/
    int NUM_CHILD;/*Number ask by user*/
    printf("How many child do you want to creat ?\n");
    scanf("%d", &NUM_CHILD);
    int j = 0; /* The variable who count the number of child process created*/

  /*Start of the creation process*/
	for (int i=1 ; i<=NUM_CHILD ; i++)
	{
		pid_t pid = fork(); /* Creation of one child process*/

		if (pid >= 0) /*Mean that fork() is a success*/
		{
            	j++; /*count + 1*/
			if (pid == 0) /*Inside the child process*/
			{

        signal(SIGINT, SIG_IGN); /* this Ingore the interupt signal*/
        signal(SIGTERM, c_handler); /* this Redirect the termination to the child handler*/
				printf("Child ( PPID: %d PID: %d", getppid(), getpid(), ")\n"); /* Print all intel about this child process*/
				sleep(1); /*Wait one second before terminate the process*/
				printf("Child process is done\n");
				exit(EXIT_SUCCESS); /* Exit the actual child process*/
			}
			else /* Enterring parent process*/
			{
        for (int k = 1 ; k <= 64 ; k++)
        {
            signal(k, p_handler); /*... redirecting signal in the parent handler*/
        }
        signal(SIGINT, i_handler); /*special handler for the interupt signal*/
        signal(SIGCHLD, SIG_DFL); /*reset by default the handler signal*/
				printf("Fork(%d) ( Parent => PID: %d", i, getpid(), ")\n");/* Print all intel about this parent process*/
				printf("Waiting for child process to finish.\n");
				wait(NULL); /*Wait the end of child process*/
			}
		}
		else /*If error in the fork ; then child not created*/
		{
      perror("Error when calling the fork() function");
      exit(1); /*Exit with code 1*/
}

sleep(1); /*Wait 1 second between the each creation of children*/
}

printf("\nYou have created %d child process.\n", j);

/*Then put all the signals back to default*/
for (int k = 1 ; k <= 64 ; k++)
{
  signal(k, SIG_DFL);
}

return EXIT_SUCCESS;
return 0;
}
