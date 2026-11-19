// !!! DON'T MODIFY IT , WE DON'T HAVE VERTEX BUFFER TO PROVIDE VERTEX DATA 
// AND  NO VERTEX ARRAY TO LINK ATTRIBUTES TO DESCRIBE THE LAYOUT

#version 460 core
out vec2 vUv;

void main() {
    vec2 p = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2); // (0,0) (2,0) (0,2)
    vUv = p;                                                // covers 0..1 inside the screen
    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);
}