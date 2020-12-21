#ifndef FINALCOMMON_CPP
#define FINALCOMMON_CPP
#include <cmath>
#include <fstream>
#include <iostream>

#include "lib/Matrix.hxx"
#include "lib/Solver.hxx"
#include "lib/Utilities.hxx"

#ifndef METHODHEADER_FINALCOMMON
    #define METHODHEADER_FINALCOMMON Tracer _t_(__FUNCTION__,true)
#endif // METHODHEADER_MAIN

using namespace std;

static void PrintResult(Matrix*& result){
    METHODHEADER_FINALCOMMON;

    ofstream file("output/solution.txt");
    uint i = sqrt((double)(result->RowCount()));

    for(uint r = 0; r < result->RowCount(); r += i){
        for(uint c = 0; c < i; c++)
            file << (*result)(r+c,0) << ' ';

        file << endl;
    }

    file.close();
}

static void ReadMatFromFile(Matrix*& m, ifstream& in){
    METHODHEADER_FINALCOMMON;

    for(uint r = 0; r < m->RowCount(); r++)
        for(uint c = 0; c < m->ColumnCount(); c++)
            in >> (*m)(r,c);
}
#endif // FINALCOMMON_CPP
