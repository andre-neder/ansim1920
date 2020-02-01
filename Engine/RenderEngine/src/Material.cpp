#include "Material.h"

Material::Material()
{
	m_material.albedo = glm::vec3(1.0);
	m_material.roughness = 0.5;
	m_material.metallic = 0.0;
	m_material.ao = 1.0f;
	m_material.albedoTextureID = 0;
	m_material.roughnessTextureID = 0;
	m_material.metallicTextureID = 0;
	m_material.aoTextureID = 0;
	m_material.normalMapID = 0;
	m_albedoTexture = nullptr;
	m_roughnessTexture = nullptr;
	m_metallicTexture = nullptr;
	m_aoTexture = nullptr;
	m_normalMap = nullptr;
}

Material::~Material()
{
	m_albedoTexture->~Texture();
	m_roughnessTexture->~Texture();
	m_metallicTexture->~Texture();
	m_aoTexture->~Texture();
	m_normalMap->~Texture();
}

Material::Material(glm::vec3 albedo, float roughness, float metallic)
{
	m_material.albedo = albedo;
	m_material.roughness = roughness;
	m_material.metallic = metallic;
	m_material.ao = 1.0f;
	m_material.albedoTextureID = 0;
	m_material.roughnessTextureID = 0;
	m_material.metallicTextureID = 0;
	m_material.aoTextureID = 0;
	m_material.normalMapID = 0;
	m_albedoTexture = nullptr;
	m_roughnessTexture = nullptr;
	m_metallicTexture = nullptr;
	m_aoTexture = nullptr;
	m_normalMap = nullptr;
}

Material::Material(std::string albedoTexture, float roughness, float metallic)
{
	m_material.albedo = glm::vec3(1.0f);
	m_material.roughness = roughness;
	m_material.metallic = metallic;
	m_material.ao = 0.0f;
	m_material.albedoTextureID = 0;
	m_material.roughnessTextureID = 0;
	m_material.metallicTextureID = 0;
	m_material.aoTextureID = 0;
	m_material.normalMapID = 0;
	m_albedoTexture = nullptr;
	m_roughnessTexture = nullptr;
	m_metallicTexture = nullptr;
	m_aoTexture = nullptr;
	m_normalMap = nullptr;
	if (albedoTexture.empty() == false) {
		m_albedoTexture = &Texture(albedoTexture);
		m_material.albedoTextureID = m_albedoTexture->getHandleARB();
	}
}

Material::Material(std::string albedoTexture, std::string roughnessTexture, std::string metallicTexture, std::string aoTexture, std::string normalMap)
{
	m_material.albedo = glm::vec3(1.0f);
	m_material.roughness = 0.5f;
	m_material.metallic = 0.0f;
	m_material.ao = 1.0f;
	m_material.albedoTextureID = 0;
	m_material.roughnessTextureID = 0;
	m_material.metallicTextureID = 0;
	m_material.aoTextureID = 0;
	m_material.normalMapID = 0;
	m_albedoTexture = nullptr;
	m_roughnessTexture = nullptr;
	m_metallicTexture = nullptr;
	m_aoTexture = nullptr;
	m_normalMap = nullptr;
	if (albedoTexture.empty() == false) {
		m_albedoTexture = &Texture(albedoTexture);
		m_material.albedoTextureID = m_albedoTexture->getHandleARB();
	}
	if (roughnessTexture.empty() == false) {
		m_roughnessTexture = &Texture(roughnessTexture);
		m_material.roughnessTextureID = m_roughnessTexture->getHandleARB();
	}
	if (metallicTexture.empty() == false) {
		m_metallicTexture = &Texture(metallicTexture);
		m_material.metallicTextureID = m_metallicTexture->getHandleARB();
	}
	if (aoTexture.empty() == false) {
		m_aoTexture = &Texture(aoTexture);
		m_material.aoTextureID = m_aoTexture->getHandleARB();
	}
	if (normalMap.empty()==false) {
		m_normalMap = &Texture(normalMap);
		m_material.normalMapID = m_normalMap->getHandleARB();
	}

}

MaterialStruct Material::getMaterial()
{
	return m_material;
}

void Material::uploadMaterial(ShaderSet* shader)
{
	glUniform3fv(glGetUniformLocation(shader->getProgramID(), "material.albedo"), 1, glm::value_ptr(m_material.albedo));
	glUniform1f(glGetUniformLocation(shader->getProgramID(), "material.metallic"), m_material.metallic);
	glUniform1f(glGetUniformLocation(shader->getProgramID(), "material.ao"), m_material.ao);
	glUniform1f(glGetUniformLocation(shader->getProgramID(), "material.roughness"), m_material.roughness);
	glUniform1ui64NV(glGetUniformLocation(shader->getProgramID(), "material.albedoTexture"), m_material.albedoTextureID);
	glUniform1ui64NV(glGetUniformLocation(shader->getProgramID(), "material.roughnessTexture"), m_material.roughnessTextureID);
	glUniform1ui64NV(glGetUniformLocation(shader->getProgramID(), "material.aoTexture"), m_material.aoTextureID);
	glUniform1ui64NV(glGetUniformLocation(shader->getProgramID(), "material.normalMap"), m_material.normalMapID);
	glUniform1ui64NV(glGetUniformLocation(shader->getProgramID(), "material.metallicTexture"), m_material.metallicTextureID);
}
