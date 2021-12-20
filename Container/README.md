# Dependency Injection Container
This README file provides detailed information about the Container library introduced in the [Injection Container](https://medium.com/p/fef5ab414a53) appendix of the Medium article.

It is structured in 3 sections:
* [Features](#Features): a breakdown of the Container API and features, with code snippets.
* [Integration](#Integration): information to be aware of in case you wish to integrate this library into your own project. 
* [Behind the scenes](#Behind-the-scenes): a deep dive into the theory behind this library, and its implementation.

## Features
### Basic usage:
```c++
A::A(){}
B::B(A&){}

int main
{
  // Add components to ComponentSet in any order
  container::ComponentSet set;
  set.add<B>();
  set.add<A>(); // No construction happens here

  container::Container container{set}; // Component construction: 'A', then 'B'

  A& a = container.get<A>(); // Get reference to component held by the Container

} // Component destruction: 'B', then 'A'
```
### Pointer / reference dependencies:
```c++
A::A(){}
B::B(A&){} // reference dependency
C::C(A*){} // pointer dependency

int main
{
  container::ComponentSet set;
  set.add<A>();
  set.add<B>(); // ok, 'A' instance passed by reference
  set.add<C>(); // ok, 'A' instance passed by pointer

  container::Container container{set};
}
```
### Const components:
```c++
A::A(){}
B::B(const A&){} // 'A' reference is const
C::C(A&){} // 'A' reference is non-const

int main
{
  container::ComponentSet set;
  set.add<const A>(); // Requests that 'A' is not modifiable by other components
  set.add<B>(); // ok
  set.add<C>(); // not ok, A is const!

  container::Container container{set}; // -> runtime error here
}
```
### External components:
```c++
A::A(){}
B::B(A&){}

int main
{
  container::ComponentSet set;
  A a{};
  set.add(a);   // Component 'A' is provided externally
  set.add<B>();

  container::Container container{set};
}
```
### Dependency inversion:
```c++
class A : Interface{};
A::A(){}
B::B(Interface&){}

int main
{
  container::ComponentSet set;
  set.add<A>().implements<Interface>(); // 'A' will be passed as dependency to components depending on 'Interface'
  set.add<B>();

  container::Container container{set};
}
```
## Integration
The Container library was created for educational purposes, so the implementation was intentionally kept as simple and readable as possible.</br>
For this reason, the library makes some assumptions that you should be aware of if you wish to integrate it into your own project:
* **RTTI**: used to access type id/name.</br>
If you want to avoid RTTI in your project, you should modify the library to use a custom type id/name implementation.
* **Exceptions**: used for ease of testing failing conditions.</br>
If you wish to avoid exceptions in your project, you should modify the library to use a different error handling policy, such as asserts.
* **C++14**: required.

## Behind the scenes
This section covers the theory and the implementation behind the Container library. If you are not satisfied by knowing how a Container is used, and are itching to understand how it works under the hood, this is the section for you.

### Theory
Implementing a Container in languages that support reflection, such as C#, is quite straightforward:
1. First, determine the component construction order:</br>
    * Reflection is used to query the parameters of component constructors (i.e. the dependencies).
    * This information is used to build a dependency graph.
    * The construction order of components is obtained by traversing the dependency graph depth-first.
2. Second, construct the components in order.

These 2 steps are independent, and can be executed separately.

In C++, there is no way to analyze constructors through reflection, and the approach to build a Container is very different:
1. Pick any component, try to create it by invoking the constructor.
2. In order to invoke the constructor, the arguments (i.e. dependencies) are requested.
3. If a dependency has not yet been created, create it (recurively going back to step 1).

In other words, just before the constructor is invoked, during the argument resolution phase:
1. The dependency graph and component construction order is determined.
2. Dependencies are created in the correct order.

Note how, differently from the reflection approach, these two steps are strongly coupled together. It is not possible to query the dependency graph or the construction order without constructing the components.

### Implementation
The C++ implementation of a Container is not trivial, as it extensively uses templates, and relies on some cornercase language features. 
This section aims to ease the learning curve by explaining the key ideas that are required to understand the implementation. Code snippets are similar to the implementation code, but simplified for explanation purposes. 

The **first key idea** is the templated cast operator, implemented in the class `any_type`:
```c++
struct any_type
{
  template <class T>  operator T& () { ... }
}
```
Thanks to the operator, `any_type` can be cast to any other class:
```c++
Component::Component(Dep0&, Dep1&);

any_type dep0;
any_type dep1;
Component component(dep0, dep1); // ok!
```

In this example, `Component` has a constructor of 2 parameters, and can be constructed by passing 2 `any_type` instances as arguments.
The general case is that every component has a constructor with N parameters that can be invoked with N `any_type` instances.

The **second key idea** is to exploit this fact, and automate the passing of dependencies.</br>
At compile time we wish to instantiate a template function that calls the correct N-arguments constructor, and passes N `any_type` instances to it.
The constructor is discovered with a mix of `std::index_sequence` and `std::is_constructible`, which implement the following logic at compile time:
```c++
ArgCount = 99;
do
{
  if (Component is constructible with "ArgCount" instances of "any_type") --> Constructor found
  else ArgCount--;  
} while (ArgCount >= 0)
```
In code, the templated function that creates a component by invoking the right N-argument constructor is `ConstructorFactory<T>::Create(...)`.

The **third key idea** is to get components from the Container within the cast operator of `any_type`.</br>
Note that if the component is not found, the Container will create it, and this operation is recursive:
```c++
struct any_type
{
  template <class T>  operator T& () { return container.getOrCreate<T>(); }
}
```

The combination of these 3 ideas is at the heart of the implementation.</br> 
To conclude, the following example shows the invocation order of the functions presented in this section, in a simple application with 3 components: 
```c++
A::A(B&, C&);
B::B(C&);
C::C();
         
ConstructorFactory<A>::Create();     // Start to create A. Create() calls A constructor with 2 any_type arguments.
B& any_type::operator();             // The first any_type argument of A constructor is casted to B&.
  getOrCreate<B>();                  // Inside the cast operator, the container is queried for B.
  ConstructorFactory<B>::Create();   // B is not found in the container, so its creation is started.
  C& any_type::operator();           // The any_type argument of B constructor is casted to C&.
    getOrCreate<C>();                // Inside the cast operator, the container is queried for C.
    ConstructorFactory<C>::Create(); // C is not found in the container, so its creation is started.
    C::C();                          // C does not have dependencies, so the constructor is invoked, and C is immediately created.
  B::B(C&);                          // Now that C is created, B constructor can be invoked with C as dependency, and B is created.
C& any_type::operator();             // The second any_type argument of A constructor is casted to C&. 
  getOrCreate<C>();                  // The container is queried for C, which is already created, so it is returned immediately.
A::A(B&,C&);                         // The constructor of A can finally be invoked with B and C as dependencies. 
``` 
