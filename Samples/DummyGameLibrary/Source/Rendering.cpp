#include <Rendering.h>
#include <State.h>
#include <IGraphics.h>
#include <iostream>

namespace dummy_game
{
    Rendering::Rendering(const State& state, const IGraphics& graphics)
        : m_state(state)
        , m_graphics(graphics)
    {
        std::cout << "Rendering: constructed." << std::endl;
    }

    Rendering::~Rendering()
    {
        std::cout << "Rendering: destructed." << std::endl;
    }

    void Rendering::render() const
    {
        m_state.read();
        std::cout << "  Rendering: render the two circles." << std::endl;
        m_graphics.draw();
    }
}