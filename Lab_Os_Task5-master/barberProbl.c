#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

void *createClient(void *_id); //Creation of a client
void *createBarberWomen(void * _id); //Creation of barber who take care of Women
void *createBarberMen(void * _id); //Creation of barber who take care of Men
void *createBarberBoth(void * _id); //Creation of barber who take care of Women AND Men
void randomTimeIni(); // Needed to use random values
char returnRandomGender(); //Creation of random who return a Men or a Women
void waitRandomTimeBetween0And(int _number); // Sleep random time between 0 and a number

#define N1 1 // Women
#define N2 1 // Men
#define N3 2 // Both
#define M 5 // Chairs
#define C 4 // Clients

bool clientAreAllDone = false; //When all the clients are done; swith to True

int tab_barberW[N1];
int tab_barberM[N2];
int tab_barberB[N3];

int tab_client[C];

//Creation of my semaphore
sem_t waiting_room;
sem_t barber_chair_W;
sem_t barber_chair_M;
sem_t barber_chair_B;
sem_t barber_bed_W;
sem_t barber_bed_M;
sem_t barber_bed_B;
sem_t waitbarber;

int main()
{
    randomTimeIni();

    pthread_t barberThreadW[N1]; //Starting thread of my barber Women
    pthread_t barberThreadM[N2]; //Starting thread of my barber Men
    pthread_t barberThreadB[N3]; //Starting thread of my barber Both
    pthread_t clientThread[C]; //Starting thread of my Clien

    for (int i = 0; i < N1; i++) // Barber for Women
    {
        tab_barberW[i] = i;
    }
    for (int i = 0; i < N2; i++) // Barber for Men
    {
        tab_barberM[i] = i;
    }
    for (int i = 0; i < N3; i++) // Barber for both Women and Men
    {
        tab_barberB[i] = i;
    }
    for (int i = 0; i < C; i++) // Client
    {
        tab_client[i] = i;
    }
    sem_init(&waiting_room, 0, M);
    sem_init(&barber_chair_W, 0, N1);
    sem_init(&barber_chair_M, 0, N2);
    sem_init(&barber_chair_B, 0, N3);
    sem_init(&barber_bed_W, 0, 0);
    sem_init(&barber_bed_M, 0, 0);
    sem_init(&barber_bed_B, 0, 0);
    sem_init(&waitbarber, 0, 0);

    //printf("Starting main program\n");
    //printf("variables:\n");
    //int* menNum = N2;
    //int* womenNum = N1;
    //int* bothNum = N3;
    //int* clientNum = C;
    //int* chairNum = M;
    //printf("Barber men:           Barber women:             Barber both: \n",menNum,womenNum,bothNum );
    //printf("Client:               Chair available:\n",clientNum,chairNum );

    // Creation of barber threads
    for (int i = 0; i < N1; i++) // Barber for Women
    {
        pthread_create(&barberThreadW[i], NULL, createBarberWomen, &tab_barberW[i]);
    }
    for (int i = 0; i < N2; i++) // Barber for Men
    {
        pthread_create(&barberThreadM[i], NULL, createBarberMen, &tab_barberM[i]);
    }
    for (int i = 0; i < N3; i++) // Barber for both Women and Men
    {
        pthread_create(&barberThreadB[i], NULL, createBarberBoth, &tab_barberB[i]);
    }
    sleep(1); //sleep for one second
    // Creation of clien thread
    for (int i = 0; i < C; i++)
    {
        pthread_create(&clientThread[i], NULL, createClient, &tab_client[i]);
    }

    for (int i = 0; i < C; i++)
    {
        pthread_join(clientThread[i], NULL);
    }

    clientAreAllDone = true;

    for (int i = 0; i < N1; i++)
    {
        sem_post(&barber_bed_W);
        pthread_join(barberThreadW[i], NULL);
    }
    for (int i = 0; i < N2; i++)
    {
        sem_post(&barber_bed_M);
        pthread_join(barberThreadM[i], NULL);
    }
    for (int i = 0; i < N3; i++)
    {
        sem_post(&barber_bed_B);
        pthread_join(barberThreadB[i], NULL);
    }
}

