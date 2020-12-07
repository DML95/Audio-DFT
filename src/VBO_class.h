#ifndef VBOCLASS_H
#define VBOCLASS_H

    #include <GLES2/gl2.h>
    #include <vector>
    #include <memory>

    #include "gl_class.h"

    //crea un VBO(Memoria de la GPU) de opengl
    template <class T>class VBOClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            GLuint vbo;
        public:
            //constructor
            VBOClass(const std::shared_ptr<GLClass> &glClass,long long elements,int usage);
            //destructor
            ~VBOClass();
            //estable el VBO
            void bind(GLenum target);
            //retira el VBO
            static void unBind(GLenum target);
            //obtine un mapa en memoria
            T* map(int access);
            //retira el mapa de memoria
            void unMap();
    };

    //implementacion template

    template <class T> VBOClass<T>::VBOClass(const std::shared_ptr<GLClass> &glClass,long long elements,int usage):
            glClass(glClass){
        int64_t size=sizeof(T)*elements;
        std::clog<<"[VBOClass] Loadding"<<std::endl;
        if(size<=0)throw std::runtime_error("size <= 0");
        glGenBuffers(1,&this->vbo);
        std::clog<<"[VBOClass] Create VBO\n\telements = "<<elements<<"\n\tsize = "<<size<<"\n\tid = "<<this->vbo<<std::endl;
        if(!this->vbo)throw std::runtime_error("[glGenBuffers]");
        this->bind(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER,size,NULL,usage);
        this->unBind(GL_ARRAY_BUFFER);
    }

    template <class T> VBOClass<T>::~VBOClass(){
        std::clog<<"[VBOClass] Unloadding id = "<<this->vbo<<std::endl;
        this->bind(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER,0,NULL,GL_STATIC_DRAW);
        this->unBind(GL_ARRAY_BUFFER);
        glDeleteBuffers(1,&this->vbo);
    }

    template <class T> void VBOClass<T>::bind(GLenum target){
        glBindBuffer(target,this->vbo);
    }

    template <class T> void VBOClass<T>::unBind(GLenum target){
        glBindBuffer(target,0);
    }

    template <class T> T* VBOClass<T>::map(int access){
        this->bind(GL_ARRAY_BUFFER);
        T *out=(T*)glMapBuffer(GL_ARRAY_BUFFER,access);
        this->unBind(GL_ARRAY_BUFFER);
        if(!out)throw std::runtime_error("[glMapBuffer]");
        return out;
    }

    template <class T> void VBOClass<T>::unMap(){
        this->bind(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        this->unBind(GL_ARRAY_BUFFER);
    }

#endif
