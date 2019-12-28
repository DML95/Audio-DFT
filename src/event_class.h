#ifndef EVENTCLASS_H
#define EVENTCLASS_H

    #include <X11/Xlib.h>
    #include <atomic>
    #include <thread>

    #include "semaphore_class.h"

    class EventClass{
        private:
            const std::shared_ptr<WindowClass> windowClass;
            const std::shared_ptr<DisplayClass> displayClass;
            std::unique_ptr<std::thread> thread;
            Atom wmDeleteWindows;
            SemaphoreClass semaphore;
            std::mutex mutex;
            std::atomic_bool run;
            std::atomic_bool load;
            std::atomic_uint events;
            std::atomic_uint width;
            std::atomic_uint height;
            std::atomic<float> maxValue;

            //hilo que recoje los eventos de X11
            static void mainThread(EventClass *eventClass);
        public:

            //gestiona los eventos de X11
            typedef struct{
                unsigned int events;
                unsigned int width;
                unsigned int height;
                float maxValue;
            }eventsStruct;

            //mascara de eventos
            enum eventMack{
                paint=0b1,
                resize=0b10,
                swap=0b100,
                max=0b1000
            };

            //constructor
            EventClass(const std::shared_ptr<WindowClass> &windowClass);
            //destructor
            ~EventClass();
            //lee los eventos en cola
            bool next(EventClass::eventsStruct &eventsStruct);
            //agrega un evento general
            void addEvents(EventClass::eventMack eventsMask);
            //agrega evento que pide intercambio de buffer y repintado
            //con el tamaño maximo del buffer para el escalado
            void swapAndPaint(float max);
    };

#endif
