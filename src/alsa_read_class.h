#ifndef ALSAREADCLASS_H
#define ALSAREADCLASS_H
    #include <vector>
    #include <alsa/asoundlib.h>

    typedef struct{
        snd_pcm_format_t format; //establece el formato de salida
        unsigned int rateNear; //establece la frecuencia mas cercana compatible en Hz
        unsigned int channels; //establece el numero de canales
        snd_pcm_uframes_t sizeBifferNear; //establece el tamaño de buffer mas cercano
        snd_pcm_uframes_t periodSizeNear;
    }CONFIG_ALSA;

    //leer desde el dispositivo de audio usando la api ALSA(no bloquea)
    class AlsaReadClass{
        private:
            snd_pcm_t *pcm;

        public:
            //constructor
            AlsaReadClass(CONFIG_ALSA &configAlsa);
            //destructor
            ~AlsaReadClass();
            //leer desde el dispositivo hacia un buffer
            template <class T> snd_pcm_sframes_t read(std::vector<T> &buffer);
            //espera hasta acabar la ejecucion
            int wait();
    };

    #include "alsa_read_class.h.cpp"
#endif
