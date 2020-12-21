#include <iostream>

using namespace std;

double dydx (double x, double y){
    return y - x + 1;
}

void RungeKutta4(double y, double lpoint, double upoint, double h){

    double x = lpoint;

    while (x < upoint){
        double k[4];
        k[0] = dydx(x, y);
        k[1] = dydx(x + (h/2), y + (h/2) * k[0]);
        k[2] = dydx(x + (h/2), y + (h/2) * k[1]);
        k[3] = dydx(x + h, y + h * k[2]);

        cout << "k0=" << k[0] << endl;
        cout << "k1=" << k[1] << endl;
        cout << "k2=" << k[2] << endl;
        cout << "k3=" << k[3] << endl;

        y += h * ((k[0] + 2*k[1] + 2*k[2] + k[3])/6);
        x += h;

        cout << "V(" << x << ") = " << y << endl << endl;
    }
}

int main(){

    RungeKutta4(0.5, 0, 1, 0.2);
    return 0;
}
