
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
	// renderpass
	this->loadShader("renderpass_deferred_pbr", "./Assets/Shaders/renderpass_deferred_pbr.vs.glsl",  "./Assets/Shaders/renderpass_deferred_pbr.fs.glsl");
	this->loadShader("deferred_pbr_bone", "./Assets/Shaders/deferred_pbr_bone.vs.glsl",  "./Assets/Shaders/deferred_pbr_bone.fs.glsl");
	this->loadShader("deferredPBR_Kowloon", "./Assets/Shaders/deferredPBR_Kowloon.vs.glsl",  "./Assets/Shaders/deferredPBR_Kowloon.fs.glsl");

	// ibl
	this->loadShader("equirectangularToCubemap", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/equirectangularToCubemap.fs.glsl");
	this->loadShader("irradianceConvolution", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/irradianceConvolution.fs.glsl");
	this->loadShader("prefilter", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/prefilter.fs.glsl");
	this->loadShader("brdf", "./Assets/Shaders/brdf.vs.glsl",  "./Assets/Shaders/brdf.fs.glsl");

	// shadow mapping
	this->loadShader("shadow_mapping", "./Assets/Shaders/shadow_mapping.vs.glsl",  "./Assets/Shaders/shadow_mapping.fs.glsl");
	this->loadShader("shadow_mapping_depth", "./Assets/Shaders/shadow_mapping_depth.vs.glsl",  "./Assets/Shaders/shadow_mapping_depth.fs.glsl");

	// test
	this->loadShader("simple_flat_color", "./Assets/Shaders/simple_flat_color.vs.glsl",  "./Assets/Shaders/simple_flat_color.fs.glsl");

	/* Model
	..............................................................................*/
	this->models.insert(std::make_pair("sphere", new Sphere()));

	this->loadModel("player", "./Assets/Models/Ganfaul/mixamo_model.fbx");
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_stay.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_idle_look.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_walk.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_run.fbx"));
	this->getModel("player")->addAnimation(new Animation("./Assets/Models/Ganfaul/mixamo_swiping.fbx"));
	this->loadTexture("player_albedo", "./Assets/Models/Ganfaul/Ganfaul_diffuse.TGA");
	this->loadTexture("player_normal", "./Assets/Models/Ganfaul/Ganfaul_normal.TGA");
	this->loadTexture("player_mask", "./Assets/Models/Ganfaul/Ganfaul_mask.TGA");
	this->createMaterial("player", this->getShader("deferred_pbr_bone"));
	this->getMaterial("player")->addTexture("albedoMap", this->getTexture("player_albedo"));
	this->getMaterial("player")->addTexture("normalMap", this->getTexture("player_normal"));
	this->getMaterial("player")->addTexture("maskMap", this->getTexture("player_mask"));

	this->loadModel("P02_Floor_4U", "./Assets/Models/Kowloon/P02_Floor_4U.FBX");
	this->loadTexture("P02_Floor_albedo", "./Assets/Models/Kowloon/P02_Floor_01_DIFF.TGA");
	this->loadTexture("P02_Floor_normal", "./Assets/Models/Kowloon/P02_Floor_01_NM.TGA");
	this->loadTexture("P02_Floor_mask", "./Assets/Models/Kowloon/P02_Floor_01_RMA.TGA");
	this->createMaterial("P02_Floor_4U", this->getShader("deferredPBR_Kowloon"));
	this->getMaterial("P02_Floor_4U")->addTexture("albedoMap", this->getTexture("P02_Floor_albedo"));
	this->getMaterial("P02_Floor_4U")->addTexture("normalMap", this->getTexture("P02_Floor_normal"));
	this->getMaterial("P02_Floor_4U")->addTexture("maskMap", this->getTexture("P02_Floor_mask"));

	this->loadModel("P02_AC_Unit_01_LOD0", "./Assets/Models/Kowloon/P02_AC_Unit_01_LOD0.FBX");
	this->loadTexture("P02_AC_Unit_01_albedo", "./Assets/Models/Kowloon/P02_AC_Unit_01_DIFF.TGA");
	this->loadTexture("P02_AC_Unit_01_normal", "./Assets/Models/Kowloon/P02_AC_Unit_01_NM.TGA");
	this->loadTexture("P02_AC_Unit_01_mask", "./Assets/Models/Kowloon/P02_AC_Unit_01_RMA.TGA");
	this->createMaterial("P02_AC_Unit_01_LOD0", this->getShader("deferredPBR_Kowloon"));
	this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("albedoMap", this->getTexture("P02_AC_Unit_01_albedo"));
	this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("normalMap", this->getTexture("P02_AC_Unit_01_normal"));
	this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("maskMap", this->getTexture("P02_AC_Unit_01_mask"));








	// Effects/FX_Textures/Clouds/
	this->loadTexture("T_FogPlane01_Clouds", "./Assets/Models/InfinityBladeIceLands/Effects/FX_Textures/Clouds/T_FogPlane01_Clouds.TGA");

	// Environments/Ice/Env_Ice_Rocky/
	this->loadTexture("Env_Ice_RuinedWalls_Snow_Tile_D", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Env_Ice_Rocky/Textures/Env_Ice_RuinedWalls_Snow_Tile_D.TGA");
	this->loadTexture("Env_Ice_RuinedWalls_Snow_Tile_N", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Env_Ice_Rocky/Textures/Env_Ice_RuinedWalls_Snow_Tile_N.TGA");
	this->loadTexture("T_Snow_Ice_Generic", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Env_Ice_Rocky/Textures/T_Snow_Ice_Generic.TGA");
	this->loadTexture("T_Snow_Ice_Generic_NRM", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Env_Ice_Rocky/Textures/T_Snow_Ice_Generic_NRM.TGA");

	// Environments/Ice/Ice_Castle/
	this->loadModel("SM_IceT3_Walkway_02b", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Castle/SM/SM_IceT3_Walkway_02b.FBX");
	this->loadModel("SM_IceT3_BossWalls_03c", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Castle/SM/SM_IceT3_BossWalls_03c.FBX");

	this->loadTexture("Ice_Fort_Floor_5_D", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Castle/Textures/Ice_Fort_Floor_5_D.TGA");
	this->loadTexture("Ice_Fort_Floor_5_N", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Castle/Textures/Ice_Fort_Floor_5_N.TGA");

	// Environments/Ice/Ice_Fortress/
	this->loadModel("SM_Ice_Fort_Brazier_1_Glow", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Brazier_1_Glow.FBX");
	this->loadModel("SM_Ice_Fort_Brazier_1", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Brazier_1.FBX");
	this->loadModel("SM_Ice_Fort_Pillar_1b", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/StaticMesh/SM_Ice_Fort_Pillar_1b.FBX");

	this->loadTexture("T_Ice_Fort_Pillar_1_D", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Ice_Fort_Pillar_1_D.TGA");
	this->loadTexture("T_Ice_Fort_Pillar_1_N", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Ice_Fort_Pillar_1_N.TGA");
	this->loadTexture("T_Ice_Fort_Trim_1_D", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Ice_Fort_Trim_1_D.TGA");
	this->loadTexture("T_Ice_Fort_Trim_1_N", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Ice_Fort_Trim_1_N.TGA");
	this->loadTexture("T_Iron_D", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Iron_D.TGA");
	this->loadTexture("T_Iron_E", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Iron_E.TGA");
	this->loadTexture("T_Iron_N", "./Assets/Models/InfinityBladeIceLands/Environments/Ice/Ice_Fortress/Textures/T_Iron_N.TGA");


	/* shaders and materials */
	// M_Env_Ice_RuinedWalls_Snow
	this->loadShader("M_Env_Ice_RuinedWalls_Snow", "./Assets/Shaders/FrozenCove/M_Env_Ice_RuinedWalls_Snow.vs.glsl",  "./Assets/Shaders/FrozenCove/M_Env_Ice_RuinedWalls_Snow.fs.glsl");
	this->createMaterial("M_Env_Ice_RuinedWalls_Snow", this->getShader("M_Env_Ice_RuinedWalls_Snow"));
	this->getMaterial("M_Env_Ice_RuinedWalls_Snow")->addTexture("albedoMap0", this->getTexture("Env_Ice_RuinedWalls_Snow_Tile_D"));
	this->getMaterial("M_Env_Ice_RuinedWalls_Snow")->addTexture("normalMap0", this->getTexture("Env_Ice_RuinedWalls_Snow_Tile_N"));
	// M_Ice_Fort_Iron_1_Glow
	this->loadShader("M_Ice_Fort_Iron_1_Glow", "./Assets/Shaders/FrozenCove/M_Ice_Fort_Iron_1_Glow.vs.glsl",  "./Assets/Shaders/FrozenCove/M_Ice_Fort_Iron_1_Glow.fs.glsl");
	this->createMaterial("M_Ice_Fort_Iron_1_Glow", this->getShader("M_Ice_Fort_Iron_1_Glow"));
	this->getMaterial("M_Ice_Fort_Iron_1_Glow")->addTexture("albedoMap0", this->getTexture("T_Iron_D"));
	this->getMaterial("M_Ice_Fort_Iron_1_Glow")->addTexture("normalMap0", this->getTexture("T_Iron_N"));
	this->getMaterial("M_Ice_Fort_Iron_1_Glow")->addTexture("glowMap0", this->getTexture("T_Iron_E"));
	// M_Ice_Fort_Iron_Env_Map
	this->loadShader("M_Ice_Fort_Iron_Env_Map", "./Assets/Shaders/FrozenCove/M_Ice_Fort_Iron_Env_Map.vs.glsl",  "./Assets/Shaders/FrozenCove/M_Ice_Fort_Iron_Env_Map.fs.glsl");
	this->createMaterial("M_Ice_Fort_Iron_Env_Map", this->getShader("M_Ice_Fort_Iron_Env_Map"));
	this->getMaterial("M_Ice_Fort_Iron_Env_Map")->addTexture("albedoMap0", this->getTexture("T_Iron_D"));
	this->getMaterial("M_Ice_Fort_Iron_Env_Map")->addTexture("normalMap0", this->getTexture("T_Iron_N"));
	// M_Ice_Fort_Pillar_1_SnowPaint
	this->loadShader("M_Ice_Fort_Pillar_1_SnowPaint", "./Assets/Shaders/FrozenCove/M_Ice_Fort_Pillar_1_SnowPaint.vs.glsl",  "./Assets/Shaders/FrozenCove/M_Ice_Fort_Pillar_1_SnowPaint.fs.glsl");
	this->createMaterial("M_Ice_Fort_Pillar_1_SnowPaint", this->getShader("M_Ice_Fort_Pillar_1_SnowPaint"));
	this->getMaterial("M_Ice_Fort_Pillar_1_SnowPaint")->addTexture("albedoMap0", this->getTexture("T_Snow_Ice_Generic"));
	this->getMaterial("M_Ice_Fort_Pillar_1_SnowPaint")->addTexture("normalMap0", this->getTexture("T_Snow_Ice_Generic_NRM"));
	this->getMaterial("M_Ice_Fort_Pillar_1_SnowPaint")->addTexture("albedoMap1", this->getTexture("T_Ice_Fort_Pillar_1_D"));
	this->getMaterial("M_Ice_Fort_Pillar_1_SnowPaint")->addTexture("normalMap1", this->getTexture("T_Ice_Fort_Pillar_1_N"));
	this->getMaterial("M_Ice_Fort_Pillar_1_SnowPaint")->addTexture("mixMap", this->getTexture("T_FogPlane01_Clouds"));
	// M_Ice_Fort_Trim_1_SnowPaint
	this->loadShader("M_Ice_Fort_Trim_1_SnowPaint", "./Assets/Shaders/FrozenCove/M_Ice_Fort_Trim_1_SnowPaint.vs.glsl",  "./Assets/Shaders/FrozenCove/M_Ice_Fort_Trim_1_SnowPaint.fs.glsl");
	this->createMaterial("M_Ice_Fort_Trim_1_SnowPaint", this->getShader("M_Ice_Fort_Trim_1_SnowPaint"));
	this->getMaterial("M_Ice_Fort_Trim_1_SnowPaint")->addTexture("albedoMap0", this->getTexture("T_Snow_Ice_Generic"));
	this->getMaterial("M_Ice_Fort_Trim_1_SnowPaint")->addTexture("normalMap0", this->getTexture("T_Snow_Ice_Generic_NRM"));
	this->getMaterial("M_Ice_Fort_Trim_1_SnowPaint")->addTexture("albedoMap1", this->getTexture("T_Ice_Fort_Trim_1_D"));
	this->getMaterial("M_Ice_Fort_Trim_1_SnowPaint")->addTexture("normalMap1", this->getTexture("T_Ice_Fort_Trim_1_N"));
	this->getMaterial("M_Ice_Fort_Trim_1_SnowPaint")->addTexture("mixMap", this->getTexture("T_FogPlane01_Clouds"));
	// M_IceT3_Fort_Floor_05
	this->loadShader("M_IceT3_Fort_Floor_05", "./Assets/Shaders/FrozenCove/M_IceT3_Fort_Floor_05.vs.glsl",  "./Assets/Shaders/FrozenCove/M_IceT3_Fort_Floor_05.fs.glsl");
	this->createMaterial("M_IceT3_Fort_Floor_05", this->getShader("M_IceT3_Fort_Floor_05"));
	this->getMaterial("M_IceT3_Fort_Floor_05")->addTexture("albedoMap0", this->getTexture("T_Snow_Ice_Generic"));
	this->getMaterial("M_IceT3_Fort_Floor_05")->addTexture("normalMap0", this->getTexture("T_Snow_Ice_Generic_NRM"));
	this->getMaterial("M_IceT3_Fort_Floor_05")->addTexture("albedoMap1", this->getTexture("Ice_Fort_Floor_5_D"));
	this->getMaterial("M_IceT3_Fort_Floor_05")->addTexture("normalMap1", this->getTexture("Ice_Fort_Floor_5_N"));







	/* Material
	..............................................................................*/
	this->createMaterial("simple_flat_color", this->getShader("simple_flat_color"));

	/* LightProbe
	..............................................................................*/
	this->loadLightProbe("hdr", "./Assets/Textures/HDR/WinterForest_Ref.hdr");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/test.jpg");
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
void Resources::loadModel(std::string name, const char* path) {
	this->models.insert(std::make_pair(name, new Model(path)));
}
Model* Resources::getModel(std::string name) {
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
void Resources::createMaterial(std::string name, Shader* shader) {
	this->materials.insert(std::make_pair(name, new Material(shader)));
}
Material* Resources::getMaterial(std::string name) {
	auto it = this->materials.find(name);
	if (it != this->materials.end()) {
		return this->materials[name];
	}
	else {
		std::cout << "[Resources] no material named " << name << '\n';
		return nullptr;
	}
}
