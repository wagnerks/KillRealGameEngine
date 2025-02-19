#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 PV;
layout(std430, binding = 1) buffer modelMatrices
{
    mat4 model[];
};

void main()
{
    gl_Position = PV * model[gl_InstanceID] * vec4(aPos, 1.0); 
}  