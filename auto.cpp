#include <stdio.h>
#include <windows.h>

#include "queue.hpp"

#define             MAX_THREADS                     (45)
#define             DEFAULT_THREADS                 (8)
#define             FREE                            (0)
#define             IDLE                            (1)
#define             BUSY                            (2)
#define             MAX_NUMBER                      (100)
#define             MAX_OPERATIONS                  (100)

enum eChoiceNumbers {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
};

//This is the structure for every thread
struct tThreadData {
    volatile    int     uValue          = 0; //Used as argument for enqueue
    volatile    int     uChoice         = 0; //Choice for the operation
    volatile    int     uThreadStatus   = FREE; //0 for free, 1 for idle, 2 for busy
};

tThreadData         gThreadPoolArray[MAX_THREADS];  //For thread pool this is the array of objects of tThreadData
ThreadSafeQueue     gQueue;                         //Threadsafe queue for all the operations
HANDLE              gThreadHandle[MAX_THREADS];     //Handles for the threads
volatile int        gTotalThreadsCount;             //Count of total threads idle or busy
volatile int        gIdleThreadsCount;              //Counting idle threads in the 


//****************Thread operations below this*********************
//Declaration
DWORD WINAPI ThreadQueueControl (LPVOID pParams);

//Counting the Idle threads
int CountIdleThreads ()
{
        int counter = 0;

    for (int i = 0; i < MAX_THREADS; i++) {

        if (gThreadPoolArray[i].uThreadStatus == IDLE) {

            counter++;
        }
    }

    gIdleThreadsCount = counter;
    return gIdleThreadsCount;
}

//Counting total threads that are idle or busy
int CountTotalThreads ()
{
        int counter = 0;

    for (int i = 0; i < MAX_THREADS; i++) {

        if (gThreadPoolArray[i].uThreadStatus != FREE) {

            counter++;
        }
    }

    gTotalThreadsCount = counter;
    return gTotalThreadsCount;
}

//Finding the first available idle thread
int FindIdleThread ()
{
        int     i;

    for (i = 0; i < MAX_THREADS; i++) {

        if (gThreadPoolArray[i].uThreadStatus == IDLE) {

            break;
        }
    }
    return i;
}

//Start the thread pool
bool InitializePool ()
{
    for (int i = 0; i < DEFAULT_THREADS; i++) {

        gThreadPoolArray[i].uThreadStatus = IDLE;
        gThreadHandle[i] = CreateThread (NULL, 0, ThreadQueueControl, (void *) &gThreadPoolArray[i], 0, 0);

        if (!gThreadHandle[i]) {

            printf ("Thread Creation Failed\n");
            return false;
        }

        CountTotalThreads ();
        CountIdleThreads ();
    }
    return true;
}

//Add new thread to the pool
bool AddNewThread ()
{
    if (CountTotalThreads () == MAX_THREADS) {

        printf("Cannot create new thread limit reached\n");
        return false;
    }

    for (int i = 0; i < MAX_THREADS; i++) {

        if (gThreadPoolArray[i].uThreadStatus == FREE) {

            gThreadPoolArray[i].uThreadStatus = IDLE;
            gThreadHandle[i] = CreateThread (NULL, 0, ThreadQueueControl, (void *) &gThreadPoolArray[i], 0, 0);

            if (!gThreadHandle[i]) {

                printf("Error adding new thread\n");
                return false;
            }

            break;
        }
    }
    return true;
}

//Delete a idle thread from the pool
bool DeleteThread ()
{
        int     idle_thread_number;

    if (CountIdleThreads () == 0 || CountTotalThreads () == 1) {

        printf("Can not remove busy threads or last thread\n");
        return true;
    }

    idle_thread_number = FindIdleThread ();
    gThreadPoolArray[idle_thread_number].uThreadStatus = FREE;
    CloseHandle (gThreadHandle[idle_thread_number]);
    return true;
}

