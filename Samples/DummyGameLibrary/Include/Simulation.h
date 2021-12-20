#pragma once

namespace dummy_game
{
    class State;
    class Physics;

    class Simulation
    {
    public:
        Simulation(State&, const Physics&);
        ~Simulation();

        void update() const;

    private:
        State& m_state;
        const Physics& m_physics;
    };
}