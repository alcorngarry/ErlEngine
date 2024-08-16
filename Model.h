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

		Model(char* path);

		void draw(Shader& shader);

		glm::vec3 getBBSize(aiMesh* mesh);
		glm::vec3 getBBLocation(aiMesh* mesh);

		//glm::vec3* getScale();
		//glm::vec3* getLocation();

	private:

		void loadModel(std::string path);
	
		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

#endif 