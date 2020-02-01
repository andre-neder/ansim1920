#version 450 core
#extension GL_ARB_bindless_texture : require
#extension GL_NV_gpu_shader5:require
#define PI 3.14159265359
#define MAX_LIGHTS 5
in vec4 passPosition;
in vec4 passPositionLightSpace[MAX_LIGHTS];
in vec3 passNormal;
in vec2 passUv;
in mat3 TBN;

out vec4 fragColor;

uniform mat4 viewMatrix;
uniform vec3 eye;
uniform uint renderShadows;

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

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

uniform struct{
	vec3 albedo;
	float roughness;
	float metallic;
	float ao;
	uint64_t albedoTexture;
	uint64_t roughnessTexture;
	uint64_t metallicTexture;
	uint64_t aoTexture;
	uint64_t normalMap;
}material;

float DistributionGGX(float NdotH, float roughness){
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH2 = NdotH*NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return a2 / max(denom, 0.001);
}

float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(float NdotV , float NdotL, float roughness){
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main(){ 
	//Material
	vec3 mat_albedo = material.albedo;
	float mat_roughness = material.roughness;
	float mat_metallic = material.metallic;
	float mat_ao = material.ao;
	vec3 N = passNormal;
	if(material.albedoTexture !=0){
		sampler2D cTexture = sampler2D( material.albedoTexture);
		mat_albedo = texture( cTexture, passUv).rgb;
	}
	if(material.roughnessTexture !=0){
		sampler2D cTexture = sampler2D( material.roughnessTexture);
		mat_roughness = texture( cTexture, passUv).r;
	}
	if(material.metallic !=0){
		sampler2D cTexture = sampler2D( material.metallicTexture);
		mat_metallic = texture( cTexture, passUv).r;
	}
	if(material.ao !=0){
		sampler2D cTexture = sampler2D( material.aoTexture);
		mat_ao = texture( cTexture, passUv).r;
	}
	if(material.normalMap !=0){
		sampler2D cTexture = sampler2D( material.normalMap);
		N = normalize(texture( cTexture, passUv).rgb);
		N = N * 2.0 -1.0;
		N= normalize(TBN * N);
	}

    vec3 V = normalize((eye - passPosition.xyz));
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, mat_albedo, material.metallic);
	float NdotV = max(dot(N, V),0.0); 
    // reflectance equation
    vec3 Lo = vec3(0.0);
	//per Light Calculations
	for(int i = 0;i<MAX_LIGHTS;i++){
		vec3 L = normalize((lights[i].position - passPosition).xyz);
		vec3 H = normalize(V + L);
		//Attenuation
		float dist    = length(lights[i].position.xyz - passPosition.xyz);
		float attenuation = 1.0 /( 1.0 + lights[i].attenuation.x* dist + lights[i].attenuation.y *(dist * dist));
		vec3 radiance     = lights[i].color.xyz * lights[i].color.w * attenuation;
		float theta = dot(L,normalize(-lights[i].direction.xyz)) ;
		if(lights[i].position.w == 0.0f){ 
			if(lights[i].cutoff == 0.0f){
				L = normalize(-lights[i].direction);
				H = normalize(V + L);
			}
			radiance = lights[i].color.xyz * lights[i].color.w;
		}
		if(lights[i].position.w == 0.0f && lights[i].cutoff != 0.0f &&  theta < cos(radians(lights[i].cutoff))){}
		else{
			float NdotL = max(dot(N, L), 0.0);
			float NdotH = max(dot(N, H), 0.0);
			// cook-torrance brdf
			float NDF = DistributionGGX(NdotH, mat_roughness);        
			float G   = GeometrySmith(NdotV, NdotL, mat_roughness);      
			vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - mat_metallic;	  
        
			vec3 numerator    = NDF * G * F;
			float denominator = 4.0 * NdotV * NdotL;
			vec3 specular     = numerator / max(denominator, 0.001);  

			//Shadow
			float shadow = 0.0;

			//Directional
			if(lights[i].shadowMapID != 0 && lights[i].position.w == 0 && renderShadows == 1){
				float bias = max(0.003*tan(acos(NdotL)),0.003);
				sampler2D shadowMap = sampler2D( lights[i].shadowMapID);
				vec3 shadowCoords = (passPositionLightSpace[i].xyz / passPositionLightSpace[i].w);
				for (int i=0;i<4;i++){
					if ( texture( shadowMap, shadowCoords.xy + poissonDisk[i]/700.0 ).r  <  shadowCoords.z - bias && shadowCoords.z < 1.0 ){
						shadow+=0.25f;
					}
				}
			}
			//Omni-Directional
			else if(lights[i].shadowMapID != 0 && lights[i].position.w == 1 && renderShadows == 1){
				float bias = max(0.003*tan(acos(NdotL)),0.003);
				samplerCube shadowMap = samplerCube( lights[i].shadowMapID);
				float nearestDepth = texture( shadowMap, -L).r;
				float currentDepth = dist/lights[i].farPlane;
				if(nearestDepth < currentDepth - bias && nearestDepth < 1.0){
					shadow = 1.0f;
				}
			}
			Lo += (kD * mat_albedo / PI + specular) * radiance * NdotL * (1.0-shadow); 
		}
	}
    vec3 ambient = vec3(0.03) * mat_albedo; //* mat_ao;
    vec3 color = ambient + Lo;
	//Gamma Correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
	
    fragColor = vec4(color, 1.0);
}