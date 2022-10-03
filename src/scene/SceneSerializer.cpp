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
		out << YAML::Key << "Id";
		out << YAML::Value << 854318965;
	
		// Transform component
		out << YAML::Key << "Position";
		serializeVector3(out, entity->getTransform()->getPosition());
		out << YAML::Key << "Scale";
		serializeVector3(out, entity->getTransform()->getScale());
		out << YAML::Key << "Rotation";
		serializeVector3(out, entity->getTransform()->getRotation());

		// RigidBody component
		RigidBody* rigidBody = entity->getRigidBody();
		if (rigidBody != nullptr) {
			out << YAML::Key << "RigidBodyComponent";
			out << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Dynamic" << YAML::Value << rigidBody->isDynamic();
			out << YAML::EndMap;
		}

		// DirectionalLightEmitter component
		DirectionalLightEmitter* directionalLightEmitter = entity->getDirectionalLightEmitter();
		if (directionalLightEmitter != nullptr) {
			out << YAML::Key << "DirectionalLightEmitter";
			out << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Direction";
				serializeVector3(out, directionalLightEmitter->getDirection());
				out << YAML::Key << "Ambient";
				serializeVector3(out, directionalLightEmitter->getAmbient());
				out << YAML::Key << "Diffuse";
				serializeVector3(out, directionalLightEmitter->getDiffuse());
				out << YAML::Key << "Specular";
				serializeVector3(out, directionalLightEmitter->getSpecular());
			out << YAML::EndMap;
		}

		PointLightEmitter* pointLightEmitter = entity->getPointLightEmitter();
		if (pointLightEmitter != nullptr) {
			out << YAML::Key << "PointLightEmitter";
			out << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Ambient";
				serializeVector3(out, pointLightEmitter->getAmbient());
				out << YAML::Key << "Diffuse";
				serializeVector3(out, pointLightEmitter->getDiffuse());
				out << YAML::Key << "Specular";
				serializeVector3(out, pointLightEmitter->getSpecular());

				out << YAML::Key << "Constant";
				out << YAML::Value << pointLightEmitter->getConstant();
				out << YAML::Key << "Linear";
				out << YAML::Value << pointLightEmitter->getLinear();
				out << YAML::Key << "Specular";
				out << YAML::Value << pointLightEmitter->getQuadratic();
			out << YAML::EndMap;
		}
	
	out << YAML::EndMap;
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
}

Entity* SceneSerializer::deserializeEntity(YAML::detail::iterator_value entityNode) {
	int id = entityNode["Id"].as<int>();
	glm::vec3 position = deserializeVector3(entityNode["Position"]);
	glm::vec3 scale = deserializeVector3(entityNode["Scale"]);
	glm::vec3 rotation = deserializeVector3(entityNode["Rotation"]);

	// Create the entity
	Entity* entity = new Entity(position, scale, rotation);
	entity->setId(id);

	// Components

	// RigidBody
	auto rigidBodyNode = entityNode["RigidBody"];
	if (rigidBodyNode) {
		RigidBody* rigidBody = deserializeRigidBody(rigidBodyNode);
		rigidBody->setPosition(entity->getTransform()->getPosition());
		entity->addRigidBody(rigidBody);
	}

	// PointLightEmitter
	auto pointLightEmitterNode = entityNode["PointLightEmitter"];
	if (pointLightEmitterNode) {
		glm::vec3 ambient = deserializeVector3(pointLightEmitterNode["Ambient"]);
		glm::vec3 diffuse = deserializeVector3(pointLightEmitterNode["Diffuse"]);
		glm::vec3 specular = deserializeVector3(pointLightEmitterNode["Specular"]);
		float constant = pointLightEmitterNode["Consstant"].as<float>();
		float linear = pointLightEmitterNode["Linear"].as<float>();
		float quadratic = pointLightEmitterNode["Quadratic"].as<float>();

		PointLightEmitter* pointLightEmitter = new PointLightEmitter(constant, linear, quadratic, ambient, diffuse, specular);
		entity->addPointLightEmitter(pointLightEmitter);
	}

	// DirectionalLightEmitter
	auto directionalLightEmitterNode = entityNode["DirectionalLightEmitter"];
	if (directionalLightEmitterNode) {
		glm::vec3 ambient = deserializeVector3(directionalLightEmitterNode["Ambient"]);
		glm::vec3 diffuse = deserializeVector3(directionalLightEmitterNode["Diffuse"]);
		glm::vec3 specular = deserializeVector3(directionalLightEmitterNode["Specular"]);
		glm::vec3 direction = deserializeVector3(directionalLightEmitterNode["Direction"]);

		DirectionalLightEmitter* directionalLightEmitter = new DirectionalLightEmitter(direction, ambient, diffuse, specular);
		entity->addDirectionalLightEmitter(directionalLightEmitter);
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