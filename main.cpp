#include <iostream>
#include <random>
#include <queue>

// CLass
#include "class/Trajectory.hpp"
#include "class/Plane.hpp"
#include "class/Airport.hpp"

// Utilities
#include "utils/Utilities.cpp"

// Manager
#include "manager/UIManager.cpp"
#include "manager/PlaneManager.cpp"
#include "manager/APPManager.cpp"
#include "manager/TWRManager.cpp"

int main() {
    queue<Plane*> queuePlaneLanded;
    vector<Plane*> vecPlane;
    vector<Airport*> vecAirport;
    bool stopThread = false;

    //============================================================================================================================================

    Plane* maxSpeedPlane = new Plane;
    maxSpeedPlane->setID("MAXSPEED");
    maxSpeedPlane->traj.setXY(40, 6.14);
    maxSpeedPlane->setSpeed(300);
    maxSpeedPlane->setHeight(5000);
    vecPlane.push_back(maxSpeedPlane);

    vecPlane = generatePlane(2);
    vecPlane.push_back(maxSpeedPlane);

    Airport* BRU = new Airport("BRU", 999, 1250, 200);
    Airport* LIL = new Airport("LIL", 2, WIDTH/2, HEIGHT/2);
    vecAirport.push_back(BRU);
    vecAirport.push_back(LIL);

    //============================================================================================================================================

    thread UI(visualization, ref(vecPlane), ref(vecAirport));
    thread APP(landingPhase, ref(vecAirport), ref(vecPlane), ref(stopThread));
    thread TWR(takeOffPhase, ref(vecAirport), ref(vecPlane), ref(stopThread));

    //============================================================================================================================================

    while (true){
        string a;
        cin >> a;
        if(a == "0"){
            cout << "Size of queuePlaneLanded is: " << queuePlaneLanded.size() << "\n\n";
            for (auto airport: vecAirport) {
                cout << *airport;
            }
            cout << "Size of vectorPlane is: " << vecPlane.size() << "\n\n";
            exit(0);
        }
    }

    UI.join();
    APP.join();
    TWR.join();

    return EXIT_SUCCESS;
}
