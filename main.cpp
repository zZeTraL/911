#include <iostream>
#include <random>
#include <iomanip>
#include "Trajectory.hpp"
#include "Plane.hpp"
#include <windows.h>

#include <SFML/Graphics.hpp>


vector<Plane*> generatePlane(int n){
    vector<Plane*> vectorPlane;
    default_random_engine generator(time(nullptr));

    uniform_real_distribution<float> randomSpeed(180, 280);
    uniform_real_distribution<float> randomRadius(1, 5);
    uniform_real_distribution<float> randomAngle(0, 2*M_PI);

    for (int i = 0; i < n; ++i) {
        Sleep(static_cast<DWORD>(1000));
        auto* plane = new Plane;
        plane->traj.setXY(randomRadius(generator), randomAngle(generator));
        vectorPlane.push_back(plane);
        cout << setprecision(3) << plane->traj << endl;
    }

    return vectorPlane;
}

void visualization(vector<Plane*> &planes){
    // Fenêtre
    sf::RenderWindow app(sf::VideoMode(1000, 1000, 32), "My Camera");

    // Frames Per Second (FPS)
    app.setFramerateLimit(30); // limite la fenêtre à 60 images par seconde



    while (app.isOpen()) // Boucle principale
    {
        sf::Event event;
        while (app.pollEvent(event)) // Boucle des évènements de la partie pause
        {
            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ||
                event.type == sf::Event::Closed)
            {
                app.close();
            }
        }

        // Affichages
        app.clear();

        for (auto &plane : planes){
            sf::CircleShape circle(plane->traj.getRadius()*100, 1000);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color::Blue);
            circle.setOrigin(circle.getRadius()/2, circle.getRadius()/2);
            circle.setPosition(500 - (plane->traj.getRadius()*100)/2 , 500 - (plane->traj.getRadius()*100/2) );

            sf::CircleShape point(5);
            point.setOutlineColor(sf::Color::Red);
            point.setOrigin(point.getRadius()/2, point.getRadius()/2);
            float angle = plane->traj.getAngle();
            point.setPosition(500 - 2.5 + ((plane->traj.getRadius()*100)*cos(angle)), 500 - 2.5 + ((plane->traj.getRadius()*100)*sin(angle)));

            app.draw(circle);
            app.draw(point);
        }

        app.display();
    }
}


int main() {
    vector<Plane*> vectorPlane;
    vectorPlane = generatePlane(10);

    thread UI(visualization, std::ref(vectorPlane));

    UI.join();

    return 0;
}
