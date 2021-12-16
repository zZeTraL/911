#include <iostream>
#include <random>
#include <iomanip>
#include <queue>

#include <windows.h>


/**
 * Generate an amount of plane
 *
 * @param n : Number of plane you want to generate
 * @return Vector of all planes generated
 */
vector<Plane *> generatePlane(int n) {
    vector<Plane *> vectorPlane;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> randomString(100, 800);
    uniform_real_distribution<float> randomSpeed(180, 280);
    uniform_real_distribution<float> randomHeight(25000, 34000);
    uniform_real_distribution<float> randomRadius(5.39, 26.99);
    uniform_real_distribution<float> randomAngle(0, 2 * M_PI);

    for (int i = 0; i < n; ++i) {
        //Sleep(static_cast<DWORD>(1000));
        auto *plane = new Plane;

        plane->traj.setXY(randomRadius(generator), randomAngle(generator));
        plane->setID("AF" + to_string(randomString(generator)));
        plane->setSpeed(randomSpeed(generator));
        plane->setHeight(randomHeight(generator));
        plane->setLandingPhase(false);
        plane->setWaitingToLand(true);

        vectorPlane.push_back(plane);
        cout << setprecision(5) << *plane << endl;
    }
    return vectorPlane;
}

/**
 * Permet de générer un seul avion
 * @return
 */
void *generateNewFlightPlan(Plane* &plane) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_real_distribution<float> randomSpeed(180, 280);
    uniform_real_distribution<float> randomHeight(25000, 34000);
    uniform_real_distribution<float> randomRadius(5.39, 26.99);
    uniform_real_distribution<float> randomAngle(0, 2 * M_PI);

    plane->traj.setXY(randomRadius(generator), randomAngle(generator));
    plane->setSpeed(randomSpeed(generator));
    plane->setHeight(randomHeight(generator));

    cout << setprecision(5) << *plane << endl;
}

/**
 * Permet de trouver un avion qui est capable d'atterrir
 *
 * @param vecPlane vector contenant tous les avions en orbites autour d'un aéroport
 * @return Retourne un avion qui est le plus apte à atterrir
 */
Plane *findPlaneToLand(vector<Airport *> &vecAirport, vector<Plane *> &vecPlane) {
    // Si des avions sont actifs (i.e dans le ciel)
    if (!vecPlane.empty()) {
        Plane *planeToLand = nullptr;

        // On cherche l'avion avec un radius très petit
        float smallestRadius = 100.f;
        for (auto &plane: vecPlane) {
            if(plane->isWaiting()){
                float currentlyPlaneRadius = plane->traj.getRadius();
                if (currentlyPlaneRadius < smallestRadius) {
                    smallestRadius = currentlyPlaneRadius;
                    planeToLand = plane;
                }
            }
        }

        /** TODO
         *   - Faire la différence entre les aéroports
         *   - Sélectionner l'aéroport le plus adéquat pour l'atterrissage (On check les normes des vecteurs etc...)
         *   - Peut être réutiliser afin d'orienter l'avion vers un aéroport (SI ON GERE PLUSIEURS AÉROPORTS)
         */
        //On check l'aéroport le plus proche de l'avion
        /*Airport* airportDestination;
        for (int i = 0; i < vecAirport.size(); ++i) {
            Airport* currentlyTargetedAirport = vecAirport.at(i);
            for(auto &airport: vecAirport){
                if(vectorNorm(currentlyTargetedAirport, planeToLand) > vectorNorm(airport, planeToLand)){
                    cout << "HERE\n\n";
                    airportDestination = currentlyTargetedAirport;
                }
            }
        }
        cout << *airportDestination;*/

        // On check si l'aéroport à des places de parking disponible
        if (vecAirport.at(1)->getAvailableSlot() != 0 && planeToLand != nullptr) {
            cout << "\nID of plane with the smallest radius: " << planeToLand->getID() << "\n";
            return planeToLand;
        } else {
            return planeToLand;
        }

    }

}
