#ifndef     LOCK_STRUCTURE_HPP
#define     LOCK_STRUCTURE_HPP

struct tLock {
    HANDLE          uMutex;
    HANDLE          uSemaphore;
    int             uReaderCounter;
};

#endif      //LOCK_STRUCTURE_HPP
