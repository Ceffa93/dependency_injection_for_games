#include <Loop.h>
#include <Simulation.h>
#include <Rendering.h>
#include <State.h>
#include <Physics.h>
#include <OpenGL.h>
#include <Container/Container.h>
#include <Container/ComponentSet.h>

using namespace dummy_game;

int main()
{
    container::ComponentSet set;
    set.add<const Loop>();
    set.add<const Physics>();
    set.add<const Simulation>();
    set.add<const Rendering>();
    set.add<const OpenGL>().implements<IGraphics>();
    set.add<State>();

    container::Container container{ set };

    return 0;
}