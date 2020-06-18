#include<iostream>
#include<cstdio>
#include<cmath>
#include<Automobile.h>

using namespace std;

double PI=acos(-1.0);
double pp = PI/180.0;

int low[6]= {0,0,10,25,35,50};
int high[6]= {0,20,30,45,60,150};

class Car: public Automobile
{
    int speedm, maxspeed, maxfuel, fuelff;
    double conv, angle;
    public:
    Car() : Automobile("Car")
    {
        angle=5;
        speedm=10;
        maxspeed=120;
        maxfuel=30;
        fuelff=8;
        fuel=maxfuel;
        speed=0;
        conv=3600*fuelff;
    }
    void TurnLeft()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(angle*pp) - y*sin(angle*pp);
        directionY=x*sin(angle*pp) + y*cos(angle*pp);
    }
    void TurnRight()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(-angle*pp) - y*sin(-angle*pp);
        directionY=x*sin(-angle*pp) + y*cos(-angle*pp);
    }
    void IncreaseSpeed()
    {
        if(speed+speedm<=maxspeed)
        speed+= speedm;
    }
     void DecreaseSpeed()
    {
        if(speed>=speedm)
        speed-= speedm;
    }
    void Move()
    {
        if(fuel>=speed/conv)
        {
            double tmp=(directionX*directionX+directionY*directionY);
            double ux, uy;
            ux=directionX/tmp;
            uy=directionY/tmp;
            posX+=(speed*1.0/3600*ux);
            posY+=(speed*1.0/3600*uy);

            fuel-=speed*1.0/conv;
        }
    }
};

class Bus: public AutomobileWithManualXmission
{
    int  speedm, maxspeed, maxfuel, fuelff;
    double conv, angle;
    public:
    Bus() : AutomobileWithManualXmission("Bus")
    {
        angle=15;
        speedm=6;
        maxspeed=80;
        maxfuel=80;
        fuelff=4;
        fuel=maxfuel;
        speed=0;
        conv=3600*fuelff;
    }
    void TurnLeft() //fin
    {
        double x=directionX, y=directionY;
        directionX=x*cos(angle*pp) - y*sin(angle*pp);
        directionY=x*sin(angle*pp) + y*cos(angle*pp);
    }
    void TurnRight()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(-angle*pp) - y*sin(-angle*pp);
        directionY=x*sin(-angle*pp) + y*cos(-angle*pp);
    }
    void IncreaseSpeed()
    {
        if(gearPosition!=5)
        {
            if(high[gearPosition]>=speed+speedm)
                speed+=speedm;
        }
        else
        {
            if(speed+speedm<=maxspeed) speed+=speedm;
        }

    }
     void DecreaseSpeed()
    {
        if(gearPosition!=0)
        {
            if(low[gearPosition]<=speed-speedm)
                speed-=speedm;
        }
        else
        {
            if(speed>=speedm) speed-=speedm;
        }
    }
    void Move()
    {
        if(fuel>=speed/conv)
        {
            double tmp=(directionX*directionX+directionY*directionY);
            double ux, uy;
            ux=directionX/tmp;
            uy=directionY/tmp;
            posX+=(speed*1.0/3600*ux);
            posY+=(speed*1.0/3600*uy);

            fuel-=speed/conv;
        }
    }
};

class SUV: public Automobile
{
    int  speedm, maxspeed, maxfuel, fuelff;
    double conv, angle;
    public:
    SUV() : Automobile("SUV")
    {
        angle=10;
        speedm=8;
        maxspeed=100;
        maxfuel=50;
        fuelff=6;
        fuel=maxfuel;
        speed=0;
        conv=3600*fuelff;
    }
    void TurnLeft()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(angle*pp) - y*sin(angle*pp);
        directionY=x*sin(angle*pp) + y*cos(angle*pp);
    }
    void TurnRight()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(-angle*pp) - y*sin(-angle*pp);
        directionY=x*sin(-angle*pp) + y*cos(-angle*pp);
    }
    void IncreaseSpeed()
    {

        if(speed+speedm<=maxspeed)
        speed+= speedm;
    }
     void DecreaseSpeed()
    {
        if(speed>=speedm)
        speed-= speedm;
    }
    void Move()
    {
        if(fuel>=speed/conv)
        {
            double tmp=(directionX*directionX+directionY*directionY);
            double ux, uy;
            ux=directionX/tmp;
            uy=directionY/tmp;
            posX+=(speed*1.0/3600*ux);
            posY+=(speed*1.0/3600*uy);

            fuel-=speed*1.0/conv;
        }
    }
};

class Truck: public AutomobileWithManualXmission
{
    int  speedm, maxspeed, maxfuel, fuelff;
    double conv, angle;
    public:
    Truck() : AutomobileWithManualXmission("Truck")
    {
        angle=25;
        speedm=4;
        maxspeed=60;
        maxfuel=100;
        fuelff=2;
        fuel=maxfuel;
        speed=0;
        conv=3600*fuelff;
    }
    void TurnLeft() //fin
    {
        double x=directionX, y=directionY;
        directionX=x*cos(angle*pp) - y*sin(angle*pp);
        directionY=x*sin(angle*pp) + y*cos(angle*pp);
    }
    void TurnRight()
    {
        double x=directionX, y=directionY;
        directionX=x*cos(-angle*pp) - y*sin(-angle*pp);
        directionY=x*sin(-angle*pp) + y*cos(-angle*pp);
    }
    void IncreaseSpeed()
    {
        if(gearPosition!=5)
        {
            if(high[gearPosition]>=speed+speedm)
                speed+=speedm;
        }
        else
        {
            if(speed+speedm<=maxspeed) speed+=speedm;
        }
    }
     void DecreaseSpeed()
    {
        if(gearPosition!=0)
        {
            if(low[gearPosition]<=speed-speedm)
                speed-=speedm;
        }
        else
        {
            if(speed>=speedm) speed-=speedm;
        }
    }
    void Move()
    {
        if(fuel>=speed/conv)
        {
            double tmp=(directionX*directionX+directionY*directionY);
            double ux, uy;
            ux=directionX/tmp;
            uy=directionY/tmp;
            posX+=(speed*1.0/3600*ux);
            posY+=(speed*1.0/3600*uy);

            fuel-=(speed*1.0/conv);
        }
    }
};


int main()
{
    //*/
    return 0;
}


