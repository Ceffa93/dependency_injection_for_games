#include <Loop.h>
#include <Simulation.h>
#include <Rendering.h>
#include <iostream>
#include <string>

namespace dummy_game
{
    Loop::Loop(const Simulation& simulation, const Rendering& rendering)
    : m_simulation(simulation)
    , m_rendering(rendering)
    {
        std::cout << "Loop: constructed." << std::endl;
        mainLoop();
    }

    Loop::~Loop()
    {
        std::cout << "Loop: destructed." << std::endl;
    }

    void Loop::mainLoop() const
    {
        constexpr auto FrameCount = 2;
        for (int i = 0; i < FrameCount; i++)
        {
            std::cout << std::endl << "Loop: start frame #" << std::to_string(i) << "." << std::endl;
            m_simulation.update();
            m_rendering.render();
        }
        std::cout << std::endl;
    }
}