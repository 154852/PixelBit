#include <iostream>

#include "OBJLoader.h"

#define OBJL_CONSOLE_OUTPUT
#include "OBJ_Loader.h"

namespace PixelBit {

void Loader::OBJLoader::load(const char* path, std::vector<Mesh*>& meshes) {
	objl::Loader loader;
	loader.LoadFile(path);	

	for (objl::Mesh mesh : loader.LoadedMeshes) {
		Mesh* out_mesh = new Mesh();

		int* indices = (int*) calloc(mesh.Indices.size(), sizeof(int));
		for (int i = 0; i < mesh.Indices.size(); i++) indices[i] = mesh.Indices[i];
		out_mesh->indices(indices, mesh.Indices.size());

		float* positions = (float*) calloc(mesh.Vertices.size() * 3, sizeof(float));
		float* normals = (float*) calloc(mesh.Vertices.size() * 3, sizeof(float));
		float* textureCoords = (float*) calloc(mesh.Vertices.size() * 2, sizeof(float));
		for (int i = 0; i < mesh.Vertices.size(); i++) {
			objl::Vertex vertex = mesh.Vertices[i];
			positions[i * 3] = vertex.Position.X; positions[(i * 3) + 1] = vertex.Position.Y; positions[(i * 3) + 2] = vertex.Position.Z;
			normals[i * 3] = vertex.Normal.X; normals[(i * 3) + 1] = vertex.Normal.Y; normals[(i * 3) + 2] = vertex.Normal.Z;
			textureCoords[i * 2] = vertex.TextureCoordinate.X; textureCoords[(i * 2) + 1] = vertex.TextureCoordinate.Y;
		}
		out_mesh->vertex_buffer(positions, mesh.Vertices.size() * 3, 3);
		out_mesh->vertex_buffer(normals, mesh.Vertices.size() * 3, 3);
		out_mesh->vertex_buffer(textureCoords, mesh.Vertices.size() * 2, 2);

		meshes.push_back(out_mesh);
	}
};

void Loader::OBJLoader::load(const char* path, std::function<void(Mesh*, OBJMaterial&)> callback) {
	objl::Loader loader;
	loader.LoadFile(path);	

	for (objl::Mesh mesh : loader.LoadedMeshes) {
		Mesh* out_mesh = new Mesh();

		int* indices = (int*) calloc(mesh.Indices.size(), sizeof(int));
		for (int i = 0; i < mesh.Indices.size(); i++) indices[i] = mesh.Indices[i];
		out_mesh->indices(indices, mesh.Indices.size());

		float* positions = (float*) calloc(mesh.Vertices.size() * 3, sizeof(float));
		float* normals = (float*) calloc(mesh.Vertices.size() * 3, sizeof(float));
		float* textureCoords = (float*) calloc(mesh.Vertices.size() * 2, sizeof(float));
		for (int i = 0; i < mesh.Vertices.size(); i++) {
			objl::Vertex vertex = mesh.Vertices[i];
			positions[i * 3] = vertex.Position.X; positions[(i * 3) + 1] = vertex.Position.Y; positions[(i * 3) + 2] = vertex.Position.Z;
			normals[i * 3] = vertex.Normal.X; normals[(i * 3) + 1] = vertex.Normal.Y; normals[(i * 3) + 2] = vertex.Normal.Z;
			textureCoords[i * 2] = vertex.TextureCoordinate.X; textureCoords[(i * 2) + 1] = vertex.TextureCoordinate.Y;
		}
		out_mesh->vertex_buffer(positions, mesh.Vertices.size() * 3, 3);
		out_mesh->vertex_buffer(normals, mesh.Vertices.size() * 3, 3);
		out_mesh->vertex_buffer(textureCoords, mesh.Vertices.size() * 2, 2);

		OBJMaterial material;
		material.ambient = new glm::vec3(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z);
		material.diffuse = new glm::vec3(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z);
		material.specular = new glm::vec3(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z);
		
		material.specular_strength = new float(mesh.MeshMaterial.Ns);
		material.optical_density = new float(mesh.MeshMaterial.Ni);
		material.disolve = new float(mesh.MeshMaterial.d);
		material.illumination = new int(mesh.MeshMaterial.illum);

		material.ambient_map = new std::string(mesh.MeshMaterial.map_Ka);
		material.diffuse_map = new std::string(mesh.MeshMaterial.map_Kd);
		material.specular_map = new std::string(mesh.MeshMaterial.map_Ks);
		material.specular_highlight_map = new std::string(mesh.MeshMaterial.map_Ns);
		material.alpha_map = new std::string(mesh.MeshMaterial.map_d);
		material.height_map = new std::string(mesh.MeshMaterial.map_bump);

		callback(out_mesh, material);
	}
}

};