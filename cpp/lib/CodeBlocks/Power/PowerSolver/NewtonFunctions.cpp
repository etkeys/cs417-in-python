#include <cmath>

#ifndef NEWTONFUNCTIONS
#define NEWTONFUNCTIONS

double FOfX(double x){
    return x - exp(-1 * pow(x, 2));
}

double FPrimeOfX(double x){
    return 1 + 2 * x * exp(-1 * pow(x, 2));
}

#endif
