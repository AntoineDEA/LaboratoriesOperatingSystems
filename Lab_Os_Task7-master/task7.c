#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define N 5 //Define the number of philosophers
#define FU 5 //Define the number of food units
#define LEFT (id + N - 1) % N //Define the id of the philosopher on the left of the current philosopher
#define RIGHT (id + 1) % N //Define the id of the philosopher on the right of the current philosopher

pthread_mutex_t mutex; // declaration of the mutex
pthread_cond_t cond[N]; // declaration of a condition
pthread_t philosopherThread[N]; // declaration for philosopher threads
bool flag_line_jump = false;
int tab_philosopher[N]; //The array to store philosopher's ids
int FOOD_UNITS = FU + 1;
enum {THINKING, HUNGRY, EATING} tab_state[N]; //The array to store the three different philosopher's state

// functions declarations
void grab_forks(int _id);
void put_away_forks(int _id);
void state(int _id);
void randomTimeIni();
void waitRandomTimeBetween0And(int _number);
void *createPhilosopher(void *_id);

//---------Main Program-----------
int main()
{
   //Starting main program
   randomTimeIni(); // initialize random function
   pthread_mutex_init(&mutex, NULL); // initialize the mutex
   for (int i = 0; i < N; i++) // initialize condition thread and philosophers id for each philosophers
   {
      pthread_cond_init(&cond[i], NULL);
      tab_philosopher[i] = i;
   }
   for (int i = 0; i < N; i++) // create all the philosopher threads for each philosophers
   {
      pthread_create(&philosopherThread[i], NULL, createPhilosopher, &tab_philosopher[i]);
      printf("Philosopher[%d] is taking place...\n", i);
   }
   printf("------------------------------------\n");
   for (int i = 0; i < N; i++) // launch all the philosopher threads
   {
      pthread_join(philosopherThread[i], NULL);
   }
   pthread_mutex_destroy(&mutex); // clean mutex
   for (int i = 0; i < N; i++) // clean condition threads
   {
      pthread_cond_destroy(&cond[i]);
   }
   return 0;
}

//---------Creation of a Philosopher-----------
void *createPhilosopher(void *_id)
{
   int id = *(int *)_id;
   int units_eaten = 0;
   sleep(1);
   while (FOOD_UNITS > 0)
   {
      printf("Philosopher[%d] is THINKING\n", id);
      waitRandomTimeBetween0And(5);
      pthread_mutex_lock(&mutex); // lock the mutex for the current philosopher
      grab_forks(id);
      FOOD_UNITS--;
      if(FOOD_UNITS > 0)
          units_eaten++;
          put_away_forks(id);
          pthread_mutex_unlock(&mutex); // unlock the mutex for the current philosopher
   }
   sleep(5);
   if(!flag_line_jump) // if loop just to create a line jump before the recap is displayed
      flag_line_jump = true;
      printf("-------------------------------------------\n");
      printf("|The Philosopher[%d] had eaten %d food units|\n", id, units_eaten);
      printf("-------------------------------------------\n");
}

void grab_forks(int _id)
{
   int id = _id;
   printf("Philosopher[%d] is HUNGRY\n", id);
   tab_state[id] = HUNGRY;
   state(id);
}

void put_away_forks(int _id)
{
   int id = _id;
   tab_state[id] = THINKING;
   state(LEFT);
   state(RIGHT);
}

void state(int _id)
{
   int id = _id;
   if(tab_state[id] == HUNGRY
   && tab_state[LEFT] != EATING
   && tab_state[RIGHT] != EATING
   && FOOD_UNITS > 0)
   {
      printf("Philosopher[%d] is EATING\n", id);
      tab_state[id] = EATING;
      printf("---> %d units of food left on %d\n", FOOD_UNITS, FU);
      waitRandomTimeBetween0And(3);
   }
 }

void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

void waitRandomTimeBetween0And(int _number) //Create a waiting time between 0 and and a number given
{
    int number = rand() % _number;
    sleep(number);
}
