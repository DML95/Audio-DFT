#ifndef SHADERCLASS_H
#define SHADERCLASS_H

    #include <GLES2/gl2.h>
    #include <string>
    #include <memory>

    class ShaderClass{
        private:
            const std::shared_ptr<GLClass> glClass;
            GLuint shader;
        public:
            //constructor
            ShaderClass(const std::shared_ptr<GLClass> &glClass,int type,const std::string &code);
            //destructor
            ~ShaderClass();
            //obtiene los logs
            std::string getLog();
            //getter setter
            GLuint get();
    };

#endif
