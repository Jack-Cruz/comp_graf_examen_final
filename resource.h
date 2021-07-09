#pragma once

#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <map>

#include <texture.h>
#include <shader.h>

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined. 
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader*> Shaders;
    static std::map<std::string, Texture*> Textures;
    
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader* LoadShader(std::string name, string vShaderFile, string fShaderFile, string shaderPath) {
        Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, shaderPath);
        return Shaders[name];
    }
    static Shader* GetShader(std::string name) {
        return Shaders[name];
    }  
    static Texture* LoadTexture(string textureFileName, string texturePath, bool alpha, std::string name) {
        Textures[name] = loadTextureFromFile(textureFileName, texturePath, alpha);
        return Textures[name];
    }
    static Texture* GetTexture(std::string name) {
        return Textures[name];
    }
    static void      Clear() {
        for (auto iter : Shaders){
            glDeleteProgram(iter.second->program);
        }
        for (auto iter : Textures) {
            glDeleteTextures(1, &iter.second->id);
        }
    }

private:
    ResourceManager() { }

    static Shader* loadShaderFromFile(string vShaderFile, string fShaderFile, string shaderPath) {
        Shader *shader = new Shader(vShaderFile, fShaderFile, shaderPath);
        return shader;
    }
    static Texture* loadTextureFromFile(string textureFileName, string texturePath, bool alpha) {
        
        Path* path = new Path(texturePath);
        string filePath = path->ruta(textureFileName);

        Texture* texture = new Texture();
        if (alpha) {
            texture->internal_format = GL_RGBA;
            texture->image_format = GL_RGBA;
        }

        // load
        i32 width, height, nrChannels;
        u8* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

        texture->Generate(width, height, data);

        stbi_image_free(data);
        return texture;
    }
};

// Initialize static variables
std::map<std::string, Texture*>    ResourceManager::Textures;
std::map<std::string, Shader*>    ResourceManager::Shaders;


#endif

