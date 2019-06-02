#include <iostream>

template <class T> snd_pcm_sframes_t AlsaReadClass::read(std::vector<T> &buffer){
    snd_pcm_sframes_t out=snd_pcm_readi(this->pcm,buffer.data(),buffer.capacity());
    if(out<0){
        throw std::runtime_error("[snd_pcm_readi] "+(std::string)snd_strerror(out));
    }
    return out;
}
