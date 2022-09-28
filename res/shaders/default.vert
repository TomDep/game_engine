#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));

    /* 
     * Inversing matrices is a costly operation for shaders, so wherever possible try to avoid
     * doing inverse operations since they have to be done on each vertex of your scene.
     */
    Normal = mat3(transpose(inverse(model))) * aNormal; // see http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    TexCoords = aTexCoords;
} 