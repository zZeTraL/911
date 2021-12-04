#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

void circleColorGradient(Plane* &plane, sf::CircleShape &circle){
    /*
     * 34 000 -> 100%
     * Height --> X%
     *
     * 255 -> 100%
     *  0  -> 0%
     *
     *  rgb(192, 57, 43) ROUGE
     *  rgb(39, 174, 96) VERT
     */
    float percentage = ((plane->getHeight())/(34000));
    circle.setFillColor(sf::Color(39, 174, 96, 255));
}