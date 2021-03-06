#ifndef BUFFERCLASS_H
#define BUFFERCLASS_H

    #include <memory>
    #include <mutex>
    #include <atomic>

    #include "gl_class.h"

    //gestiona los buffers usados para dijado y escritura
    template <class T> class BufferClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            SemaphoreClass semaphore;
            std::mutex mutex;
            std::atomic_bool option;
            std::atomic_bool swap;
            long long elements;

            T *buffer;

            std::unique_ptr<VBOClass<T> > vboClass[2];
        public:
            //constructor
            BufferClass(const std::shared_ptr<GLClass> &glClass,long long elements);
            //destructor
            ~BufferClass();
            //intercambia los VBO
            VBOClass<T>* swapAndGetVBO();
            //espara hasta que se intercambian los bufferes y devuelve el ultimo
            T* waitAndGetBuffer();
            long long getElements();
    };

    //implementacion template

    template <class T> BufferClass<T>::BufferClass(const std::shared_ptr<GLClass> &glClass,long long elements):
            glClass(glClass){
        std::clog<<"[BufferClass] Loadding"<<std::endl;
        this->elements=elements;
        this->swap=true;
        this->option=false;
        this->vboClass[0]=std::make_unique<VBOClass<T> >(glClass,elements,GL_DYNAMIC_DRAW);
        this->vboClass[1]=std::make_unique<VBOClass<T> >(glClass,elements,GL_DYNAMIC_DRAW);
        this->swapAndGetVBO();
    }

    template <class T> BufferClass<T>::~BufferClass(){
        std::clog<<"[BufferClass] Unloadding"<<std::endl;
        this->buffer=NULL;
        this->swap=false;
        semaphore.notify_all();
    }

    template <class T> VBOClass<T>* BufferClass<T>::swapAndGetVBO(){
    	std::unique_lock<std::mutex> lock(this->mutex,std::defer_lock);
        while(!lock.try_lock()){
            this->semaphore.notify_all();
            std::this_thread::yield();
        }
        bool option=this->option;
        VBOClass<T> *vboClass=this->vboClass[option].get();
        vboClass->unMap();
        option=!option;
        this->buffer=this->vboClass[option]->map(GL_READ_WRITE);
        this->option=option;
        this->swap=false;
        return vboClass;
    }

    template <class T> T* BufferClass<T>::waitAndGetBuffer(){
        T *buffer;
        bool swap;
        do{
        	std::lock_guard<std::mutex> lock(this->mutex);
            buffer=this->buffer;
            swap=this->swap.exchange(true);
            if(swap)semaphore.wait();
        }while(swap);
        return buffer;
    }

    template <class T> long long BufferClass<T>::getElements(){
        return this->elements;
    }

#endif
