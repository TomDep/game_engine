#include "Renderer.h"

#include <glad/glad.h>	
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include "Material.h"

Renderer::Renderer(Camera* camera) : camera(camera) {
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	spdlog::info("* Maximum number of vertex attributes supported: {}", nrAttributes);

	shader = new Shader("res/shaders/default");
	shader->use();

	// Vertex Buffer Object
	spdlog::debug("[INITIALIZATION][RENDERER] Creating VBO");

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	/*
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	// VAO
	spdlog::debug("[INITIALIZATION][RENDERER] Creating cube VAO");
	glGenVertexArrays(1, &VAO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);

	spdlog::debug("[INITIALIZATION][RENDERER] Creating light VAO");
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Camera :
	spdlog::debug("[INITIALIZATION][RENDERER] Setting camera settings");
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), (float)camera->getWidth() / (float)camera->getHeight(), 0.1f, 100.0f);

	glm::mat4 model = glm::mat4(1.0f);
	shader->setMatrix4x4("model", model);
	shader->setMatrix4x4("view", camera->getView());
	shader->setMatrix4x4("projection", projection);

	shader->setVector3("viewPos", camera->getPosition());
	
	// Material
	spdlog::debug("[INITIALIZATION][RENDERER] Loading texture");
	Texture* texture = new Texture("res/images/container2.png", true);

	spdlog::debug("[INITIALIZATION][RENDERER] Creating material");
	material = new Material(texture, glm::vec3(0.5f), 128.0f);
	material->use(shader);

	// Render the light source
	spdlog::debug("[INITIALIZATION][RENDERER] Creating the light shader");
	lightShader = new Shader("res/shaders/lightShader");
	lightShader->use();

	lightShader->setMatrix4x4("view", camera->getView());
	lightShader->setMatrix4x4("projection", projection);
}

void Renderer::render() {
	std::vector<Entity*> entities = *currentScene->getEntities();
	for (Entity* entity : entities) {
		// Check for components
		if (entity->getPointLightEmitter() != nullptr && pointLights.size() < MAX_POINT_LIGHT) {
			pointLights.push_back(entity->getPointLightEmitter());
			pointLightPositions.push_back(entity->getTransform()->getPosition());
		}
		
		if (entity->getDirectionalLightEmitter() != nullptr)
			directionalLight = entity->getDirectionalLightEmitter();

		if (entity->getMeshRenderer() != nullptr && entity->getMesh() != nullptr) {
			renderEntity(entity);
		}
	}

	renderLights();

	// Reset the lights
	directionalLight = nullptr;
	pointLights.clear();
	pointLightPositions.clear();
}

void Renderer::renderEntity(Entity * entity) {
	shader->use();
	shader->setVector3("viewPos", camera->getPosition());
	shader->setMatrix4x4("view", camera->getView());

	material->use(shader);

	// For now there is only one mesh for all entities
	glBindVertexArray(VAO);

	// render entity
	shader->setMatrix4x4("model", entity->getTransform()->getModelMatrix());
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::renderLights() {
	shader->use();
	glBindVertexArray(lightVAO);

	// Camera position
	shader->setVector3("viewPos", camera->getPosition());

	// Directional light
	if (directionalLight != nullptr) {
		shader->setVector3("dirLight.direction", directionalLight->getDirection());

		shader->setVector3("dirLight.ambient", directionalLight->getAmbient());
		shader->setVector3("dirLight.diffuse", directionalLight->getDiffuse());
		shader->setVector3("dirLight.specular", directionalLight->getSpecular());
	}
	else {
		spdlog::debug("[RENDERER] There is no directional light");
	}
	
	// Point lights

	// Number of point lights
	shader->setInt("nbOfPointLights", pointLights.size());
	
	for (int i = 0; i < pointLights.size(); i++) {
		PointLightEmitter* light = pointLights[i];

		shader->use();
		shader->setVector3("pointLights[" + to_string(i) + "].position", pointLightPositions.at(i));

		shader->setVector3("pointLights[" + to_string(i) + "].ambient", light->getAmbient());
		shader->setVector3("pointLights[" + to_string(i) + "].diffuse", light->getDiffuse());
		shader->setVector3("pointLights[" + to_string(i) + "].specular", light->getSpecular());
		
		shader->setFloat("pointLights[" + to_string(i) + "].constant", light->getConstant());
		shader->setFloat("pointLights[" + to_string(i) + "].linear", light->getLinear());
		shader->setFloat("pointLights[" + to_string(i) + "].quadratic", light->getQuadratic());
	
		// Render cubes for point lights
		lightShader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions.at(i));
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader->setMatrix4x4("model", model);
		lightShader->setMatrix4x4("view", camera->getView());
		lightShader->setVector3("lightColor", light->getDiffuse());
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}