#ifndef SEMAPHORECLASS_H
#define SEMAPHORECLASS_H

    #include <condition_variable>
    #include <mutex>

    //clase para detener y reanudar hilos
    class SemaphoreClass{
        private:
            std::mutex mutex;
            std::condition_variable semaphore;
        public:
            //constructor
            SemaphoreClass();
            //detiene el hilo
            void wait();
            //activa los hilos detenidos
            void notify_all();
    };

#endif
