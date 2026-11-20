#version 460      
// !! DON'T REMOVE THEM IF YOU'RE GONNA USE THEM
out vec4 FragColor;         // Output color 
in  vec2 vUv;               // texture coordinates

uniform sampler2D uTexture; // texture slot
uniform float uTime;        // shader playback time in seconds
uniform vec2 uResolution;   // xy: viewport resolution in pixels


void main(){

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / uResolution;

 
    vec4 TexColor=texture(uTexture,vUv);
    float Fade= sin(uTime) * 0.5 + 0.5;	
    vec4 Tinted= TexColor * vec4(1.0,0.0,0.0,1.0);

    // Output to screen
    FragColor=mix(TexColor,Tinted,Fade);
    
}