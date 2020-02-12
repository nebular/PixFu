attribute  vec3 aPos;
attribute  vec3 aColor;
attribute  vec2 aTexCoord;

// for android older specs

varying vec3 ourColor;
varying vec2 TexCoord;


void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, 1.0f - aTexCoord.y);
}

