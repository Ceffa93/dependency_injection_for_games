#pragma once

namespace dummy_game
{
    class Simulation;
    class Rendering;

    class Loop
    {
    public:
        Loop(const Simulation&, const Rendering&);
        ~Loop();

    private:
        void mainLoop() const;

        const Simulation& m_simulation;
        const Rendering& m_rendering;
    };
}