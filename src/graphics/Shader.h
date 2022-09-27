#pragma once

#ifndef SHADER_H    // Évite d'inclure plusieurs fois ce fichier
#define SHADER_H

#include <glad/glad.h>
// inclure glad pour disposer de tout en-tête OpenGL 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// Reference de la classe shader - https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=les-shaders#L7-3

class Shader {

public:
    Shader(const char* path);
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVector3(const std::string& name, glm::vec3 vector) const;
    void setVector3(const std::string& name, float x, float y, float z) const;
    void setVector4(const std::string& name, glm::vec4 vector) const;
    void setMatrix4x4(const std::string& name, glm::mat4 matrix) const;

    ~Shader();
private:
    // the program ID
    unsigned int id;

};

#endif