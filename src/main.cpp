#include <X11/Xlib.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <exception>

#define GL_GLEXT_PROTOTYPES
#define GL_EXT_PROTOTYPES


#include "main.h"

std::shared_ptr<Main> Main::main;

Main::Main(){
    std::clog<<"[Main] load"<<std::endl;
    ConfigLog configLog;
    this->displayClassWindows=std::make_shared<DisplayClass>();
    this->displayClassGL=std::make_shared<DisplayClass>();
    this->windowClass=std::make_shared<WindowClass>(displayClassWindows,0,0,600,400,0x0,true);
    this->windowClass->setTitle("Audio DFT");
    this->eventClass=std::make_shared<EventClass>(windowClass);
    this->glClass=std::make_shared<GLClass>(displayClassGL,windowClass,true);
    this->bufferClass=std::make_shared<BufferClass<TYPE_BUFFER> >(glClass,Main::size);
    this->audioClass=std::make_shared<AudioClass<TYPE_BUFFER> >(eventClass,bufferClass);
    glLoad();
}

void Main::loadIndex(){
    this->vboClassIndex=std::make_shared<VBOClass<float> >(this->glClass,Main::size,GL_STATIC_DRAW);
    float *indexPtr=this->vboClassIndex->map(GL_WRITE_ONLY);
    for(int cont=0;cont<Main::size;++cont){
        indexPtr[cont]=(cont*2./(Main::size-1.))-1.;
    }
    this->vboClassIndex->unMap();
}

void Main::glLoad(){
    std::vector<std::shared_ptr<ShaderClass> > shaderList;
    glEnable(GL_MULTISAMPLE);
    this->loadIndex();
    this->vertexShaderClass=std::make_shared<ShaderClass>(glClass,GL_VERTEX_SHADER,ShaderCodeClass::VERTEX_SHADER);
    shaderList.push_back(vertexShaderClass);
    this->fragmentShaderClass=std::make_shared<ShaderClass>(glClass,GL_FRAGMENT_SHADER,ShaderCodeClass::FRAGMENT_SHADER);
    shaderList.push_back(fragmentShaderClass);
    this->programClass=std::make_shared<ProgramClass>(glClass,shaderList);
    this->vertexAttribute=programClass->getAttribLocation("vertex");
    this->indexAttribute=programClass->getAttribLocation("index");
}

std::shared_ptr<Main>& Main::getHinstance(){
    if(!Main::main){
        Main::main.reset(new Main());
    }
    return Main::main;
}

void Main::run(){
    std::clog<<"[Main] mainLoop init"<<std::endl;
    EventClass::eventsStruct events;
    VBOClass<TYPE_BUFFER> *vboClass=NULL;
    UtilsClass::setRealtimeThread();
    while(this->eventClass->next(events)){
        bool draw=false;
        if(events.events&EventClass::eventMack::max){
            //se establece el valor maximo del eje y
            this->programClass->use();
                this->programClass->setUniformVar("maxValue",events.maxValue);
            this->programClass->notUse();
            draw=true;
        }
        if(events.events&EventClass::eventMack::resize){
            //se redimensiona la escena
            glViewport(0,0,events.width,events.height);
            draw=true;
        }
        if(events.events&EventClass::eventMack::swap){
            //se intecamvia el vbo del eje y
            vboClass=bufferClass->swapAndGetVBO();
            draw=true;
        }
        if(draw){
            //se borra la escena anterior
            glClearColor(0.,0.,0.,0.);
            glClear(GL_COLOR_BUFFER_BIT);
            if(vboClass){
                //se carga el programa
                this->programClass->use();
                    //se carga el vbo con el eje y
                    vboClass->bind(GL_ARRAY_BUFFER);
                        glVertexAttribPointer(vertexAttribute,1,GL_FLOAT,0,0,0);
                    //se carga el vbo con el eje x
                    this->vboClassIndex->bind(GL_ARRAY_BUFFER);
                        glVertexAttribPointer(indexAttribute,1,GL_FLOAT,0,0,0);
                    //se descargan todos los vbos
                    this->vboClassIndex->unBind(GL_ARRAY_BUFFER);
                    //de dibuja la escena
                    glDrawArrays(GL_LINE_STRIP,0,Main::size);
                //se descarga el programa
                this->programClass->notUse();
            }
            //se espera a que acabe el dibujado
            glFinish();
        }
        if(events.events&EventClass::eventMack::paint){
            //se intercabia el bufer atiguo por el nuevo para dibujarlo en pantalla
            this->glClass->swapBuffers();
        }
    }
    std::clog<<"[Main] mainLoop end"<<std::endl;
}

int main(){
    std::shared_ptr<Main> mainHinstance=Main::getHinstance();
    mainHinstance->run();
    return 0;
}
