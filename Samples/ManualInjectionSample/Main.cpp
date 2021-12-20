#include <Loop.h>
#include <Simulation.h>
#include <Rendering.h>
#include <State.h>
#include <Physics.h>
#include <OpenGL.h>

using namespace dummy_game;

int main()
{
    State state{};
    Physics physics{};
    Simulation simulation{ state, physics };
    OpenGL openGL{};
    Rendering rendering{ state, openGL };
    Loop loop{ simulation, rendering };

    return 0;
}