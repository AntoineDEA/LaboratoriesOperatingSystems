#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define N 5 //Define the number of philosophers
#define LEFT (id+N-1) % N //Define the id of the philosopher on the left of the current philosopher
#define RIGHT (id+1) % N //Define the id of the philosopher on the right of the current philosopher

pthread_mutex_t mutex; // declaration of the mutex
pthread_cond_t cond[N]; // declaration of a condition
pthread_t philosopherThread[N]; // declaration for philosopher threads
bool flag_line_jump = false;
int philosophers[N]; //The array to store philosopher's ids
enum {THINKING, HUNGRY, EATING} tab_state[N]; //The array to store the three different philosopher's state

// functions declarations
void grab_forks(int _id);
void put_away_forks(int _id);
void state(int _id);
void *createPhilosopher(void *_id);

//---------Main Program-----------
int main()
{
   //Starting main program
   pthread_mutex_init(&mutex,NULL); // initialize mutex
   for (int i = 0; i < N; i++) // initialize condition thread and philosophers id for each philosophers
   {
      pthread_cond_init(&cond[i],NULL);
      philosophers[i] =i;
   }
   for (int i = 0; i < N; i++) // create all philosopher threads for each philosophers
   {
      pthread_create(&philosopherThread[i],NULL,createPhilosopher,&philosophers[i]);
      printf("Philosopher[%d] is seating around the table...\n", i);
   }
   printf("----------------------------------------------\n");
   for (int i =0;i < N;i++) // launching all philosopher threads
   {
      pthread_join(philosopherThread[i],NULL);
   }
   pthread_mutex_destroy(&mutex); // cleaning mutex
   for (int i =0;i < N;i++) // cleaning condition threads
   {
      pthread_cond_destroy(&cond[i]);
   }
   return 0;
}

//---------Creation of a Philosopher-----------
void *createPhilosopher(void *_id)
{
   int id =*(int*)_id;
   sleep(1);
   int loop =1;
   while (loop > 0)
   {
      printf("Philosopher[%d] is THINKING\n", id);
      sleep(2);
      pthread_mutex_lock(&mutex); // lock the mutex on the current philosopher
      grab_forks(id);
      if(loop > 0)
          put_away_forks(id);
          pthread_mutex_unlock(&mutex); // unlock the mutex for the current philosopher
   }
   sleep(5);
}

void grab_forks(int idPhilo)
{
   int id =idPhilo;
   printf("Philosopher[%d] is HUNGRY\n", id);
   tab_state[id] =HUNGRY;
   state(id);
}

void put_away_forks(int idPhilo)
{
   int id = idPhilo;
   tab_state[id] =THINKING;
   state(RIGHT);
   state(LEFT);
}

void state(int idPhilo)
{
   int id = idPhilo;
   if(tab_state[id] ==HUNGRY
   && tab_state[RIGHT] !=EATING
   && tab_state[LEFT] !=EATING)
   {
      printf("Philosopher[%d] is EATING\n", id);
      tab_state[id] =EATING;
      sleep(2);
   }
 }
