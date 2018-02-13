
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
LutController::LutController() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
LutController::~LutController() {

}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void LutController::start() {
	Game* game = Game::getInstance();
	// defalut
	this->luts.push_back(game->resources->getTexture("clut_default_a")->id);
	// luts
	game->resources->loadTexture("clut_filmic_a", "./Assets/Textures/LUT/clut_filmic_a.TGA");
	Texture::setTexMode(game->resources->getTexture("clut_filmic_a")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("clut_filmic_a")->id);
	
	game->resources->loadTexture("clut_filmic_b", "./Assets/Textures/LUT/clut_filmic_b.TGA");
	Texture::setTexMode(game->resources->getTexture("clut_filmic_b")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("clut_filmic_b")->id);
	
	game->resources->loadTexture("LUT_BleachBypass", "./Assets/Textures/LUT/LUT_BleachBypass.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_BleachBypass")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_BleachBypass")->id);
	
	game->resources->loadTexture("LUT_Bunker_01", "./Assets/Textures/LUT/LUT_Bunker_01.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Bunker_01")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Bunker_01")->id);
	
	game->resources->loadTexture("LUT_CandleLight", "./Assets/Textures/LUT/LUT_CandleLight.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_CandleLight")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_CandleLight")->id);
	
	game->resources->loadTexture("LUT_CoolContrast", "./Assets/Textures/LUT/LUT_CoolContrast.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_CoolContrast")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_CoolContrast")->id);
	
	game->resources->loadTexture("LUT_DesaturatedFog", "./Assets/Textures/LUT/LUT_DesaturatedFog.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_DesaturatedFog")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_DesaturatedFog")->id);
	
	game->resources->loadTexture("LUT_Fall", "./Assets/Textures/LUT/LUT_Fall.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Fall")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Fall")->id);
	
	game->resources->loadTexture("LUT_Filmic", "./Assets/Textures/LUT/LUT_Filmic.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic")->id);
	
	game->resources->loadTexture("LUT_Filmic2", "./Assets/Textures/LUT/LUT_Filmic2.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic2")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic2")->id);
	
	game->resources->loadTexture("LUT_Filmic3", "./Assets/Textures/LUT/LUT_Filmic3.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic3")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic3")->id);
	
	game->resources->loadTexture("LUT_Filmic4", "./Assets/Textures/LUT/LUT_Filmic4.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic4")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic4")->id);
	
	game->resources->loadTexture("LUT_Filmic5", "./Assets/Textures/LUT/LUT_Filmic5.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic5")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic5")->id);
	
	game->resources->loadTexture("LUT_Filmic6", "./Assets/Textures/LUT/LUT_Filmic6.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic6")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic6")->id);
	
	game->resources->loadTexture("LUT_Filmic9", "./Assets/Textures/LUT/LUT_Filmic9.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Filmic9")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Filmic9")->id);
	
	game->resources->loadTexture("LUT_Warm", "./Assets/Textures/LUT/LUT_Warm.TGA");
	Texture::setTexMode(game->resources->getTexture("LUT_Warm")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("LUT_Warm")->id);
	
	game->resources->loadTexture("TX_LUT1", "./Assets/Textures/LUT/TX_LUT1.TGA");
	Texture::setTexMode(game->resources->getTexture("TX_LUT1")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("TX_LUT1")->id);
	
	game->resources->loadTexture("TX_LUT2", "./Assets/Textures/LUT/TX_LUT2.TGA");
	Texture::setTexMode(game->resources->getTexture("TX_LUT2")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("TX_LUT2")->id);
	
	game->resources->loadTexture("TX_LUT3", "./Assets/Textures/LUT/TX_LUT3.TGA");
	Texture::setTexMode(game->resources->getTexture("TX_LUT3")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("TX_LUT3")->id);
	
	game->resources->loadTexture("TX_LUT4", "./Assets/Textures/LUT/TX_LUT4.TGA");
	Texture::setTexMode(game->resources->getTexture("TX_LUT4")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("TX_LUT4")->id);
	
	game->resources->loadTexture("TX_LUT5", "./Assets/Textures/LUT/TX_LUT5.TGA");
	Texture::setTexMode(game->resources->getTexture("TX_LUT5")->id, TEXTURE_MODE_NEAREST);
	this->luts.push_back(game->resources->getTexture("TX_LUT5")->id);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void LutController::update() {
	if (this->game->input->getJoystickTrigger(JOY_RIGHT)) {
		this->currentLutIndex ++;
		if (this->currentLutIndex > this->luts.size() - 1)
			this->currentLutIndex = 0;
		this->game->renderPipeline->renderPasses[7]->dynamicTextureAttachments[1].texture = this->luts.at(this->currentLutIndex);
	}
	
	if (this->game->input->getJoystickTrigger(JOY_LEFT)) {
		this->currentLutIndex --;
		if (this->currentLutIndex < 0)
			this->currentLutIndex = this->luts.size() - 1;
		this->game->renderPipeline->renderPasses[7]->dynamicTextureAttachments[1].texture = this->luts.at(this->currentLutIndex);
	}
	
	if (this->game->input->getButtonTrigger(GLFW_KEY_LEFT)) {
		this->currentLutIndex ++;
		if (this->currentLutIndex > this->luts.size() - 1)
			this->currentLutIndex = 0;
		this->game->renderPipeline->renderPasses[7]->dynamicTextureAttachments[1].texture = this->luts.at(this->currentLutIndex);
	}
	
	if (this->game->input->getButtonTrigger(GLFW_KEY_RIGHT)) {
		this->currentLutIndex --;
		if (this->currentLutIndex < 0)
			this->currentLutIndex = this->luts.size() - 1;
		this->game->renderPipeline->renderPasses[7]->dynamicTextureAttachments[1].texture = this->luts.at(this->currentLutIndex);
	}
}
