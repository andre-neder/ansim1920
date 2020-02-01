#version 450

layout(location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main(){
	gl_Position = projMatrix * viewMatrix * modelMatrix * position;
}