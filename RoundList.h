#ifndef EDA2TRABALHOPRATICO_ROUNDLIST_H
#define EDA2TRABALHOPRATICO_ROUNDLIST_H

#include <string>
#include <iostream>
#include "MatchQueue.h"

using namespace std;

class RoundList {
public:
    struct Round{
        int id;
        MatchQueue* matchQueue;
        Round* next;
        Round(int id);
    };

    RoundList(int size);

    bool isEmpty();

    void add(int id);

    void addMatch(int idRound, MatchQueue::Match* match);

    void printCalendar();

private:
    Round* head;
    Round* tail;
};


#endif //EDA2TRABALHOPRATICO_ROUNDLIST_H