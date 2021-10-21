#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "queue.hpp"

ThreadSafeQueue :: ThreadSafeQueue ()
{
    vLockObj.uReaderCounter = 0;
    vLockObj.uMutex = CreateMutex (NULL, FALSE, NULL);
    vLockObj.uSemaphore = CreateSemaphore (NULL, 1, 1, NULL);
}

//Add element to the queue at end
void ThreadSafeQueue :: Enqueue (int pNum)
{
    WriteLock wl (&vLockObj);

    //if the queue is null else add at the end
    if (vRear == NULL) {

        vRear = (tQueue *) malloc (sizeof(tQueue));
        vRear->uNext = NULL;
        vRear->uData = pNum;
        vFront =  vRear;
    } else {

        vTemp = (tQueue *) malloc (sizeof(tQueue));
        vRear->uNext = vTemp;
        vTemp->uData = pNum;
        vTemp->uNext = NULL;
        vRear = vTemp;
    }
    vSize++;
}

//delete at the end
void ThreadSafeQueue :: Dequeue ()
{
    WriteLock wl (&vLockObj);

    if (vSize == 0) {

        printf ("Can not delete from empty queue\n");
        return;
    }

    vTemp = vFront;

    //if the last element else move the front ahead
    if (vTemp->uNext == NULL) {

        free (vFront);
        vFront = NULL;
        vRear = NULL;
    } else {

        vTemp = vTemp->uNext;
        free (vFront);
        vFront = vTemp;
    }

    vSize--;
}

//print the queue
void ThreadSafeQueue :: Display ()
{
    ReadLock rl (&vLockObj);

    if (vSize == 0) {

        printf("Empty queue\n");
        return;
    }

    vTemp = vFront;
    printf("The queue is - ");

    while (vTemp != NULL) {

        printf ("%d ", vTemp->uData);
        vTemp = vTemp->uNext;
    }

    printf("\n");
}

//set front and rear pointers to null
void ThreadSafeQueue :: EmptyQueue ()
{
    WriteLock wl (&vLockObj);
    vFront = NULL;
    vRear = NULL;
    vSize = 0;
    printf("Queue made empty\n");
}

//count the number of nodes and return vSize
int ThreadSafeQueue :: CountNodes ()
{
    ReadLock rl (&vLockObj);
    return vSize;
}
