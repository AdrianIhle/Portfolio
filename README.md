# Portfolio
Hey! The purpose of this repository is to showcase some of my scripting abilities. Each branch represents a feature which is made as plug-and-play as possible

My [Generic 2D Grid](https://github.com/AdrianIhle/Portfolio/tree/Generic2DGrid) for example, is my solution to regularly needing grids in 2D to achieve various world space segmentation tasks. I have implemented a simple integer value grid and a AStar pathfinding grid feature as two examples of what I have used it for in the past. 
It's made specifically for Unity and is plug and play within that ecosystem, but also easily adaptable into any scripting language as it relies only on simple data types like vectors and transforms. 

[My Custom Event System](https://github.com/AdrianIhle/Portfolio/tree/EventSystem) was my solution to needing an event-based message system for my thesis to communicate world changes to other systems. It implements some basic threading and focuses on implementing some performance considerations like limiting event calls and splitting up event cues to allow for faster searches. 

For my thesis I also implemented a [Custom Camera Controller](https://github.com/AdrianIhle/Portfolio/tree/CameraController) to allow players to change between an RTS-like and RPG-like camera view, while making it easy as a designer to control the values and behaviour of the zoom. it uses Unity's animation curves for example to help define the shape of the camera's zoom path. 

A highlight of my bachelor program was also making my own [Game Engine](https://github.com/AdrianIhle/Portfolio/tree/GameEngine) that implements the fundamentals of physics, shaders and allowing for input and scripting through JavaScript. 
