#include <iostream>

#include "semaphore_class.h"

SemaphoreClass::SemaphoreClass():
        semaphore(){
    std::clog<<"[SemaphoreClass] Loadding"<<std::endl;
}

void SemaphoreClass::wait(){
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    this->semaphore.wait(uniqueLock);
}

void SemaphoreClass::notify_all(){
    this->semaphore.notify_all();
}
