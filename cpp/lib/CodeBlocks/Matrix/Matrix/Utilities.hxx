#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include "Utilities.Tracer.def.hxx"

typedef unsigned int uint;

bool DoubleEquals(double lhs, double rhs);

bool DoubleEquals(double lhs, double rhs, double epsilon);

void PrintDebug(std::ostream&, std::string message);

void PrintTrace(std::ostream&, std::string message);

#endif
