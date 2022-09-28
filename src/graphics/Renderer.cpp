#include "Renderer.h"

#include <glad/glad.h>	
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include "Light.h"
#include "Material.h"

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

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Camera :
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), (float)camera->getWidth() / (float)camera->getHeight(), 0.1f, 100.0f);

	glm::mat4 model = glm::mat4(1.0f);
	shader->setMatrix4x4("model", model);
	shader->setMatrix4x4("view", camera->getView());
	shader->setMatrix4x4("projection", projection);

	shader->setVector3("viewPos", camera->getPosition());
	
	// Lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	light = new Light(lightPos, glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));
	light->use(shader);

	// Material
	Texture* texture = new Texture("res/images/container2.png", true);
	material = new Material(texture, glm::vec3(0.5f), 128.0f);
	material->use(shader);

	// Render the light source
	lightShader = new Shader("res/shaders/lightShader");
	lightShader->use();

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader->setMatrix4x4("model", model);
	lightShader->setMatrix4x4("view", camera->getView());
	lightShader->setMatrix4x4("projection", projection);
	lightShader->setVector3("lightColor", light->getDiffuse());

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
	shader->setVector3("viewPos", camera->getPosition());
	shader->setMatrix4x4("view", camera->getView());

	material->use(shader);
	light->use(shader);

	// For now there is only one mesh for all entities
	glBindVertexArray(VAO);

	// render entities
	std::vector<Entity*> entities = currentScene->getEntities();
	for (Entity *entity : entities) {
		shader->setMatrix4x4("model", entity->getModelMatrix());

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
