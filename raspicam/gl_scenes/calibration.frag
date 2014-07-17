/**
 * Example Calibration edge detct shader. The texture format for
 * EGL_IMAGE_BRCM_MULTIMEDIA_Y is a one byte per pixel greyscale
 * GL_LUMINANCE.  If the output is to be fed into another image processing
 * shader then it may be worth changing this code to take 4 input Y pixels
 * and pack the result into a 32bpp RGBA pixel.
 */

#extension GL_OES_EGL_image_external : require

uniform samplerExternalOES tex;
varying vec2 texcoord;
uniform vec2 tex_unit;

float sum(vec4 p) {
  return p[0] + p[1] + p[2];
}

void main(void) {
    float x = texcoord.x;
    float y = texcoord.y;
    float x1 = x - tex_unit.x;
    float y1 = y - tex_unit.y;
    float x2 = x + tex_unit.x;
    float y2 = y + tex_unit.y;
    vec4 p0 = texture2D(tex, vec2(x1, y1));
    vec4 p1 = texture2D(tex, vec2(x, y1));
    vec4 p2 = texture2D(tex, vec2(x2, y1));
    vec4 p3 = texture2D(tex, vec2(x1, y));
    vec4 p4 = texture2D(tex, vec2(x, y));
    vec4 p5 = texture2D(tex, vec2(x2, y));
    vec4 p6 = texture2D(tex, vec2(x1, y2));
    vec4 p7 = texture2D(tex, vec2(x, y2));
    vec4 p8 = texture2D(tex, vec2(x2, y2));

    float sum4 = sum(p4);
    if (sum4 >= 2.4 &&
        sum4 == sum(p0) &&
        sum4 == sum(p1) &&
        sum4 == sum(p2) &&
        sum4 == sum(p3) &&
        sum4 == sum(p5) &&
        sum4 == sum(p6) &&
        sum4 == sum(p7) &&
        sum4 == sum(p8)) {
      gl_FragColor = p4;
    } else {
      gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }

    gl_FragColor.a = 1.0;
}
