#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include "VBO.h"

// Vertex Array Object (VAO) class
// 頂点配列オブジェクト（VAO）クラス
class VAO
{
public:

	// OpenGL VAO identifier
	// OpenGLのVAO識別ID
	GLuint ID;

	// Constructor:
	// Generates a new VAO
	// コンストラクタ：
	// 新しいVAOを生成する
	VAO();

	// Links a VBO attribute to this VAO
	//
	// Parameters:
	// - VBO: The vertex buffer containing vertex data
	// - layout: Attribute location in the shader
	// - numComponents: Number of components (e.g., 3 for vec3)
	// - type: Data type (e.g., GL_FLOAT)
	// - stride: Byte offset between consecutive vertices
	// - offset: Starting offset within the vertex structure
	//
	// VBOの頂点属性をこのVAOに関連付ける
	//
	// 引数：
	// - VBO: 頂点データを含むバッファ
	// - layout: シェーダー内の属性ロケーション
	// - numComponents: 要素数（例：vec3なら3）
	// - type: データ型（例：GL_FLOAT）
	// - stride: 頂点間のバイト間隔
	// - offset: 構造体内の開始位置
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	// Bind this VAO as the active vertex array
	// このVAOをアクティブな頂点配列としてバインドする
	void Bind();

	// Unbind the currently active VAO
	// 現在のVAOのバインドを解除する
	void Unbind();

	// Delete the VAO and free GPU memory
	// VAOを削除し、GPUメモリを解放する
	void Delete();
};

#endif