#include <iostream>

#include "window_class.h"
#include "gl_class.h"

#include "shader_class.h"

ShaderClass::ShaderClass(const std::shared_ptr<GLClass> &glClass,int type,const std::string &code):
        glClass(glClass){
    std::clog<<"[ShaderClass] Loadding\n\ttype = "<<type<<"\n\tcode:\n"<<code<<std::endl;
    GLint isCompiled;
    this->shader=glCreateShader(type);
    if(!this->shader)throw std::runtime_error("[glCreateShader]");
    const char *codePtr=code.c_str();
    glShaderSource(this->shader,1,&codePtr,NULL);
    glCompileShader(this->shader);
    glGetShaderiv(this->shader,GL_COMPILE_STATUS,&isCompiled);
    if(isCompiled==GL_FALSE){
        std::string log=this->getLog();
        this->~ShaderClass();
        throw std::runtime_error("[glCompileShader]\n\t[error] "+log);
    }
}

ShaderClass::~ShaderClass(){
    std::clog<<"[ShaderClass] Unloadding"<<std::endl;
    glDeleteShader(this->shader);
}

GLuint ShaderClass::get(){
    return this->shader;
}

std::string ShaderClass::getLog(){
    GLint length;
	glGetShaderiv(this->shader,GL_INFO_LOG_LENGTH,&length);
	std::string log;
	log.resize(length);
	glGetShaderInfoLog(this->shader,length,&length,(GLchar*)log.c_str());
	log.resize(length);
	return log;
}
