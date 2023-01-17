#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColor;

uniform mat4 uViewProjection;

out vec4 fColor;

void main(){
	gl_Position = uViewProjection * vec4(inPos, 1.0);
	fColor = inColor;
}