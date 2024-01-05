// Updated shader code
uniform vec2 resolution;
uniform vec2 viewTopLeft;   // Top-left corner of the view
uniform float zoomFactor;   // New uniform for the zoom factor
uniform float maxDensity;
uniform sampler2D densityTexture;

void main() {
    // Adjust texture coordinates to account for zoom and wrap around
    vec2 uv = (gl_FragCoord.xy - viewTopLeft) / (resolution * zoomFactor);
    uv = fract(uv); // Use the fractional part for wrapping

    float density = texture(densityTexture, uv).g;

    // Normalize the density value to a range of 0-1
    float normalizedDensity = clamp(density / maxDensity, 0.0, 1.0);

    // Set the color based on the density
    vec3 color = vec3(0.0, normalizedDensity, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
