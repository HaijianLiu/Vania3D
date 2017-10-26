
#include "Engine.hpp"

/*------------------------------------------------------------------------------
< Constructor >
------------------------------------------------------------------------------*/
Animation::Animation() {

}


/*------------------------------------------------------------------------------
< Destructor >
------------------------------------------------------------------------------*/
Animation::~Animation() {
	deleteVector(this->keyframes);
}



const Keyframe* Animation::findKeyframe(const std::string nodeName) const {
    for (unsigned int i = 0 ; i < this->keyframes.size() ; i++) {
        const Keyframe* keyframe = this->keyframes[i];

			if (keyframe->nodeName == nodeName) {
            return keyframe;
        }
    }

    return nullptr;
}
