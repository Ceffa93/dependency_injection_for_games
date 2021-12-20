#include <Physics.h>
#include <iostream>

namespace dummy_game
{
    Physics::Physics()
    {
        std::cout << "Physics: constructed." << std::endl;
    }

    Physics::~Physics()
    {
        std::cout << "Physics: destructed." << std::endl;
    }

    void Physics::resolveCollisions() const
    {
        std::cout << "    Physics: resolve collisions between circles." << std::endl;
    }
}