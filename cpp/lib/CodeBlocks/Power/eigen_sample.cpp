#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

int main()
{
double maxx=0;
double eigen=0;
double A[2][2];
///initialize A
A[0][0]=-4;
A[0][1]=10;
A[1][0]=7;
A[1][1]=5;
///really terrible first guess bk
double b[2], bk[2];
bk[0]=-300;
bk[1]=1700;

for(int i=0; i<68; i++){
        ///compute A*old b
b[0]=(A[0][0]*bk[0])+(A[0][1]*bk[1]);
b[1]=(A[1][0]*bk[0])+(A[1][1]*bk[1]);
    ///find the max value in b
if(b[0]>b[1]){maxx=b[0];}
else {maxx=b[1];}
///compute the rayleigh quotient = dominant eigenvalue
eigen=(bk[0]*b[0])+(bk[1]*b[1]);
eigen=eigen/((bk[0]*bk[0])+(bk[1]*bk[1]));
///normalize
b[0]=b[0]/maxx;
b[1]=b[1]/maxx;

cout<<setw(10)<<maxx<<setw(10)<<"  "<<b[0]<<setw(10)<<"  "<<b[1]<<endl;
cout<<"Eigenvalue = "<<eigen<<endl<<endl;
///set bk for next iteration
bk[0]=b[0];
bk[1]=b[1];
}

return 0;}