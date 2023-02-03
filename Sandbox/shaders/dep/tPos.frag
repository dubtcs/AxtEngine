#version 330 core

uniform sampler2D uTexture;

out vec4 outColor;

in vec2 fTexPos;

void main() {
    outColor = texture(uTexture, fTexPos);
    //outColor = vec4(fTexPos, 0.0, 1.0);//uColor;
}
