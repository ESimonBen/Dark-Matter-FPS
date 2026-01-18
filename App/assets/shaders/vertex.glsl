#version 330 core // Most recent version of OpenGL

layout(location=0) in vec3 position; // The position variable is stored in location 0
layout(location=1) in vec3 color; // The color variable is stored in location 1

out vec4 vertex_color;

uniform mat4 model;
uniform mat4 viewProjection;

void main(){
	gl_Position = viewProjection  * model * vec4(position, 1.0);
	vertex_color = vec4(color, 1.0);
}