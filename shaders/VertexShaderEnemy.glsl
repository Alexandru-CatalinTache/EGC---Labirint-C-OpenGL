#version 330

// Input

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
layout (location=3) in vec3 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output

out vec3 fColor;

void main()
{
    
    fColor = vec3(1, 0, 0);
    gl_Position = Projection * View * Model * vec4(position, 1);

}
