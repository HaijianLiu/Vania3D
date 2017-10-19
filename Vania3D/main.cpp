#include "Engine.hpp"


/*==============================================================================
< main >
==============================================================================*/
int main() {
	Game* game = Game::getInstance();
	game->start();

	// FMOD::System     *system;
	// FMOD::Sound      *sound, *sound_to_play;
	// FMOD::Channel    *channel = 0;
	// FMOD_RESULT       result;
	// unsigned int      version;
	// void             *extradriverdata = 0;
	// int               numsubsounds;
	//
	// result = FMOD::System_Create(&system);
	// result = system->getVersion(&version);
	//
	// if (version < FMOD_VERSION)
	// {
	// 	printf("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	// }
	//
	// result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	// result = system->createStream("./Assets/Audios/BGM/wave.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	// result = sound->getNumSubSounds(&numsubsounds);
	//
	// if (numsubsounds)
	// {
	// 	sound->getSubSound(0, &sound_to_play);
	// }
	// else
	// {
	// 	sound_to_play = sound;
	// }
	//
	// result = system->playSound(sound_to_play, 0, false, &channel);


	LightProbe* lightProbe = new LightProbe(game->resources->getTexture("hdr"));


	RenderPass* renderPass = new RenderPass();
	renderPass->init(1);

	game->resources->getShader("renderPass")->use();
	game->resources->getShader("renderPass")->setInt("test", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, lightProbe->brdf);


	// Game Loop
	while (!glfwWindowShouldClose(game->window->window)) {
		if (glfwGetKey(game->window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(game->window->window, true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderPass->begin();
		game->update();
		renderPass->end();
		renderPass->render();
		glfwSwapBuffers(game->window->window);
		glfwPollEvents();
	}

	delete game;
	delete renderPass;
	glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW resources.
	return 0;
}
