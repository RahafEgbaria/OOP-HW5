#ifndef PART1_SUBJECT_H
#define PART1_SUBJECT_H

#include "Observer.h"
#include "OOP5EventException.h"
#include <vector>

using namespace std;

template<typename T>
class Subject {
    vector<Observer<T>*> observers;

public:
    /*Subject() {
        observers = vector<Observer<T>*>();
    }*/

    void notify(const T& t) {
        for(typename vector<Observer<T>*>::iterator it = observers.begin(); it != observers.end(); it++) {
            (*it)->handleEvent(t);
        }
    }

    void addObserver(Observer<T>& o) {
        for(typename vector<Observer<T>*>::iterator it = observers.begin(); it != observers.end(); it++) {
            if((*it) == &o) {
                throw ObserverAlreadyKnownToSubject();
            }
        }
        observers.push_back(&o);
    }

    void removeObserver(Observer<T>& o) {
        for(typename vector<Observer<T>*>::iterator it = observers.begin(); it != observers.end(); it++) {
            if((*it) == &o) {
                this->observers.erase(it);
                return;
            }
        }
        throw ObserverUnknownToSubject();
    }

    Subject<T>& operator+=(Observer<T>& o) {
        this->addObserver(o);
        return *this;
    }

    Subject<T>& operator-=(Observer<T>& o) {
        this->removeObserver(o);
        return *this;
    }

    Subject<T>& operator()(const T& t) {
        this->notify(t);
        return *this;
    }
};

#endif //PART1_SUBJECT_H