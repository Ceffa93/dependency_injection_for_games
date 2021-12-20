#pragma once

namespace dummy_game
{
    class State;
    class IGraphics;

    class Rendering
    {
    public:
        Rendering(const State&, const IGraphics&);
        ~Rendering();

        void render() const;

    private:
        const State& m_state;
        const IGraphics& m_graphics;
    };
}