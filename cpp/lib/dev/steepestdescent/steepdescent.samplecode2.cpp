#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

double HprimeX(double x, double y);///this will solve for a
double HprimeY(double x, double y);///this will solve for b
double Hsolver(double x, double y, double a, double b);///this solves for h
///Hsolver may be different for example if h^2 then quadratic formula, h^5 you need newtons
int main()
{

    ///minimum solution to y^2/2  + xy  + x^2  -3y
    int its=1;
    double x, y, xnew, ynew, a, b, h, hnew, diff;
    cout<<"input initial guess x and y"<<endl;
    cin>>x>>y;


   diff=10;
   while(diff>.01)
    {
    cout<<"iteration: "<<its<<endl;
    ///compute the gradient
    a=HprimeX(x,y);
    b=HprimeY(x,y);
    ///computing new h by setting current G' = 0
    h = Hsolver(x,y,a,b);
    cout<<"h = "<<h<<endl;
    xnew = x + (h*a);
    ynew = y + (h*b);
    diff=sqrt(pow((xnew-x),2.0)+(pow((ynew-y),2.0)));

    x = xnew;
    y = ynew;

    cout<<"("<<x<<", "<<y<<")"<<endl;

    its++;
    }

    return 0;
}

///function to evaluate the derivative with respect to x
double HprimeX(double x, double y)
    {   ///= y + 2x
        double ans=0.0;
        ans = y+(2.0*x);
        return ans;
    }

///function to evaluate the derivative with respect to y
double HprimeY(double x, double y)
{/// = y + x -3
    double ans=0.0;
    ans = y + x - 3;
    return ans;
}

double Hsolver(double x, double y, double a, double b)
{///setting G' = 0.0 ans solving for h
    double h=0.0;
    h = h - ((y*b)+(x*b)+(y*a)+(2*a*x)-(3*b));
    h = h/(pow(b,2.0)+(2*a*b)+(2*pow(a,2.0)));
    return h;
}