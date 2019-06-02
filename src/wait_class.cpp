#include <iostream>

#include "wait_class.h"

WaitClass::WaitClass(){
    std::clog<<"[WaitClass] Loadding"<<std::endl;
    this->event=std::make_unique<std::condition_variable>();
}

void WaitClass::wait(){
    std::unique_lock<std::mutex> uniqueLock(this->mutex);
    this->event->wait(uniqueLock);
}

void WaitClass::notify_all(){
    this->event->notify_all();
}
