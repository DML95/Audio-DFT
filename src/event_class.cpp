#include <iostream>

#include "window_class.h"

#include "event_class.h"

void EventClass::mainThread(EventClass *eventClass){
    std::clog<<"[EventClass] Thread init"<<std::endl;
    XEvent event;
    eventClass->load.exchange(false);
    eventClass->waitClass.notify_all();
    while(eventClass->run){
        eventClass->displayClass->next(event);
        switch(event.type){
            case ClientMessage:
                if((Atom)event.xclient.data.l[0]==eventClass->wmDeleteWindows){
                    eventClass->run.exchange(false);
                    eventClass->waitClass.notify_all();
                }
                break;
            case ConfigureNotify:
                eventClass->width.exchange(event.xconfigure.width);
                eventClass->height.exchange(event.xconfigure.height);
                eventClass->events.fetch_or(EventClass::RESIZE);
                eventClass->waitClass.notify_all();
                break;
            case Expose:
                eventClass->events.fetch_or(EventClass::PAINT);
                eventClass->waitClass.notify_all();
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
    while(this->load)this->waitClass.wait();
}

EventClass::~EventClass(){
    std::clog<<"[EventClass] Unloadding"<<std::endl;
    this->thread->join();
}

bool EventClass::next(EVENTS &eventsStruct){
    bool run;
    do{
        eventsStruct.events=this->events.exchange(0);
        if(!eventsStruct.events)this->waitClass.wait();
        run=this->run;
        if(eventsStruct.events&EventClass::RESIZE){
            eventsStruct.height=this->height;
            eventsStruct.width=this->width;
        }
        if(eventsStruct.events&EventClass::MAX)eventsStruct.max=this->max;
    }while(run&&(!eventsStruct.events));
    return run;
}

void EventClass::addEvents(unsigned int eventsMask){
    this->events.fetch_or(eventsMask);
    this->waitClass.notify_all();
}

void EventClass::swapAndPaint(float max){
    this->max.exchange(max);
    this->events.fetch_or(EventClass::SWAP|EventClass::PAINT|EventClass::MAX);
    this->waitClass.notify_all();
}
