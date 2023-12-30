uniform sampler2D texture;
uniform float hueShift;

// Code taken from http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec3 hsv = rgb2hsv(pixel.rgb);

    hsv.x += hueShift;

    if (hsv.x > 1.0) { // Assuming hsv.x is normalized [0, 1]
        hsv.x -= 1.0;
    } else if (hsv.x < 0.0) { // Also adjust if hueShift is negative and causes wrap around
        hsv.x += 1.0;
    }

    // Convert back to RGB and output the color
    gl_FragColor = vec4(hsv2rgb(hsv), pixel.a);
}

