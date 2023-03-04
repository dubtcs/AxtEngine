
#ifdef GLSL_VERTEX
    layout (location = 0) in vec3 inPos;
    layout (location = 1) in vec4 inColor;
    layout (location = 2) in vec2 inTexPos;

    uniform mat4 uViewProjection;

    out vec4 fColor;

    void main(){
        gl_Position = uViewProjection * vec4(inPos, 1.0);
        fColor = inColor;
    }
#endif

#ifdef GLSL_FRAGMENT

    out vec4 outColor;

    in vec4 fColor;

    void main() {
        outColor = fColor;//texture(uTexture, fTexPos) * uColor;//uColor;
    }
#endif