// Some functions
void *createClient(void *_id) //Creation of a client
{
    int id = *(int *)_id;
    char gender = returnRandomGender(); //Creation of a Men or a Women

    int place_available; //Places available in the waiting room
    int w; //Chairs available for women
    int m; //Chairs available for men
    int b; //Chairs available for both
    bool is_barber_versatile; //If a barber can do men and women or not

    waitRandomTimeBetween0And(30);
    printf("Client(%c) %d  arrive in the barber shop\n", gender, id);
    sem_getvalue(&waiting_room, &place_available);
    if (place_available == 0) //No free seats
    {
        printf("Client(%c) %d leave because there is no barber available.\n", gender, id);
    }
    else //Free seat so client take one
    {
        printf("Client(%c) %d  take place in the waiting room\n", gender, id);
        sem_wait(&waiting_room);
        if (gender == 'W') //Client is a Women
        {
            do
            {
                sem_getvalue(&barber_chair_W, &w);

                if (w > 0) //Is there a barber for women available
                {
                    sem_wait(&barber_chair_W);
                    is_barber_versatile = false;
                    sem_post(&waiting_room);
                    printf("Client(Women) %d  waking the barber up.\n", id);
                    sem_post(&barber_bed_W);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);

                    if (b > 0) //Is there a versatil barber
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;
                        sem_post(&waiting_room);
                        printf("Client(Women) %d  waking the barber up.\n", id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (w == 0 && b == 0);

        }

        if (gender == 'M')//Clien is a men
        {
            do
            {
                sem_getvalue(&barber_chair_M, &m);

                if (m > 0) //Is there a barber for men available
                {
                    sem_wait(&barber_chair_M);
                    is_barber_versatile = false;

                    sem_post(&waiting_room);

                    printf("Client(Men) %d  waking the barber up.\n", id);
                    sem_post(&barber_bed_M);
                }
                else
                {
                    sem_getvalue(&barber_chair_B, &b);

                    if (b > 0)//Is there a versatil barber
                    {
                        sem_wait(&barber_chair_B);
                        is_barber_versatile = true;
                        sem_post(&waiting_room);
                        printf("Client(Men) %d  waking the barber up.\n", id);
                        sem_post(&barber_bed_B);
                    }
                }

            } while (m == 0 && b == 0);
        }

        sem_wait(&waitbarber);

        if (is_barber_versatile) //The barber is versatile
        {
            sem_post(&barber_chair_B);
        }

        if (gender == 'W') //The barber is a barber for women
        {
            sem_post(&barber_chair_W);
        }

        if (gender == 'M') //The case is a barber for men
        {
            sem_post(&barber_chair_M);
        }

        printf("Client(%c) %d  leaving barber shop.\n", gender, id);
    }
}


void *createBarberWomen(void *_id) //Creation Barber for Women
{
    int id = *(int *)_id;

    while (!clientAreAllDone)
    {
        printf("The barber(Women) %d is spleeping.\n", id);
        sem_wait(&barber_bed_W);

        if (!clientAreAllDone)
        {
            printf("The barber(Women) %d is cutting hair\n", id);
            waitRandomTimeBetween0And(5);
            printf("The barber(Women) %d has finished cutting hair.\n", id);
            sem_post(&waitbarber);
        }
    }
    printf("The barber(Women) %d is done for today.\n", id);
    sem_post(&barber_bed_W);
}

void *createBarberMen(void *_id) //Creation barber for Men
{
    int id = *(int *)_id;

    while (!clientAreAllDone)
    {
        printf("The barber(Men) %d is spleeping.\n", id);
        sem_wait(&barber_bed_M);

        if (!clientAreAllDone)
        {
            printf("The barber(Men) %d is cutting hair\n", id);
            waitRandomTimeBetween0And(5);
            printf("The barber(Men) %d has finished cutting hair.\n", id);
            sem_post(&waitbarber);
        }
    }
    printf("The barber(Men) %d is done for today.\n", id);
    sem_post(&barber_bed_M);
}

void *createBarberBoth(void *_id) //Creation a borber for men and women
{
    int id = *(int *)_id;

   while (!clientAreAllDone)
    {
        printf("The barber(Both) %d is spleeping.\n", id);
        sem_wait(&barber_bed_B);

        if (!clientAreAllDone)
        {
            printf("The barber(Both) %d is cutting hair\n", id);
            waitRandomTimeBetween0And(5);
            printf("The barber(Both) %d has finished cutting hair.\n", id);
            sem_post(&waitbarber);
        }
    }
    printf("The barber(Both) %d is done for today.\n", id);
    sem_post(&barber_bed_B);
}

char returnRandomGender()
{
    int number;
    number = rand() % 2;
    if (number == 0)
    {
        return 'W';
    }
    else
    {
        return 'M';
    }
}
void randomTimeIni()
{
    system("clear");
    srand(time(NULL));
}
void waitRandomTimeBetween0And(int _number)
{
    int number = rand() % _number;
    sleep(number);
}
