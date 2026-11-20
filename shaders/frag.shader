#version 460      

// DON'T FUCK WITH THESE -_-
out vec4 FragColor;
in  vec2 vUv;

uniform sampler2D uTexture;
uniform float uTime;
uniform vec2 uResolution;

void main(){

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / uResolution;

 
    vec4 TexColor=texture(uTexture,vUv);
    float Fade= sin(uTime) * 0.5 + 0.5;	
    vec4 Tinted= TexColor * vec4(1.0,0.0,0.0,1.0);

    // Output to screen
    FragColor=mix(TexColor,Tinted,Fade);
    
}