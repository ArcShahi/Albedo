#version 460         
out vec4 FragColor;
in  vec2 vUv;

uniform sampler2D uTexture;
uniform float uTime;

void main(){

     float fade= sin(uTime)/2.0+0.5f;
     vec4 TextColor=texture(uTexture,vUv);
	TextColor*=fade;
	//textColor=textColor*vec4(0.0,0.0,1.0,1.0);
     FragColor = TextColor;
}