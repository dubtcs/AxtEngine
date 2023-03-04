
#ifdef GLSL_VERTEX
    layout (location = 0) in vec3 inPos;

    uniform mat4 uViewProjection;
    uniform mat4 uModelTransform;

    void main(){
        gl_Position = uViewProjection * uModelTransform * vec4(inPos, 1.0);
    }
#endif

#ifdef GLSL_FRAGMENT
    uniform vec4 uColor;

    out vec4 outColor;

    void main() {
        outColor = uColor;
    }
#endif