//Make all elements of pool array to zero and then close thread handles
bool DeletePool ()
{
    for (int i = 0; i < gTotalThreadsCount; i++) {

        gThreadPoolArray[i].uChoice = 0;
        gThreadPoolArray[i].uThreadStatus = FREE;
    }

    CloseHandle (gThreadHandle);
    printf("Deleted pool\n");
    return true;
}

//Control the queue operations throught this by taking thread parameters
DWORD WINAPI ThreadQueueControl (LPVOID pParams)
{
        tThreadData *   data                = (tThreadData *)pParams;
        bool            checker;
        bool            work_loop           = true;

    while (work_loop) {

        while (data->uChoice == ZERO);

        if (data->uChoice >= ONE && data->uChoice <= FIVE) {

            data->uThreadStatus = BUSY;
        }

        switch (data->uChoice) {

            //Enqueue to queue
            case ONE:

                gQueue.Enqueue (data->uValue);
                break;

                //Dequeue from queue
            case TWO:

                printf("Dequeue\n");
                gQueue.Dequeue ();
                break;

            //Display the queue
            case THREE:

                gQueue.Display ();
                break;

            //Empty the queue 
            case FOUR:

                gQueue.EmptyQueue ();
                break;

            //count nodes
            case FIVE:

                printf ("The number of nodes are %d\n", gQueue.CountNodes ());
                break;

            case SIX:
            case SEVEN:
            case EIGHT:
            case NINE:
                work_loop = false;
                return 0;
        }

        if (data->uChoice >= ONE && data->uChoice <= FIVE) {

            data->uThreadStatus = IDLE;
            data->uChoice = ZERO;
        }
    }

    return 0;
}

int main ()
{
        int         choice;
        int         i;
        int         number;
        int         input_number;
        int         counter         = 0;
        bool        work_loop       = true;
        bool        checker;

    checker = InitializePool();

    if (!checker) {

        printf("Pool Intialization failed");
        return 1;
    }

    while (work_loop) {

        if (!counter) {

            choice = ONE;
        }

        printf("%d\n", choice);
        switch (choice) {

            case ONE:

                number = FindIdleThread ();
                input_number = (rand () % MAX_NUMBER) + 1;
                printf("Enqueue %d\n", input_number);
                gThreadPoolArray[number].uValue = input_number;
                gThreadPoolArray[number].uChoice = choice;
                break;

            case TWO:

                number = FindIdleThread ();
                gThreadPoolArray[number].uChoice = choice;
                break;

            case THREE:

                number = FindIdleThread ();
                gThreadPoolArray[number].uChoice = choice;
                break;

            case FOUR:

                number = FindIdleThread ();
                gThreadPoolArray[number].uChoice = choice;
                break;

            case FIVE:

                number = FindIdleThread ();
                gThreadPoolArray[number].uChoice = choice;
                break;

            case SIX:

                checker = AddNewThread ();

                if (!checker) {

                    printf("Adding thread failed\n");
                } else {

                    printf ("Thread added: Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
                }

                gThreadPoolArray[number].uChoice = ZERO;
                break;

            case SEVEN:

                checker = DeleteThread ();

                if (!checker) {

                    printf("Deleting thread failed\n");
                    DeletePool ();
                }

                gThreadPoolArray[number].uChoice = ZERO;
                printf ("Deleted Thread: Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
                break;

            case EIGHT:

                gThreadPoolArray[number].uChoice = ZERO;
                printf ("Count Threads: Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
                break;

            case NINE:

                checker = DeletePool ();

                if (!checker) {

                    printf("Pool Deletion Failed\n");
                }

                gThreadPoolArray[number].uChoice = ZERO;
                work_loop = false;
                break;

            default:

                printf ("Something is wrong\n");
                return 1;
        }

        if (counter == MAX_OPERATIONS) {

            choice = NINE;
        } else {

            choice = (rand () % EIGHT) + 1;
            counter++;
        }
    }

    return 0;
}
