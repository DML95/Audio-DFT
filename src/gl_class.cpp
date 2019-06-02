#include <iostream>

#include "window_class.h"

#include "gl_class.h"

const int GLClass::glAttribs[]={
    GLX_RENDER_TYPE,GLX_RGBA_BIT,
    GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
    GLX_DOUBLEBUFFER,GL_TRUE,
    GLX_RED_SIZE,1,
    GLX_GREEN_SIZE,1,
    GLX_BLUE_SIZE,1,
    GLX_ALPHA_SIZE,1,
    GLX_SAMPLE_BUFFERS,GL_TRUE,// <-- MSAA
    GLX_SAMPLES,4,// <-- MSAA
    None
};

GLClass::GLClass(const std::shared_ptr<DisplayClass> &displayClass,
                const std::shared_ptr<WindowClass> &windowClass,
                const bool direct):
        windowClass(windowClass),
        displayClass(displayClass){
    std::clog<<"[GLClass] Loadding"<<std::endl;
    this->vi=glXChooseVisual(displayClass->get(),XDefaultScreen(displayClass->get()),(int*)GLClass::glAttribs);
    if(!this->vi)throw std::runtime_error("[glXChooseVisual]");
    this->gl=glXCreateContext(displayClass->get(),vi,NULL,direct);
    if(!this->gl)throw std::runtime_error("[glXChooseVisual]");
    if(!glXMakeCurrent(displayClass->get(),windowClass->get(),this->gl)){
        this->~GLClass();
        throw std::runtime_error("[glXMakeCurrent]");
    }
}

GLClass::~GLClass(){
    std::clog<<"[GLClass] Unloadding"<<std::endl;
    glXMakeCurrent(displayClass->get(),NULL,NULL);
    glXDestroyContext(displayClass->get(),this->gl);
}

void GLClass::swapBuffers(){
    glXSwapBuffers(displayClass->get(),windowClass->get());
}

GLXContext GLClass::get(){
    return this->gl;
}
