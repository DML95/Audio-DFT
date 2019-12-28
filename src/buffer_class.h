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

    //implementacion

    #include "buffer_class.h.cpp"
#endif
