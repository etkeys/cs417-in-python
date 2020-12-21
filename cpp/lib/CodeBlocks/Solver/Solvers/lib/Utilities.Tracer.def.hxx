#ifndef UTILITIES_TRACER_DEF_HXX_INCLUDED
#define UTILITIES_TRACER_DEF_HXX_INCLUDED

#include <string>

class Tracer{
private:
    std::string _method;
public:
    Tracer(const char*);
    ~Tracer();
};


#endif // UTILITIES_TRACER_DEF_HXX_INCLUDED
