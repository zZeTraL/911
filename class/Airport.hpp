#include <SFML/Graphics.hpp>

class Airport{
    string id;
    queue<Plane*> queuePlaneParked;
    float x, y;
    int availableSlot;
    int maxSlot;
public:
    Airport(string str, int slot, float posX, float posY){
        this->id = str;
        this->availableSlot = slot;
        this->maxSlot = slot;
        this->x = posX;
        this->y = posY;
    }

    // Adders
    void addPlaneLanded(Plane* &plane){
        this->availableSlot--;
        queuePlaneParked.push(plane);
    }
    void addSlot(int number){
        availableSlot += number;
    }

    // Setters
    void setAvailableSlot(int number){this->availableSlot = number;}
    void setID(string str) {this->id = str;}
    void setXY(float posX, float posY){
        this->x = posX;
        this->y = posY;
    }

    // Getters
    int getAvailableSlot() const {return this->availableSlot;}
    int getMaxSlot() const {return this->maxSlot;}
    string getID() const {return this->id;}
    float getX() const {return this->x;}
    float getY() const {return this->y;}
    queue<Plane*> getQueuePlaneParked() const {return queuePlaneParked;}

    // Methods
    void planeParkedQueuePop(){queuePlaneParked.pop();}

    friend ostream& operator<<(ostream &os, const Plane &plane);



};

ostream& operator<<(ostream& os, const Airport &airport){
    os << airport.getID() << "\nPosition: (" << airport.getX() << ", " << airport.getY() << ")\nNombre de places disponible: " << airport.getAvailableSlot() << "\nAvions dans le parking: " << airport.getQueuePlaneParked().size() << "\n\n";
    return os;
}