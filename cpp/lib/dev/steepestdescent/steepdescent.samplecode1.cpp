#include <iostream>
#include <cmath>
using namespace std;
double HofXY(double x, double y);
double HprimeX(double x, double y);
double HprimeY(double x, double y);

int main()
{
    double x, y, xnew, ynew, a, b, h;
    x  = 4;
    y  =-4;
    ///  xnew = x - h * HprimeX(x,y)
    ///  xnew = 4 - h * HprimeX(4,-4);
    a = HprimeX(4,-4);
    b = HprimeY(4,-4);
    cout<<"a = "<<a<<", b = "<<b<<endl;
    ///therefore xnew = 4+ 4h
    ///and       ynew = -4+6h
    ///substituion into HofXY yields G(h) which is solved = .191
    h=.191;
    for(int i=0; i<10; i++)
    {
    xnew = x -h*HprimeX(x,y);
    ynew = y -h*HprimeY(x,y);
    x = xnew;
    y = ynew;
    cout<<"("<<x<<", "<<y<<")"<<endl;

    }
    return 0;
}
///function to evaluate the original equation
double HofXY(double x, double y)
{
    double ans=0.0;
    ans = pow(x,2.0)-(4*x)+(2*x*y)+(2*pow(y,2.0))+(2*y)+14;
    return ans;
}
///function to evaluate the derivative with respect to x
double HprimeX(double x, double y)
    {
        double ans=0.0;
        ans = (2*x) + (2*y) -4.0;
        return ans;
    }

///function to evaluate the derivative with respect to y
double HprimeY(double x, double y)
{
    double ans=0.0;
    ans = (2*x) + (4*y)+2;
    return ans;
}