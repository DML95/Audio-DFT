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
    waitClass.notify_all();
}

template <class T> VBOClass<T>* BufferClass<T>::swapAndGetVBO(){
    bool option=this->option;
    VBOClass<T> *vboClass=this->vboClass[option].get();
    vboClass->unMap();
    option=!option;
    this->buffer=this->vboClass[option]->map(GL_READ_WRITE);
    this->option=option;
    this->swap=false;
    waitClass.notify_all();
    return vboClass;
}

template <class T> T* BufferClass<T>::getBuffer(){
    while(this->swap.exchange(true)){
        waitClass.wait();
    }
    return this->buffer;
}

template <class T> long long BufferClass<T>::getElements(){
    return this->elements;
}
