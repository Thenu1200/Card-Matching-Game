#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string.h>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"textureClass.h"

class Mesh
{
public:

	// Vertex data (positions, normals, colors, texture coordinates)
	// 頂点データ（座標・法線・カラー・テクスチャ座標）
	std::vector <Vertex> vertices;

	// Index data for indexed drawing
	// インデックス描画用のインデックスデータ
	std::vector <GLuint> indices;

	// List of textures used by this mesh
	// このメッシュで使用されるテクスチャの一覧
	std::vector <Texture> textures;

	// Vertex Array Object
	// 頂点配列オブジェクト（VAO）
	VAO VAO;

	// Constructor that initializes mesh data and GPU buffers
	// メッシュデータおよびGPUバッファを初期化するコンストラクタ
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint> indices, std::vector <Texture>& textures);

	// Draws the mesh with optional transformation parameters
	// 変換パラメータを指定してメッシュを描画する
	void Draw
	(
		Shader& shader,
		Camera& camera,

		// Model matrix (optional)
		// モデル行列（省略可能）
		glm::mat4 matrix = glm::mat4(1.0f),

		// Translation vector
		// 平行移動ベクトル
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),

		// Rotation (quaternion)
		// 回転（クォータニオン）
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),

		// Scale vector
		// 拡大縮小ベクトル
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};

#endif