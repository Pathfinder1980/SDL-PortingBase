#version 330 core

in vec3 vColor;
in vec2 vUV;

uniform sampler2D uTexture;

out vec4 fragColor;

void main() { 
    fragColor = texture(uTexture, vUV) *  vec4(vColor, 1.0);
}