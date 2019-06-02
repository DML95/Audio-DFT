#ifndef EVENTCLASS_H
#define EVENTCLASS_H

    #include <X11/Xlib.h>
    #include <atomic>
    #include <thread>

    #include "wait_class.h"

    //gestiona los eventos de X11
    typedef struct{
        unsigned int events;
        unsigned int width;
        unsigned int height;
        float max;
    }EVENTS;

    class EventClass{
        private:
            const std::shared_ptr<WindowClass> windowClass;
            const std::shared_ptr<DisplayClass> displayClass;
            std::unique_ptr<std::thread> thread;
            Atom wmDeleteWindows;
            WaitClass waitClass;
            std::atomic_bool run;
            std::atomic_bool load;
            std::atomic_uint events;
            std::atomic_uint width;
            std::atomic_uint height;
            std::atomic<float> max;

            //hilo que recoje los eventos de X11
            static void mainThread(EventClass *eventClass);
        public:
            //constantes
            static constexpr unsigned int PAINT=1;
            static constexpr unsigned int RESIZE=PAINT<<1;
            static constexpr unsigned int SWAP=RESIZE<<1;
            static constexpr unsigned int MAX=RESIZE<<1;
            //constructor
            EventClass(const std::shared_ptr<WindowClass> &windowClass);
            //destructor
            ~EventClass();
            //lee los eventos en cola
            bool next(EVENTS &eventsStruct);
            //agrega un evento general
            void addEvents(unsigned int eventsMask);
            //agrega evento que pide intercambio de buffer y repintado
            //con el tamaño maximo del buffer para el escalado
            void swapAndPaint(float max);
    };

#endif
