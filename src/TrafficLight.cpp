#include <iostream>
#include <random>

#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
 
template <typename T>
T MessageQueue<T>::receive()
{   
    // Perform queue modifications under the lock
    std::unique_lock<std::mutex> uLock(_mutex);
    _condition.wait(uLock, [this] { return !_queue.empty(); });
    
    // Remove first element from queue
    T msg = std::move(_queue.front());
    _queue.clear(); // used to prevent spurious wake-ups

    // Will not be copied due to Return Value Optimization (RVO) in C++
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.emplace_back(msg);
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::RED;
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        // receive wakes up when a new element is available in the queue
        auto msg = msgQueue.receive();
        
        if (TrafficLightPhase::GREEN == msg)
            break;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(4000,6000);
    auto cycleDuration = distrib(gen);
    auto t1 = std::chrono::high_resolution_clock::now();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto t2 = std::chrono::high_resolution_clock::now();

        auto timeDelta = (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1)).count();

        if (timeDelta >= cycleDuration)
        {
            _currentPhase = _currentPhase == TrafficLightPhase::RED ? TrafficLightPhase::GREEN : TrafficLightPhase::RED;
            msgQueue.send(std::move(_currentPhase));
            t1 = std::chrono::system_clock::now();
            cycleDuration = distrib(gen);
        }
    }
}
