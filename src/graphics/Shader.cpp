#include "Shader.h"

#include <spdlog/spdlog.h>

// le constructeur lit et construit le shader
Shader::Shader(const char* path) {
    // 1. récupère le code du vertex/fragment shader depuis filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // s'assure que les objets ifstream peuvent envoyer des exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // ouverture des fichiers
        vShaderFile.open(string(path) + ".vert");
        fShaderFile.open(string(path) + ".frag");
        std::stringstream vShaderStream, fShaderStream;
        // lecture des fichiers et place le contenu dans des flux
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // fermeture des fichiers
        vShaderFile.close();
        fShaderFile.close();
        // convertions des flux en string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        spdlog::error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compiler les shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // affiche les erreurs de compilation si besoin
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}",infoLog);
    };

    // de même pour le fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // affiche les erreurs de compilation si besoin
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog);
    };

    // program shader
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    // affiche les erreurs d'édition de liens si besoin
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n {}", infoLog);
    }

    // supprime les shaders qui sont maintenant liés dans le programme et qui ne sont plus nécessaires
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

// Activation du shader
void Shader::use() {
    glUseProgram(id);
}

// fonctions utiles pour l'uniform
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setVector4(const std::string& name, parea::Vector4 value) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4x4(const std::string& name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}