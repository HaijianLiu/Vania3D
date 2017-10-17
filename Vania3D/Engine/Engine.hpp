#ifndef Engine_hpp
#define Engine_hpp

/*------------------------------------------------------------------------------
< Macross >
------------------------------------------------------------------------------*/
#define PIXEL_TO_UNIT (0.01f)
#define UNIT_TO_PIXEL (100)
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)
#define PI (3.141593f)


/*------------------------------------------------------------------------------
< Libraries >
------------------------------------------------------------------------------*/
// Include standard libraries
#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

// include GLEW (include before gl.h and glfw.h)
#include <GL/glew.h>
// include GLFW
#include <GLFW/glfw3.h>
// include GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include FMOD
#include <fmod.hpp>
// include assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// include stbi
#include "stb_image.h"


/*------------------------------------------------------------------------------
< Core >
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
< Engine >
------------------------------------------------------------------------------*/
#include "Window.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Resources.hpp"
#include "RenderPass.hpp"
#include "Camera.hpp"


/*------------------------------------------------------------------------------
< GameObject >
------------------------------------------------------------------------------*/
// #include "NoneObject.hpp"
// #include "OffsetObject.hpp"
// #include "TileObject.hpp"
// #include "Player.hpp"


/*------------------------------------------------------------------------------
< Scene >
------------------------------------------------------------------------------*/
#include "Game.hpp"
// #include "Scene.hpp"
// #include "SceneManager.hpp"
// #include "Scene00.hpp"


/*------------------------------------------------------------------------------
< Global Functions >
------------------------------------------------------------------------------*/
// main.cpp
Window* getWindow();
Game* getGame();

// Engine.cpp
// void instantiate(GameObject* gameObject, Transform* transform);
// float randomRange(float min, float max);
// void destroyByTime(GameObject* gameObject, float time);
// float distancePow(glm::vec3 a, glm::vec3 b);

template <typename T, typename U> void doMap(T map, void (*function)(U)) {
	for (typename T::iterator it = map.begin(); it != map.end(); it++) {
		function(it->first);
	}
}

template <typename T> void deleteMap(T map) {
	for (typename T::iterator it = map.begin(); it != map.end(); it++) {
		delete it->second;
	}
	map.clear();
}

template <typename T> void deleteVector(T vector) {
	for (unsigned int i = 0; i < vector.size(); i++) {
		delete vector[i];
	}
	vector.shrink_to_fit();
}

template <typename T> void deleteVectorMap(T map) {
	for (typename T::iterator it = map.begin(); it != map.end(); it++) {
		deleteVector(it->second);
	}
	map.clear();
}

#endif /* Engine_hpp */
