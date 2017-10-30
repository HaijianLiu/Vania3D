
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
	// this->loadModel("vampire", "./Assets/Models/Maw/Maw_J_Laygo.FBX");
	// this->loadModel("vampire_walk", "./Assets/Models/Maw/Maw_J_Laygo_Walk.FBX");
	// this->getModel("vampire")->animations.push_back(this->getModel("vampire_walk")->animations[0]);

	// this->loadModel("vampire", "./Assets/Models/Maria/Maximo_Maria.FBX");
	// this->loadModel("vampire_walk", "./Assets/Models/Maria/Maria_J_J_Ong_Walk.FBX");
	// this->getModel("vampire")->animations.push_back(this->getModel("vampire_walk")->animations[0]);

	// this->loadModel("vampire", "./Assets/Models/Ganfaul/Mixamo_Ganfault_Aure.FBX");
	// Animation* animation = new Animation("./Assets/Models/Ganfaul/Mixamo_Ganfault_idle.FBX");
	this->loadModel("vampire", "./Assets/Models/Ganfaul/mixamo_model.fbx");
	this->getModel("vampire")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_stay.fbx"));
	this->getModel("vampire")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_look.fbx"));
	this->getModel("vampire")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_walk.fbx"));
	this->getModel("vampire")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_run.fbx"));
	this->getModel("vampire")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_swiping.fbx"));


	// this->loadModel("vampire", "./Assets/Models/Heraklios/Mixamo_Heraklios.FBX");
	// Animation* animation = new Animation("./Assets/Models/Heraklios/Mixamo_Heraklios_Walk.FBX");
	// this->getModel("vampire")->animations.push_back(animation);

	// this->loadModel("vampire", "./Assets/Models/Vanguard/Vanguard_by_T__Choonyung.FBX");
	// this->loadModel("vampire_walk", "./Assets/Models/Vanguard/Vanguard_by_T__Choonyung_Walk.FBX");
	// this->getModel("vampire")->animations.push_back(this->getModel("vampire_walk")->animations[0]);

	// this->loadModel("vampire", "./Assets/Models/Goblin/Mixamo_Goblin.FBX");
	// this->loadModel("vampire_walk", "./Assets/Models/Goblin/Mixamo_Goblin_Walk.FBX");
	// this->getModel("vampire")->animations.push_back(this->getModel("vampire_walk")->animations[0]);




	/* Texture
	..............................................................................*/
	// this->loadTexture("WPN_AKM_albedo", "./Assets/Models/WPN_AKM/WPNT_AKM_Base_Color.tga");
	// this->loadTexture("WPN_AKM_normal", "./Assets/Models/WPN_AKM/WPNT_AKM_DirectX.tga");
	// this->loadTexture("WPN_AKM_metallic", "./Assets/Models/WPN_AKM/WPNT_AKM_Metallic.tga");
	// this->loadTexture("WPN_AKM_roughness", "./Assets/Models/WPN_AKM/WPNT_AKM_Roughness.tga");
	// this->loadTexture("WPN_AKM_ao", "./Assets/Models/WPN_AKM/WPNT_AKM_Ambient_occlusion.tga");

	// this->loadTexture("vampire_albedo", "./Assets/Models/Maw/MAW_diffuse.TGA");
	// this->loadTexture("vampire_normal", "./Assets/Models/Maw/MAW_normal.TGA");
	// this->loadTexture("vampire_mask", "./Assets/Models/Maw/MAW_mask.TGA");

	// this->loadTexture("vampire_albedo", "./Assets/Models/Maria/maria_diffuse.TGA");
	// this->loadTexture("vampire_normal", "./Assets/Models/Maria/maria_normal.TGA");
	// this->loadTexture("vampire_mask", "./Assets/Models/Maria/maria_mask.TGA");

	this->loadTexture("vampire_albedo", "./Assets/Models/Ganfaul/Ganfaul_diffuse.TGA");
	this->loadTexture("vampire_normal", "./Assets/Models/Ganfaul/Ganfaul_normal.TGA");
	this->loadTexture("vampire_mask", "./Assets/Models/Ganfaul/Ganfaul_mask.TGA");

	// this->loadTexture("vampire_albedo", "./Assets/Models/Heraklios/battalion-leader-heraklios_diffuse.TGA");
	// this->loadTexture("vampire_normal", "./Assets/Models/Heraklios/battalion-leader-heraklios_normal.TGA");
	// this->loadTexture("vampire_mask", "./Assets/Models/Heraklios/battalion-leader-heraklios_mask.TGA");

	// this->loadTexture("vampire_albedo", "./Assets/Models/Vanguard/vanguard_diffuse.TGA");
	// this->loadTexture("vampire_normal", "./Assets/Models/Vanguard/vanguard_normal.TGA");
	// this->loadTexture("vampire_mask", "./Assets/Models/Vanguard/vanguard_mask.TGA");

	// this->loadTexture("vampire_albedo", "./Assets/Models/Goblin/goblin_diffuse.TGA");
	// this->loadTexture("vampire_normal", "./Assets/Models/Goblin/goblin_normal.TGA");
	// this->loadTexture("vampire_mask", "./Assets/Models/Goblin/goblin_mask.TGA");


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
