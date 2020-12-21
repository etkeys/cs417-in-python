/// fx = x^2 -sin(x)-.5
#include <iostream>
#include <cmath>
using namespace std;

double fx(double x);
int main()
{
    double x0, x1, xnew, soln, tol;
    int itcount=0;
    x0=0;
    x1= 2;
    tol=.001;
    xnew=x1;
    soln=fx(xnew);
    itcount= 0;

    cout<<"checking end points for opposite sign"<<endl;
    cout<<fx(x0)<<" and "<<fx(x1)<<endl;

    while( fabs(soln)>tol )
    {
        xnew  = (x0+x1)/2.0;
        soln = fx(xnew);
        if((soln>0)&&(x1>0))
            {
            x1=xnew;
            }
        else {x0=xnew;}
        itcount++;
        cout<<x0<<" "<<xnew<<" "<<x1<<endl;
    }
    cout<<"Solution = "<<xnew<<" has fx= "<<soln<<" in "<<itcount<<" iterations"<<endl;
    return 0;

}

double fx(double x){
x = pow(x,2)-sin(x)-.5;
return x;
}