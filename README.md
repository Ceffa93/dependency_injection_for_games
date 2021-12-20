# Dependency Injection for Games
This repository contains the code accompanying the [Dependency Injection for Games](https://medium.com/p/519cf2a545ee) Medium article.

## Repository Structure
The root folder contains the `DependencyInjectionForGames.sln` solution file, which references every project in the repository.

The `Samples/` folder contains:
* `DummyGameLibrary/`: A library containing the systems that make up the dummy game application described in the article, such as __Rendering__ or __Physics__. All systems are implemented as dummies, and will print their actions to the standard output for demonstration purposes. Please note that  __Rendering__ depends on the __IGraphics__ interface, following the example introduced in the [Dependency Inversion](https://medium.com/p/20c3dada66c4) appendix.
* `ManualInjectionSample/`: The first executable of the dummy game application, which demonstrates how to manually setup Dependency Injection to organize the game systems defined in `DummyGameLibrary/`.
* `ContainerInjectionSample/`: The second executable of the dummy game application, which demonstrates how a Container can automate the process of setting up Dependency Injection. This topic is covered in the [Injection Container](https://medium.com/p/fef5ab414a53) appendix.
  
The behavior of the two game executables is intended to be the same: running the game for 2 frames before terminating.

The `Container/` folder contains:
* `Container/`: A Dependency Injection Container library, such as the one presented in the [Injection Container](https://medium.com/p/fef5ab414a53) appendix.
* `Test/`: Unit tests for the Container library.
* [README.md](Container/README.md): Documentation for the Container library, including:
  * Feature overview;
  * Integration info;
  * Behind the scenes: theory and implementation details.
