#include <iostream>
#include <windows.h>
#include <SFML/Graphics.hpp>

/**
 * Re-maps a number from one range to another.
 *
 * @param x
 * @param in_min
 * @param in_max
 * @param out_min
 * @param out_max
 * @return number
 */
long mathMap(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


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