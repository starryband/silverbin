#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include "shaders.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(const char *path) {
            loadModel(path);
        }

        void Draw(Shader &shader);
    private:
        std::vector<Mesh> meshes;
        std::string directory;

        std::vector<Texture_Data> textures_loaded;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<Texture_Data> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif