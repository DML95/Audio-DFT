#include <iostream>
#include <iomanip>
#include <X11/Xutil.h>

#include "window_class.h"

WindowClass::WindowClass(
                const std::shared_ptr<DisplayClass> &displayClass,
                const int x,
                const int y,
                const unsigned int width,
                const unsigned int height,
                unsigned int color,
                const bool resize):
                displayClass(displayClass){
    color&=0xffffff;
    std::clog<<"[WindowClass] Loadding"<<std::endl<<
            "\tx = "<<x<<std::endl<<
            "\ty = "<<y<<std::endl<<
            "\twidth = "<<width<<std::endl<<
            "\theight = "<<height<<std::endl<<
            "\tcolor = 0x"<<std::setfill('0')<<std::setw(6)<<std::hex<<color<<std::dec<<std::endl<<
            "\tresize = "<<resize<<std::endl;
    this->atoms=std::make_unique<std::vector<Atom> >();
    Display *display=this->displayClass->get();
    int defaultScreen=XDefaultScreen(display);
    this->window=XCreateSimpleWindow(
            display,
            RootWindow(display,defaultScreen),
            x,y,width,height,1,
            XBlackPixel(display,defaultScreen),color);
    if(!resize)WindowClass::setNormalSize(width,height,width,height);
}

WindowClass::~WindowClass(){
    std::clog<<"[WindowClass] Unloadding"<<std::endl;
    XSetWMProtocols(
            this->displayClass->get(),
            this->window,
            nullptr,0);
    XUnmapWindow(this->displayClass->get(),this->window);
    XDestroyWindow(this->displayClass->get(),this->window);
}

void WindowClass::map(){
    XMapWindow(this->displayClass->get(),this->window);
}

Atom WindowClass::addWMProtocol(const std::string &name){
    std::clog<<"[WindowClass] Add WMProtocol \""<<name<<"\""<<std::endl;
    Atom atom=XInternAtom(this->displayClass->get(),
            name.c_str(),false);
    this->atoms->push_back(atom);
    XSetWMProtocols(
            this->displayClass->get(),
            this->window,
            this->atoms->data(),this->atoms->size());
    return atom;
}

void WindowClass::setTitle(const std::string &title){
    std::clog<<"[WindowClass] Set title \""<<title<<"\""<<std::endl;
    XStoreName(
            this->displayClass->get(),
            this->window,
            title.c_str());
}

void WindowClass::setNormalSize(int minWidth,int minHeight,int maxWidth,int maxHeight){
    std::clog<<"[WindowClass] Set normal size"<<std::endl<<
            "\tminWidth = "<<minWidth<<std::endl<<
            "\tminHeight = "<<minHeight<<std::endl<<
            "\tmaxWidth = "<<maxWidth<<std::endl<<
            "\tmaxHeight = "<<maxHeight<<std::endl;
    XSizeHints *sh=XAllocSizeHints();
    sh->flags=PMinSize|PMaxSize;
    sh->min_width=minWidth;
    sh->min_height=minHeight;
    sh->max_width=maxWidth;
    sh->max_height=maxHeight;
    XSetWMNormalHints(
            this->displayClass->get(),
            this->window,
            sh);
    XFree(sh);
}

void WindowClass::setEventMask(int mask){
    std::clog<<"[WindowClass] Set mask event"<<std::endl<<
            "\tmask = 0x"<<std::setfill('0')<<std::setw(8)<<std::hex<<mask<<std::dec<<std::endl;
    XSelectInput(
            this->displayClass->get(),
            this->window,
            mask);
}

Window WindowClass::get(){
    return this->window;
}

const std::shared_ptr<DisplayClass> WindowClass::getDisplay(){
    return this->displayClass;
}
