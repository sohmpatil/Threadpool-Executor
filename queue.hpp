#ifndef     QUEUE_HPP
#define     QUEUE_HPP

#include "read_lock.hpp"
#include "write_lock.hpp"

//Queue structure
struct tQueue {
    tQueue *        uNext;
    int             uData;
};

class ThreadSafeQueue {
  public:
                    ThreadSafeQueue     ();
        void        Enqueue             (int pNum);
        void        Dequeue             ();
        void        Display             ();
        void        EmptyQueue          ();
        int         CountNodes          ();

  private:
        tLock       vLockObj;
        tQueue *    vFront      = NULL;
        tQueue *    vRear       = NULL;
        tQueue *    vTemp       = NULL;
        int         vSize       = 0;
};

#endif      //QUEUE.HPP
