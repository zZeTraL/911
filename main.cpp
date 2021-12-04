#include <iostream>
#include <random>
#include <queue>

#include "class/Trajectory.hpp"
#include "class/Plane.hpp"

#include "manager/UIManager.cpp"
#include "manager/PlaneManager.cpp"

#include <windows.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

mutex APPmutex;

void visualization(vector<Plane*> &planes){
    // Fenêtre
    sf::RenderWindow app(sf::VideoMode(1000, 1000, 32), "Projet");
    // Frames Per Second (FPS)
    app.setFramerateLimit(30); // limite la fenêtre à 60 images par seconde

    // Boucle principale
    while (app.isOpen()){
        sf::Event event;
        // Boucle des évènements de la partie pause
        while (app.pollEvent(event)){
            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed){app.close();}
        }

        app.clear();

        sf::Font font;
        if(!font.loadFromFile("../fonts/WorkSans-Regular.ttf")){
            exit(0);
        }

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        for (auto &plane : planes){

            /** TODO
             *   - Point de couleur vert = HEIGHT MAX
             *   - Point de couleur rouge = HEIGHT MIN
             */

            // Création du cercle = la trajectoire de l'avion
            sf::CircleShape circle(plane->traj.getRadius()*50, 2000);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color(52, 152, 219, 125));
            circle.setOrigin(circle.getRadius()/2, circle.getRadius()/2);
            circle.setPosition(500 - (plane->traj.getRadius()*50)/2 , 500 - (plane->traj.getRadius()*50/2) );

            // Création du point représentant l'avion
            sf::CircleShape point(5, 500);

            // STYLE
            //point.setFillColor(sf::Color(39, 174, 96, 255));
            circleColorGradient(plane, point);

            point.setOrigin(point.getRadius()/2, point.getRadius()/2);

            float angle = plane->traj.getAngle();
            point.setPosition(500 - 2.5 + ((plane->traj.getRadius()*50)*cos(angle)), 500 - 2.5 + ((plane->traj.getRadius()*50)*sin(angle)));
            text.setPosition(500 + ((plane->traj.getRadius()*50)*cos(angle)), 500 + ((plane->traj.getRadius()*50)*sin(angle)));
            text.setString(plane->getID());

            app.draw(circle);
            app.draw(point);
            app.draw(text);
        }
        app.display();
    }
}

void landingPhase(queue<Plane*> &queuePlaneLanded ,vector<Plane*> &vecPlane, bool &boolean){
    while (!boolean) {
        if (!vecPlane.empty()) {
            // On lock le thread car un plane à la fois atteri
            //APPmutex.lock();
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
             *
             * stepHeight = HEIGHT*STEPRADIUS/RADIUS;
             *
             */
            float stepRadius = 0.1f; //0.025f;
            float stepHeight = planeToLand->getHeight()*stepRadius/planeToLand->traj.getRadius();
            while (planeToLand->traj.getRadius() != 0) {
                cout << "Plane height is: " << planeToLand->getHeight() << endl;
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
            //APPmutex.unlock();
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
    // DEBUG
    //cout << vectorPlane.size();

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
