// kleines cpp programm um pi zu approximieren
#include <iostream>
#include <cstdlib>
#include <cmath>

int main(){
    int circlecounter(0), squarecount(0);
    for(int i = 0; i < 1000; i++){
        double x(rand()/RAND_MAX), y(rand()/RAND_MAX);
        if( (x*x + y*y) <= 1){ // is point in circle?
            circlecounter++;
        }
        squarecount++;
    }
    // area circle a=pi*r^2 -> only first quardant a->A/4 --> relative to square area
    double MyPi(4*double(circlecounter)/double(squarecount));
    std::cout << "Monte-Carlo-Pi = "<<MyPi<<"\n cmath-Pi = "<<M_PI<<"\n  relative error = "<< (MyPi-M_PI)/M_PI;

    return 0;
}