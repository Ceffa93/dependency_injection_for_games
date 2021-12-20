#include <State.h>
#include <iostream>

namespace dummy_game
{
    State::State()
    {
        std::cout << "State: constructed." << std::endl;
    }

    State::~State()
    {
        std::cout << "State: destructed." << std::endl;
    }

    void State::read() const 
    {
    }
    void State::write()
    {
        std::cout << "    State: circle positions and velocities updated." << std::endl;
    }
}