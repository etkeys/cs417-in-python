#include "Utilities.hxx"
#include "Utilities.Tracer.def.hxx"

#include <string>

Tracer::Tracer(const char* method){
    _method = std::string(method);
    PrintTrace(std::cerr, "Push " + _method);
}

Tracer::~Tracer(){
    PrintTrace(std::cerr, "Pop " + _method);
}
