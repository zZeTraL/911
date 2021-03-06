/*-----------------------------------------------------------------------
 *                              APP MANAGER
 *
 * Ce fichier contient toutes les fonctions de l'APP (tour de contrôle)
 * des avions en approche de l'aéroport.
 *
 *-----------------------------------------------------------------------*/


mutex APPMutex;
mutex TWRMutex;
default_random_engine generator;

void landingPhase(vector<Airport*> &vecAirport, vector<Plane*> &vecPlane, bool &boolean){
    while (!boolean) {
        this_thread::sleep_for(1s);
        if (!vecPlane.empty() && vecAirport.at(1)->getAvailableSlot() != 0) {
            if(APPMutex.try_lock()) {
                // On lock l'exécution du thread qui gère le décollage (car une piste disponible)
                Plane *planeToLand = findPlaneToLand(vecAirport, vecPlane);

                if (planeToLand != nullptr && planeToLand->isWaiting()) {

                    planeToLand->setWaitingToLand(false);
                    planeToLand->setLandingPhase(true);
                    // stepHeight = HEIGHT * STEPRADIUS / RADIUS;
                    float stepRadius = 3.f; //0.025f;
                    float stepHeight = planeToLand->getHeight() * stepRadius / planeToLand->traj.getRadius();
                    while (planeToLand->traj.getRadius() != 0) {
                        /** TODO
                         *   - Le temps pour update le radius à redéfinir           (optionnel)
                         *   - Réduire la vitesse de l'avion lors de la descente    (optionnel)
                         */
                        // DEBUG
                        //cout << "Plane height is: " << planeToLand->getHeight() << endl;
                        if (planeToLand->traj.getRadius() - stepRadius < 0) {
                            planeToLand->traj.setRadius(0);
                            planeToLand->setHeight(0);
                            break;
                        }
                        planeToLand->update(stepRadius, 0, stepHeight);
                        Sleep(DWORD(1000));
                    }

                    // On supprime l'avion et on reset les booleans
                    vecPlane.erase(std::remove(vecPlane.begin(), vecPlane.end(), planeToLand), vecPlane.end());
                    planeToLand->setLandingPhase(false);

                    // On ajoute l'avion atterri dans le parking de l'aéroport
                    for (auto &airport: vecAirport) {
                        if (airport->getID() == "LIL") {
                            airport->addPlaneLanded(planeToLand);
                            break;
                        }
                    }

                    // On n'oublie pas de déverrouiller le mutex
                    APPMutex.unlock();
                } else {
                    cout << "Airport is full\n";
                }

                /*
                 * RADIUS -> STEPRADIUS
                 * HEIGHT -> DESCENDING PHASE
                 * stepHeight = HEIGHT*STEPRADIUS/RADIUS;
                 */
                /*float stepRadius = 3.f; //0.025f;
                float stepHeight = planeToLand->getHeight()*stepRadius/planeToLand->traj.getRadius();
                while (planeToLand->traj.getRadius() != 0) {
                    // DEBUG
                    //cout << "Plane height is: " << planeToLand->getHeight() << endl;
                    /** TODO
                     *   - Le temps pour update le radius à redéfinir
                     *   - Bien évidement, il faut réduire la height du plane lors de l'atterissage synchronisé avec le radius
                     *   - La réduction du radius va se faire en fonction de la vitesse / height du plane
                     *   - ...
                     */

                /*     Sleep(DWORD(1000));
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
                 for(auto &airport: vecAirport){
                     if(airport->getID() == "LIL"){
                         airport->addPlaneLanded(planeToLand);
                     }
                 }


                 // DEBUG
                 //cout << "The smallest radius detected is: " << smallestRadius << endl;
                 //cout << planeToLand->traj;
                 //cout << "Plane height is: " << planeToLand->getHeight() << endl;

                 // On n'oublie pas d'unlock le thread sinon problème
                 APPMutex.unlock();
                 TWRMutex.unlock();
             }*/
            } else {
                this_thread::sleep_for(1s);
                cout << "thread is locked\n";
            }
        } else {
            // DEBUG
            cout << "APP THREAD IS WAITING FOR DATA...\n";
            this_thread::sleep_for(1s);
        }
    }
}