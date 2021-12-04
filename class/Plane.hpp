#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <utility>
using namespace std;

class Plane;
void updatePlaneCoordinates(Plane &plane, bool &boolean);

class Plane{
    string ID;
    float speed{};
    float height{};
    bool stopThread = false;
    thread planeThread;
public:
    Trajectory traj;
    Plane(){
        planeThread = thread(updatePlaneCoordinates, ref(*this), ref(stopThread));
    }

    // Methods
    void update(float stepRadius, float stepAngle, float stepHeight){
        this->height -= stepHeight;
        this->traj.setXY(static_cast<float>(traj.getRadius() - stepRadius), static_cast<float>(traj.getAngle() + stepAngle));
    }

    friend ostream& operator<<(ostream &os, const Plane &plane);

    // Getters
    float getSpeed() const {return speed;}
    float getHeight() const {return height;}
    string getID() const {return ID;}

    // Setters
    void setSpeed(float number){speed = number;}
    void setHeight(float number){height = number;}
    void setID(string str){ID = std::move(str);}


    // Destructor
    ~Plane(){
        stopThread = true;
        planeThread.join();
    }
};

void updatePlaneCoordinates(Plane &plane, bool &boolean){
    while(!boolean){
        /*
         * 180 kts = 1.25s
         * 280 kts = 0.25s
         * Function: f(x) = -1/100*X+3.05
         */

        /** TODO
         *   - L'update des coordonnées va dépendre de la vitesse et la
         */
        int timeToWait = static_cast<int>((-(plane.getSpeed()*0.01) + 3.05) * 1000);
        this_thread::sleep_for(std::chrono::milliseconds(timeToWait));
        plane.update(0, 0.01, 0);
    }
}

ostream& operator<<(ostream& os, const Plane &plane){
    os << plane.ID << "\nSpeed (kts): " << plane.speed << "\nHeight (ft): " << plane.height << "\n" << plane.traj;
    return os;
}