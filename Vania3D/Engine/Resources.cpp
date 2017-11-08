
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
	this->loadShader("deferredPBRforUEmask", "./Assets/Shaders/deferredPBRforUEmask.vs.glsl",  "./Assets/Shaders/deferredPBRforUEmask.fs.glsl");
	this->loadShader("equirectangularToCubemap", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/equirectangularToCubemap.fs.glsl");
	this->loadShader("irradianceConvolution", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/irradianceConvolution.fs.glsl");
	this->loadShader("prefilter", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/prefilter.fs.glsl");
	this->loadShader("brdf", "./Assets/Shaders/brdf.vs.glsl",  "./Assets/Shaders/brdf.fs.glsl");

	this->loadShader("simple", "./Assets/Shaders/simple.vs.glsl",  "./Assets/Shaders/simple.fs.glsl");

	// /* Model
	// ..............................................................................*/
	this->models.insert(std::make_pair("sphere", new Sphere()));
	//
	// this->loadModel("player", "./Assets/Models/Ganfaul/mixamo_model.fbx");
	// this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_stay.fbx"));
	// this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_look.fbx"));
	// this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_walk.fbx"));
	// this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_run.fbx"));
	// this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_swiping.fbx"));
	//
	// /* Texture
	// ..............................................................................*/
	// this->loadTexture("player_albedo", "./Assets/Models/Ganfaul/Ganfaul_diffuse.TGA");
	// this->loadTexture("player_normal", "./Assets/Models/Ganfaul/Ganfaul_normal.TGA");
	// this->loadTexture("player_mask", "./Assets/Models/Ganfaul/Ganfaul_mask.TGA");
	//
	// /* Material
	// ..............................................................................*/
	// this->createMaterial("player", this->getShader("deferredPBRforUEmask"));
	// this->getMaterial("player")->addTexture("albedoMap", this->getTexture("player_albedo"));
	// this->getMaterial("player")->addTexture("normalMap", this->getTexture("player_normal"));
	// this->getMaterial("player")->addTexture("maskMap", this->getTexture("player_mask"));


	// player
	this->loadModel("player", "./Assets/Models/PolygonDungeon/Characters/hero_male.fbx");
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/PolygonDungeon/Characters/hero_male_Idle.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/PolygonDungeon/Characters/hero_male_walking.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/PolygonDungeon/Characters/hero_male_running.fbx"));
	this->loadTexture("player_albedo", "./Assets/Models/PolygonDungeon/Textures/Dungeons_Texture_01.png");
	this->createMaterial("player", this->getShader("deferredPBRforUEmask"));
	this->getMaterial("player")->addTexture("albedoMap", this->getTexture("player_albedo"));

	this->createMaterial("simple", this->getShader("simple"));

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
	auto it = this->shaders.find(name);
	if (it != this->shaders.end()) {
		return this->shaders[name];
	}
	else {
		std::cout << "[Resources] no shader named " << name << '\n';
		return nullptr;
	}
}
void Resources::loadTexture(const char* name, const char* path) {
	this->textures.insert(std::make_pair(name, new Texture(path)));
}
Texture* Resources::getTexture(const char* name) {
	auto it = this->textures.find(name);
	if (it != this->textures.end()) {
		return this->textures[name];
	}
	else {
		std::cout << "[Resources] no texture named " << name << '\n';
		return nullptr;
	}
}
void Resources::loadModel(const char* name, const char* path) {
	this->models.insert(std::make_pair(name, new Model(path)));
}
Model* Resources::getModel(const char* name) {
	auto it = this->models.find(name);
	if (it != this->models.end()) {
		return this->models[name];
	}
	else {
		std::cout << "[Resources] no model named " << name << '\n';
		return nullptr;
	}
}
void Resources::loadLightProbe(const char* name, const char* path) {
	this->lightProbes.insert(std::make_pair(name, new LightProbe(path)));
}
LightProbe* Resources::getLightProbe(const char* name) {
	auto it = this->lightProbes.find(name);
	if (it != this->lightProbes.end()) {
		return this->lightProbes[name];
	}
	else {
		std::cout << "[Resources] no lightProbe named " << name << '\n';
		return nullptr;
	}
}
void Resources::createMaterial(const char* name, Shader* shader) {
	this->materials.insert(std::make_pair(name, new Material(shader)));
}
Material* Resources::getMaterial(const char* name) {
	auto it = this->materials.find(name);
	if (it != this->materials.end()) {
		return this->materials[name];
	}
	else {
		std::cout << "[Resources] no material named " << name << '\n';
		return nullptr;
	}
}
