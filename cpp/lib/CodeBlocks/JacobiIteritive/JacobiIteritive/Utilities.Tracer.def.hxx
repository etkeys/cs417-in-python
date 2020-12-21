#ifndef UTILITIES_TRACER_DEF_HXX_INCLUDED
#define UTILITIES_TRACER_DEF_HXX_INCLUDED

#include <string>

class Tracer{
private:
    bool _enable;
    std::string _method;
public:
    Tracer(const char*, bool);
    ~Tracer();
};


#endif // UTILITIES_TRACER_DEF_HXX_INCLUDED
