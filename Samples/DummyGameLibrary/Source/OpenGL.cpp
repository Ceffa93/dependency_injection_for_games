#include <OpenGL.h>
#include <iostream>

namespace dummy_game
{
    OpenGL::OpenGL()
    {
        std::cout << "OpenGL: constructed." << std::endl;
    }

    OpenGL::~OpenGL()
    {
        std::cout << "OpenGL: destructed." << std::endl;
    }

    void OpenGL::draw() const
    {
        std::cout << "    OpenGL: graphics API draw call." << std::endl;
    }
}