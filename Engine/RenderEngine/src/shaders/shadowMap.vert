#version 450

layout(location = 0) in vec4 position;

out vec4 passPos;

uniform mat4 modelMatrix;
uniform mat4 lightMatrix;

void main(){
	passPos = modelMatrix * position;
	gl_Position =  lightMatrix * passPos;
}