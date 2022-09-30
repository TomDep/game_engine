#pragma once

#include "Scene.h"
#include "../gameObjects/Entity.h"
#include "../graphics/lights/DirectionalLight.h"
#include "../graphics/lights/PointLight.h"
#include "../graphics/lights/Light.h"
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
	void serializeLight(YAML::Emitter& out, Light* light);
	void serializeDirectionalLight(YAML::Emitter& out, DirectionalLight* directionalLight);
	void serializePointLight(YAML::Emitter& out, PointLight* pointLight);

	void deserialize(const std::string& filepath);

	Entity* deserializeEntity(YAML::detail::iterator_value entity);
	glm::vec3 deserializeVector3(YAML::Node);
	
	// Components
	RigidBody* deserializeRigidBody(YAML::Node rigidBodyNode);
	
	// Lights
	void deserializeLight(YAML::Node lightNode, Light* light);
	DirectionalLight* deserializeDirectionalLight(YAML::Node directionalLightNode);
	PointLight* deserializePointLight(YAML::Node);
private:
	Scene* scene;
};
