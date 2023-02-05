# OpenGLRenderer - 1/14/23 - Julian Haurin

A simple 3D OpenGL renderer built using:
 - GLFW for cross-platform window handling, OpenGL context creation, and handling user input
 - GLEW for loading OpenGL function pointers
 - glm for matrices, vectors, and important linear algebra calculations
 - tinyobjloader for importing and parsing wavefront (.obj) files
 - stb_image for loading images, mainly for use in textures

Provides basic camera functionality (basic movement, looking around with mouse, zooming) and handles rendering textured .obj files

Working on implementing basic lighting (2/4/23)

Learning resources include:
    - learnopengl.com tutorial
    - vulkan-tutorial.com
    - github.com/tinyobjloader/tinyobjloader

Updated 2/4/2023 