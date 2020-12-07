#ifndef AUDIOCLASS_H
#define AUDIOCLASS_H

	#include "dft_class.h"
	#include "alsa_read_class.h"
	#include "utils_class.h"

    template <class T> class AudioClass{
        private:
            const std::shared_ptr<EventClass> eventClass;
            const std::shared_ptr<BufferClass<T> > bufferClass;
            std::unique_ptr<std::thread> thread;
            std::atomic_bool run;

            static void mainThread(AudioClass<T> *eventClass);
            static bool setRealtimeThread();
        public:
            //constructor
            AudioClass(const std::shared_ptr<EventClass> &eventClass,const std::shared_ptr<BufferClass<T> > &bufferClass);
            //destructor
            ~AudioClass();
            //logica

    };

    //implemetacion template

	template <class T> void AudioClass<T>::mainThread(AudioClass<T> *audioClass){
		std::clog<<"[AudioClass] Thread init"<<std::endl;
		const unsigned long long elements=audioClass->bufferClass->getElements();
		const unsigned long long mediaElements=elements<<1;
		T *buffer;
		std::vector<float> audio(mediaElements);
		DFTClass dftClass;
		UtilsClass utilsClass;
		float max=0;
		AlsaReadClass::config_alsa configAlsa={
			SND_PCM_FORMAT_FLOAT,
			10000,
			1,
			0,
			elements
		};
		AlsaReadClass alsaReadClass(configAlsa);

		UtilsClass::setRealtimeThread();

		while(audioClass->run){
			buffer=audioClass->bufferClass->waitAndGetBuffer();
			if(buffer){
				alsaReadClass.wait();
				while(!dftClass.dftAndAbs(buffer,audio)){
					dftClass.cache(mediaElements);
				}
				buffer[0]=0.;
				max=utilsClass.max(buffer,elements);
				alsaReadClass.read(audio);
			}
			audioClass->eventClass->swapAndPaint(max);
		}
		std::clog<<"[AudioClass] Thread end"<<std::endl;
	}

	template <class T> AudioClass<T>::AudioClass(
			const std::shared_ptr<EventClass> &eventClass,
			const std::shared_ptr<BufferClass<T> > &bufferClass):
			eventClass(eventClass),
			bufferClass(bufferClass){
		std::clog<<"[AudioClass] Loadding"<<std::endl;
		this->run=true;
		this->thread=std::make_unique<std::thread>(AudioClass<T>::mainThread,this);
	}

	template <class T> AudioClass<T>::~AudioClass(){
		std::clog<<"[AudioClass] Unloadding"<<std::endl;
		this->run=false;
		this->bufferClass->swapAndGetVBO();
		this->thread->join();
	}

#endif
