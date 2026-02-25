#include "VAO.h"

// Constructor: Generates a Vertex Array Object (VAO)
// コンストラクタ：頂点配列オブジェクト（VAO）を生成する
VAO::VAO()
{
	// Generate one VAO and store its ID
	// VAOを1つ生成し、そのIDを取得する
	glGenVertexArrays(1, &ID);
}

// Links a VBO attribute to this VAO
// Associates vertex data layout with a shader attribute location
//
// VBOの属性をこのVAOに関連付ける
// 頂点データのレイアウトをシェーダーの属性ロケーションに設定する
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	// Bind the VBO before defining attribute pointer
	// 属性ポインタを設定する前にVBOをバインドする
	VBO.Bind();

	// Define the layout of the vertex attribute
	//
	// Arguments:
	// 1. layout location in the shader
	// 2. Number of components (e.g., 3 for vec3)
	// 3. Data type (e.g., GL_FLOAT)
	// 4. Whether to normalize the data
	// 5. Stride (distance between consecutive vertices)
	// 6. Offset (starting position inside the vertex structure)
	//
	// 頂点属性のレイアウトを定義する
	//
	// 引数：
	// 1. シェーダー内のlayoutロケーション
	// 2. 要素数（例：vec3なら3）
	// 3. データ型（例：GL_FLOAT）
	// 4. 正規化するかどうか
	// 5. ストライド（頂点間のバイト間隔）
	// 6. オフセット（構造体内の開始位置）
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);

	// Enable the vertex attribute so the shader can access it
	// シェーダーから使用できるように頂点属性を有効化する
	glEnableVertexAttribArray(layout);

	// Unbind the VBO after linking
	// 設定後、VBOのバインドを解除する
	VBO.Unbind();
}

// Bind this VAO as the active vertex array object
// このVAOをアクティブな頂点配列オブジェクトとしてバインドする
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Unbind any currently bound VAO
// 現在バインドされているVAOを解除する
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Delete the VAO from GPU memory
// GPUメモリからVAOを削除する
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}