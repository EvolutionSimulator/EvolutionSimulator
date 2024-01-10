// stomach_shader.frag
uniform float fullness; // The ratio of fullness, range [0,1]
uniform sampler2D texture; // The texture of the sprite
uniform vec4 fillColor; // The fill color (RGBA)

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texColor = texture2D(texture, uv);

    // Identify if the pixel color is within the gray range we want to fill
    float colorIntensity = (texColor.r + texColor.g + texColor.b) / 3.0;
    bool isGray = abs(texColor.r - colorIntensity) < 0.1 && abs(texColor.g - colorIntensity) < 0.1 && abs(texColor.b - colorIntensity) < 0.1;

    // Calculate the y-coordinate threshold for the current fullness level
    // The factors are included due to the relationship between the full texture (with transparent background) and the actual sprite
    float fillThreshold = 0.75 - fullness*7/16;

    // If the pixel is in the gray area and below the fill threshold, fill it with yellow
    if (uv.y > fillThreshold && isGray) {
        // Blend the yellow color with the texture's grayscale value
        gl_FragColor = mix(texColor, fillColor, colorIntensity);
    } else {
        // Otherwise, keep the original texture color
        gl_FragColor = texColor;
    }
}
