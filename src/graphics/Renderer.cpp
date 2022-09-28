#include "Renderer.h"

#include <glad/glad.h>	
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Adding texture
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("res/images/container.jpg", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		spdlog::error("Failed to load the texture !");
	}

	stbi_image_free(data);

	data = stbi_load("res/images/awesomeface.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		spdlog::error("Failed to load the texture !");
	}

	stbi_image_free(data);

	shader->setInt("texture1", 0);
	shader->setInt("texture2", 1);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Adding perspective
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

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
	
	shader->setVector3("objectColor", 1.0f, 0.5f, 0.31f);
	shader->setVector3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->setVector3("lightPos", lightPos);
	shader->setVector3("viewPos", camera->getPosition());

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));

	lightShader->use();
	lightShader->setMatrix4x4("model", model);
	lightShader->setMatrix4x4("view", camera->getView());
	lightShader->setMatrix4x4("projection", projection);

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
	shader->setMatrix4x4("view", camera->getView());

	glBindVertexArray(VAO);

	// Get entities
	std::vector<Entity*> entities = currentScene->getEntities();
	for (Entity *entity : entities) {
		glm::vec3 scale = entity->getScale();
		//spdlog::debug("Scale : ({}, {}, {})", scale.x, scale.y, scale.z);

		shader->setMatrix4x4("model", entity->getModel());

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
