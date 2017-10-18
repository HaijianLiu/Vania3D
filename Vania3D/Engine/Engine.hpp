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

#define UNIFORM_TEX_ALBEDO "albedo"
#define UNIFORM_TEX_NORMAL "normal"
#define UNIFORM_TEX_METALLIC "metallic"
#define UNIFORM_TEX_ROUGHNESS "roughness"
#define UNIFORM_TEX_AO "ao"
#define UNIFORM_TEX_EQUIRECTANGULAR "equirectangular"
#define UNIFORM_TEX_ENVIRONMENT "environment"
#define UNIFORM_MATRIX_MVP "matrixMVP"
#define UNIFORM_MATRIX_MODEL "model"
#define UNIFORM_MATRIX_VIEW "view"
#define UNIFORM_MATRIX_PROJECTION "projection"
#define UNIFORM_EYE_WORLDSPACE "eyeWorldspace"


/*------------------------------------------------------------------------------
< Libraries >
------------------------------------------------------------------------------*/
// Include standard libraries
#include <iostream>
#include <thread>
#include <random>
#include <typeindex>
#include <vector>
#include <map>
#include <unordered_map>
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
< Engine >
------------------------------------------------------------------------------*/
class GameObject;

#include "Window.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "Quad.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "LightProbe.hpp"
#include "Resources.hpp"
#include "RenderPass.hpp"
#include "Camera.hpp"
#include "Transform.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Game.hpp"


/*------------------------------------------------------------------------------
< Component >
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
< Scene >
------------------------------------------------------------------------------*/
#include "SceneManager.hpp"


/*------------------------------------------------------------------------------
< Global Functions >
------------------------------------------------------------------------------*/
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
