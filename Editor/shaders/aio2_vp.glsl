
#ifdef GLSL_VERTEX
    layout (location = 0) in vec3 inPos;
    layout (location = 1) in vec4 inColor;
    layout (location = 2) in vec2 inTexPos;
    layout (location = 3) in float inTexIndex;
    layout (location = 4) in uint inEntityId;

    uniform mat4 uViewProjection;

    out vec2 fTexPos;
    out vec4 fColor;
    out float fTexIndex;
    flat out uint fEntityId;

    void main(){
        gl_Position = uViewProjection * vec4(inPos, 1.0);
        fTexPos = inTexPos;
        fColor = inColor;
        fTexIndex = inTexIndex;
        
        fEntityId = inEntityId; // remove when/if bounding box is added
    }
#endif

#ifdef GLSL_FRAGMENT
    uniform sampler2D uTextures[32];
    //uniform float uTilingMagnitude = 1.0;

    in vec2 fTexPos;
    in vec4 fColor;
    in float fTexIndex;
    in flat uint fEntityId;

    layout (location = 0) out vec4 outColor;
    layout (location = 1) out uint outEntity;

    void main() {
        outColor = texture(uTextures[int(fTexIndex)], fTexPos) * fColor;
        outEntity = fEntityId;
    }
#endif
