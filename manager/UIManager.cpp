#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

#define WIDTH 1500.f
#define HEIGHT 720.f

float zoom = 8.f;

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
    long red = mathMap<long>((long)plane->getHeight(), 0, 34000, 192, 39);
    long green = mathMap<long>((long)plane->getHeight(), 0, 34000, 57, 174);
    long blue = mathMap<long>((long)plane->getHeight(), 0, 34000, 43, 93);

    if (plane->getHeight() < 25000) {
        circle.setFillColor(sf::Color(red, green, blue, 255));
        // DEBUG
        //cout << red << " " << " " << green << " " << blue << endl;
    } else {
        circle.setFillColor(sf::Color(39, 174, 96, 255));
    }
}

/**
 * Permet de charger tous les textes (tooltip au niveau de chaque avion)
 *
 * @param app
 * @param vecPlane
 */
void loadPlaneTag(sf::RenderWindow &app, vector<Plane*> &vecPlane){
    sf::Font font;
    if(!font.loadFromFile("../fonts/WorkSans-Regular.ttf")){
        exit(0);
    }

    for(auto &plane : vecPlane){
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        text.setPosition((int)(WIDTH/2) + ((plane->traj.getRadius()*zoom)*cos(plane->traj.getAngle())), (int)(HEIGHT/2) + ((plane->traj.getRadius()*zoom)*sin(plane->traj.getAngle())));
        text.setString(plane->getID());
        app.draw(text);
    }
}

/**
 * Permet de charger un background prédéfini
 *
 * @param app
 */
void loadBackground(sf::RenderWindow &app){
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../img/background.jpg")){
        exit(0);
    }

    sf::Sprite background;
    background.setTexture(backgroundTexture);
    app.draw(background);
}

/**
 * Permet de charger tous les avions à afficher (debug = true affiche les trajectoire des avions)
 *
 * @param app
 * @param vecPlane
 * @param debug
 */
void loadComponent(sf::RenderWindow &app, vector<Plane*> &vecPlane, bool debug){
    for (auto &plane : vecPlane){
        float angle = plane->traj.getAngle();
        float radius = plane->traj.getRadius();

        // Création du cercle = la trajectoire de l'avion
        if(debug){
            sf::CircleShape circle(radius*zoom, 2000);
            circle.setFillColor(sf::Color::Transparent);
            circle.setOutlineThickness(1);
            circle.setOutlineColor(sf::Color(52, 152, 219, 125));
            circle.setOrigin(circle.getRadius()/2, circle.getRadius()/2);
            circle.setPosition((int)(WIDTH/2) - (radius*zoom)/2 , (int)(HEIGHT/2) - (radius*zoom/2) );
            app.draw(circle);
        }

        // Création du point représentant l'avion
        sf::CircleShape point(5, 500);
        circleColorGradient(plane, point);
        point.setOrigin(point.getRadius()/2, point.getRadius()/2);
        point.setPosition((float)((WIDTH/2) - 2.5) + ((radius*zoom)*cos(angle)), (float)((HEIGHT/2) - 2.5) + ((radius*zoom)*sin(angle)));

        app.draw(point);
    }
}

/**
 * Permet de charger et d'afficher chaque aéroport
 *
 * @param app
 * @param vecAirport
 */
void loadAirport(sf::RenderWindow &app, vector<Airport*> &vecAirport){
    for(auto airport : vecAirport){
        sf::CircleShape circle(25, 2000);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(2);
        circle.setOutlineColor(sf::Color(127, 140, 141, 255));
        circle.setOrigin(circle.getRadius()/2, circle.getRadius()/2);
        circle.setPosition(airport->getX() - (circle.getRadius()/2), airport->getY() - (circle.getRadius()/2));
        app.draw(circle);

        sf::Font font;
        if(!font.loadFromFile("../fonts/WorkSans-Regular.ttf")){
            exit(0);
        }
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);

        text.setPosition(airport->getX() + 20, airport->getY() + 15);
        text.setString(airport->getID());

        app.draw(circle);
        app.draw(text);
        }
}

void visualization(vector<Plane*> &vecPlane, vector<Airport*> &vecAirport){
    sf::RenderWindow app(sf::VideoMode(WIDTH, HEIGHT, 32), "Projet");
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
        loadAirport(app, vecAirport);
        loadPlaneTag(app, vecPlane);
        loadComponent(app, vecPlane, true);

        app.display();
    }
}