#version 460 core

layout(location=0) in vec3 aPosition;
layout(location=1) in vec2 aTextCoord;

// varying : vertex shader to fragShader
out vec2 vUv;

// Model view project matrix :TODO

void main(){
	gl_Position=vec4(aPosition,1.0);
	vUv=aTextCoord;
}
