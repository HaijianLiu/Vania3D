
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
	this->loadShader("renderpass_deferred_pbr", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_deferred_pbr.fs.glsl", "./Assets/Shaders/Functions/cookTorranceBRDF.fs.glsl", "./Assets/Shaders/Functions/shadowMapping.fs.glsl");
	this->loadShader("deferred_pbr_bone", "./Assets/Shaders/Vertex/bones_5_locations.vs.glsl",  "./Assets/Shaders/Fragment/bgra_to_mrca_4_passes.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->loadShader("simple", "./Assets/Shaders/Vertex/static_1_locations.vs.glsl", "./Assets/Shaders/Fragment/color_white_1_passes.fs.glsl");
	this->createMaterial("simple", this->getShader("simple"));

	// ibl
	this->loadShader("equirectangularToCubemap", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/equirectangularToCubemap.fs.glsl");
	this->loadShader("irradianceConvolution", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/irradianceConvolution.fs.glsl");
	this->loadShader("prefilter", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/prefilter.fs.glsl");
	this->loadShader("brdf", "./Assets/Shaders/brdf.vs.glsl",  "./Assets/Shaders/brdf.fs.glsl");
	this->loadShader("texture_output", "./Assets/Shaders/Vertex/quad.vs.glsl", "./Assets/Shaders/Fragment/texture_output.fs.glsl");
	this->loadShader("mix_two_textures", "./Assets/Shaders/Vertex/quad.vs.glsl", "./Assets/Shaders/Fragment/mix_two_textures.fs.glsl");
	this->loadShader("texture_output", "./Assets/Shaders/Vertex/quad.vs.glsl", "./Assets/Shaders/Fragment/texture_output.fs.glsl");

	// shadow mapping
	this->loadShader("shadow_mapping_depth", "./Assets/Shaders/Vertex/bones_3_locations_shadowmapping.vs.glsl",  "./Assets/Shaders/Fragment/null.fs.glsl");
	// fx
	this->loadShader("fx_uv_animation", "./Assets/Shaders/Vertex/instance_7_locations_fx_animation.vs.glsl",  "./Assets/Shaders/Fragment/fx_1_passes.fs.glsl");
	this->loadShader("fx_image", "./Assets/Shaders/Vertex/instance_7_locations_fx.vs.glsl",  "./Assets/Shaders/Fragment/fx_1_passes.fs.glsl");

	/* Model
	..............................................................................*/
	this->loadModel("player", MESH_ATTRIBUTE_BONE, "./Assets/Models/Ganfaul/mixamo_model.fbx");
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
	this->getMaterial("player")->twoSides = true;

	this->loadModel("sphere", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/Basic/sphere.fbx");


	// this->loadShader("deferredPBR_Kowloon", "./Assets/Shaders/deferredPBR_Kowloon.vs.glsl",  "./Assets/Shaders/deferredPBR_Kowloon.fs.glsl");
	// this->loadModel("P02_Floor_4U", "./Assets/Models/Kowloon/P02_Floor_4U.FBX");
	// this->loadTexture("P02_Floor_albedo", "./Assets/Models/Kowloon/P02_Floor_01_DIFF.TGA");
	// this->loadTexture("P02_Floor_normal", "./Assets/Models/Kowloon/P02_Floor_01_NM.TGA");
	// this->loadTexture("P02_Floor_mask", "./Assets/Models/Kowloon/P02_Floor_01_RMA.TGA");
	// this->createMaterial("P02_Floor_4U", this->getShader("deferredPBR_Kowloon"));
	// this->getMaterial("P02_Floor_4U")->addTexture("albedoMap", this->getTexture("P02_Floor_albedo"));
	// this->getMaterial("P02_Floor_4U")->addTexture("normalMap", this->getTexture("P02_Floor_normal"));
	// this->getMaterial("P02_Floor_4U")->addTexture("maskMap", this->getTexture("P02_Floor_mask"));
	//
	// this->loadModel("P02_AC_Unit_01_LOD0", "./Assets/Models/Kowloon/P02_AC_Unit_01_LOD0.FBX");
	// this->loadTexture("P02_AC_Unit_01_albedo", "./Assets/Models/Kowloon/P02_AC_Unit_01_DIFF.TGA");
	// this->loadTexture("P02_AC_Unit_01_normal", "./Assets/Models/Kowloon/P02_AC_Unit_01_NM.TGA");
	// this->loadTexture("P02_AC_Unit_01_mask", "./Assets/Models/Kowloon/P02_AC_Unit_01_RMA.TGA");
	// this->createMaterial("P02_AC_Unit_01_LOD0", this->getShader("deferredPBR_Kowloon"));
	// this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("albedoMap", this->getTexture("P02_AC_Unit_01_albedo"));
	// this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("normalMap", this->getTexture("P02_AC_Unit_01_normal"));
	// this->getMaterial("P02_AC_Unit_01_LOD0")->addTexture("maskMap", this->getTexture("P02_AC_Unit_01_mask"));

	/* FantasyDungeon
	..............................................................................*/
	// shaders
	this->loadShader("FantasyDungeonSRM", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/FantasyDungeon/bgra_to_mrca_4_passes.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->loadShader("FantasyDungeonWet", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/FantasyDungeon/bgra_to_mrca_wet_4_passes.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");


	// BaseTexture
	this->loadTexture("Mask1", "./Assets/Models/FantasyDungeon/Textures/BaseTexture/Mask1.TGA");
	// FX
	this->loadTexture("fx_fire_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_D.TGA");
	this->createMaterial("fx_fire", this->getShader("fx_uv_animation"));
	this->getMaterial("fx_fire")->addTexture("albedoMap", this->getTexture("fx_fire_D"));


	// BrickDamege
	this->loadModel("Brick01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick01.FBX");
	this->getModel("Brick01")->materialNames.push_back("BrickDamege");
	this->loadModel("Brick02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick02.FBX");
	this->getModel("Brick02")->materialNames.push_back("BrickDamege");
	// this->loadModel("Brick03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick03.FBX");
	// this->getModel("Brick03")->materialNames.push_back("BrickDamege");
	// this->loadModel("Brick04", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick04.FBX");
	// this->getModel("Brick04")->materialNames.push_back("BrickDamege");
	// this->loadModel("Brick05", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick05.FBX");
	// this->getModel("Brick05")->materialNames.push_back("BrickDamege");
	// this->loadModel("Brick06", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/Brick06.FBX");
	// this->getModel("Brick06")->materialNames.push_back("BrickDamege");
	this->loadModel("rock01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/rock01.FBX");
	this->getModel("rock01")->materialNames.push_back("BrickDamege");
	this->loadModel("rock02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/rock02.FBX");
	this->getModel("rock02")->materialNames.push_back("BrickDamege");
	this->loadModel("rock03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/BrickDamege/rock03.FBX");
	this->getModel("rock03")->materialNames.push_back("BrickDamege");
	this->loadTexture("BrickDamage_D", "./Assets/Models/FantasyDungeon/Textures/BrickDamege/BrickDamage_D.TGA");
	this->loadTexture("BrickDamage_N", "./Assets/Models/FantasyDungeon/Textures/BrickDamege/BrickDamage_N.TGA");
	this->loadTexture("BrickDamage_SRM", "./Assets/Models/FantasyDungeon/Textures/BrickDamege/BrickDamage_SRM.TGA");

	// Chandelier
	this->loadModel("torch03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Chandelier/torch03.FBX");
	this->getModel("torch03")->materialNames.push_back("Chandelier");
	this->loadTexture("chandelier_D", "./Assets/Models/FantasyDungeon/Textures/Chandelier/chandelier_D.TGA");
	this->loadTexture("chandelier_N", "./Assets/Models/FantasyDungeon/Textures/Chandelier/chandelier_N.TGA");
	this->loadTexture("chandelier_SRM", "./Assets/Models/FantasyDungeon/Textures/Chandelier/chandelier_SRM.TGA");

	// Column
	this->loadModel("Column02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Column/Column02.FBX");
	this->getModel("Column02")->materialNames.push_back("column");
	this->loadModel("Column04", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Column/Column04.FBX");
	this->getModel("Column04")->materialNames.push_back("column");
	this->loadTexture("Column_D", "./Assets/Models/FantasyDungeon/Textures/Column/Column_D.TGA");
	this->loadTexture("Column_N", "./Assets/Models/FantasyDungeon/Textures/Column/Column_N.TGA");
	this->loadTexture("Column_SRM", "./Assets/Models/FantasyDungeon/Textures/Column/Column_SRM.TGA");

	// Curbstone
	this->loadModel("curbstone01_2", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone01_2.FBX");
	this->getModel("curbstone01_2")->materialNames.push_back("curbstone");
	this->getModel("curbstone01_2")->materialNames.push_back("pavement1");
	this->loadModel("curbstone01_Door1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone01_Door1.FBX");
	this->getModel("curbstone01_Door1")->materialNames.push_back("curbstone");
	this->getModel("curbstone01_Door1")->materialNames.push_back("pavement1");
	this->loadModel("curbstone01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone01.FBX");
	this->getModel("curbstone01")->materialNames.push_back("curbstone");
	this->getModel("curbstone01")->materialNames.push_back("pavement1");
	this->loadModel("curbstone02_2", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02_2.FBX");
	this->getModel("curbstone02_2")->materialNames.push_back("curbstone");
	this->loadModel("curbstone02_door1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02_door1.FBX");
	this->getModel("curbstone02_door1")->materialNames.push_back("curbstone");
	this->loadModel("curbstone02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02.FBX");
	this->getModel("curbstone02")->materialNames.push_back("curbstone");
	this->loadTexture("curbstone_D", "./Assets/Models/FantasyDungeon/Textures/Curbstone/curbstone_D.TGA");
	this->loadTexture("curbstone_N", "./Assets/Models/FantasyDungeon/Textures/Curbstone/curbstone_N.TGA");	this->loadTexture("curbstone_SRM", "./Assets/Models/FantasyDungeon/Textures/Curbstone/curbstone_SRM.TGA");

	// Door
	this->loadModel("Door02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Door/Door02.FBX");
	this->getModel("Door02")->materialNames.push_back("Door1");
	this->loadModel("DoorArch01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Door/DoorArch01.FBX");
	this->getModel("DoorArch01")->materialNames.push_back("DoorArch");
	this->loadTexture("Door1_D", "./Assets/Models/FantasyDungeon/Textures/Door/Door1_D.TGA");
	this->loadTexture("Door1_N", "./Assets/Models/FantasyDungeon/Textures/Door/Door1_N.TGA");
	this->loadTexture("Door1_SRM", "./Assets/Models/FantasyDungeon/Textures/Door/Door1_SRM.TGA");
	this->loadTexture("DoorArch_D", "./Assets/Models/FantasyDungeon/Textures/Door/DoorArch_D.TGA");
	this->loadTexture("DoorArch_N", "./Assets/Models/FantasyDungeon/Textures/Door/DoorArch_N.TGA");
	this->loadTexture("DoorArch_SRM", "./Assets/Models/FantasyDungeon/Textures/Door/DoorArch_SRM.TGA");

	// Floor
	this->loadModel("Floor01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Floor/Floor01.FBX");
	this->getModel("Floor01")->materialNames.push_back("pavement2");
	this->loadTexture("Dirt_D", "./Assets/Models/FantasyDungeon/Textures/Floor/Dirt_D.TGA");
	this->loadTexture("Dirt_N", "./Assets/Models/FantasyDungeon/Textures/Floor/Dirt_N.TGA");
	this->loadTexture("Dirt_SRM", "./Assets/Models/FantasyDungeon/Textures/Floor/Dirt_SRM.TGA");
	this->loadTexture("pavement1_D", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement1_D.TGA");
	this->loadTexture("pavement1_N", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement1_N.TGA");
	this->loadTexture("pavement1_SRM", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement1_SRM.TGA");
	this->loadTexture("pavement2_D", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement2_D.TGA");
	this->loadTexture("pavement2_N", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement2_N.TGA");
	this->loadTexture("pavement2_SRM", "./Assets/Models/FantasyDungeon/Textures/Floor/pavement2_SRM.TGA");
	this->loadTexture("Sink_D", "./Assets/Models/FantasyDungeon/Textures/Floor/Sink_D.TGA");
	this->loadTexture("Sink_N", "./Assets/Models/FantasyDungeon/Textures/Floor/Sink_N.TGA");
	this->loadTexture("Sink_SRM", "./Assets/Models/FantasyDungeon/Textures/Floor/Sink_SRM.TGA");

	// Jail
	this->loadTexture("Jail_D", "./Assets/Models/FantasyDungeon/Textures/Jail/Jail_D.TGA");
	this->loadTexture("Jail_N", "./Assets/Models/FantasyDungeon/Textures/Jail/Jail_N.TGA");
	this->loadTexture("Jail_SRM", "./Assets/Models/FantasyDungeon/Textures/Jail/Jail_SRM.TGA");

	// Roof
	this->loadModel("Roof03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Roof/Roof03.FBX");
	this->getModel("Roof03")->materialNames.push_back("brick1");
	this->loadModel("RoofBorder01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Roof/RoofBorder01.FBX");
	this->getModel("RoofBorder01")->materialNames.push_back("Roof");
	this->loadTexture("Roof_D", "./Assets/Models/FantasyDungeon/Textures/Roof/Roof_D.TGA");
	this->loadTexture("Roof_N", "./Assets/Models/FantasyDungeon/Textures/Roof/Roof_N.TGA");
	this->loadTexture("Roof_SRM", "./Assets/Models/FantasyDungeon/Textures/Roof/Roof_SRM.TGA");

	// Stairs
	this->loadTexture("Stairs_D", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_D.TGA");
	this->loadTexture("Stairs_N", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_N.TGA");
	this->loadTexture("Stairs_SRM", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_SRM.TGA");

	// Wall
	this->loadModel("Niche", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Niche.FBX");
	this->getModel("Niche")->materialNames.push_back("Stairs");
	this->getModel("Niche")->materialNames.push_back("brick2");
	this->loadModel("Wall01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Wall01.FBX");
	this->getModel("Wall01")->materialNames.push_back("brick2");
	this->loadModel("wall01Arch1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/wall01Arch1.FBX");
	this->getModel("wall01Arch1")->materialNames.push_back("brick2");
	this->loadModel("wall09", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/wall09.FBX");
	this->getModel("wall09")->materialNames.push_back("brick2");
	this->loadModel("WallDamaged02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/WallDamaged02.FBX");
	this->getModel("WallDamaged02")->materialNames.push_back("BrickDamege");
	this->loadModel("WallDamaged03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/WallDamaged03.FBX");
	this->getModel("WallDamaged03")->materialNames.push_back("BrickDamege");
	this->loadModel("Windows", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Windows.FBX");
	this->getModel("Windows")->materialNames.push_back("Sink");
	this->getModel("Windows")->materialNames.push_back("Jail");
	this->loadTexture("brick2_D", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_D.TGA");
	this->loadTexture("brick2_N", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_N.TGA");
	this->loadTexture("brick2_SRM", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_SRM.TGA");
	this->loadTexture("brick1_D", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_D.TGA");
	this->loadTexture("brick1_N", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_N.TGA");
	this->loadTexture("brick1_SRM", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_SRM.TGA");



	// Chandelier
	this->createMaterial("Chandelier", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Chandelier")->addTexture("albedoMap", this->getTexture("chandelier_D"));
	this->getMaterial("Chandelier")->addTexture("normalMap", this->getTexture("chandelier_N"));
	this->getMaterial("Chandelier")->addTexture("maskMap", this->getTexture("chandelier_SRM"));
	// BrickDamege
	this->createMaterial("BrickDamege", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("BrickDamege")->addTexture("albedoMap", this->getTexture("BrickDamage_D"));
	this->getMaterial("BrickDamege")->addTexture("normalMap", this->getTexture("BrickDamage_N"));
	this->getMaterial("BrickDamege")->addTexture("maskMap", this->getTexture("BrickDamage_SRM"));
	// brick1
	this->createMaterial("brick1", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("brick1")->addTexture("albedoMap", this->getTexture("brick1_D"));
	this->getMaterial("brick1")->addTexture("normalMap", this->getTexture("brick1_N"));
	this->getMaterial("brick1")->addTexture("maskMap", this->getTexture("brick1_SRM"));
	// brick2
	this->createMaterial("brick2", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("brick2")->addTexture("albedoMap", this->getTexture("brick2_D"));
	this->getMaterial("brick2")->addTexture("normalMap", this->getTexture("brick2_N"));
	this->getMaterial("brick2")->addTexture("maskMap", this->getTexture("brick2_SRM"));
	// column
	this->createMaterial("column", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("column")->addTexture("albedoMap", this->getTexture("Column_D"));
	this->getMaterial("column")->addTexture("normalMap", this->getTexture("Column_N"));
	this->getMaterial("column")->addTexture("maskMap", this->getTexture("Column_SRM"));
	// curbstone
	this->createMaterial("curbstone", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("curbstone")->addTexture("albedoMap", this->getTexture("curbstone_D"));
	this->getMaterial("curbstone")->addTexture("normalMap", this->getTexture("curbstone_N"));
	this->getMaterial("curbstone")->addTexture("maskMap", this->getTexture("curbstone_SRM"));
	// Door1
	this->createMaterial("Door1", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Door1")->addTexture("albedoMap", this->getTexture("Door1_D"));
	this->getMaterial("Door1")->addTexture("normalMap", this->getTexture("Door1_N"));
	this->getMaterial("Door1")->addTexture("maskMap", this->getTexture("Door1_SRM"));
	// DoorArch
	this->createMaterial("DoorArch", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("DoorArch")->addTexture("albedoMap", this->getTexture("DoorArch_D"));
	this->getMaterial("DoorArch")->addTexture("normalMap", this->getTexture("DoorArch_N"));
	this->getMaterial("DoorArch")->addTexture("maskMap", this->getTexture("DoorArch_SRM"));
	// Jail
	this->createMaterial("Jail", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Jail")->addTexture("albedoMap", this->getTexture("Jail_D"));
	this->getMaterial("Jail")->addTexture("normalMap", this->getTexture("Jail_N"));
	this->getMaterial("Jail")->addTexture("maskMap", this->getTexture("Jail_SRM"));
	// pavement1
	this->createMaterial("pavement1", this->getShader("FantasyDungeonWet"));
	this->getMaterial("pavement1")->addTexture("albedoMap", this->getTexture("pavement1_D"));
	this->getMaterial("pavement1")->addTexture("normalMap", this->getTexture("pavement1_N"));
	this->getMaterial("pavement1")->addTexture("maskMap", this->getTexture("pavement1_SRM"));
	this->getMaterial("pavement1")->addTexture("factorMap", this->getTexture("Mask1"));
	// pavement2
	this->createMaterial("pavement2", this->getShader("FantasyDungeonWet"));
	this->getMaterial("pavement2")->addTexture("albedoMap", this->getTexture("pavement2_D"));
	this->getMaterial("pavement2")->addTexture("normalMap", this->getTexture("pavement2_N"));
	this->getMaterial("pavement2")->addTexture("maskMap", this->getTexture("pavement2_SRM"));
	this->getMaterial("pavement2")->addTexture("factorMap", this->getTexture("Mask1"));
	// Roof
	this->createMaterial("Roof", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Roof")->addTexture("albedoMap", this->getTexture("Roof_D"));
	this->getMaterial("Roof")->addTexture("normalMap", this->getTexture("Roof_N"));
	this->getMaterial("Roof")->addTexture("maskMap", this->getTexture("Roof_SRM"));
	// Sink
	this->createMaterial("Sink", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Sink")->addTexture("albedoMap", this->getTexture("Sink_D"));
	this->getMaterial("Sink")->addTexture("normalMap", this->getTexture("Sink_N"));
	this->getMaterial("Sink")->addTexture("maskMap", this->getTexture("Sink_SRM"));
	// Stairs
	this->createMaterial("Stairs", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Stairs")->addTexture("albedoMap", this->getTexture("Stairs_D"));
	this->getMaterial("Stairs")->addTexture("normalMap", this->getTexture("Stairs_N"));
	this->getMaterial("Stairs")->addTexture("maskMap", this->getTexture("Stairs_SRM"));



	/* ElvenRuins
	..............................................................................
	// Environments/Misc/Exo_Deco02
	this->loadModel("SM_Exo_Pew", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Misc/Exo_Deco02/StaticMesh/SM_Exo_Pew.FBX");

	this->loadTexture("T_Pew_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Misc/Exo_Deco02/Textures/T_Pew_D.TGA");
	this->loadTexture("T_Pew_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Misc/Exo_Deco02/Textures/T_Pew_N.TGA");


	// Environments/Plains/Env_Plains_Floors/
	this->loadModel("SM_Plains_Floors_Block01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Block01.FBX");
	this->loadModel("SM_Plains_Floors_Block02_Broken", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Block02_Broken.FBX");
	this->loadModel("SM_Plains_Floors_Block02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Block02.FBX");
	this->loadModel("SM_Plains_Floors_Block03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Block03.FBX");
	this->loadModel("SM_Plains_Floors_Block04", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Block04.FBX");
	this->loadModel("SM_Plains_Floors_Flat01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Flat01.FBX");
	this->loadModel("SM_Plains_Floors_Flat02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Flat02.FBX");
	this->loadModel("SM_Plains_Floors_Rubble", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Rubble.FBX");
	this->loadModel("SM_Plains_Floors_Rubble02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Rubble02.FBX");
	this->loadModel("SM_Plains_Floors_Rubble03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Rubble03.FBX");
	this->loadModel("SM_Plains_Floors_Rubble04", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Rubble04.FBX");
	this->loadModel("SM_Plains_Floors_Rubble05", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/StaticMesh/SM_Plains_Floors_Rubble05.FBX");
	this->getModel("SM_Plains_Floors_Rubble")->distanceCulling = true;
	this->getModel("SM_Plains_Floors_Rubble02")->distanceCulling = true;
	this->getModel("SM_Plains_Floors_Rubble03")->distanceCulling = true;
	this->getModel("SM_Plains_Floors_Rubble04")->distanceCulling = true;
	this->getModel("SM_Plains_Floors_Rubble05")->distanceCulling = true;

	this->loadTexture("T_Plains_FloorsTrim_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/Textures/T_Plains_FloorsTrim_D.TGA");
	this->loadTexture("T_Plains_FloorsTrim_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/Textures/T_Plains_FloorsTrim_N.TGA");
	this->loadTexture("T_Plains_Terrain_Floors01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/Textures/T_Plains_Terrain_Floors01_D.TGA");
	this->loadTexture("T_Plains_Terrain_Floors01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Floors/Textures/T_Plains_Terrain_Floors01_N.TGA");


	// Environments/Plains/Env_Plains_Flora/
	this->loadModel("SM_Plains_Fern01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Fern01.FBX");
	this->loadModel("SM_Plains_Fern02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Fern02.FBX");
	this->loadModel("SM_Plains_Fern03", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Fern03.FBX");
	this->loadModel("SM_Plains_Fern04", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Fern04.FBX");
	this->loadModel("SM_Plains_Flowers01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Flowers01.FBX");
	this->loadModel("SM_Plains_Flowers03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Flowers03.FBX");
	this->loadModel("SM_Plains_Grass01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Grass01.FBX");
	this->loadModel("SM_Plains_Tree_Branch01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Tree_Branch01.FBX");
	this->loadModel("SM_Plains_Tree_Branch02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Tree_Branch02.FBX");
	this->loadModel("SM_Plains_Tree_Stump01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Tree_Stump01.FBX");
	this->loadModel("SM_Plains_Tree01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Tree01.FBX");
	this->loadModel("SM_Plains_Vine02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/StaticMesh/SM_Plains_Vine02.FBX");
	this->getModel("SM_Plains_Fern01")->distanceCulling = true;
	this->getModel("SM_Plains_Fern02")->distanceCulling = true;
	this->getModel("SM_Plains_Fern03")->distanceCulling = true;
	this->getModel("SM_Plains_Fern04")->distanceCulling = true;
	this->getModel("SM_Plains_Flowers01")->distanceCulling = true;
	this->getModel("SM_Plains_Flowers03")->distanceCulling = true;
	this->getModel("SM_Plains_Grass01")->distanceCulling = true;
	this->getModel("SM_Plains_Vine02")->distanceCulling = true;

	this->loadTexture("T_Plains_Fern01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/Textures/T_Plains_Fern01_D.TGA");
	this->loadTexture("T_Plains_Fern01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/Textures/T_Plains_Fern01_N.TGA");
	this->loadTexture("T_Plains_Tree01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/Textures/T_Plains_Tree01_D.TGA");
	this->loadTexture("T_Plains_Tree01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Flora/Textures/T_Plains_Tree01_N.TGA");


	// Environments/Plains/Env_Plains_Organic/
	this->loadModel("SM_Plains_LargeRock_SpireLOD", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock_SpireLOD.FBX");
	this->loadModel("SM_Plains_LargeRock_SpireLOD2", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock_SpireLOD2.FBX");
	this->loadModel("SM_Plains_LargeRock_SpireLOD4", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock_SpireLOD4.FBX");
	this->loadModel("SM_Plains_LargeRock_SpireLOD5", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock_SpireLOD5.FBX");
	this->loadModel("SM_Plains_LargeRock01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock01.FBX");
	this->loadModel("SM_Plains_LargeRock02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock02.FBX");
	this->loadModel("SM_Plains_LargeRock03", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/StaticMesh/SM_Plains_LargeRock03.FBX");
	this->loadTexture("T_Plains_Cliff01", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Cliff01.TGA");
	this->loadTexture("T_Plains_Cliff001", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Cliff001.TGA");
	this->loadTexture("T_Plains_Ground01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Ground01_D.TGA");
	this->loadTexture("T_Plains_Ground01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Ground01_N.TGA");
	this->loadTexture("T_Plains_Rock01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Rock01_D.TGA");
	this->loadTexture("T_Plains_Rock01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Organic/Textures/T_Plains_Rock01_N.TGA");


	// Environments/Plains/Env_Plains_Ruins/
	this->loadModel("SM_Plains_Buttress_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Buttress_01.FBX");
	this->loadModel("SM_Plains_Buttress_02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Buttress_02.FBX");
	this->loadModel("SM_Plains_Castle_Bridge_Wall_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Bridge_Wall_01.FBX");
	this->loadModel("SM_Plains_Castle_Circle_Railing_01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Circle_Railing_01.FBX");
	this->loadModel("SM_Plains_Castle_Fountain_01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.FBX");
	this->loadModel("SM_Plains_Castle_Railing_Curved_45", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Railing_Curved_45.FBX");
	this->loadModel("SM_Plains_Castle_Railing_Curved", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Railing_Curved.FBX");
	this->loadModel("SM_Plains_CastleTower_01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleTower_01.FBX");
	this->loadModel("SM_Plains_CastleTower_Ruin_01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleTower_Ruin_01.FBX");
	this->loadModel("SM_Plains_CastleTower_Ruin_02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleTower_Ruin_02.FBX");
	this->loadModel("SM_Plains_CastleWall_Curved_Ruin_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_CastleWall_Curved_Ruin_01.FBX");
	this->loadModel("SM_Plains_Column_Large_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Column_Large_01.FBX");
	this->loadModel("SM_Plains_Column_Large_Broken_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Column_Large_Broken_01.FBX");
	this->loadModel("SM_Plains_Column_Large_Broken_02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Column_Large_Broken_02.FBX");
	this->loadModel("SM_Plains_Column_Large_Broken_03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Column_Large_Broken_03.FBX");
	this->loadModel("SM_Plains_Ruin_BuildingLOD01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Ruin_BuildingLOD01.FBX");
	this->loadModel("SM_Plains_Ruin_BuildingLOD02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Ruin_BuildingLOD02.FBX");
	this->loadModel("SM_Plains_Ruin_BuildingLOD03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Ruin_BuildingLOD03.FBX");
	this->loadModel("SM_Plains_Wall_Straight_01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Wall_Straight_01.FBX");

	this->loadTexture("T_Plains_ExtCastle_StoneWall_01_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_ExtCastle_StoneWall_01_D.TGA");
	this->loadTexture("T_Plains_ExtCastle_StoneWall_01_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_ExtCastle_StoneWall_01_N.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_02_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_02_D.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_02_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_02_N.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_03_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_03_D.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_03_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_03_N.TGA");

	this->loadTexture("T_Plains_Ruins_Pillars_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_D.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_M", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_M.TGA");
	this->loadTexture("T_Plains_Ruins_Pillars_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/Textures/T_Plains_Ruins_Pillars_N.TGA");




	// Environments/Plains/Env_Plains_Statues/
	this->loadModel("SM_Plains_Angel_Statue_01_nobase_torch", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/StaticMesh/SM_Plains_Angel_Statue_01_nobase_torch.FBX");
	this->loadModel("SM_Plains_Angel_Statue_01_nobase", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/StaticMesh/SM_Plains_Angel_Statue_01_nobase.FBX");

	this->loadTexture("T_Plains_Angel_Statue_D", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/Textures/T_Plains_Angel_Statue_D.TGA");
	this->loadTexture("T_Plains_Angel_Statue_N", "./Assets/Models/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Statues/Textures/T_Plains_Angel_Statue_N.TGA");
*/

	/* Shaders/ElvenRuins/
	..............................................................................
	// M_Pew
	this->loadShader("M_Pew", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Pew.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Pew", this->getShader("M_Pew"));
	this->getMaterial("M_Pew")->addTexture("albedoMap", this->getTexture("T_Pew_D"));
	this->getMaterial("M_Pew")->addTexture("normalMap", this->getTexture("T_Pew_N"));
	// M_Plains_Angel_Statue
	this->loadShader("M_Plains_Angel_Statue", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Angel_Statue.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Angel_Statue", this->getShader("M_Plains_Angel_Statue"));
	this->getMaterial("M_Plains_Angel_Statue")->addTexture("albedoMap", this->getTexture("T_Plains_Angel_Statue_D"));
	this->getMaterial("M_Plains_Angel_Statue")->addTexture("normalMap", this->getTexture("T_Plains_Angel_Statue_N"));
	// M_Plains_Cliff1
	this->loadShader("M_Plains_Cliff1", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Cliff1.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Cliff1", this->getShader("M_Plains_Cliff1"));
	this->getMaterial("M_Plains_Cliff1")->addTexture("albedoMap", this->getTexture("T_Plains_Cliff01"));
	this->getMaterial("M_Plains_Cliff1")->addTexture("normalMap", this->getTexture("T_Plains_Cliff001"));
	this->getMaterial("M_Plains_Cliff1")->twoSides = true;
	// M_PLains_Fern01
	this->loadShader("M_PLains_Fern01", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_PLains_Fern01.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_PLains_Fern01", this->getShader("M_PLains_Fern01"));
	this->getMaterial("M_PLains_Fern01")->addTexture("albedoMap", this->getTexture("T_Plains_Fern01_D"));
	this->getMaterial("M_PLains_Fern01")->addTexture("normalMap", this->getTexture("T_Plains_Fern01_N"));
	this->getMaterial("M_PLains_Fern01")->twoSides = true;
	// M_Plains_Floors_Block
	this->loadShader("M_Plains_Floors_Block", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Floors_Block.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Floors_Block", this->getShader("M_Plains_Floors_Block"));
	this->getMaterial("M_Plains_Floors_Block")->addTexture("albedoMap", this->getTexture("T_Plains_Terrain_Floors01_D"));
	this->getMaterial("M_Plains_Floors_Block")->addTexture("normalMap", this->getTexture("T_Plains_Terrain_Floors01_N"));
	// M_Plains_FloorTrims
	this->loadShader("M_Plains_FloorTrims", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_FloorTrims.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_FloorTrims", this->getShader("M_Plains_FloorTrims"));
	this->getMaterial("M_Plains_FloorTrims")->addTexture("albedoMap", this->getTexture("T_Plains_FloorsTrim_D"));
	this->getMaterial("M_Plains_FloorTrims")->addTexture("normalMap", this->getTexture("T_Plains_FloorsTrim_N"));
	// M_Plains_FloorTrims
	this->loadShader("M_Plains_GroundBrick", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_GroundBrick.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_GroundBrick", this->getShader("M_Plains_GroundBrick"));
	this->getMaterial("M_Plains_GroundBrick")->addTexture("albedoMap", this->getTexture("T_Plains_Ground01_D"));
	this->getMaterial("M_Plains_GroundBrick")->addTexture("normalMap", this->getTexture("T_Plains_Ground01_N"));
	// M_Plains_LargeRock
	this->loadShader("M_Plains_LargeRock", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_LargeRock.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_LargeRock", this->getShader("M_Plains_LargeRock"));
	this->getMaterial("M_Plains_LargeRock")->addTexture("albedoMap", this->getTexture("T_Plains_Rock01_D"));
	this->getMaterial("M_Plains_LargeRock")->addTexture("normalMap", this->getTexture("T_Plains_Rock01_N"));
	// M_Plains_Pillars
	this->loadShader("M_Plains_Pillars", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Pillars.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Pillars", this->getShader("M_Plains_Pillars"));
	this->getMaterial("M_Plains_Pillars")->addTexture("albedoMap", this->getTexture("T_Plains_Ruins_Pillars_D"));
	this->getMaterial("M_Plains_Pillars")->addTexture("normalMap", this->getTexture("T_Plains_Ruins_Pillars_N"));
	this->getMaterial("M_Plains_Pillars")->addTexture("metallicMap", this->getTexture("T_Plains_Ruins_Pillars_M"));
	// M_Plains_Pillars02
	this->loadShader("M_Plains_Pillars02", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Pillars02.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Pillars02", this->getShader("M_Plains_Pillars02"));
	this->getMaterial("M_Plains_Pillars02")->addTexture("albedoMap", this->getTexture("T_Plains_Ruins_Pillars_02_D"));
	this->getMaterial("M_Plains_Pillars02")->addTexture("normalMap", this->getTexture("T_Plains_Ruins_Pillars_02_N"));
	// M_Plains_Pillars03
	this->loadShader("M_Plains_Pillars03", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Pillars03.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Pillars03", this->getShader("M_Plains_Pillars03"));
	this->getMaterial("M_Plains_Pillars03")->addTexture("albedoMap", this->getTexture("T_Plains_Ruins_Pillars_03_D"));
	this->getMaterial("M_Plains_Pillars03")->addTexture("normalMap", this->getTexture("T_Plains_Ruins_Pillars_03_N"));
	// M_Plains_StoneWall
	this->loadShader("M_Plains_StoneWall", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_StoneWall.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_StoneWall", this->getShader("M_Plains_StoneWall"));
	this->getMaterial("M_Plains_StoneWall")->addTexture("albedoMap", this->getTexture("T_Plains_ExtCastle_StoneWall_01_D"));
	this->getMaterial("M_Plains_StoneWall")->addTexture("normalMap", this->getTexture("T_Plains_ExtCastle_StoneWall_01_N"));
	// M_Plains_Tree_Stump01
	this->loadShader("M_Plains_Tree_Stump01", "./Assets/Shaders/Vertex/static_3_locations.vs.glsl",  "./Assets/Shaders/ElvenRuins/M_Plains_Tree_Stump01.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->createMaterial("M_Plains_Tree_Stump01", this->getShader("M_Plains_Tree_Stump01"));
	this->getMaterial("M_Plains_Tree_Stump01")->addTexture("albedoMap", this->getTexture("T_Plains_Tree01_D"));
	this->getMaterial("M_Plains_Tree_Stump01")->addTexture("normalMap", this->getTexture("T_Plains_Tree01_N"));
*/


	/* FrozenCove
	..............................................................................*/
	/*
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

	// shaders and materials
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
	*/



	/* Material
	..............................................................................*/
	// this->createMaterial("simple_flat_color", this->getShader("simple_flat_color"));

	/* LightProbe
	..............................................................................*/
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/WinterForest_Ref.hdr");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/Road_to_MonumentValley_8k.jpg");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/Stadium_Center_8k.jpg");
	this->loadLightProbe("hdr", "./Assets/Models/InfinityBladeGrassLands/Maps/LevelContent/HDRI/HDRI_Epic_Courtyard_Daylight.HDR");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/test.jpg");
}


/*------------------------------------------------------------------------------
< Load & Get >
------------------------------------------------------------------------------*/
void Resources::loadShader(const char* name, const char* vertexPath, const char* fragmentPath) {
	Shader* shader = new Shader();
	shader->addVertexCode(vertexPath);
	shader->addFragmentCode(fragmentPath);
	shader->complie();
	this->shaders.insert(std::make_pair(name, shader));
}
void Resources::loadShader(const char* name, const char* vertexPath, const char* fragmentPath, const char* functionPath) {
	Shader* shader = new Shader();
	shader->addVertexCode(vertexPath);
	shader->addFragmentCode(fragmentPath);
	shader->addFragmentCode(functionPath);
	shader->complie();
	this->shaders.insert(std::make_pair(name, shader));
}
void Resources::loadShader(const char* name, const char* vertexPath, const char* fragmentPath, const char* functionPath1, const char* functionPath2) {
	Shader* shader = new Shader();
	shader->addVertexCode(vertexPath);
	shader->addFragmentCode(fragmentPath);
	shader->addFragmentCode(functionPath1);
	shader->addFragmentCode(functionPath2);
	shader->complie();
	this->shaders.insert(std::make_pair(name, shader));
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
void Resources::loadModel(std::string name, unsigned int attributeType, const char* path) {
	this->models.insert(std::make_pair(name, new Model(attributeType, path)));
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
