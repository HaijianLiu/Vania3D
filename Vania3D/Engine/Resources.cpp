
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Resources::Resources() {

}

Resources* Resources::getInstance() {
	static Resources* resources = new Resources();
	return resources;
}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Resources::~Resources() {
	deleteMap(this->shaders);
	deleteMap(this->models);
	deleteMap(this->textures);
	deleteMap(this->lightProbes);
	deleteMap(this->materials);
	delete this->quad;
	delete this->skybox;
}


/*------------------------------------------------------------------------------
< start > before GameObject defaultStart()
------------------------------------------------------------------------------*/
void Resources::start() {
	/* Meshes
	..............................................................................*/
	this->quad = new Quad();
	this->skybox = new Skybox();

	/* Shader
	..............................................................................*/
	this->loadShader("renderPass", "./Assets/Shaders/RenderPassCombineDeferredPBR.vs.glsl",  "./Assets/Shaders/RenderPassCombineDeferredPBR.fs.glsl");
	// this->loadShader("deferredPBR", "./Assets/Shaders/deferredPBR.vs.glsl",  "./Assets/Shaders/deferredPBR.fs.glsl");
	this->loadShader("deferredPBRforUEmask", "./Assets/Shaders/deferredPBRforUEmask.vs.glsl",  "./Assets/Shaders/deferredPBRforUEmask.fs.glsl");
	this->loadShader("equirectangularToCubemap", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/equirectangularToCubemap.fs.glsl");
	this->loadShader("irradianceConvolution", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/irradianceConvolution.fs.glsl");
	this->loadShader("prefilter", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/prefilter.fs.glsl");
	this->loadShader("brdf", "./Assets/Shaders/brdf.vs.glsl",  "./Assets/Shaders/brdf.fs.glsl");
	// this->loadShader("background", "./Assets/Shaders/background.vs.glsl",  "./Assets/Shaders/background.fs.glsl");

	/* Model
	..............................................................................*/
	// this->loadModel("WPN_AKM", "./Assets/Models/WPN_AKM/WPN_AKM.obj");
	this->loadModel("Maw_J_Laygo", "./Assets/Models/Mixamo_Maw/Maw_J_Laygo_blender.FBX");

	/* Texture
	..............................................................................*/
	// this->loadTexture("WPN_AKM_albedo", "./Assets/Models/WPN_AKM/WPNT_AKM_Base_Color.tga");
	// this->loadTexture("WPN_AKM_normal", "./Assets/Models/WPN_AKM/WPNT_AKM_DirectX.tga");
	// this->loadTexture("WPN_AKM_metallic", "./Assets/Models/WPN_AKM/WPNT_AKM_Metallic.tga");
	// this->loadTexture("WPN_AKM_roughness", "./Assets/Models/WPN_AKM/WPNT_AKM_Roughness.tga");
	// this->loadTexture("WPN_AKM_ao", "./Assets/Models/WPN_AKM/WPNT_AKM_Ambient_occlusion.tga");
	this->loadTexture("Maw_J_Laygo_albedo", "./Assets/Models/Mixamo_Maw/MAW_diffuse.TGA");
	this->loadTexture("Maw_J_Laygo_normal", "./Assets/Models/Mixamo_Maw/MAW_normal.TGA");
	this->loadTexture("Maw_J_Laygo_mask", "./Assets/Models/Mixamo_Maw/MAW_mask.TGA");

	/* Material
	..............................................................................*/
	// this->createMaterial("WPN_AKM", getShader("deferredPBR"));
	// this->materials["WPN_AKM"]->addTexture("albedoMap", this->textures["WPN_AKM_albedo"]);
	// this->materials["WPN_AKM"]->addTexture("normalMap", this->textures["WPN_AKM_normal"]);
	// this->materials["WPN_AKM"]->addTexture("metallicMap", this->textures["WPN_AKM_metallic"]);
	// this->materials["WPN_AKM"]->addTexture("roughnessMap", this->textures["WPN_AKM_roughness"]);
	// this->materials["WPN_AKM"]->addTexture("aoMap", this->textures["WPN_AKM_ao"]);
	// this->createMaterial("Maw_J_Laygo", getShader("deferredPBRforUEmask"));
	// this->materials["Maw_J_Laygo"]->addTexture("albedoMap", this->textures["Maw_J_Laygo_albedo"]);
	// this->materials["Maw_J_Laygo"]->addTexture("normalMap", this->textures["Maw_J_Laygo_normal"]);
	// this->materials["Maw_J_Laygo"]->addTexture("maskMap", this->textures["Maw_J_Laygo_mask"]);

	/* LightProbe
	..............................................................................*/
	this->loadLightProbe("hdr", "./Assets/Textures/HDR/Road_to_MonumentValley_8k.jpg");
}


/*------------------------------------------------------------------------------
< Load & Get >
------------------------------------------------------------------------------*/
void Resources::loadShader(const char* name, const char* vertexPath, const char* fragmentPath) {
	this->shaders.insert(std::make_pair(name, new Shader(vertexPath,fragmentPath)));
}
Shader* Resources::getShader(const char* name) {
	return this->shaders[name];
}
void Resources::loadTexture(const char* name, const char* path) {
	this->textures.insert(std::make_pair(name, new Texture(path)));
}
Texture* Resources::getTexture(const char* name) {
	return this->textures[name];
}
void Resources::loadModel(const char* name, const char* path) {
	this->models.insert(std::make_pair(name, new Model(path)));
}
Model* Resources::getModel(const char* name) {
	return this->models[name];
}
void Resources::loadLightProbe(const char* name, const char* path) {
	this->lightProbes.insert(std::make_pair(name, new LightProbe(path)));
}
LightProbe* Resources::getLightProbe(const char* name) {
	return this->lightProbes[name];
}
void Resources::createMaterial(const char* name, Shader* shader) {
	this->materials.insert(std::make_pair(name, new Material(shader)));
}
Material* Resources::getMaterial(const char* name) {
	return this->materials[name];
}
