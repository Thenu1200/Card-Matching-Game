#include "shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
// テキストファイルを読み込み、その内容をすべて文字列として返す
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFIle and fragmentFile and store the strings
	// vertexFileとfragmentFileを読み込み、文字列として保存する
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the strings into character objects
	// 文字列をCスタイルの文字配列（char*）に変換する
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// glShaderSource
	// glShaderSource関数
	/* args:
	*	1. Shader object to compile to
	*	2. Specify how many strings to pass as source code (only 1 in this case)
	*	3. Actual source code of the vertex shader
	*	4. Leave this as NULL
	*/
	/* 引数:
	*	1. コンパイル対象のシェーダーオブジェクト
	*	2. ソースコードとして渡す文字列の数（この場合は1つ）
	*	3. 頂点シェーダーの実際のソースコード
	*	4. NULLのままでよい
	*/
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// check if compilation was successful after the call to glCompileShader
	// glCompileShader呼び出し後にコンパイルが成功したか確認する
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Repeat the same process used to compile the vertexShader but for the fragmentShader
	// vertexShaderと同じ手順をfragmentShaderにも適用する
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// check if compilation was successful
	// コンパイルが成功したか確認する
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create a shader program object which is the final linked version of multiple shaders combined
	// 複数のシェーダーを結合した最終的なリンク済みプログラムオブジェクトを作成する
	ID = glCreateProgram();

	// Attach the shaders to shaderProgram
	// シェーダーをshaderProgramにアタッチする
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// Link the shaders with glLinkProgram
	// glLinkProgramでシェーダーをリンクする
	glLinkProgram(ID);

	// Check if linked correctly
	// 正しくリンクされたか確認する
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders after linking as we no longer need them anymore
	// リンク後は不要になるため、個別のシェーダーオブジェクトを削除する
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Update(const Transform& transform)
{
	// Get the model matrix from Transform and send it to the shader
	// Transformからモデル行列を取得し、シェーダーに送信する
	glm::mat4 model = transform.GetModel();
	glUniformMatrix4fv(glGetUniformLocation(ID, "transform"), 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::Activate()
{
	// Activate this shader program for rendering
	// このシェーダープログラムをレンダリング用に有効化する
	glUseProgram(ID);
}

void Shader::Delete()
{
	// Delete the shader program and free GPU resources
	// シェーダープログラムを削除し、GPUリソースを解放する
	glDeleteProgram(ID);
}