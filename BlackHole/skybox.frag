#version 410 core

uniform samplerCube skybox;
uniform vec3 cameraPos;
uniform float mass;

in vec3 TexCoords;
out vec4 FragColor;

const float PI = 3.141592653589793;
const float CRIT_B = 5.196152422706632; // 3 * sqrt(3)

vec3 rotate(vec3 v, vec3 k, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return v * cosA + cross(k, v) * sinA + k * dot(k, v) * (1.0 - cosA);
}

float calculateDelta(float b) {
    float coeff = 2.0 * mass / b;
    
    // Newton's method
    float w1 = 1.0;
    for (int i = 0; i < 10; i++) {
        float f = 1.0 - w1 * w1 + coeff * w1 * w1 * w1;
        float df = -2.0 * w1 + 3.0 * coeff * w1 * w1;
        w1 -= f / df;
        if (abs(f) < 1e-8) { break; }
    }
    w1 = min(w1, 1.0);
    
    // midpoint method
    const int N = 250;
    float dw = w1 / float(N);
    float I = 0.0;
    
    for (int i = 0; i < N; i++) {
        float w = (float(i) + 0.5) * dw;
        float numen = 1 + coeff * w * 0.5;
        float denom = sqrt(max(1e-8, 1.0 - w * w + coeff * w));
        I += dw * numen / denom;
    }
    
    return 2.0 * I - PI;
}

void main() {
    vec3 rayDir = normalize(TexCoords);
    vec3 front = normalize(-cameraPos); // assuming target is at (0,0,0)
    float r = length(cameraPos);
    
    float sinAlpha = length(cross(rayDir, front));
    float b = r * sinAlpha;
    
    float b_crit = CRIT_B * mass;
    
    if (b <= b_crit) {
        // horizon line
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    
    vec3 axis = cross(front, rayDir);
    float axisLen = length(axis);
    if (axisLen < 1e-6) { 
        // parallel means straight into hole anyway
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }
    axis /= axisLen;
    
    float delta = calculateDelta(b);

    // === dimming ===
    
    float b_safe = b_crit * 2.0;  // cutoff for dimming
    float t = smoothstep(b_crit, b_safe, b);
    
    float dimming; // [0,1]
    if (delta > 2.0 * PI) {
        // the ray loops around the black hole
        float winds = floor(delta / (2.0 * PI));
        delta = mod(delta, 2.0 * PI);
        dimming = exp(-winds * 0.5) * t;
    } else {
        dimming = t;
    }

    // ===============
    
    vec3 sourceDir = rotate(rayDir, axis, delta);
    vec3 color = texture(skybox, sourceDir).rgb * dimming;
    
    FragColor = vec4(color, 1.0);
}