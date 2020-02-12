varying vec3 ourColor;
varying vec2 TexCoord;

// texture sampler
uniform sampler2D glbuffer;

void main() {
	gl_FragColor = texture2D(glbuffer, TexCoord);
}

/*

in vec -> varying vec

*/