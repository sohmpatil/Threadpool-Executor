#include <stdio.h>
#include <windows.h>

#include "write_lock.hpp"

WriteLock::WriteLock (tLock * pObject)
{
    vWriteObject = pObject;

    if (WaitForSingleObject(pObject->uSemaphore, INFINITE) != WAIT_OBJECT_0) {

        printf("Waiting for semaphore signal failed\n");
        return;
    }
}

WriteLock::~WriteLock ()
{
    if (!ReleaseSemaphore (vWriteObject->uSemaphore, 1, 0)) {

        printf ("Release semaphore failed");
        return;
    }
}
