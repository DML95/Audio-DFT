#include <iostream>

#include "window_class.h"

#include "event_class.h"

void EventClass::mainThread(EventClass *eventClass){
    std::clog<<"[EventClass] Thread init"<<std::endl;
    XEvent event;
    eventClass->load.exchange(false);
    eventClass->semaphore.notify_all();
    while(eventClass->run){
        eventClass->displayClass->next(event);
        switch(event.type){
            case ClientMessage:
                if((Atom)event.xclient.data.l[0]==eventClass->wmDeleteWindows){
                    eventClass->mutex.lock();
                    eventClass->run.exchange(false);
                    eventClass->semaphore.notify_all();
                    eventClass->mutex.unlock();
                }
                break;
            case ConfigureNotify:
                eventClass->mutex.lock();
                eventClass->width.exchange(event.xconfigure.width);
                eventClass->height.exchange(event.xconfigure.height);
                eventClass->events.fetch_or(EventClass::eventMack::resize);
                eventClass->semaphore.notify_all();
                eventClass->mutex.unlock();
                break;
            case Expose:
                eventClass->mutex.lock();
                eventClass->events.fetch_or(EventClass::eventMack::paint);
                eventClass->semaphore.notify_all();
                eventClass->mutex.unlock();
                break;
        }
    }
    std::clog<<"[EventClass] Thread end"<<std::endl;
}

EventClass::EventClass(const std::shared_ptr<WindowClass> &windowClass):
        windowClass(windowClass),
        displayClass(windowClass->getDisplay()){
    std::clog<<"[EventClass] Loadding"<<std::endl;
    this->wmDeleteWindows=this->windowClass->addWMProtocol("WM_DELETE_WINDOW");
    this->windowClass->setEventMask(StructureNotifyMask|ExposureMask);
    this->windowClass->map();
    this->run=true;
    this->events=0;
    this->load=true;
    this->thread=std::make_unique<std::thread>(EventClass::mainThread,this);
    while(this->load)this->semaphore.wait();
}

EventClass::~EventClass(){
    std::clog<<"[EventClass] Unloadding"<<std::endl;
    this->thread->join();
}

bool EventClass::next(EventClass::eventsStruct &eventsStruct){
    bool run;
    do{
        this->semaphore.wait();
        this->mutex.lock();
        eventsStruct.events=this->events.exchange(0);
        run=this->run;
        if(eventsStruct.events&EventClass::eventMack::resize){
            eventsStruct.height=this->height;
            eventsStruct.width=this->width;
        }
        if(eventsStruct.events&EventClass::eventMack::max){
            eventsStruct.maxValue=this->maxValue;
        }
        this->mutex.unlock();
    }while(run&&(!eventsStruct.events));
    return run;
}

void EventClass::addEvents(EventClass::eventMack eventsMask){
    this->mutex.lock();
    this->events.fetch_or(eventsMask);
    this->semaphore.notify_all();
    this->mutex.unlock();
}

void EventClass::swapAndPaint(float max){
    this->mutex.lock();
    this->maxValue.exchange(max);
    this->events.fetch_or(EventClass::eventMack::swap|EventClass::eventMack::paint|EventClass::eventMack::max);
    this->semaphore.notify_all();
    this->mutex.unlock();
}
