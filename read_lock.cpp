#include <stdio.h>
#include <windows.h>

#include "read_lock.hpp"

ReadLock::ReadLock (tLock * pObject)
{
    vReadObject = pObject;

    if (WaitForSingleObject (pObject->uMutex, INFINITE) != WAIT_OBJECT_0) {

        printf("Waiting for mutex signal failed\n");
        return;
    }

    (pObject->uReaderCounter)++;

    if (pObject->uReaderCounter == 1 && (WaitForSingleObject (pObject->uSemaphore, INFINITE) != WAIT_OBJECT_0)) {

            printf("Waiting for semaphore signal failed\n");
            return;
    }

    if (!ReleaseMutex (pObject->uMutex)) {

        printf ("Release mutex failed");
        return;
    }

}

ReadLock::~ReadLock ()
{
    if (WaitForSingleObject (vReadObject->uMutex, INFINITE) != WAIT_OBJECT_0) {

        printf("Waiting for mutex signal failed\n");
        return;
    }

    (vReadObject->uReaderCounter)--;

    if (vReadObject->uReaderCounter == 0 && (!ReleaseSemaphore (vReadObject->uSemaphore, 1, 0))) {

            printf ("Release semaphore failed");
            return;
    }

    if (!ReleaseMutex (vReadObject->uMutex)) {

        printf ("Release mutex failed");
        return;
    }

}
