#ifndef VBOCLASS_H
#define VBOCLASS_H

    #include <GLES2/gl2.h>
    #include <vector>
    #include <memory>

    #include "gl_class.h"

    //crea un VBO(Memoria de la GPU) de opengl
    template <class T>class VBOClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            GLuint vbo;
        public:
            //constructor
            VBOClass(const std::shared_ptr<GLClass> &glClass,long long elements,int usage);
            //destructor
            ~VBOClass();
            //estable el VBO
            void bind(GLenum target);
            //retira el VBO
            static void unBind(GLenum target);
            //obtine un mapa en memoria
            T* map(int access);
            //retira el mapa de memoria
            void unMap();
    };

    //implementacion

    #include "VBO_class.h.cpp"

#endif
