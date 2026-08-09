#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uM4444VP;

out vec3 vColor;
        
void main() { 
    gl_Position = uMVP * vec4(aPosition, 1.0);
    vColor = aNormal * 0.5 + 0.5;
}
        