#include <iostream>
#include<cmath>

#include "dft_class.h"

void DFTClass::standardDftAndAbs(DFTClass *dftClass,float *out,const std::vector<float> &real){
    const int size=real.capacity();
    const int mediaSize=size>>1;
    COMPLEX *anglePtr=dftClass->angle->data();
    for (int cont1=0;cont1<mediaSize;++cont1) {
        float sumReal=0.;
        float sumImag=0.;
        for (int cont2=1,cont3=1;cont2<size;++cont2,cont3+=cont1){
            int pos=cont3%size;
            sumReal+=real[cont2]*anglePtr[pos].re;
            sumImag+=real[cont2]*anglePtr[pos].im;
        }
        out[cont1]=std::sqrt(sumReal*sumReal+sumImag*sumImag);
    }
}

#if ( defined(__x86__) || defined(__x86_64__) ) && !DFT_TEST_DEFAULT

    #include <cpuid.h>
    #include <xmmintrin.h>

    void DFTClass::sseDftAndAbs(DFTClass *dftClass,float *out,const std::vector<float> &real){
        const int size=real.capacity();
        const int mediaSize=size>>1;
        COMPLEX *anglePtr=dftClass->angle->data();
        for (int cont1=0;cont1<mediaSize;++cont1){
            COMPLEX abs;
            __m128 sumSIMD=_mm_setzero_ps();
            __m128 angleSIMD=_mm_undefined_ps();
            for (int cont2=1,cont3=1;cont2<size;++cont2,cont3+=cont1){
                angleSIMD=_mm_loadl_pi(angleSIMD,(__m64*)&anglePtr[cont3%size]);
                sumSIMD=_mm_add_ps(sumSIMD,_mm_mul_ps(_mm_set_ps1(real[cont2]),angleSIMD));
            }
            sumSIMD=_mm_mul_ps(sumSIMD,sumSIMD);
            _mm_storel_pi((__m64*)&abs,sumSIMD);
            out[cont1]=std::sqrt(abs.re+abs.im);
        }
    }

    DFTClass::DFTClass(){
        int eax, ebx, ecx, edx;
        __cpuid(1, eax, ebx, ecx, edx);
        if(edx&bit_SSE){
            std::clog<<"[DFTClass] DFT: x86 SSE"<<std::endl;
            this->internalDftAndAbs=&this->sseDftAndAbs;
        }else{
            std::clog<<"[DFTClass] DFT: standard"<<std::endl;
            this->internalDftAndAbs=&this->standardDftAndAbs;
        }
        this->angle=std::make_unique<std::vector<COMPLEX> >(0);
    }

#else

    DFTClass::DFTClass(){
        std::clog<<"[DFTClass] DFT: standard"<<std::endl;
        this->internalDftAndAbs=&this->standardDftAndAbs;
        this->angle=std::make_unique<std::vector<COMPLEX> >(0);
    }

#endif

bool DFTClass::dftAndAbs(float *out,const std::vector<float> &real){
    const int size=real.capacity();
    const int mediaSize=size>>1;
    bool run=(bool)this->angle;
    if(run){
        run=size==this->angle->capacity();
        if(run){
            this->internalDftAndAbs(this,out,real);
        }
    }
    return run;
}

void DFTClass::cache(const int size){
    std::clog<<"[DFTClass] create cache size: "<<size<<std::endl;
    this->angle=std::make_unique<std::vector<COMPLEX> >(size);
    COMPLEX *anglePtr=this->angle->data();
    for(int cont=0;cont<size;++cont){
        const float angle=2.*M_PI*cont/size;
        anglePtr[cont].re=std::cos(angle);
        anglePtr[cont].im=-std::sin(angle);
    }
}
