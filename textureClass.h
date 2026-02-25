#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shaderClass.h"

class Texture
{
public:

	// OpenGL texture object ID
	// OpenGLのテクスチャオブジェクトID
	GLuint ID;

	// Type of the texture (e.g., diffuse, specular, etc.)
	// テクスチャの種類（例：diffuse、specularなど）
	const char* type;

	// Texture unit slot number
	// テクスチャユニットのスロット番号
	GLuint unit;

	// Constructor that loads and creates a texture
	// テクスチャを読み込み生成するコンストラクタ
	Texture(const char* image, const char* texType, GLuint slot, GLenum texFilter);

	// Assigns the texture unit to a shader uniform
	// シェーダーのuniform変数にテクスチャユニットを割り当てる
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	// Binds the texture
	// テクスチャをバインドする
	void Bind();

	// Unbinds the texture
	// テクスチャのバインドを解除する
	void Unbind();

	// Deletes the texture from GPU memory
	// GPUメモリからテクスチャを削除する
	void Delete();
};

#endif