#ifndef Engine_hpp
#define Engine_hpp

/*------------------------------------------------------------------------------
< Macross >
------------------------------------------------------------------------------*/
#define PIXEL_TO_UNIT (0.01)
#define UNIT_TO_PIXEL (100)
#define SCREEN_WIDTH (960)
#define SCREEN_HEIGHT (540)
//#define SCREEN_WIDTH (640)
//#define SCREEN_HEIGHT (360)
#define PI (3.141593)
#define NUM_BONES_PER_VEREX (4)

#define UNIFORM_TEX_ALBEDO "albedoMap" // GL_TEXTURE0
#define UNIFORM_TEX_NORMAL "normalMap" // GL_TEXTURE1
#define UNIFORM_TEX_MASK "maskMap" // GL_TEXTURE2

#define UNIFORM_TEX_IRRADIANCE "irradianceMap" // GL_TEXTURE10
#define UNIFORM_TEX_PREFILTER "prefilterMap" // GL_TEXTURE11
#define UNIFORM_TEX_BRDFLUT "brdfLUT" // GL_TEXTURE12
#define UNIFORM_TEX_SHADOW "shadowMap" // GL_TEXTURE13

#define UNIFORM_TEX_EQUIRECTANGULAR "equirectangular" // light probe
#define UNIFORM_TEX_ENVIRONMENT "environment" // light probe
#define UNIFORM_TEX_PASSES "passes" // render pass shader

#define UNIFORM_VEC3_CAMERA_POSITION "cameraPosition"
#define UNIFORM_VEC3_LIGHT_POSITION "lightPositions" // light
#define UNIFORM_VEC3_LIGHT_COLOR "lightColors" // light
#define UNIFORM_VEC3_COLOR "color" // flat color shader

#define UNIFORM_MATRIX_MODEL "model"
#define UNIFORM_MATRIX_VIEW "view"
#define UNIFORM_MATRIX_PROJECTION "projection"
#define UNIFORM_MATRIX_BONE "bones"
#define UNIFORM_MATRIX_LIGHTSPACE "lightSpaceMatrix"


/*------------------------------------------------------------------------------
< Libraries >
------------------------------------------------------------------------------*/
// Include standard libraries
#include <iostream>
#include <thread>
#include <math.h>
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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// include assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// include stbi
#include "stb_image.h"

// include FMOD
#include <fmod.hpp>

class GameObject;
class Game;
class SceneManager;
class Camera;
class Scene;


/*------------------------------------------------------------------------------
< Engine >
------------------------------------------------------------------------------*/
#include "Window.hpp"
#include "Input.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "Quad.hpp"
#include "Time.hpp"

#include "Node.hpp"
#include "Mesh.hpp"
#include "Bone.hpp"
#include "Keyframe.hpp"
#include "Animation.hpp"
#include "Model.hpp"
#include "Level.hpp"

#include "Sphere.hpp"

#include "Material.hpp"
#include "LightProbe.hpp"

#include "Component.hpp"
#include "Transform.hpp"
#include "MeshRenderer.hpp"
#include "PointLight.hpp"

#include "Resources.hpp"
#include "RenderPass.hpp"
#include "ShadowMapping.hpp"
#include "Camera.hpp"

#include "GameObject.hpp"

#include "Game.hpp"


/*------------------------------------------------------------------------------
< Component >
------------------------------------------------------------------------------*/
#include "Offset.hpp"
#include "PlayerController.hpp"
#include "CameraController.hpp"


/*------------------------------------------------------------------------------
< Scene >
------------------------------------------------------------------------------*/
#include "Scene.hpp"
#include "Scene00.hpp"
#include "Scene01.hpp"
#include "SceneManager.hpp"


/*------------------------------------------------------------------------------
< delete container >
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


/*------------------------------------------------------------------------------
< convert from Assimp to glm >
------------------------------------------------------------------------------*/
glm::vec3 assignment(const aiVector3D& assimpVector);
glm::quat assignment(const aiQuaternion& assimpQuaternion);
glm::mat4 assignment(const aiMatrix4x4& assimpMatrix);
glm::mat4 assignment(const aiMatrix3x3& assimpMatrix);


#endif /* Engine_hpp */
