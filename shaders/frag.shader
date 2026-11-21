

void main(){

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / uResolution;

 
    vec4 TexColor=texture(uTexture,vUv);
    float Fade= sin(uTime) * 0.5 + 0.5;	
    vec4 Tinted= TexColor * vec4(1.0,0.0,0.0,1.0);

    // Output to screen
    FragColor=mix(TexColor,Tinted,Fade);
    
}