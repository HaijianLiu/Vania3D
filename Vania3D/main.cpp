#include "Engine.hpp"

/*------------------------------------------------------------------------------
< globals >
------------------------------------------------------------------------------*/
Window* window = new Window("vania", SCREEN_WIDTH, SCREEN_HEIGHT);
RenderPass* renderPass = new RenderPass();
Game* game = new Game();


/*------------------------------------------------------------------------------
< clear >
------------------------------------------------------------------------------*/
void clear() {
	delete window;
	delete renderPass;
	delete game;
	glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW resources.
}


/*------------------------------------------------------------------------------
< start >
------------------------------------------------------------------------------*/
void start() {
	game->start();
	renderPass->init(1);
}


/*------------------------------------------------------------------------------
< update >
------------------------------------------------------------------------------*/
void update() {
	game->update();
}


/*------------------------------------------------------------------------------
< get functions >
------------------------------------------------------------------------------*/
Window* getWindow() {
	return window;
}
Game* getGame() {
	return game;
}


/*==============================================================================
< main >
==============================================================================*/
int main() {
	FMOD::System     *system;
	FMOD::Sound      *sound, *sound_to_play;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;
	void             *extradriverdata = 0;
	int               numsubsounds;

	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);

	if (version < FMOD_VERSION)
	{
		printf("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
	}

	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	result = system->createStream("./Assets/Audios/BGM/wave.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	result = sound->getNumSubSounds(&numsubsounds);

	if (numsubsounds)
	{
		sound->getSubSound(0, &sound_to_play);
	}
	else
	{
		sound_to_play = sound;
	}

result = system->playSound(sound_to_play, 0, false, &channel);





	// Start
	start();

	// Game Loop
	while (!glfwWindowShouldClose(window->window)) {
		if (glfwGetKey(window->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window->window, true);
		// Update
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderPass->begin();
			update();
		renderPass->end();
		renderPass->render();
		glfwSwapBuffers(window->window);
		glfwPollEvents();
	}

	// clear
	clear();
	return 0;
}
