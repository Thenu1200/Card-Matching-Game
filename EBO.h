#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

// Element Buffer Object (EBO) class
// エレメントバッファオブジェクト（EBO）クラス
// Stores index data used for indexed drawing
// インデックス描画に使用されるインデックスデータを保存する
class EBO
{
public:

	// OpenGL buffer identifier
	// OpenGLバッファの識別ID
	GLuint ID;

	// Constructor:
	// Generates an EBO and uploads index data to GPU memory
	// コンストラクタ：
	// EBOを生成し、インデックスデータをGPUメモリに転送する
	EBO(std::vector<GLuint> indices);

	// Bind this EBO as the active GL_ELEMENT_ARRAY_BUFFER
	// このEBOをGL_ELEMENT_ARRAY_BUFFERとしてバインドする
	void Bind();

	// Unbind the current GL_ELEMENT_ARRAY_BUFFER
	// 現在のGL_ELEMENT_ARRAY_BUFFERのバインドを解除する
	void Unbind();

	// Delete the EBO and free GPU memory
	// EBOを削除し、GPUメモリを解放する
	void Delete();
};

#endif