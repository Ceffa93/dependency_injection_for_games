#pragma once

namespace dummy_game
{
    class State
    {
    public:
        State();
        ~State();

        void read() const;
        void write();
    };
}