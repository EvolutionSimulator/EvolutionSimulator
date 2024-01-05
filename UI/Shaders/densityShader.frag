uniform vec2 resolution;
uniform vec2 viewTopLeft;   // Top-left corner of the view
uniform vec2 viewSize;      // Size of the view (taking zoom into account)
uniform float maxDensity;
uniform sampler2D densityTexture;

void main() {
    // Adjust for the difference in y-coordinate systems
    vec2 correctedViewTopLeft = vec2(resolution.x - viewTopLeft.x -viewSize.x, viewTopLeft.y);
    vec2 normalizedPosition = (gl_FragCoord.xy - correctedViewTopLeft) / viewSize;
    vec2 uv = fract(normalizedPosition);  // Wrap around using the fractional part

    float density = texture(densityTexture, uv).g;

    // Normalize the density value to a range of 0-1
    float normalizedDensity = clamp(density / maxDensity, 0.0, 1.0);

    // Set the color based on the density
    vec3 color = vec3(0.0, normalizedDensity, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
