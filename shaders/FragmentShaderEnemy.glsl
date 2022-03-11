#version 330

// Input

in vec3 fColor;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = vec4(fColor * 1 + 0.2, 1);

}
