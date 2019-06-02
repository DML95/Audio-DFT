#include "shader_code_class.h"

const std::string ShaderCodeClass::VERTEX_SHADER=
        "#version 110\n"
        "uniform float maxValue;\n"
        "attribute float vertex;\n"
        "attribute float index;\n"
        "void main(){\n"
        "   float valueAbs=(vertex*2./maxValue)-1.;\n"
        "   float color=(valueAbs+1.)/3.;\n"
        "   gl_Position = vec4(index,valueAbs,1.,1.);\n"
        "   gl_FrontColor = vec4(1.-color,color,0.,1.);\n"
        "}\n";

const std::string ShaderCodeClass::FRAGMENT_SHADER=
        "#version 110\n"
        "void main(){\n"
        "   gl_FragColor = gl_Color;\n"
        "}\n";
