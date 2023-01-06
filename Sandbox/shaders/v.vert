#version 330 core

layout (location = 0) in vec3 inPos;
out vec3 fPosition;

void main(){
	gl_Position = vec4(inPos, 1.0);
	fPosition = inPos;
}