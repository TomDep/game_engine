#include "Renderer.h"

#include <glad/glad.h>	
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(Camera* camera) : camera(camera) {
	spdlog::info("Initializing the Renderer ...");

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	spdlog::info("* Maximum number of vertex attributes supported: {}", nrAttributes);

	shader = new Shader("res/shaders/default");
	shader->use();

	// Vertex Buffer Object
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

	// Adding texture
	texture = new Texture("res/images/container2.png", true);
	shader->setInt("material.diffuse", 0);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Adding perspective
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float) 1080 / (float) 720, 0.1f, 100.0f);

	// Transformation
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Camera :
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

	shader->setMatrix4x4("model", model);
	shader->setMatrix4x4("view", camera->getView());
	shader->setMatrix4x4("projection", projection);
	
	// Lighting
	lightShader = new Shader("res/shaders/lightShader");

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	shader->setVector3("material.ambient", 1.0f, 0.5f, 0.31f);
	shader->setVector3("material.diffuse", 1.0f, 0.5f, 0.31f);
	shader->setVector3("material.specular", 0.5f, 0.5f, 0.5f);
	shader->setFloat("material.shininess", 32.0f);
	
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

	shader->setVector3("light.ambient", ambientColor);
	shader->setVector3("light.diffuse", diffuseColor); // darken diffuse light a bit
	shader->setVector3("light.specular", glm::vec3(1.0f));
	shader->setVector3("light.color", lightColor);
	shader->setVector3("light.position", lightPos);
	shader->setVector3("viewPos", camera->getPosition());

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader->use();
	lightShader->setMatrix4x4("model", model);
	lightShader->setMatrix4x4("view", camera->getView());
	lightShader->setMatrix4x4("projection", projection);
	lightShader->setVector3("lightColor", lightColor);

	spdlog::info("Done!");
}

void Renderer::render() {

	// Draw the light
	lightShader->use();
	lightShader->setMatrix4x4("view", camera->getView());

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw the rest
	shader->use();
	texture->use();

	shader->setMatrix4x4("view", camera->getView());

	glBindVertexArray(VAO);

	// Get entities
	std::vector<Entity*> entities = currentScene->getEntities();
	for (Entity *entity : entities) {
		glm::vec3 scale = entity->getScale();
		//spdlog::debug("Scale : ({}, {}, {})", scale.x, scale.y, scale.z);

		shader->setMatrix4x4("model", entity->getModelMatrix());

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
