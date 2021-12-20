#include <gtest/gtest.h>
#include <Container/ComponentSet.h>
#include <Container/Container.h>
#include <Container/Exception.h>
#include "TestClasses.h"

using namespace container;

TEST(ContainerTest, OneExternalService) {
    A a;
    ComponentSet componentSet;
    componentSet.add(a);
    Container container(componentSet);
    EXPECT_EQ(container.get<A>().name, 'A');
}

TEST(ContainerTest, OneInternalService) {
    ComponentSet componentSet;
    componentSet.add<A>();
    Container container(componentSet);
    EXPECT_EQ(container.get<A>().name, 'A');
}

TEST(ContainerTest, ReferenceDependency) {
    ComponentSet componentSet;
    componentSet.add<A>();
    componentSet.add<B>();
    Container container(componentSet);
    EXPECT_EQ(container.get<B>().a.name, 'A');
}

TEST(ContainerTest, PointerDependency) {
    ComponentSet componentSet;
    componentSet.add<A>();
    componentSet.add<C>();
    Container container(componentSet);
    EXPECT_EQ(container.get<C>().a.name, 'A');
}


TEST(ComponentSetTest, GetTests) {
    ComponentSet componentSet;
    componentSet.add<A>();
    componentSet.add<const B>();
    Container container(componentSet);
    EXPECT_EQ(container.get<A>().name, 'A');
    EXPECT_EQ(container.get<A*>()->name, 'A');
    EXPECT_EQ(container.get<const A>().name, 'A');
    EXPECT_EQ(container.get<const A*>()->name, 'A');
    EXPECT_EQ(container.get<const B>().name, 'B');
    EXPECT_EQ(container.get<const B*>()->name, 'B');
}

TEST(ComponentSetTest, GetMissingComponent) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<A>();
        Container container(componentSet);
        container.get<E>();
    }
    catch (ContainerException e)
    {
        EXPECT_EQ(e.code(), ContainerException::Code::GetMissingComponent);
        return;
    }
    FAIL();
}

TEST(ComponentSetTest, GetConstMismatch) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<const A>();
        Container container(componentSet);
        container.get<A>();
    }
    catch (ContainerException e)
    {
        EXPECT_EQ(e.code(), ContainerException::Code::GetConstMismatch);
        return;
    }
    FAIL();
}

TEST(ContainerTest, DiamondDependency) {
    ComponentSet componentSet;
    componentSet.add<D>();
    componentSet.add<C>();
    componentSet.add<B>();
    componentSet.add<A>();
    Container container(componentSet);
    EXPECT_EQ(container.get<D>().name, 'D');
    EXPECT_EQ(container.get<D>().b.name, 'B');
    EXPECT_EQ(container.get<D>().c.name, 'C');
    EXPECT_EQ(container.get<D>().b.a.name, 'A');
}

TEST(ContainerTest, MissingDependency) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<B>();
        Container container(componentSet);
    }
    catch (ContainerException e)
    {
        EXPECT_EQ(e.code(), ContainerException::Code::MissingDependency);
        return;
    }
    FAIL();
}

TEST(ContainerTest, ConstDependencyConstParameter) {
    ComponentSet componentSet;
    componentSet.add<A>();
    componentSet.add<E>();
    Container container(componentSet);
    auto c = container.get<E>();
    EXPECT_EQ(container.get<E>().a.name, 'A');
}

TEST(ContainerTest, MutableDependencyConstParameter) {
    ComponentSet componentSet;
    componentSet.add<A>();
    componentSet.add<E>();
    Container container(componentSet);
    EXPECT_EQ(container.get<E>().a.name, 'A');
}

TEST(ContainerTest, ConstDependencyMutableParameter) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<const A>();
        componentSet.add<B>();
        Container container(componentSet);
    }
    catch (ContainerException e)
    {
        EXPECT_EQ(e.code(), ContainerException::Code::DependencyConstMismatch);
        return;
    }
    FAIL();
}

TEST(ContainerTest, GetInterfaceTest) {
    ComponentSet componentSet;
    componentSet.add<Derived>().implements<Base>();
    Container container{ componentSet };
    EXPECT_EQ(container.get<Derived*>()->getInt(), 20);
    EXPECT_EQ(container.get<Derived>().getInt(), 20);
    EXPECT_EQ(container.get<Base*>()->getInt(), 20);
    EXPECT_EQ(container.get<Base>().getInt(), 20);
}

