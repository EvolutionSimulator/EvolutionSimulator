uniform vec2 resolution;
uniform float maxDensity;
uniform sampler2D densityTexture;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    float density = texture2D(densityTexture, uv).g; // Assuming density is stored in the red channel

    // Normalize the density value to a range of 0-1
    float normalizedDensity = clamp(density / maxDensity, 0.0, 1.0);

    // Set the color based on the density
    vec3 color = vec3(0.0, normalizedDensity, 0.0);

    gl_FragColor = vec4(color, 1.0);
}
