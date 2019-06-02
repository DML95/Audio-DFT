#include <X11/Xlib.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <exception>

//eliminar
#include <chrono>
#include <thread>
#include <random>
//eliminar

#define GL_GLEXT_PROTOTYPES
#define GL_EXT_PROTOTYPES

#include "main.h"
#include "config_log_class.h"
#include "window_class.h"
#include "gl_class.h"
#include "event_class.h"
#include "VBO_class.h"
#include "buffer_class.h"
#include "dft_class.h"
#include "audio_class.h"
#include "shader_class.h"
#include "program_class.h"
#include "shader_code_class.h"

#define TYPE_BUFFER float
#define SIZE 512

static std::shared_ptr<DisplayClass> displayClassWindows;
static std::shared_ptr<DisplayClass> displayClassGL;
static std::shared_ptr<WindowClass> windowClass;
static std::shared_ptr<GLClass> glClass;
static std::shared_ptr<EventClass> eventClass;
static std::shared_ptr<BufferClass<TYPE_BUFFER> > bufferClass;
static std::shared_ptr<AudioClass<TYPE_BUFFER> > audioClass;
static std::shared_ptr<VBOClass<float> > vboClassIndex;
//Shader
static std::shared_ptr<ShaderClass> vertexShaderClass;
static std::shared_ptr<ShaderClass> fragmentShaderClass;
//program
static std::shared_ptr<ProgramClass> programClass;
//atributos program
static GLint vertexAttribute;
static GLint indexAttribute;


static void glLoop(){
    std::clog<<"[Main.cpp] mainLoop init"<<std::endl;
    EVENTS events;
    VBOClass<TYPE_BUFFER> *vboClass=NULL;
    UtilsClass::setRealtimeThread();
    while(eventClass->next(events)){
        bool draw=false;
        if(events.events&EventClass::MAX){
            programClass->use();
                programClass->setUniformVar("maxValue",events.max);
            programClass->notUse();
            draw=true;
        }
        if(events.events&EventClass::RESIZE){
            glViewport(0,0,events.width,events.height);
            draw=true;
        }
        if(events.events&EventClass::SWAP){
            vboClass=bufferClass->swapAndGetVBO();
            draw=true;
        }
        if(draw){
            glClearColor(0.,0.,0.,0.);
            glClear(GL_COLOR_BUFFER_BIT);
            if(vboClass){
                /*glBegin(GL_TRIANGLES);
                    glColor3f(1.,0.,0.);glVertex2f(1.,1.);
                    glColor3f(0.,1.,0.);glVertex2f(-1.,1.);
                    glColor3f(0.,0.,1.);glVertex2f(0.,-1.);
                glEnd();*/
                programClass->use();
                    vboClass->bind(GL_ARRAY_BUFFER);
                        //glVertexPointer(1,GL_FLOAT,0,0);
                        glVertexAttribPointer(vertexAttribute,1,GL_FLOAT,0,0,0);
                    //glColorPointer(3,GL_FLOAT,sizeof(float)*5,(GLvoid*)(sizeof(float)*2));
                    vboClassIndex->bind(GL_ARRAY_BUFFER);
                    //glGetAttribLocation
                        glVertexAttribPointer(indexAttribute,1,GL_FLOAT,0,0,0);
                        //glVertexPointer(1,GL_FLOAT,0,0);
                    vboClassIndex->unBind(GL_ARRAY_BUFFER);
                    glDrawArrays(GL_LINE_STRIP,0,SIZE);
                programClass->notUse();
            }
            glFinish();
        }
        if(events.events&EventClass::PAINT){
            glClass->swapBuffers();
        }
    }
    std::clog<<"[Main.cpp] mainLoop end"<<std::endl;
}

static void loadIndex(){
    vboClassIndex=std::make_shared<VBOClass<float> >(glClass,SIZE,GL_STATIC_DRAW);
    float *indexPtr=vboClassIndex->map(GL_WRITE_ONLY);
    for(int cont=0;cont<SIZE;++cont){
        indexPtr[cont]=(cont*2./(SIZE-1.))-1.;
    }
    vboClassIndex->unMap();
}

static void glLoad(){
    std::vector<std::shared_ptr<ShaderClass> > shaderList;
    glEnable(GL_MULTISAMPLE);
    //glEnableClientState(GL_VERTEX_ARRAY);
    loadIndex();
    vertexShaderClass=std::make_shared<ShaderClass>(glClass,GL_VERTEX_SHADER,ShaderCodeClass::VERTEX_SHADER);
    shaderList.push_back(vertexShaderClass);
    fragmentShaderClass=std::make_shared<ShaderClass>(glClass,GL_FRAGMENT_SHADER,ShaderCodeClass::FRAGMENT_SHADER);
    shaderList.push_back(fragmentShaderClass);
    programClass=std::make_shared<ProgramClass>(glClass,shaderList);
    vertexAttribute=programClass->getAttribLocation("vertex");
    indexAttribute=programClass->getAttribLocation("index");

}

int main(){
    ConfigLog configLog;
    displayClassWindows=std::make_shared<DisplayClass>();
    displayClassGL=std::make_shared<DisplayClass>();
    windowClass=std::make_shared<WindowClass>(displayClassWindows,0,0,300,200,0x0,true);
    windowClass->setTitle("Audio DFT");
    eventClass=std::make_shared<EventClass>(windowClass);
    glClass=std::make_shared<GLClass>(displayClassGL,windowClass,true);
    bufferClass=std::make_shared<BufferClass<TYPE_BUFFER> >(glClass,SIZE);
    audioClass=std::make_shared<AudioClass<TYPE_BUFFER> >(eventClass,bufferClass);
    glLoad();
    glLoop();
    return 0;
}
