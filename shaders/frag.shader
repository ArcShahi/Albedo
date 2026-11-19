#version 460         
out vec4 FragColor;
in  vec2 vUv;

uniform sampler2D uTexture;
uniform float uTime;
uniform vec2 uResolution;

void main(){
     vec4 TexColor=texture(uTexture,vUv);
     float Fade= sin(uTime) * 0.5 + 0.5;
	
     vec4 Tinted= TexColor * vec4(1.0,0.0,0.0,1.0);
     FragColor=mix(TexColor,Tinted,Fade);
    
}