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
    while(!this->mutex.try_lock()){
        this->semaphore.notify_all();
    }
    bool option=this->option;
    VBOClass<T> *vboClass=this->vboClass[option].get();
    vboClass->unMap();
    option=!option;
    this->buffer=this->vboClass[option]->map(GL_READ_WRITE);
    this->option=option;
    this->swap=false;
    this->mutex.unlock();
    return vboClass;
}

template <class T> T* BufferClass<T>::waitAndGetBuffer(){
    T *buffer;
    bool swap;
    do{
        this->mutex.lock();
        buffer=this->buffer;
        swap=this->swap.exchange(true);
        if(swap)semaphore.wait();
        this->mutex.unlock();
    }while(swap);
    return buffer;
}

template <class T> long long BufferClass<T>::getElements(){
    return this->elements;
}
