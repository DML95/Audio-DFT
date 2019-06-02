#ifndef UTILSCLASS_H
#define UTILSCLASS_H
    #define UTILS_TEST_DEFAULT true

    class UtilsClass{
        private:
            float (*internalMax)(float *array,const int size);

            //busca el valor maximo multiprocesador
            float static standardMax(float *array,const int size);
            #if ( defined(__x86__) || defined(__x86_64__) ) && !UTILS_TEST_DEFAULT
                //busca el valor maximo SSE(x86)
                float static sseMax(float *array,const int size);
            #endif
        public:
            //constructor
            UtilsClass();
            //busca el valor maximo
            float max(float *array,const int size);
            //establece la prioridad del hilo a tiempo real
            bool static setRealtimeThread();
    };
#endif
