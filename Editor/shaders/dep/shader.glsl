
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

    out vec4 outColor;

    in vec2 fTexPos;

    void main() {
        outColor = texture(uTexture, fTexPos);
        //outColor = vec4(fTexPos, 0.0, 1.0);//uColor;
    }
#endif
