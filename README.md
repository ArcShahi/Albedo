<h1 align="center">Albedo</h1>

![Version](https://img.shields.io/badge/Version-1.0-blue?style=for-the-badge)
![Windows](https://img.shields.io/badge/Windows-passing-brightgreen?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-untested-yellow?style=for-the-badge&logo=linux&logoColor=black)



GLSL fragment shader renderer and editor.


Program your shader and see it changing side by side. Written in modern C++ for speed.

## Features
- Fast shader compilation and rendering 
- Resizable panels : editors, scene , log 
- Syntax highlighting and error message.
- Dark window title bar ( I'm so proud of this ) 


## Gallery 




## Setup



## Credits
1. TheCherno: His OpenGL series has helped me a lot and I've also learned to architech software(somewhat) from him. What I had was an overengineered , overkill abomination. The app flow is much simpler now.
1. LearnOpenGL: What a great site to learn OpenGL 


## Vendors

- **GLFW** : For Window
- **GLAD** : For OpenGL "Loader" to give us the GPU drive implemented OpenGL specification.
- **ImGUI**: For UI
- **STB**  : For texture loading 
- **GLM**  : Why not.
- **goossens/ImGuiColorTextEdit** : Text editor and syntax highlighting

## AI POLICY 

AI is a tool and is used as a tool to make things convinient.. not to suck the fun of art and craft.

AI usage for Code generation and documentation is forbidden for this project.

### AI usage disclosure

The follwing are  LLM generated : 
- `dependencies.cmake` 
- `CMakePresets.json` 
- CMLs in Vendors directories.


> Shahi ( *prefers natural stupidity over artifical intelligence*)


## TODO
- Optimize shader compilation
- Maybe multithreading
- Mulipass : I'll not add it until I am able to create some shader art that does require multipass.
- Some bugs

----