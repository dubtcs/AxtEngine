#version 330 core

out vec4 outColor;
in vec3 fPosition;

void main(){
	outColor = vec4(fPosition + 0.5, 1.0);
}