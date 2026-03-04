#include "VBO.h"

// Constructor: Generates and initializes a Vertex Buffer Object (VBO)
// コンストラクタ：頂点バッファオブジェクト（VBO）を生成し、初期化する
VBO::VBO(std::vector<Vertex>& vertices)
{
	// Generate one buffer object and store its ID
	// バッファオブジェクトを1つ生成し、そのIDを取得する
	glGenBuffers(1, &ID);

	// Bind the buffer as the current GL_ARRAY_BUFFER
	// 生成したバッファを GL_ARRAY_BUFFER としてバインドする
	glBindBuffer(GL_ARRAY_BUFFER, ID);

	// Upload vertex data to GPU memory
	// Arguments:
	// 1. Target buffer type
	// 2. Size of data in bytes
	// 3. Pointer to vertex data
	// 4. Usage pattern (static draw = data does not change often)
	//
	// 頂点データをGPUメモリに転送する
	// 引数：
	// 1. 対象バッファの種類
	// 2. データサイズ（バイト単位）
	// 3. 頂点データへのポインタ
	// 4. 使用パターン（GL_STATIC_DRAW = 頻繁に変更しないデータ）
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Bind this VBO as the active array buffer
// このVBOをアクティブな配列バッファとしてバインドする
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbind the current array buffer (bind to 0)
// 現在の配列バッファを解除する（0をバインド）
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Delete the buffer from GPU memory
// GPUメモリからバッファを削除する
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}