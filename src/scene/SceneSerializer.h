#pragma once

#include "Scene.h"
#include "../gameObjects/Entity.h"
#include "../gameObjects/components/lights/DirectionalLightEmitter.h"
#include "../gameObjects/components/lights/PointLightEmitter.h"
#include "../gameObjects/components/lights/LightEmitter.h"
#include "../physics/RigidBody.h"

#include <string>
#include <yaml-cpp/yaml.h>

#include <glm/glm.hpp> 

class SceneSerializer {

public:
	SceneSerializer(Scene* scene);

	void serialize(const std::string& filepath);

	void serializeVector3(YAML::Emitter& out, glm::vec3 vector);
	void serializeEntity(YAML::Emitter& out, Entity* entity);

	void deserialize(const std::string& filepath);

	Entity* deserializeEntity(YAML::detail::iterator_value entity);
	glm::vec3 deserializeVector3(YAML::Node);
	
	// Components
	RigidBody* deserializeRigidBody(YAML::Node rigidBodyNode);
	
	// Lights
	void deserializeLight(YAML::Node lightNode, LightEmitter* light);
	DirectionalLightEmitter* deserializeDirectionalLight(YAML::Node directionalLightNode);
	PointLightEmitter* deserializePointLight(YAML::Node);
private:
	Scene* scene;
};
