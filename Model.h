#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Mesh.h"


class Model
{
	public:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		bool gammaCorrection;
		Model() = default;
		Model(char* path);

		void draw(Shader& shader);
		glm::vec3 getMinAABB();
		glm::vec3 getMaxAABB();

	private:
		glm::vec3 minAABB;
		glm::vec3 maxAABB;

		void loadModel(std::string path);
	
		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

#endif 