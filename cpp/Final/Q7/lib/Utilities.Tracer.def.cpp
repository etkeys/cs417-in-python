#include <iostream>

#include "Utilities.hxx"
#include "Utilities.Tracer.def.hxx"

#include <string>

Tracer::Tracer(const char* method, bool enable){
    _enable = enable;
    _method = std::string(method);
    if (_enable)
        PrintTrace(std::cerr, "Push " + _method);
}

Tracer::~Tracer(){
    if (_enable)
        PrintTrace(std::cerr, "Pop " + _method);
}
