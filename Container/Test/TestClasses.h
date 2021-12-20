#pragma once

struct A
{
    A() {}
    char name = 'A';
};

struct B
{
    B(A& a) : a(a) {}
    char name = 'B';
    A& a;
};

struct C
{
    C(A* a) : a(*a) {}
    char name = 'C';
    A& a;
};

struct D
{
    D(B& b, C& c) : b(b), c(c) {}
    char name = 'D';
    B& b;
    C& c;
};

struct E
{
    E(const A& a) : a(a) {}
    char name = 'E';
    const A& a;
};

struct Interface
{
};

struct Base
{
    virtual int getInt() { return 10; }
};

struct Derived : public Base
{
    virtual int getInt() final { return 20; }
};

struct Derived2 : public Base
{
};

struct DependOnBase
{
    DependOnBase(Base& base) : base(base) {}
    Base& base;
};

