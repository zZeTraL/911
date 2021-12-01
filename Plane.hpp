#include <iostream>
#include <thread>
#include <SFML/Graphics/RenderWindow.hpp>
using namespace std;

class Plane;
void updatePlaneCoordinates(Plane &plane, bool &boolean);

class Plane{
    float speed;
    bool stopThread = false;
    thread planeThread;
public:
    Trajectory traj;
    Plane(){
        planeThread = thread(updatePlaneCoordinates, ref(*this), ref(stopThread));
    }

    float getSpeed() const {return speed;}

    void update(){
        this->traj.setXY(traj.getRadius() - 0.025, static_cast<float>(traj.getAngle() + (0.075)));
    }

    ~Plane(){
        stopThread = true;
        planeThread.join();
    }
};

void updatePlaneCoordinates(Plane &plane, bool &boolean){
    while(!boolean){
        /*
         * 280 kts = 0.5s
         * 180 kts = 1.5s
         */
        default_random_engine generator(time(nullptr));
        uniform_int_distribution gen(1,5);
        int time = (((plane.getSpeed()*1.5)/280));
        this_thread::sleep_for(std::chrono::seconds(gen(generator)));
        plane.update();
    }
}

void updateGraphic(Plane &plane, sf::RenderWindow win){

}