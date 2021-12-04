#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

/**
 * Effet fade qui change la couleur du point en fonction de la hauteur
 *
 * @param plane
 * @param circle
 */
void circleColorGradient(Plane* &plane, sf::CircleShape &circle) {
    /*
     *  rgb(192, 57, 43) ROUGE
     *  rgb(39, 174, 96) VERT
     *  On a:
     *      RED[39, 192]
     *      GREEN[57, 174]
     *      BLUE[43, 93]
     */
    long red = mathMap(plane->getHeight(), 0, 34000, 192, 39);
    long green = mathMap(plane->getHeight(), 0, 34000, 57, 174);
    long blue = mathMap(plane->getHeight(), 0, 34000, 43, 93);

    if (plane->getHeight() < 25000) {
        circle.setFillColor(sf::Color(red, green, blue, 255));
        // DEBUG
        //cout << red << " " << " " << green << " " << blue << endl;
    } else {
        circle.setFillColor(sf::Color(39, 174, 96, 255));
    }
}
void loadText(sf::RenderWindow &app, vector<Plane*> &vecPlane){
    sf::Font font;
    if(!font.loadFromFile("../fonts/WorkSans-Regular.ttf")){
        exit(0);
    }

    for(auto &plane : vecPlane){
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        text.setPosition(500 + ((plane->traj.getRadius()*50)*cos(plane->traj.getAngle())), 500 + ((plane->traj.getRadius()*50)*sin(plane->traj.getAngle())));
        text.setString(plane->getID());
        app.draw(text);
    }

}
void loadBackground(sf::RenderWindow &app){
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../background.jpg")){
        exit(0);
    }

    sf::Sprite background;
    background.setTexture(backgroundTexture);
    app.draw(background);
}
void loadComponent(sf::RenderWindow &app, vector<Plane*> &vecPlane){
    for (auto &plane : vecPlane){
        float angle = plane->traj.getAngle();
        float radius = plane->traj.getRadius();

        // Création du cercle = la trajectoire de l'avion
        sf::CircleShape circle(radius*50, 2000);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(1);
        circle.setOutlineColor(sf::Color(52, 152, 219, 125));
        circle.setOrigin(circle.getRadius()/2, circle.getRadius()/2);
        circle.setPosition(500 - (radius*50)/2 , 500 - (radius*50/2) );

        // Création du point représentant l'avion
        sf::CircleShape point(5, 500);
        circleColorGradient(plane, point);
        point.setOrigin(point.getRadius()/2, point.getRadius()/2);
        point.setPosition(500 - 2.5 + ((radius*50)*cos(angle)), 500 - 2.5 + ((radius*50)*sin(angle)));

        app.draw(circle);
        app.draw(point);
    }
}

void visualization(vector<Plane*> &planes){
    sf::RenderWindow app(sf::VideoMode(1000, 1000, 32), "Projet");
    app.setFramerateLimit(60);

    // Tant que ma fenêtre est open
    while (app.isOpen()){
        sf::Event event;
        // Boucle des évènements de la partie pause
        while (app.pollEvent(event)){
            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || event.type == sf::Event::Closed){app.close();}
        }

        app.clear();

        //loadBackground(app);
        loadText(app, planes);
        loadComponent(app, planes);

        app.display();
    }
}