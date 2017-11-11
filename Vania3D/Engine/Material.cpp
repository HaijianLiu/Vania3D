
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Material::Material(Shader* shader) {
	this->shader = shader;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Material::~Material() {

}


/*------------------------------------------------------------------------------
< add texture >
------------------------------------------------------------------------------*/
void Material::addTexture(const char* textureUniform, Texture* texture) {
	this->textures.push_back(texture);
	this->texturesMapping.insert(std::make_pair(textureUniform, this->index));
	this->index++;
}


/*------------------------------------------------------------------------------
< set uniform locations >
------------------------------------------------------------------------------*/
void Material::setUniformLocations() {
	for (const auto & it : this->texturesMapping)
		this->shader->setInt(it.first, it.second);
}


/*------------------------------------------------------------------------------
< bind textures >
------------------------------------------------------------------------------*/
void Material::bindTextures() {
	for (unsigned int i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]->id);
	}
	
	// for test
	this->shader->setVec3(UNIFORM_VEC3_COLOR, this->color);
}
