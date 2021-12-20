#include <gtest/gtest.h>
#include <Container/ComponentSet.h>
#include <Container/Exception.h>
#include "TestClasses.h"

using namespace container;

TEST(ComponentSetTest, InternalComponent) {
    ComponentSet componentSet;
    componentSet.add<A>();
}
TEST(ComponentSetTest, InternalConstComponent) {
    ComponentSet componentSet;
    componentSet.add<const A>();
}
TEST(ComponentSetTest, ExternalComponent) {
    A a;
    ComponentSet componentSet;
    componentSet.add(a);
}
TEST(ComponentSetTest, ExternalConstComponent) {
    const A a;
    ComponentSet componentSet;
    componentSet.add(a);
}
TEST(ComponentSetTest, ExternalPointerComponent) {
    A a;
    ComponentSet componentSet;
    componentSet.add(&a);
}
TEST(ComponentSetTest, ExternalConstPointerComponent) {
    const A a;
    ComponentSet componentSet;
    componentSet.add(&a);
}

TEST(ComponentSetTest, DuplicatedInternalComponent) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<A>();
        componentSet.add<A>();
    }
    catch (ComponentSetException e)
    {
        EXPECT_EQ(e.code(), ComponentSetException::Code::ComponentAlreadyAdded);
        return;
    }
    FAIL();
}

TEST(ComponentSetTest, DuplicatedExternalComponent) {
    try
    {
        A a;
        ComponentSet componentSet;
        componentSet.add(a);
        componentSet.add(a);
    }
    catch (ComponentSetException e)
    {
        EXPECT_EQ(e.code(), ComponentSetException::Code::ComponentAlreadyAdded);
        return;
    }
    FAIL();
}

TEST(ComponentSetTest, DuplicatedInternalComponentDifferentConst) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<A>();
        componentSet.add<const A>();
    }
    catch (ComponentSetException e)
    {
        EXPECT_EQ(e.code(), ComponentSetException::Code::ComponentAlreadyAdded);
        return;
    }
    FAIL();
}

TEST(ComponentSetTest, DuplicatedExternalComponentDifferentConst) {
    try
    {
        A a;
        const A a2;
        ComponentSet componentSet;
        componentSet.add(a);
        componentSet.add(a2);
    }
    catch (ComponentSetException e)
    {
        EXPECT_EQ(e.code(), ComponentSetException::Code::ComponentAlreadyAdded);
        return;
    }
    FAIL();
}

TEST(ComponentSetTest, ImplementInterface) {
    ComponentSet componentSet;
    componentSet.add<Derived>().implements<Base>();
}

TEST(ComponentSetTest, DuplicatedInterface) {
    try
    {
        ComponentSet componentSet;
        componentSet.add<Derived>().implements<Base>();
        componentSet.add<Derived2>().implements<Base>();
    }
    catch (ComponentSetException e)
    {
        EXPECT_EQ(e.code(), ComponentSetException::Code::InterfaceAlreadyImplemented);
        return;
    }
    FAIL();
}

