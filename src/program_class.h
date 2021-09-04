#ifndef PROGRAMCLASS_H
#define PROGRAMCLASS_H

    #include <GLES2/gl2.h>
    #include <memory>

    class ProgramClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            const std::vector<std::shared_ptr<ShaderClass> > shaderList;
            GLuint program;
        public:
            //constructor
            ProgramClass(const std::shared_ptr<GLClass> &glClass,const std::vector<std::shared_ptr<ShaderClass> > shaderList);
            //destructor
            ~ProgramClass();
            //estable el programa
            void use();
            //retira el programa
            static void notUse();
            //establece una cariable uforme de tipo float
            void setUniformVar(const std::string &name,float value);
            //optiene el identigicador de un atrubuto
            GLint getAttribLocation(const std::string &name);
            //obtiene los logs
            std::string getLog();
            //getter setter
            GLuint get();
    };

#endif
