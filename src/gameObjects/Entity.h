#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components/Transform.h"
#include "../physics/RigidBody.h"
#include "components/lights/DirectionalLightEmitter.h"
#include "components/lights/PointLightEmitter.h"
#include "components/Mesh.h"
#include "components/MeshRenderer.h"

#include <string>
#include <vector>

class Entity {
public:
	Entity();
	Entity(glm::vec3 p, glm::vec3 s, glm::vec3 r);

	void addChild(Entity* entity) { children.push_back(entity); }
	std::vector<Entity*> getChildren() const { return children; }
	bool hasChildren() const { return children.size() > 0; }

	/* ---------- Getters & Setters ---------- */
	int getId() const { return id; }
	void setId(int _id) { id = _id; }

	std::string getName() const { return name; }
	void setName(std::string newName) { name = newName; }

	void addDirectionalLightEmitter(DirectionalLightEmitter* emitter) { directionalLightEmitter = emitter; }
	DirectionalLightEmitter* getDirectionalLightEmitter() const { return directionalLightEmitter; }

	void addPointLightEmitter(PointLightEmitter* emitter) { pointLightEmitter = emitter; }
	PointLightEmitter* getPointLightEmitter() const { return pointLightEmitter; }

	void addRigidBody(RigidBody* rb) { rigidBody = rb; }
	RigidBody* getRigidBody() const { return rigidBody; }

	Transform* getTransform() const { return transform; }

	void addMesh(Mesh* m) { mesh = m; }
	Mesh* getMesh() const { return mesh; }

	void addMeshRenderer(MeshRenderer* mR) { meshRenderer = mR; }
	MeshRenderer* getMeshRenderer() const { return meshRenderer; }
private:
	int id = 0;
	std::string name = "Entity";

	// Children
	std::vector<Entity*> children;

	/* Components */
	Transform* transform = nullptr;
	RigidBody* rigidBody = nullptr;
	DirectionalLightEmitter* directionalLightEmitter = nullptr;
	PointLightEmitter* pointLightEmitter = nullptr;
	Mesh* mesh = nullptr;
	MeshRenderer* meshRenderer = nullptr;
};

