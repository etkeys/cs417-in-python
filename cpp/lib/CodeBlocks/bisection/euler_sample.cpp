#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int main()
{
/// dy/dx = y
/// y(0)=1
/// y(x) = e^x

double e = 2.71828182846;
double dx;
cout<<"what is the value of dx? "; cin>>dx;
cout<<"we are finding "<<1.0/dx<<" solutions between 0 and 3"<<endl;
double x = 0.0;
double y = 1.0;
double dydx;
double acty;
acty=pow(e,x);
dydx=dx*y;
cout<<"X      Y    dy/dx       acty"<<endl;
cout<<setprecision(4)<<setw(7)<<left<<x<<setw(7)<<y<<setw(7)<<dydx<<setw(7)<<acty<<endl;
while(x<=3.0)
{ x = x+dx;
  y = y+(dx*dydx);
  dydx = y;
  acty = pow(e,x);
}
  cout<<setprecision(4)<<setw(7)<<left<<x<<setw(7)<<y<<setw(7)<<dydx<<setw(14)<<acty<<endl;

return 0;}