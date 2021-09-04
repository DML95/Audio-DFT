#include <iostream>

#include "window_class.h"
#include "gl_class.h"
#include "shader_class.h"

#include "program_class.h"

ProgramClass::ProgramClass(const std::shared_ptr<GLClass> &glClass,const std::vector<std::shared_ptr<ShaderClass> > shaderList):
        glClass(glClass),shaderList(shaderList){
    std::clog<<"[ProgramClass] Loadding"<<std::endl;
    GLint isLink;
    this->program=glCreateProgram();
    if(!this->program)throw std::runtime_error("[glCreateProgram]");
    for(int cont=0,size=shaderList.size();cont<size;++cont){
        glAttachShader(this->program,shaderList[cont]->get());
    }
    glLinkProgram(this->program);
    glGetProgramiv(this->program,GL_LINK_STATUS,&isLink);
    if(isLink==GL_FALSE){
        std::string log=this->getLog();
        this->~ProgramClass();
        throw std::runtime_error("[glLinkProgram]\n\t[error] "+log);
    }
}

ProgramClass::~ProgramClass(){
    std::clog<<"[ProgramClass] Unloadding"<<std::endl;
    glDeleteProgram(this->program);
}

void ProgramClass::use(){
    glUseProgram(this->program);
}

void ProgramClass::notUse(){
    glUseProgram(0);
}

GLuint ProgramClass::get(){
    return this->program;
}

void ProgramClass::setUniformVar(const std::string &name,float value){
    std::clog<<"[ProgramClass] setUniformVar '"<<name<<"' = "<<value<<std::endl;
    GLint loc=glGetUniformLocation(this->program,name.c_str());
    if(loc==-1)throw std::runtime_error("[glGetUniformLocation]");
    glUniform1f(loc,value);
}

GLint ProgramClass::getAttribLocation(const std::string &name){
    GLint loc=glGetAttribLocation(this->program,name.c_str());
    std::clog<<"[ProgramClass] getAttribLocation '"<<name<<"' = "<<loc<<std::endl;
    if(loc==-1)throw std::runtime_error("[glGetAttribLocation]");
    glEnableVertexAttribArray(loc);
    return loc;
}

std::string ProgramClass::getLog(){
    GLint length;
	glGetProgramiv(this->program,GL_INFO_LOG_LENGTH,&length);
	std::string log;
	log.resize(length);
	glGetProgramInfoLog(this->program,length,&length,(GLchar*)log.c_str());
	log.resize(length);
	return log;
}
