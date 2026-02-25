#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

// Structure representing a single vertex
// 1つの頂点を表す構造体
struct Vertex
{
	// Position of the vertex in 3D space
	// 頂点の3次元空間での位置
	glm::vec3 position;

	// Normal vector (used for lighting calculations)
	// 法線ベクトル（ライティング計算に使用）
	glm::vec3 normal;

	// Vertex color
	// 頂点カラー
	glm::vec3 color;

	// Texture coordinates (UV mapping)
	// テクスチャ座標（UVマッピング）
	glm::vec2 texUV;
};


// Vertex Buffer Object (VBO) class
// GPUに頂点データを保存するための頂点バッファオブジェクトクラス
class VBO
{
public:

	// OpenGL buffer ID
	// OpenGLバッファの識別ID
	GLuint ID;

	// Constructor:
	// Generates a VBO and uploads vertex data to GPU memory
	// コンストラクタ：
	// VBOを生成し、頂点データをGPUメモリに転送する
	VBO(std::vector<Vertex>& vertices);

	// Bind this VBO as the active GL_ARRAY_BUFFER
	// このVBOをGL_ARRAY_BUFFERとしてバインドする
	void Bind();

	// Unbind the current GL_ARRAY_BUFFER
	// 現在のGL_ARRAY_BUFFERのバインドを解除する
	void Unbind();

	// Delete the VBO and free GPU memory
	// VBOを削除し、GPUメモリを解放する
	void Delete();
};

#endif