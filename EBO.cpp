#include "EBO.h"

// Constructor: Generates and initializes an Element Buffer Object (EBO)
// コンストラクタ：エレメントバッファオブジェクト（EBO）を生成し、初期化する
EBO::EBO(std::vector<GLuint> indices)
{
	// Generate one buffer object and store its ID
	// バッファオブジェクトを1つ生成し、そのIDを取得する
	glGenBuffers(1, &ID);

	// Bind the buffer as the current GL_ELEMENT_ARRAY_BUFFER
	// 生成したバッファを GL_ELEMENT_ARRAY_BUFFER としてバインドする
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);

	// Upload index data to GPU memory
	//
	// Arguments:
	// 1. Target buffer type (index buffer)
	// 2. Size of data in bytes
	// 3. Pointer to index data
	// 4. Usage pattern (GL_STATIC_DRAW = rarely changed)
	//
	// インデックスデータをGPUメモリに転送する
	//
	// 引数：
	// 1. 対象バッファの種類（インデックスバッファ）
	// 2. データサイズ（バイト単位）
	// 3. インデックスデータへのポインタ
	// 4. 使用パターン（GL_STATIC_DRAW = 頻繁に変更しない）
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		indices.data(),
		GL_STATIC_DRAW
	);
}

// Bind this EBO as the active element array buffer
// このEBOをアクティブなインデックスバッファとしてバインドする
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbind the current element array buffer
// 現在のインデックスバッファのバインドを解除する
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Delete the EBO and free GPU memory
// EBOを削除し、GPUメモリを解放する
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}