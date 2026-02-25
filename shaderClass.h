#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/gtc/type_ptr.hpp>
#include "Transform.h"

// Reads the contents of a file and returns it as a string
// ファイルの内容を読み込み、文字列として返す
std::string get_file_contents(const char* filename);

class Shader
{
public:

	// Reference ID of the Shader Program
	// シェーダープログラムの参照ID
	GLuint ID;

	// Constructor that builds the Shader Program from 2 different shaders
	// 2つの異なるシェーダーからシェーダープログラムを構築するコンストラクタ
	Shader(const char* vertexFile, const char* fragmentFile);

	// Updates the shader with the latest transform data
	// 最新のTransformデータでシェーダーを更新する
	void Update(const Transform& transform);

	// Activates the Shader Program
	// シェーダープログラムを有効化する
	void Activate();

	// Deletes the Shader Program
	// シェーダープログラムを削除する
	void Delete();
};

#endif