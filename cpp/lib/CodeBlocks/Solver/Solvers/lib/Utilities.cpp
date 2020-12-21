#include <cmath>
#include <iostream>
#include <string>
#ifdef DEBUG
	#include <sstream>
#endif

#include "Utilities.hxx"

bool DoubleEquals(double lhs, double rhs){
    const double epsilon = 0.0001;
	bool result = false;

	result = (std::abs(lhs - rhs) < epsilon);

	return result;
}

void PrintDebug(std::ostream& out, std::string message){
#ifdef DEBUG
    out << "[DEBUG] " << message << std::endl;
#endif
}

void PrintTrace(std::ostream& out, std::string message){
#ifdef TRACE
    out << "[TRACE] " << message << std::endl;
#endif
}
