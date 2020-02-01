#version 450
#extension GL_ARB_bindless_texture : require
#extension GL_NV_gpu_shader5:require
#define MAX_LIGHTS 5
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
//layout (location = 4) in vec3 bitangent;

struct light{
	vec4 color;
	vec4 position;
	vec3 direction;
	float cutoff;
	vec2 attenuation;
	uint64_t shadowMapID;
	mat4 lightMatrix;
	float farPlane, pad0, pad1, pad2;
};

layout( std430, binding = 0) buffer lightBuffer{
	light lights[];
};


uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 passPosition;
out vec4 passPositionLightSpace[MAX_LIGHTS];
out vec3 passNormal;
out vec2 passUv;
out mat3 TBN;

mat4 biasMatrix = mat4(
0.5, 0.0, 0.0, 0.0,
0.0, 0.5, 0.0, 0.0,
0.0, 0.0, 0.5, 0.0,
0.5, 0.5, 0.5, 1.0
);

void main()
{
	vec3 T = normalize(vec3(modelMatrix*vec4(tangent,0.0)));
	vec3 N = normalize(vec3(modelMatrix*vec4(normal,0.0)));
	//vec3 B = normalize(vec3(normalMatrix*vec4(bitangent,0.0)));
	//T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N,T));
	if (dot(cross(N, T), B) < 0.0f){
		T = T * -1.0f;
	}
	passPosition = modelMatrix * position;
	for(int i = 0;  i < MAX_LIGHTS;i++){
		passPositionLightSpace[i] = biasMatrix * lights[i].lightMatrix * passPosition;
	}
	TBN = mat3(T,B,N);
	passUv =  uv;
	passNormal = normalize(mat3(normalMatrix) * normal);
	gl_Position = projMatrix * viewMatrix * passPosition;
}