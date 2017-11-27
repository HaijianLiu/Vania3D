
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
	delete this->quadinstance2vec3;
	delete this->skybox;
}


/*------------------------------------------------------------------------------
< start > before GameObject defaultStart()
------------------------------------------------------------------------------*/
void Resources::start() {
	/* Meshes
	..............................................................................*/
	this->quad = new Quad();
	this->quadinstance2vec3 = new QuadInstance2vec3();
	this->skybox = new Skybox();

	/* Shader
	..............................................................................*/
	// renderpass
	this->loadShader("renderpass_combine", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_combine.fs.glsl");
	this->loadShader("ambient_pass", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_ambient_1_passes.fs.glsl");
	this->loadShader("lighting_pass", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_lighting_1_passes.fs.glsl", "./Assets/Shaders/Functions/cookTorranceBRDF.fs.glsl");
	this->loadShader("shadow_pass", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_shadow_1_passes.fs.glsl", "./Assets/Shaders/Functions/shadowMapping.fs.glsl");
	this->loadShader("ssao_pass", "./Assets/Shaders/Vertex/quad.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_ssao_1_passes.fs.glsl");

	// fx
	this->loadShader("fx_uv_animation", "./Assets/Shaders/Vertex/instance_7_locations_fx_animation.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_fx_1_passes.fs.glsl");
	this->loadShader("fx_image", "./Assets/Shaders/Vertex/instance_7_locations_fx.vs.glsl",  "./Assets/Shaders/RenderPass/renderpass_fx_1_passes.fs.glsl");

	// shadow mapping
	this->loadShader("shadow_mapping_depth", "./Assets/Shaders/Vertex/bones_3_locations_shadowmapping.vs.glsl",  "./Assets/Shaders/Fragment/null.fs.glsl");

	// ibl
	this->loadShader("equirectangularToCubemap", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/equirectangularToCubemap.fs.glsl");
	this->loadShader("irradianceConvolution", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/irradianceConvolution.fs.glsl");
	this->loadShader("prefilter", "./Assets/Shaders/cubemap.vs.glsl",  "./Assets/Shaders/prefilter.fs.glsl");
	this->loadShader("brdf", "./Assets/Shaders/brdf.vs.glsl",  "./Assets/Shaders/brdf.fs.glsl");

	// other
	this->loadShader("deferred_pbr_bone", "./Assets/Shaders/Vertex/bones_5_locations.vs.glsl",  "./Assets/Shaders/Fragment/bgra_to_mrca_4_passes.fs.glsl", "./Assets/Shaders/Functions/getNormalFromMap.fs.glsl");
	this->loadShader("simple", "./Assets/Shaders/Vertex/static_1_locations.vs.glsl", "./Assets/Shaders/Fragment/color_white_1_passes.fs.glsl");
	this->createMaterial("simple", this->getShader("simple"));

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

	// sphere
	this->loadModel("sphere", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/Basic/sphere.fbx");

	/* LightProbe
	..............................................................................*/
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/WinterForest_Ref.hdr");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/Road_to_MonumentValley_8k.jpg");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/Stadium_Center_8k.jpg");
	this->loadLightProbe("hdr", "./Assets/Models/InfinityBladeGrassLands/Maps/LevelContent/HDRI/HDRI_Epic_Courtyard_Daylight.HDR");
	// this->loadLightProbe("hdr", "./Assets/Textures/HDR/test.jpg");


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
	this->loadTexture("fx_fire_smoke_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_smoke_D.TGA");
	this->createMaterial("fx_fire_smoke", this->getShader("fx_image"));
	this->getMaterial("fx_fire_smoke")->addTexture("albedoMap", this->getTexture("fx_fire_smoke_D"));
	this->loadTexture("fx_fire_drop_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_drop_D.TGA");
	this->createMaterial("fx_fire_drop", this->getShader("fx_image"));
	this->getMaterial("fx_fire_drop")->addTexture("albedoMap", this->getTexture("fx_fire_drop_D"));
	this->loadTexture("fx_fire_spark_D", "./Assets/Models/FantasyDungeon/Textures/FX/fx_fire_spark_D.TGA");
	this->createMaterial("fx_fire_spark", this->getShader("fx_image"));
	this->getMaterial("fx_fire_spark")->addTexture("albedoMap", this->getTexture("fx_fire_spark_D"));


	// Arch
	this->loadModel("Arch01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Arch/Arch01.FBX");
	this->getModel("Arch01")->materialNames.push_back("Arch");
	this->getModel("Arch01")->materialNames.push_back("brick2");
	this->loadTexture("Arch_D", "./Assets/Models/FantasyDungeon/Textures/Arch/Arch_D.TGA");
	this->loadTexture("Arch_N", "./Assets/Models/FantasyDungeon/Textures/Arch/Arch_N.TGA");

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
	this->loadModel("Column02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Column/Column02.FBX");
	this->getModel("Column02")->materialNames.push_back("column");
	this->loadModel("Column03", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Column/Column03.FBX");
	this->getModel("Column03")->materialNames.push_back("column");
	this->loadModel("Column04", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Column/Column04.FBX");
	this->getModel("Column04")->materialNames.push_back("column");
	this->loadTexture("Column_D", "./Assets/Models/FantasyDungeon/Textures/Column/Column_D.TGA");
	this->loadTexture("Column_N", "./Assets/Models/FantasyDungeon/Textures/Column/Column_N.TGA");
	this->loadTexture("Column_SRM", "./Assets/Models/FantasyDungeon/Textures/Column/Column_SRM.TGA");
	this->loadTexture("Column_damage_D", "./Assets/Models/FantasyDungeon/Textures/Column/Column_damage_D.TGA");
	this->loadTexture("Column_damage_N", "./Assets/Models/FantasyDungeon/Textures/Column/Column_damage_N.TGA");

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
	this->loadModel("curbstone02_2a", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02_2a.FBX");
	this->getModel("curbstone02_2a")->materialNames.push_back("curbstone");
	this->loadModel("curbstone02_door1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02_door1.FBX");
	this->getModel("curbstone02_door1")->materialNames.push_back("curbstone");
	this->loadModel("curbstone02", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Curbstone/curbstone02.FBX");
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
	this->loadModel("Floor01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Floor/Floor01.FBX");
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
	this->loadModel("Roof01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Roof/Roof01.FBX");
	this->getModel("Roof01")->materialNames.push_back("brick1");
	this->getModel("Roof01")->materialNames.push_back("Roof");
	this->loadModel("Roof03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Roof/Roof03.FBX");
	this->getModel("Roof03")->materialNames.push_back("brick1");
	this->loadModel("Roof16", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Roof/Roof16.FBX");
	this->getModel("Roof16")->materialNames.push_back("brick1");
	this->getModel("Roof16")->materialNames.push_back("Roof");
	this->loadModel("RoofBorder01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Roof/RoofBorder01.FBX");
	this->getModel("RoofBorder01")->materialNames.push_back("Roof");
	this->loadTexture("Roof_D", "./Assets/Models/FantasyDungeon/Textures/Roof/Roof_D.TGA");
	this->loadTexture("Roof_N", "./Assets/Models/FantasyDungeon/Textures/Roof/Roof_N.TGA");

	// Stairs
	this->loadModel("Railing01_1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Stairs/Railing01_1.FBX");
	this->getModel("Railing01_1")->materialNames.push_back("Stairs");
	this->getModel("Railing01_1")->materialNames.push_back("Railing");
	this->loadModel("Railing_base01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Stairs/Railing_base01.FBX");
	this->getModel("Railing_base01")->materialNames.push_back("Stairs");
	this->loadModel("Stairs01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Stairs/Stairs01.FBX");
	this->getModel("Stairs01")->materialNames.push_back("Stairs");
	this->getModel("Stairs01")->materialNames.push_back("brick2");
	this->loadModel("StairsDamage01", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Stairs/StairsDamage01.FBX");
	this->getModel("StairsDamage01")->materialNames.push_back("Stairs");
	this->getModel("StairsDamage01")->materialNames.push_back("column_damage");
	this->getModel("StairsDamage01")->materialNames.push_back("brick2");
	this->loadTexture("Railing_D", "./Assets/Models/FantasyDungeon/Textures/Stairs/Railing_D.TGA");
	this->loadTexture("Railing_N", "./Assets/Models/FantasyDungeon/Textures/Stairs/Railing_N.TGA");
	this->loadTexture("Stairs_D", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_D.TGA");
	this->loadTexture("Stairs_N", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_N.TGA");
	this->loadTexture("Stairs_SRM", "./Assets/Models/FantasyDungeon/Textures/Stairs/Stairs_SRM.TGA");

	// Wall
	this->loadModel("Niche", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Niche.FBX");
	this->getModel("Niche")->materialNames.push_back("Stairs");
	this->getModel("Niche")->materialNames.push_back("brick2");
	this->loadModel("Wall01", MESH_ATTRIBUTE_INSTANCE, "./Assets/Models/FantasyDungeon/Meshes/Wall/Wall01.FBX");
	this->getModel("Wall01")->materialNames.push_back("brick2");
	this->loadModel("Wall08", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Wall08.FBX");
	this->getModel("Wall08")->materialNames.push_back("brick2");
	this->loadModel("wall09", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/wall09.FBX");
	this->getModel("wall09")->materialNames.push_back("brick2");
	this->loadModel("wall01Arch1", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/wall01Arch1.FBX");
	this->getModel("wall01Arch1")->materialNames.push_back("brick2");
	this->loadModel("WallDamaged02", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/WallDamaged02.FBX");
	this->getModel("WallDamaged02")->materialNames.push_back("BrickDamege");
	this->loadModel("WallDamaged03", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/WallDamaged03.FBX");
	this->getModel("WallDamaged03")->materialNames.push_back("BrickDamege");
	this->loadModel("Windows", MESH_ATTRIBUTE_DEFAULT, "./Assets/Models/FantasyDungeon/Meshes/Wall/Windows.FBX");
	this->getModel("Windows")->materialNames.push_back("Sink");
	this->getModel("Windows")->materialNames.push_back("Jail");
	this->loadTexture("brick1_D", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_D.TGA");
	this->loadTexture("brick1_N", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_N.TGA");
	this->loadTexture("brick1_SRM", "./Assets/Models/FantasyDungeon/Textures/Wall/brick1_SRM.TGA");
	this->loadTexture("brick2_D", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_D.TGA");
	this->loadTexture("brick2_N", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_N.TGA");
	this->loadTexture("brick2_SRM", "./Assets/Models/FantasyDungeon/Textures/Wall/brick2_SRM.TGA");
	this->loadTexture("Brick4_D", "./Assets/Models/FantasyDungeon/Textures/Wall/Brick4_D.TGA");
	this->loadTexture("Brick4_N", "./Assets/Models/FantasyDungeon/Textures/Wall/Brick4_N.TGA");
	this->loadTexture("Brick4_SRM", "./Assets/Models/FantasyDungeon/Textures/Wall/Brick4_SRM.TGA");

	// Materials
	// Arch
	this->createMaterial("Arch", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Arch")->addTexture("albedoMap", this->getTexture("Arch_D"));
	this->getMaterial("Arch")->addTexture("normalMap", this->getTexture("Arch_N"));
	this->getMaterial("Arch")->addTexture("maskMap", this->getTexture("Brick4_SRM"));
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
	// column_damage
	this->createMaterial("column_damage", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("column_damage")->addTexture("albedoMap", this->getTexture("Column_damage_D"));
	this->getMaterial("column_damage")->addTexture("normalMap", this->getTexture("Column_damage_N"));
	this->getMaterial("column_damage")->addTexture("maskMap", this->getTexture("Brick4_SRM"));
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
	// Railing
	this->createMaterial("Railing", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Railing")->addTexture("albedoMap", this->getTexture("Railing_D"));
	this->getMaterial("Railing")->addTexture("normalMap", this->getTexture("Railing_N"));
	this->getMaterial("Railing")->addTexture("maskMap", this->getTexture("Brick4_SRM"));
	// Roof
	this->createMaterial("Roof", this->getShader("FantasyDungeonSRM"));
	this->getMaterial("Roof")->addTexture("albedoMap", this->getTexture("Roof_D"));
	this->getMaterial("Roof")->addTexture("normalMap", this->getTexture("Roof_N"));
	this->getMaterial("Roof")->addTexture("maskMap", this->getTexture("Brick4_SRM"));
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
