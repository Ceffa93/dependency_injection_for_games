#pragma once
#include "IGraphics.h"

namespace dummy_game
{
    class OpenGL : public IGraphics
    {
    public:
        OpenGL();
        ~OpenGL();

        virtual void draw() const final;
    };
}