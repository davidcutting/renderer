# String

*/striNG/*
*noun.*

a set of things tied or threaded together on a thin cord.

## Motivation

Simple Tiny RenderING engine, or String is my hobby rendering engine. I found many existing game engines lend themselves to game developers who are not interested in the nitty gritty of rendering or software in general. Many of the large engines are very featureful. Those engines have cool features and very robust tools. It can be too much. I'm not interested in having many of the features marketed to less technically minded individuals such as custom scripting languages, visual block coding, monolithic editors, ect. Simply, the purpose of this project is not to be the fastest most flexible game engine ever made, but just to be a simple sandbox without strange hacks to support your grandmother's smart watch or help little Timmy learn how to make his first video game.

The goal is to Keep It Stupid Simple so that one could easily pick up the engine and run simulations, experiments, games, whatever. I'd like something loosely bound such that the engine can be extended, disabled, discarded, or whatever my project calls for.

Major priorities include:
1. Bleeding edge. Within reason of course, but I will not be supporting very old versions of libraries, targetting every platform, or optimizing for ancient hardware. Aiming to utilize modern techniques and keeping up with language features in a way to best represent the intention of the code.
2. Clarity. Keeping things clear and readable if possible.
3. Speed. In that order, within reason. Speed is of course a priority in high performance applications such as this, but ideally no sacrifice in clarity to squeeze out max performance.

A fair warning; most of this project is experimentation and I'm no expert so implementation of many things may be naive. I will accept pull requests gladly, but don't expect me to implement your features for you if I'm not already interested in doing so.

## Platforms and Versions
| Operating System  | Library (version) | Supported     |
| :---------------- | :---------------: | :-----------: |
| Linux 64-bit      | Vulkan (1.3)      | Yes           |
| Windows 64-bit    | Vulkan (1.3)      | No            |
| Mac 64-bit        | Metal/MoltenVK    | No            |

## Dependencies

- meson
- VulkanSDK
- glm
- glfw3
- glslang
- spdlog

## Building

I suggest you go for your favorite Linux distro, because I'm not supporting Windows or Mac currently. You'll need to install the listed dependencies to your system including the VulkanSDK.

Afterward, set up the build with Meson using `meson setup build` then `cd build` and build the engine + install using `sudo ninja -j8 install`

## License

This project is MIT licensed. Third party libraries may/do have their own licensing.

## ToDo

- Refactor spdlog into fmtlog and utilize multithreaded logging

## Inspiration

- Vulkan Tutorial: A Vulkan API E-book
- Sascha Willems, Adam Sawicki, Khronos: Vulkan-Samples
- Austin Morlan: A simple Entity Component System (ECS) [C++]
- Yan Chernikov: Hazel Game Engine
- Godot Team: The Godot Game Engine
