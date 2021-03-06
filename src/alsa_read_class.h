#ifndef ALSAREADCLASS_H
#define ALSAREADCLASS_H
    #include <vector>
    #include <alsa/asoundlib.h>
	#include <string>
	#include <stdexcept>

    //leer desde el dispositivo de audio usando la api ALSA(no bloquea)
    class AlsaReadClass{
        private:
            snd_pcm_t *pcm;

        public:

            typedef struct{
                snd_pcm_format_t format; //establece el formato de salida
                unsigned int rateNear; //establece la frecuencia mas cercana compatible en Hz
                unsigned int channels; //establece el numero de canales
                snd_pcm_uframes_t sizeBifferNear; //establece el tamaño de buffer mas cercano
                snd_pcm_uframes_t periodSizeNear;
            }config_alsa;

            //constructor
            AlsaReadClass(AlsaReadClass::config_alsa &configAlsa);
            //destructor
            ~AlsaReadClass();
            //leer desde el dispositivo hacia un buffer
            template <class T> snd_pcm_sframes_t read(std::vector<T> &buffer);
            //espera hasta acabar la ejecucion
            int wait();
    };

    //implementacion template

    template <class T> snd_pcm_sframes_t AlsaReadClass::read(std::vector<T> &buffer){
        snd_pcm_sframes_t out=snd_pcm_readi(this->pcm,buffer.data(),buffer.capacity());
        if(out<0){
            throw std::runtime_error("[snd_pcm_readi] "+(std::string)snd_strerror(out));
        }
        return out;
    }
#endif
