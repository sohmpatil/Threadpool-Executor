#ifndef     WRITE_LOCK_HPP
#define     WRITE_LOCK_HPP

#include "lock_structure.hpp"

class WriteLock {

  public:
                    WriteLock        (tLock * pObject);
                    ~WriteLock       ();

  private:
        tLock *     vWriteObject;
};

#endif      //WRITE_LOCK_HPP
