#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define N 5 //Define the number of philosophers
#define M 5 //Define the number of meals
#define LEFT (id + N - 1) % N //Define the id of the philosopher on the left of the current philosopher
#define RIGHT (id + 1) % N //Define the id of the philosopher on the right of the current philosopher

pthread_mutex_t mutex; // declaration of the mutex
pthread_cond_t cond[N]; // declaration of a condition
pthread_t philosopherThread[N]; // declaration for philosopher threads
bool flag_line_jump = false;
int tab_philosopher[N]; //The array to store philosopher's ids
int MEALS = M;
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
   int meals_eaten = 0;
   sleep(1);
   while (MEALS > 0)
   {
      printf("Philosopher[%d] is THINKING\n", id);
      waitRandomTimeBetween0And(5);
      pthread_mutex_lock(&mutex); // lock the mutex for the current philosopher
      grab_forks(id);
      MEALS--;
      if(MEALS >= 0)
          meals_eaten++;
          put_away_forks(id);
          pthread_mutex_unlock(&mutex); // unlock the mutex for thr current philosopher
   }
   sleep(5);
   if(!flag_line_jump) // if loop just to create a line jump before the recap is displayed
      flag_line_jump = true;
      printf("--------------------------------------\n");
      printf("|The Philosopher[%d] had eaten %d meals|\n", id, meals_eaten);
      printf("--------------------------------------\n");
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
   && MEALS > 0)
   {
      printf("Philosopher[%d] is EATING\n", id);
      tab_state[id] = EATING;
      printf("---> %d meals left on %d\n", MEALS-1, M);
      waitRandomTimeBetween0And(3);
   }
 }

void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}

void waitRandomTimeBetween0And(int _number) //Create a waiting time between 0 and a given number
{
    int number = rand() % _number;
    sleep(number);
}
