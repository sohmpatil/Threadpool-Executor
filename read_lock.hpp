#ifndef     READ_LOCK_HPP
#define     READ_LOCK_HPP

#include "lock_structure.hpp"

class ReadLock {

  public:
                    ReadLock        (tLock * pObject);
                    ~ReadLock       ();

  private:
        tLock *     vReadObject;
};

#endif      //LOCK_HPP
