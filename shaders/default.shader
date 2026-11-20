#version 460
// !! DON'T REMOVE THEM IF YOU'RE GONNA USE THEM
out vec4 FragColor;         // Output color 
in  vec2 vUv;               // texture coordinates

uniform sampler2D uTexture; // texture slot
uniform float uTime;        // shader playback time in seconds
uniform vec2 uResolution;   // xy: viewport resolution in pixels
uniform vec2 uMouse;        // xy: mouse coordinates


void main() {

    // Normalized pixel coordinates (from 0 to 1)
    vec2 npc = gl_FragCoord.xy / uResolution;


    // Knobs
    const float Radius = 0.20; 
    const float WaveFreq = 70.0;  
    const float WaveSpeed = 7.0;   
    const float WaveAmp = 0.012; 
    const float Magnify = 0.10; 
    const float Aberr = 0.010; 
    const float Dim = 0.35;  

    float aspect  = uResolution.x / uResolution.y;
    vec2  mouseUv = uMouse / uResolution;

 
    vec2  delta = (vUv - mouseUv) * vec2(aspect, 1.0);
    float dist  = length(delta);
    vec2  dir   = dist > 1e-4 ? delta / dist : vec2(0.0);


    float influence = 1.0 - smoothstep(0.0, Radius, dist);

    float wave   = sin(dist * WaveFreq - uTime * WaveSpeed) * influence;
    vec2  ripple = dir * wave * WaveAmp;
    ripple.x /= aspect; 

    vec2 uv = vUv + ripple + (mouseUv - vUv) * influence * influence * Magnify;

    // Chromatic aberration split RGB 
    vec2 split = dir * influence * Aberr;
    split.x /= aspect;
    vec3 color = vec3(texture(uTexture, uv + split).r,
                      texture(uTexture, uv).g,
                      texture(uTexture, uv - split).b);

    //  dim everything far from the mouse
    color *= mix(Dim, 1.0, 1.0 - smoothstep(0.0, 0.9, dist));

    // Output to screen
    FragColor = vec4(color, 1.0);
}