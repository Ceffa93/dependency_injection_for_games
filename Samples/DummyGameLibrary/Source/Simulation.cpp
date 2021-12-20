#include <Simulation.h>
#include <State.h>
#include <Physics.h>
#include <iostream>

namespace dummy_game
{
    Simulation::Simulation(State& state, const Physics& physics)
        : m_state(state)
        , m_physics(physics)
    {
        std::cout << "Simulation: constructed." << std::endl;
    }

    Simulation::~Simulation()
    {
        std::cout << "Simulation: destructed." << std::endl;
    }

    void Simulation::update() const
    {
        m_state.read();
        std::cout << "  Simulation: update circle positions and velocities." << std::endl;
        m_physics.resolveCollisions();
        m_state.write();
    }
}