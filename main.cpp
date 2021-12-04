#include <iostream>
#include <random>
#include <queue>

#include "class/Trajectory.hpp"
#include "class/Plane.hpp"

// Utilities
#include "utils/utilities.cpp"

// Manager
#include "manager/UIManager.cpp"
#include "manager/PlaneManager.cpp"

#include <windows.h>

mutex APPmutex;
void landingPhase(queue<Plane*> &queuePlaneLanded ,vector<Plane*> &vecPlane, bool &boolean){
    while (!boolean) {
        if (!vecPlane.empty()) {
            // On lock le thread car un plane à la fois atteri
            APPmutex.lock();
            Plane *planeToLand;
            float smallestRadius = 100.f;
            for (auto &plane: vecPlane) {
                float currentlyPlaneRadius = plane->traj.getRadius();
                if (currentlyPlaneRadius < smallestRadius) {
                    smallestRadius = currentlyPlaneRadius;
                    planeToLand = plane;
                }
            }

            /*
             * RADIUS -> STEPRADIUS
             * HEIGHT -> DESCENDING PHASE
             * stepHeight = HEIGHT*STEPRADIUS/RADIUS;
             */
            float stepRadius = 0.1f; //0.025f;
            float stepHeight = planeToLand->getHeight()*stepRadius/planeToLand->traj.getRadius();
            while (planeToLand->traj.getRadius() != 0) {
                //cout << "Plane height is: " << planeToLand->getHeight() << endl;
                /** TODO
                 *   - Le temps pour update le radius à redéfinir
                 *   - Bien évidement, il faut réduire la height du plane lors de l'atterissage synchronisé avec le radius
                 *   - La réduction du radius va se faire en fonction de la vitesse / height du plane
                 *   - ...
                 */

                Sleep(DWORD(1000));
                if (planeToLand->traj.getRadius() - stepRadius < 0) {
                    planeToLand->traj.setRadius(0);
                    planeToLand->setHeight(0);
                    break;
                }
                planeToLand->update(stepRadius, 0, stepHeight);
            }

            // On supprimer l'avion
            vecPlane.erase(std::remove(vecPlane.begin(), vecPlane.end(), planeToLand), vecPlane.end());

            // On ajoute l'avion atteri dans la landingQueue
            queuePlaneLanded.push(planeToLand);

            // DEBUG
            //cout << "The smallest radius detected is: " << smallestRadius << endl;
            //cout << planeToLand->traj;
            //cout << "Plane height is: " << planeToLand->getHeight() << endl;

            // On n'oublie pas de unlock le thread sinon problème
            APPmutex.unlock();
        } else {
            // DEBUG
            cout << "APP THREAD IS WAITING FOR DATA (10s)\n";
            this_thread::sleep_for(10s);
            cout << "APP THREAD CAN NOW RECEIVING DATA\n";
        }
    }
}

int main() {
    queue<Plane*> queuePlaneLanded;
    vector<Plane*> vectorPlane;
    bool stopThread = false;

    Plane* maxSpeedPlane = new Plane;
    maxSpeedPlane->setID("MAXSPEED");
    maxSpeedPlane->traj.setXY(9, 3.14);
    maxSpeedPlane->setSpeed(300);
    maxSpeedPlane->setHeight(30000);
    vectorPlane.push_back(maxSpeedPlane);

    vectorPlane = generatePlane(10);
    vectorPlane.push_back(maxSpeedPlane);

    thread UI(visualization, ref(vectorPlane));
    thread APP(landingPhase, ref(queuePlaneLanded), ref(vectorPlane), ref(stopThread));

    while (true){
        string a;
        cin >> a;
        if(a == "0"){
            cout << "Size of queuePlaneLanded is: " << queuePlaneLanded.size() << "\n";
            /*while(!queuePlaneLanded.empty()){
                cout << " " << &queuePlaneLanded.front();
                queuePlaneLanded.pop();
            }*/
            cout << "Size of vectorPlane is: " << vectorPlane.size() << "\n";
            exit(0);
        }
    }

    UI.join();
    APP.join();

    return EXIT_SUCCESS;
}
