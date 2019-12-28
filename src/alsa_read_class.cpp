#include "alsa_read_class.h"
#include <iostream>

AlsaReadClass::AlsaReadClass(AlsaReadClass::config_alsa &configAlsa){
    std::clog<<"[AlsaReadClass] Loadding\n\tformat: "<<configAlsa.format<<
            "\n\tnear rate: "<<configAlsa.rateNear<<" Hz"<<
            "\n\tchannels: "<<configAlsa.channels<<
            "\n\tbuffer size near: "<<configAlsa.sizeBifferNear<<
            "\n\tperiod size near; "<<configAlsa.periodSizeNear<<std::endl;
    int err=snd_pcm_open(&this->pcm,"default",(snd_pcm_stream_t)(SND_PCM_STREAM_CAPTURE|SND_PCM_NONBLOCK),0);
    if(err<0){
        throw std::runtime_error("[snd_pcm_open] "+(std::string)snd_strerror(err));
    }
    snd_pcm_hw_params_t *hwParams;
    err=snd_pcm_hw_params_malloc(&hwParams);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_malloc] "+(std::string)snd_strerror(err));
    }
    err=snd_pcm_hw_params_any(this->pcm,hwParams);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_any] "+(std::string)snd_strerror(err));
    }
    err=snd_pcm_hw_params_set_access(this->pcm,hwParams,SND_PCM_ACCESS_RW_INTERLEAVED);
	if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_access] "+(std::string)snd_strerror(err));
    }
    err=snd_pcm_hw_params_set_format(this->pcm,hwParams,configAlsa.format);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_format] "+(std::string)snd_strerror(err));
    }
    err=snd_pcm_hw_params_set_rate_near(this->pcm,hwParams,&configAlsa.rateNear,NULL);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_rate_near] "+(std::string)snd_strerror(err));
    }
    std::clog<<"[AlsaReadClass] rate real: "<<configAlsa.rateNear<<" Hz"<<std::endl;
    err=snd_pcm_hw_params_set_channels(this->pcm,hwParams,configAlsa.channels);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_rate_near] "+(std::string)snd_strerror(err));
    }
    err=snd_pcm_hw_params_set_buffer_size_near(this->pcm,hwParams,&configAlsa.sizeBifferNear);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_buffer_size_near] "+(std::string)snd_strerror(err));
    }
    std::clog<<"[AlsaReadClass] period size real: "<<configAlsa.sizeBifferNear<<std::endl;
    err=snd_pcm_hw_params_set_period_size_near(this->pcm,hwParams,&configAlsa.periodSizeNear,NULL);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params_set_period_size_near] "+(std::string)snd_strerror(err));
    }
    std::clog<<"[AlsaReadClass] period size real: "<<configAlsa.periodSizeNear<<std::endl;
    err=snd_pcm_hw_params(this->pcm,hwParams);
    if(err<0){
        this->~AlsaReadClass();
        throw std::runtime_error("[snd_pcm_hw_params] "+(std::string)snd_strerror(err));
    }
    snd_pcm_hw_params_free(hwParams);
}

AlsaReadClass::~AlsaReadClass(){
    std::clog<<"[AlsaReadClass] Unloadding"<<std::endl;
    snd_pcm_close(this->pcm);
}

int AlsaReadClass::wait(){
    return snd_pcm_wait(this->pcm,-1);
}
