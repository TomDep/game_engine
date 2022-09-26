#pragma once
/*

#ifndef SHADER_H    // Évite d'inclure plusieurs fois ce fichier
#define SHADER_H

#include <GL/glew.h>
// inclure glad pour disposer de tout en-tête OpenGL 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Reference de la classe shader - https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=les-shaders#L7-3
// + qql modifications apportés

class Shader
{
    public:

    // the program ID
    unsigned int ID;

    // le constructeur lit et construit le shader
    Shader(const char* vertexPath, const char* fragmentPath)
    {
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
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
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
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
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
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // program shader
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // affiche les erreurs d'édition de liens si besoin
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // supprime les shaders qui sont maintenant liés dans le programme et qui ne sont plus nécessaires
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    ~Shader() {
        glDeleteProgram(ID);
    }
    
    // Activation du shader
    void use(){
        glUseProgram(ID);
    }

    // fonctions utiles pour l'uniform
    void setBool(const std::string& name, bool value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const{
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const{
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
};

#endif

*/