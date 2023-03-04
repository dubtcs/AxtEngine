#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexPos;

uniform mat4 uViewProjection;
uniform mat4 uModelTransform;

out vec2 fTexPos;

void main(){
	gl_Position = uViewProjection * uModelTransform * vec4(inPos, 1.0);
	fTexPos = inTexPos;
}