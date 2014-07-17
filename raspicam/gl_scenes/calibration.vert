attribute vec2 vertex;
varying vec2 texcoord;

void main(void) {
   texcoord = 0.5 * (vertex + 1.0);
   gl_Position = vec4(vertex, 0.0, 1.0);
}
