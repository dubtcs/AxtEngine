#version 330 core

out vec4 outColor;

in vec3 fPosition;
in vec4 fColor;

void main(){
	outColor = fColor;//vec4(fPosition + 0.5, 1.0);
}