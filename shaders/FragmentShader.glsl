#version 330

// Input

in vec3 fColor;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = vec4(fColor * 0.5 + 0.5, 1);

}
