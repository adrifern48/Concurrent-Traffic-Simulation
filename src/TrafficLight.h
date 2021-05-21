#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase { RED, GREEN };

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

class TrafficLight : TrafficObject
{
public:
    // constructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behavior methods
    void waitForGreen();
    void simulate();

private:
    // typical behavior methods
    void cycleThroughPhases();

    MessageQueue<TrafficLightPhase> msgQueue;
    TrafficLightPhase _currentPhase;
    std::mutex _mutex;
    std::condition_variable _condition;
};

#endif