#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 uViewProjection;
uniform mat4 uModelTransform;

out vec4 fColor;

void main(){
	gl_Position = uViewProjection * uModelTransform * vec4(inPos, 1.0);
	fColor = vec4(0.5, 0.4, 0.7, 1.0);
}