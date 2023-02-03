
#ifdef GLSL_VERTEX
    layout (location = 0) in vec3 inPos;
    layout (location = 1) in vec2 inTexPos;

    uniform mat4 uViewProjection;
    uniform mat4 uModelTransform;

    out vec2 fTexPos;

    void main(){
        gl_Position = uViewProjection * uModelTransform * vec4(inPos, 1.0);
        fTexPos = inTexPos;
    }
#endif

#ifdef GLSL_FRAGMENT
    uniform sampler2D uTexture;
    uniform vec4 uColor;

    in vec2 fTexPos;

    out vec4 outColor;

    void main() {
        outColor = texture(uTexture, fTexPos) * uColor;//uColor;
    }
#endif

