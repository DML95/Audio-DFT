#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H
    template <class T> class AudioClass{
        private:
            const std::shared_ptr<EventClass> eventClass;
            const std::shared_ptr<BufferClass<T> > bufferClass;
            std::unique_ptr<std::thread> thread;
            std::atomic_bool run;

            static void mainThread(AudioClass<T> *eventClass);
            static bool setRealtimeThread();
        public:
            //constructor
            AudioClass(const std::shared_ptr<EventClass> &eventClass,const std::shared_ptr<BufferClass<T> > &bufferClass);
            //destructor
            ~AudioClass();
            //logica

    };

    //implemetacion

    #include "audio_class.h.cpp"
#endif
