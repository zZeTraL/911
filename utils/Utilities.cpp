/*-----------------------------------------------------------------------
 *                            U T I L I T I E S
 *
 * Ce fichier contient toutes les fonctions utiles qui vont nous aider
 * dans notre projet.
 *
 *-----------------------------------------------------------------------*/

float vectorNorm(Airport* airport, Plane* plane){
    float x = (airport->getX() + plane->traj.getX()) - airport->getX();
    float y = (airport->getY() + plane->traj.getY()) - airport->getY();

    auto norm = (float)sqrt(pow(x, 2) + pow(y, 2));
    // DEBUG
    //cout << "\nX: " << x << "\nY: " << y << "\nNorm is: " << norm << "\n";
    return norm;
}


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
template<typename T>
T mathMap(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
