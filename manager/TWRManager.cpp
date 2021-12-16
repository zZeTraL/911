/*-----------------------------------------------------------------------
 *                              TWR MANAGER
 *
 * Ce fichier contient toutes les fonctions du TWR (tour de contrôle au sol)
 * des avions qui vont décoller.
 *
 *-----------------------------------------------------------------------*/

void takeOffPhase(vector<Airport*> &vecAirport, vector<Plane*> &vecPlane, bool &boolean){
    while(!boolean){
        if(vecAirport.at(1)->getAvailableSlot() == 0 || (vecPlane.empty() && (vecAirport.at(1)->getAvailableSlot() != vecAirport.at(1)->getMaxSlot())) ){
            if(APPMutex.try_lock()){

                /**
                 *  TODO
                 *   - On check les aéroport qui sont full et on fait décoller un avion
                 */

                auto* newPlane = vecAirport.at(1)->getQueuePlaneParked().back();
                vecAirport.at(1)->planeParkedQueuePop();
                vecAirport.at(1)->addSlot(1);

                // DEBUG
                //cout << *vecAirport.at(1)->getQueuePlaneParked().back();

                generateNewFlightPlan(newPlane);

                // Sauvegarde de mes valeurs
                float savedRadius = newPlane->traj.getRadius() + 50;
                float savedSpeed = newPlane->getSpeed();
                float saveHeight = newPlane->getHeight();
                cout << savedRadius << " " << savedSpeed << endl;

                newPlane->traj.setRadius(0);

                // On ajoute l'avion à notre vector de plane
                newPlane->setLandingPhase(false);
                newPlane->setWaitingToLand(false);
                vecPlane.push_back(newPlane);

                float currentSpeed = 0.f;
                float currentHeight = 20.f;

                float stepSpeed = 10.f;
                while(currentSpeed != savedSpeed){
                    if(currentSpeed >+ savedSpeed){
                        newPlane->setHeight(saveHeight);
                        newPlane->setSpeed(savedSpeed);
                        break;
                    }
                    currentSpeed += stepSpeed;
                    currentHeight = saveHeight/savedSpeed * currentSpeed;
                    newPlane->setSpeed(currentSpeed);
                    newPlane->setHeight(currentHeight);
                    cout << currentSpeed << " / " << currentHeight << "\n";
                    Sleep(DWORD(500));
                }

                /*float currentRadius = 1.f;
                float currentSpeed = 0.f;
                float currentHeight = 20.f;

                float stepSpeed = 10.f;
                while(currentRadius != savedRadius){
                    if(currentRadius >+ savedRadius){
                        newPlane->traj.setRadius(savedRadius);
                        newPlane->setHeight(saveHeight);
                        newPlane->setSpeed(savedSpeed);
                        break;
                    }
                    cout << currentSpeed << " / " << currentRadius << "\n";
                    currentSpeed += stepSpeed;
                    currentRadius = savedRadius/savedSpeed * currentSpeed;
                    currentHeight = saveHeight/savedSpeed * currentSpeed;

                    newPlane->traj.setRadius(currentRadius);
                    newPlane->setSpeed(currentSpeed);
                    newPlane->setHeight(currentHeight);
                    Sleep(DWORD(500));
                }*/

                // On supprime l'avion
                //vecPlane.erase(std::remove(vecPlane.begin(), vecPlane.end(), newPlane), vecPlane.end());

                this_thread::sleep_for(5s);
                APPMutex.unlock();
            } else {
                this_thread::sleep_for(1s);
                cout << "A plane is landing\n";
            }
        }
    }
}
