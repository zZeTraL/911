/*-----------------------------------------------------------------------
 *                              TWR MANAGER
 *
 * Ce fichier contient toutes les fonctions du TWR (tour de contrôle au sol)
 * des avions qui vont décoller.
 *
 *-----------------------------------------------------------------------*/

void takeOffPhase(vector<Airport*> &vecAirport, vector<Plane*> &vecPlane, bool &boolean){
    while(!boolean){
        this_thread::sleep_for(1s);
        if(vecAirport.at(1)->getAvailableSlot() == 0){
            if(APPMutex.try_lock()){

                auto* newPlane = vecAirport.at(1)->getQueuePlaneParked().back();
                vecAirport.at(1)->getQueuePlaneParked().pop();

                newPlane->setSpeed(180);
                newPlane->traj.setXY(20, 3.14);
                newPlane->setHeight(25000);
                vecPlane.push_back(newPlane);

                // DEBUG
                //cout << *vecAirport.at(1)->getQueuePlaneParked().back();
                vecAirport.at(1)->addSlot(1);

                this_thread::sleep_for(5s);
                APPMutex.unlock();
                TWRMutex.unlock();
            } else {
                cout << "A plane is landing\n";
            }
        }
    }
}


/*TWRMutex.lock();
           if(vecAirport.at(1)->getAvailableSlot() != 0){

               cout << "A plane is taking off the airport" << endl;

               /** TODO
                *   - Créer un nouveau plan de vol pour l'avion à sortir
                *   - Redéfinir sa speed, sa hauteur, son radius, son angle etc...
                */

/* auto* maxSpeedPlane = new Plane;
 maxSpeedPlane->setID("MAXSPEED");
 maxSpeedPlane->traj.setXY(40, 6.14);
 maxSpeedPlane->setSpeed(300);
 maxSpeedPlane->setHeight(5000);
 vecPlane.push_back(maxSpeedPlane);

 // DEBUG
 //cout << *vecAirport.at(1)->getQueuePlaneParked().back();
 vecAirport.at(1)->addSlot(1);

 this_thread::sleep_for(10s);
 APPMutex.unlock();
 TWRMutex.unlock();
} else {
 APPMutex.unlock();
 TWRMutex.unlock();
 this_thread::sleep_for(1s);
}*/