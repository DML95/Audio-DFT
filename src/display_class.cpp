#include <stdexcept>
#include <iostream>

#include "window_class.h"

DisplayClass::DisplayClass(){
    std::clog<<"[DisplayClass] Loadding"<<std::endl;
    XInitThreads();
    this->display=XOpenDisplay(0);
    if(!this->display)throw std::runtime_error("[XOpenDisplay]");
    this->atoms=std::make_unique<std::vector<Atom> >();
}

DisplayClass::~DisplayClass(){
    std::clog<<"[DisplayClass] Unloadding"<<std::endl;
    XCloseDisplay(this->display);
}

void DisplayClass::next(XEvent &event){
    XNextEvent(this->display,&event);
}

Display* DisplayClass::get(){
    return this->display;
}
