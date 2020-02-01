#version 450

in vec4 passPos;
uniform vec4 lightPos;
uniform float farPlane;

void main(){
	if(lightPos.w ==1.0){
		float depth = length(passPos.xyz -lightPos.xyz);
		depth /= farPlane;
		gl_FragDepth = depth;
	}
	else{
		gl_FragDepth = gl_FragCoord.z;
	}
}
