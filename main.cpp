#include <stdio.h>
#include <windows.h>

#include "queue.hpp"

#define             MAX_THREADS                     (45)
#define             DEFAULT_THREADS                 (8)
#define             FREE                            (0)
#define             IDLE                            (1)
#define             BUSY                            (2)

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
    int             uValue                          = 0; //Used as argument for enqueue
    int             uChoice                         = 0; //Choice for the operation
    int             uThreadStatus                   = FREE; //0 for free, 1 for idle, 2 for busy
};

tThreadData         gThreadPoolArray[MAX_THREADS];   //For thread pool this is the array of objects of tThreadData
ThreadSafeQueue     gQueue;
HANDLE              gThreadHandle[MAX_THREADS];      //Handles for the threads
int                 gTotalThreadsCount;             //Count of total threads idle or busy
int                 gIdleThreadsCount;              //Counting idle threads in the 


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

//Print the menu to let the user know
void PrintMenu ()
{
    printf("\nEnter the choice\n"
           " 1. Enqueue\n 2. Dequeue\n 3. Display\n 4. Empty the queue\n"
           " 5. Count number of nodes in queue\n 6. Add a thread\n"
           " 7. Remove a thread\n 8. Count of free threads\n 9. Exit\n");
}

//Control the queue operations throught this by taking thread parameters
DWORD WINAPI ThreadQueueControl (LPVOID pParams)
{
        tThreadData *   data       = (tThreadData *)pParams;
        bool            checker;

    while (1) {

        while (data->uChoice == ZERO);

        switch (data->uChoice) {

            case ONE:

                data->uThreadStatus = BUSY;
                gQueue.Enqueue (data->uValue);
                data->uChoice = ZERO;
                break;

            case TWO:

                data->uThreadStatus = BUSY;
                gQueue.Dequeue ();
                data->uChoice = ZERO;
                break;

            case THREE:

                data->uThreadStatus = BUSY;
                gQueue.Display ();
                data->uChoice = ZERO;
                break;

            case FOUR:

                data->uThreadStatus = BUSY;
                gQueue.EmptyQueue ();
                data->uChoice = ZERO;
                break;

            case FIVE:

                printf ("The number of nodes are %d", gQueue.CountNodes ());
                data->uChoice = ZERO;
                break;

            default:

                break;
        }

        PrintMenu ();
        data->uThreadStatus = IDLE;
    }
    return 0;
}

int main ()
{
        int         choice;
        int         i;
        int         number;
        int         input_number;
        bool        checker;

    checker = InitializePool();

    if (!checker) {

        printf("Pool Intialization failed");
        return 1;
    }

    PrintMenu ();
    while (1) {

        while (!(scanf ("%ld", &choice) && choice > ZERO && choice < TEN)) {

            printf ("Invalid input - should be between 1 to 9\n");
            getchar ();
        }

        switch (choice) {
        case ONE:

            printf ("Enter the element to enqueue - ");
            number = FindIdleThread ();
            scanf ("%d", &input_number);
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

                printf ("Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
            }

            gThreadPoolArray[number].uChoice = ZERO;
            PrintMenu ();
            break;
        case SEVEN:

            checker = DeleteThread ();

            if (!checker) {

                printf("Deleting thread failed\n");
                DeletePool ();
            }

            gThreadPoolArray[number].uChoice = ZERO;
            printf ("Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
            PrintMenu ();
            break;
        case EIGHT:

            gThreadPoolArray[number].uChoice = ZERO;
            printf ("Total threads: %d, Total Idle threads: %d\n", CountTotalThreads (), CountIdleThreads ());
            PrintMenu ();
            break;
        case NINE:

            checker = DeletePool ();

            if (!checker) {

                printf("Pool Deletion Failed\n");
            }

            gThreadPoolArray[number].uChoice = ZERO;
            return 0;
        }
    }

    return 0;
}
