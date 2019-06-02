#ifndef GLCLASS_H
#define GLCLASS_H

    #include <GLES2/gl2.h>
    #include <GL/glx.h>
    #include <memory>

    //inicia un contexto de OpenGL
    class GLClass{
        private:
            static const int glAttribs[];

            const std::shared_ptr<WindowClass> windowClass;
            const std::shared_ptr<DisplayClass> displayClass;
            XVisualInfo *vi;
            GLXContext gl;

        public:
            //constructor
            GLClass(const std::shared_ptr<DisplayClass> &displayClass,
                    const std::shared_ptr<WindowClass> &windowClass,
                    const bool direct);
            //destructor
            ~GLClass();
            //intercambia los buffers de la pantalla
            void swapBuffers();
            //getter y setter
            GLXContext get();
    };

#endif
