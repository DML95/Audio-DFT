#include <iostream>

#include "config_log_class.h"

//#define _DEBUG

ConfigLog::ConfigLog(){
    #if !defined(_DEBUG)
		std::clog<<"[ConfigLog] Log disable"<<std::endl;
        std::clog.setstate(std::ios_base::failbit);
    #endif
}
