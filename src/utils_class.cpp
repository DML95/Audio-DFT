#include <iostream>

#include "utils_class.h"

float UtilsClass::standardMax(float *array,const int size){
    float max=-1./0.;
    for(int cont=0;cont<size;++cont){
        if(max<array[cont]){
            max=array[cont];
        }
    }
    return max;
}

float UtilsClass::max(float *array,const int size){
    return this->internalMax(array,size);
}

bool UtilsClass::setRealtimeThread(){
    pthread_t this_thread=pthread_self();
    std::clog<<"[UtilsClass] set Realtime Thread: "<<this_thread<<std::endl;
    struct sched_param params;
    params.sched_priority=sched_get_priority_max(SCHED_FIFO);
    return pthread_setschedparam(this_thread,SCHED_FIFO,&params);
}

#if ( defined(__x86__) || defined(__x86_64__) ) && !UTILS_TEST_DEFAULT

    #include <cpuid.h>
    #include <xmmintrin.h>

    UtilsClass::UtilsClass(){
        int eax, ebx, ecx, edx;
        __cpuid(1, eax, ebx, ecx, edx);
        if(edx&bit_SSE){
            std::clog<<"[UtilsClass] max: x86 SSE"<<std::endl;
            this->internalMax=&this->sseMax;
        }else{
            std::clog<<"[UtilsClass] max: standard"<<std::endl;
            this->internalMax=&this->standardMax;
        }
    }

    float UtilsClass::sseMax(float *array,const int size){
        __m128 maxSIMD=_mm_set_ps1(-1./0.);
        for(int cont=0;cont<size;cont+=4){
            maxSIMD=_mm_max_ps(maxSIMD,_mm_load_ps(&array[cont]));
        }
        float max[4];
        _mm_store_ps(max,maxSIMD);
        return UtilsClass::standardMax(max,4);
    }

#else

    UtilsClass::UtilsClass(){
        std::clog<<"[UtilsClass] max: standard"<<std::endl;
        this->internalMax=&this->standardMax;
    }

#endif
