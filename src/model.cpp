#include "model.h"

void Model::Draw(Shader &shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}  

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture_Data> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        vertex.Normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture_Data> diffuse = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse.begin(), diffuse.end());

        std::vector<Texture_Data> specular = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular.begin(), specular.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture_Data> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture_Data> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool already_loaded = false;
        for (auto &t : textures_loaded) {
            if (t.path == str.C_Str()) {
                textures.push_back(t);
                already_loaded = true;
                break;
            }
        }

        if (!already_loaded) {
            std::string texturePath = directory + '/' + str.C_Str();

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int tex_width, tex_height, nrChannels;
            unsigned char *data = stbi_load(texturePath.c_str(), &tex_width, &tex_height, &nrChannels, 0);

            if (data) {
                GLenum format = GL_RGB;
                if (nrChannels == 1)      format = GL_RED;
                else if (nrChannels == 3) format = GL_RGB;
                else if (nrChannels == 4) format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, tex_width, tex_height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                std::cout << "Failed to load texture: " << texturePath << std::endl;
            }

            stbi_image_free(data);

            Texture_Data texture;
            texture.id = textureID;
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}