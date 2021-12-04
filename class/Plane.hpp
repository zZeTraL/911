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
    mutex lock;
    string ID;
    float speed;
    float height;
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
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    while(!boolean){
        /*
         * 280 kts = 0.5s
         * 180 kts = 1.5s
         */

        /** TODO
         *   - L'update des coordonnées va varier en fonction des différents paramètres
         */

        default_random_engine generator(seed);
        uniform_int_distribution gen(1,5);

        //int time = (((plane.getSpeed()*1.5)/280));

        this_thread::sleep_for(std::chrono::seconds(gen(generator)));
        plane.update(0, 0.5, 0);
    }
}

ostream& operator<<(ostream& os, const Plane &plane){
    os << plane.ID << "\nSpeed (kts): " << plane.speed << "\nHeight (ft): " << plane.height << "\n" << plane.traj;
    return os;
}