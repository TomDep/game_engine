#include "SceneSerializer.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <ostream>

SceneSerializer::SceneSerializer(Scene* scene) : scene(scene) {

}

void SceneSerializer::serialize(const std::string& filepath) {
	YAML::Emitter out;
	out << YAML::BeginMap;
	
	out << YAML::Key << "Scene";
	out << YAML::Value << "Scene name";
	
	out << YAML::Key << "Entities";
	out << YAML::Value << YAML::BeginSeq;
		for (Entity* entity : *scene->getEntities()) {
			serializeEntity(out, entity);
		}
	out << YAML::EndSeq;

	out << YAML::Key << "Lights";
	out << YAML::Value << YAML::BeginMap;
		DirectionalLight* directionalLight = scene->getDirectionalLight();
		if(directionalLight != nullptr) serializeDirectionalLight(out, directionalLight);

		int nbOfPointLights = scene->getNumberOfPointLight();
		if (nbOfPointLights > 0) {
			out << YAML::Key << "PointLights";
			out << YAML::Value << YAML::BeginSeq;
			for (int i = 0; i < nbOfPointLights; i++) {
				serializePointLight(out, scene->getPointLights()[i]);
			}
			out << YAML::EndSeq;
		}
	out << YAML::EndMap;

	out << YAML::EndMap;
	std::ofstream fout(filepath);
	fout << out.c_str();
}

void SceneSerializer::serializeVector3(YAML::Emitter& out, glm::vec3 vector) {
	out << YAML::BeginMap;
		out << YAML::Key << "X";
		out << YAML::Value << vector.x;
		out << YAML::Key << "Y";
		out << YAML::Value << vector.y;
		out << YAML::Key << "Z";
		out << YAML::Value << vector.z;
	out << YAML::EndMap;
}

void SceneSerializer::serializeEntity(YAML::Emitter& out, Entity* entity) {
	out << YAML::BeginMap;
		out << YAML::Key << "Entity";
		out << YAML::Value << 854318965;
	
		// Transform component
		out << YAML::Key << "Position";
		serializeVector3(out, entity->getPosition());
		out << YAML::Key << "Scale";
		serializeVector3(out, entity->getScale());
		out << YAML::Key << "Rotation";
		serializeVector3(out, entity->getRotation());

		// RigidBody component
		RigidBody* rigidBody = entity->getRigidBody();
		if (rigidBody != nullptr) {
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Dynamic" << YAML::Value << rigidBody->isDynamic();
			out << YAML::EndMap;
		}
	
	out << YAML::EndMap;
}

void SceneSerializer::serializeDirectionalLight(YAML::Emitter& out, DirectionalLight* directionalLight) {	
	out << YAML::Key << "DirectionalLight";
	out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Direction";
		serializeVector3(out, directionalLight->getDirection());
		serializeLight(out, directionalLight);
	out << YAML::EndMap;
}

void SceneSerializer::serializePointLight(YAML::Emitter& out, PointLight* pointLight) {
	out << YAML::BeginMap;
		out << YAML::Key << "PointLight" << YAML::Value << pointLight->getId();
		out << YAML::Key << "Position";
		serializeVector3(out, pointLight->getPosition());
		out << YAML::Key << "Constant";
		out << YAML::Value << pointLight->getConstant();
		out << YAML::Key << "Linear";
		out << YAML::Value << pointLight->getLinear();
		out << YAML::Key << "Quadratic";
		out << YAML::Value << pointLight->getQuadratic();
		serializeLight(out, pointLight);
	out << YAML::EndMap;
}

void SceneSerializer::serializeLight(YAML::Emitter& out, Light* light) {
	out << YAML::Key << "Ambient";
	serializeVector3(out, light->getAmbient());
	out << YAML::Key << "Diffuse";
	serializeVector3(out, light->getDiffuse());
	out << YAML::Key << "Specular";
	serializeVector3(out, light->getSpecular());
}

void SceneSerializer::deserialize(const std::string& filepath) {
	std::ifstream stream(filepath);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Scene"]) {
		spdlog::warn("Could not load scene : file format is incorrect.");
	}

	std::string sceneName = data["Scene"].as<std::string>();
	auto entities = data["Entities"];
	if (entities) {
		for (auto entity : entities) {
			scene->addEntity(deserializeEntity(entity));
		}
	}

	auto lights = data["Lights"];
	auto directionalLightNode = lights["DirectionalLight"];
	if (directionalLightNode) {
		scene->setDirectionalLight(deserializeDirectionalLight(directionalLightNode));
	}

	auto pointLights = lights["PointLights"];
	if (pointLights) {
		for (auto pointLight : pointLights) {
			scene->addPointLight(deserializePointLight(pointLight));
		}
	}
}

Entity* SceneSerializer::deserializeEntity(YAML::detail::iterator_value entityNode) {
	//int id = entityNode["Id"].as<int>();
	glm::vec3 position = deserializeVector3(entityNode["Position"]);
	glm::vec3 scale = deserializeVector3(entityNode["Scale"]);
	glm::vec3 rotation = deserializeVector3(entityNode["Rotation"]);

	// Create the entity
	Entity* entity = new Entity(position, scale, rotation);
	//entity->setId(id);

	// Components
	auto rigidBodyNode = entityNode["RigidBody"];
	if (rigidBodyNode) {
		RigidBody* rigidBody = deserializeRigidBody(rigidBodyNode);
		rigidBody->setPosition(entity->getPosition());
		entity->addRigidBody(rigidBody);
	}

	return entity;
}

glm::vec3 SceneSerializer::deserializeVector3(YAML::Node vector) {
	float x = vector["X"].as<float>();
	float y = vector["Y"].as<float>();
	float z = vector["Z"].as<float>();
	return glm::vec3(x, y, z);
}

RigidBody* SceneSerializer::deserializeRigidBody(YAML::Node rigidBodyNode) {
	RigidBody* rigidBody = new RigidBody();

	bool dynamic = rigidBodyNode["Dynamic"].as<bool>();
	rigidBody->setDynamic(dynamic);

	return rigidBody;
}

void SceneSerializer::deserializeLight(YAML::Node lightNode, Light* light) {
	glm::vec3 ambient = deserializeVector3(lightNode["Ambient"]);
	glm::vec3 diffuse = deserializeVector3(lightNode["Diffuse"]);
	glm::vec3 specular = deserializeVector3(lightNode["Specular"]);
	//int id = lightNode["Id"].as<int>();

	light->setAmbient(ambient);
	light->setDiffuse(diffuse);
	light->setSpecular(specular);
	//light->setId(id);
}

DirectionalLight* SceneSerializer::deserializeDirectionalLight(YAML::Node directionalLightNode) {
	glm::vec3 direction = deserializeVector3(directionalLightNode["Direction"]);
	
	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->setDirection(direction);

	deserializeLight(directionalLightNode, directionalLight);
	return directionalLight;
}

PointLight* SceneSerializer::deserializePointLight(YAML::Node pointLightNode) {
	glm::vec3 position = deserializeVector3(pointLightNode["Position"]);
	float constant = pointLightNode["Constant"].as<float>();
	float linear = pointLightNode["Linear"].as<float>();
	float quadratic = pointLightNode["Quadratic"].as<float>();

	PointLight* pointLight = new PointLight();
	pointLight->setPosition(position);
	pointLight->setConstant(constant);
	pointLight->setLinear(linear);
	pointLight->setQuadratic(quadratic);

	deserializeLight(pointLightNode, pointLight);
	return pointLight;
}