#ifndef MAIN_H
#define MAIN_H

    #include "config_log_class.h"
    #include "window_class.h"
    #include "gl_class.h"
    #include "event_class.h"
    #include "VBO_class.h"
    #include "buffer_class.h"
    #include "dft_class.h"
    #include "audio_class.h"
    #include "shader_class.h"
    #include "program_class.h"
    #include "shader_code_class.h"

    class Main{
        private:
            #define TYPE_BUFFER float

            static constexpr int size=512;

            static std::shared_ptr<Main> main;

            std::shared_ptr<DisplayClass> displayClassWindows;
            std::shared_ptr<DisplayClass> displayClassGL;
            std::shared_ptr<WindowClass> windowClass;
            std::shared_ptr<GLClass> glClass;
            std::shared_ptr<EventClass> eventClass;
            std::shared_ptr<BufferClass<TYPE_BUFFER> > bufferClass;
            std::shared_ptr<AudioClass<TYPE_BUFFER> > audioClass;
            std::shared_ptr<VBOClass<float> > vboClassIndex;
            //Shader
            std::shared_ptr<ShaderClass> vertexShaderClass;
            std::shared_ptr<ShaderClass> fragmentShaderClass;
            //program
            std::shared_ptr<ProgramClass> programClass;
            //atributos program
            GLint vertexAttribute;
            GLint indexAttribute;

            //constructor
            Main();
            //intriduce en el vbo las coordenadas del eje x
            void loadIndex();
            //carga los Shader de opengl
            void glLoad();
            //comprueva si la version de opengl es compatible
            void checkVersionGL();
        public:
            //devuelve la hisnatccia de la clase
            static std::shared_ptr<Main>& getHinstance();
            //bucle principal de dibujado
            void run();
    };

    //contiene el hilo principal del programa
    int main();

#endif
