#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Trajectory{
private:
    float x = 0, y = 0, radius = 0, angle = 0;
public:
    Trajectory()= default;

    Trajectory(float r, float a){
        radius = r;
        angle = a;
    }


    // Getters
    float getRadius() const{return radius;}
    float getAngle() const{return angle;}
    float getX() const{return x;}
    float getY() const{return y;}


    // Setters
    void setRadius(float number){radius = number;}

    void setXY(float r, float a){
        angle = a;
        radius = r;
        x = r * cos(a);
        y = r * sin(a);
    }

    // Methods
    friend ostream& operator<<(ostream &os, const Trajectory &trajectory);
};

ostream& operator<<(ostream& os, const Trajectory &trajectory){
    os << "Trajectory (" << trajectory.radius << ", " << trajectory.angle << ")\n" << "X: " << trajectory.x << "\n" << "Y: " << trajectory.y << "\n";
    return os;
}