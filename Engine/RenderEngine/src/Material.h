#pragma once
#include <string>
#include "glm/ext.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Texture.h"
#include "ShaderSet.h"

struct MaterialStruct {
	glm::vec3 albedo;
	float roughness;
	float metallic;
	float ao;
	GLuint64 albedoTextureID;
	GLuint64 roughnessTextureID;
	GLuint64 metallicTextureID;
	GLuint64 aoTextureID;
	GLuint64 normalMapID;
};

class Material
{
public:
	Material();
	~Material();
	Material(glm::vec3 albedo,float roughness,float metallic);
	Material(std::string albedoTexture, float roughness, float metallic);
	Material(std::string albedoTexture, std::string roughnessTexture, std::string metallicTexture, std::string aoTexture, std::string normalMap);
	MaterialStruct getMaterial();
	void uploadMaterial(ShaderSet* shader);

protected:
	
	MaterialStruct m_material;
	
	Texture* m_albedoTexture;
	Texture* m_roughnessTexture;
	Texture* m_metallicTexture;
	Texture* m_aoTexture;
	Texture* m_normalMap;
};

