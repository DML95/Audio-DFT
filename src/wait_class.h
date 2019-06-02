#ifndef WAITCLASS_H
#define WAITCLASS_H

    #include <condition_variable>
    #include <mutex>

    //clase para detener y reanudar hilos
    class WaitClass{
        private:
            std::mutex mutex;
            std::unique_ptr<std::condition_variable> event;
        public:
            //constructor
            WaitClass();
            //detiene el hilo
            void wait();
            //activa los hilos detenidos
            void notify_all();
    };

#endif
