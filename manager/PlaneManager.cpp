#include <iostream>
#include <random>
#include <iomanip>
#include <queue>

#include <windows.h>

/**
 * Generate an amount of plane
 *
 * @param n : Number of plane you want to generate
 * @return Vector of all planes generated
 */
vector<Plane*> generatePlane(int n){
    vector<Plane*> vectorPlane;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> randomString(100, 800);
    uniform_real_distribution<float> randomSpeed(180, 280);
    uniform_real_distribution<float> randomHeight(25000, 34000);
    uniform_real_distribution<float> randomRadius(1, 1.5);
    uniform_real_distribution<float> randomAngle(0, 2*M_PI);

    for (int i = 0; i < n; ++i) {
        Sleep(static_cast<DWORD>(1000));
        auto* plane = new Plane;

        plane->traj.setXY(randomRadius(generator), randomAngle(generator));
        plane->setID("AF" + to_string(randomString(generator))) ;
        plane->setSpeed(randomSpeed(generator));
        plane->setHeight(randomHeight(generator));

        vectorPlane.push_back(plane);
        cout << setprecision(5) << *plane << endl;
    }
    return vectorPlane;
}