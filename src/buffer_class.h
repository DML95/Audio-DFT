#ifndef BUFFERCLASS_H
#define BUFFERCLASS_H

    #include <memory>
    #include <mutex>
    #include <atomic>

    //gestiona los buffers usados para dijado y escritura
    template <class T> class BufferClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            WaitClass waitClass;
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
            //logica
            VBOClass<T>* swapAndGetVBO();
            T* getBuffer();
            long long getElements();
    };

    //implementacion

    #include "buffer_class.h.cpp"
#endif
