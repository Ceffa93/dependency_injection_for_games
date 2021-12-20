#pragma once

namespace dummy_game
{
    class Physics
    {
    public:
        Physics();
        ~Physics();

        void resolveCollisions() const;
    };
}