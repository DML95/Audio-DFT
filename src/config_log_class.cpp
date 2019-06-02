#include <iostream>

#include "config_log_class.h"

ConfigLog::ConfigLog(){
    #if !defined(_DEBUG)
        std::clog.setstate(std::ios_base::failbit);
    #endif
    std::clog<<"[ConfigLog] Log enable"<<std::endl;
}
