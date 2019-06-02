#ifndef DFTCLASS_H
#define DFTCLASS_H
    #define DFT_TEST_DEFAULT false

    #define _USE_MATH_DEFINES

    #include <cmath>
    #include <memory>
    #include <vector>

    typedef struct{
        float re;
        float im;
    }COMPLEX;

    class DFTClass{
        private:
            std::unique_ptr<std::vector<COMPLEX> > angle;
            void (*internalDftAndAbs)(DFTClass *dftClass,float *out,const std::vector<float> &real);

            //version de dft multiprocesador
            void static standardDftAndAbs(DFTClass *dftClass,float *out,const std::vector<float> &real);
            #if ( defined(__x86__) || defined(__x86_64__) ) && !DFT_TEST_DEFAULT
                //version de dft usanado instrucciones SIMD SSE(x86)
                void static sseDftAndAbs(DFTClass *dftClass,float *out,const std::vector<float> &real);
            #endif
        public:
            //constructor
            DFTClass();
            //dft (retorna false cuando el tamaño del vector de entrada no coincide con la cache)
            bool dftAndAbs(float *out,const std::vector<float> &real);
            //estable la cache en meoria con los senos y cosenos precalculados
            void cache(const int size);
    };

#endif
