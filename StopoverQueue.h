#ifndef EDA2TRABALHOPRATICO_STOPOVERLIST_H
#define EDA2TRABALHOPRATICO_STOPOVERLIST_H

#include <iostream>

using namespace std;

class StopoverQueue {
public:
    struct Stopover{
        int idCity;
        double distance;
        Stopover(int idCity, double distance);
        Stopover* next;
    };

    StopoverQueue();

    bool isEmpty();

    void enqueue(Stopover* stopover);

    Stopover* dequeue();

    double getTotalDistance();

//private:
    Stopover* head;
    Stopover* tail;
};


#endif //EDA2TRABALHOPRATICO_STOPOVERLIST_H
