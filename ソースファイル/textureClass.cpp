#include "textureClass.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum texFilter)
{
	// Define the type of the texture
	// テクスチャの種類を定義する
	type = texType;

	// Create variables for width, height, and the number of color channels
	// 画像の幅・高さ・カラーチャンネル数を格納する変数を作成
	int widthImg, heightImg, numColCh;

	// Since OpenGL's coordinate system differs from the stb library's system, we flip the image vertically
	// OpenGLの座標系はstbライブラリと異なるため、画像を上下反転する
	stbi_set_flip_vertically_on_load(true);

	// Takes the image file name, and modifies width height and number of color channel values.
	// The last argument lets you force a number of channels
	// Returns the raw pixel data or NULL
	// 画像ファイルを読み込み、幅・高さ・カラーチャンネル数を取得する
	// 最後の引数でチャンネル数を強制指定できる
	// 生のピクセルデータ、またはNULLを返す
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// makes GLuint ID an array that holds a number (1) of texture objects
	// GLuint ID に1つのテクスチャオブジェクトを生成する
	glGenTextures(1, &ID);

	// makes the active texture the specified slot (example GL_TEXTURE0)
	// 指定されたスロット（例：GL_TEXTURE0）をアクティブにする
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;

	// Binds the texture object array to the specified type (example GL_TEXTURE_2D)
	// 指定されたタイプ（例：GL_TEXTURE_2D）にテクスチャをバインドする
	glBindTexture(GL_TEXTURE_2D, ID);

	// Determine how textures are rendered when they are minified or magnified than the native resolution.
	// We can use GL_LINEAR or GL_NEAREST
	// テクスチャが拡大・縮小表示される際のフィルタリング方法を設定する
	// GL_LINEAR や GL_NEAREST を使用可能
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFilter);

	// Determine what happens passed the boundaries of the texture
	// テクスチャの境界を超えた場合の処理を設定する
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	// Create smaller resolutions of the same image to handle cases where the texture is being rendered within a small area of the screen
	// テクスチャが小さく表示される場合に備えてミップマップを生成する
	glGenerateMipmap(GL_TEXTURE_2D);

	// deallocate memory for bytes
	// 画像データのメモリを解放する
	stbi_image_free(bytes);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform variable in the shader
	// シェーダー内のuniform変数の場所を取得する
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);

	// Activates the shader before setting the uniform
	// uniformを設定する前にシェーダーを有効化する
	shader.Activate();

	// Assigns the texture unit to the uniform
	// テクスチャユニットをuniformに割り当てる
	glUniform1i(texUni, unit);
}

// Binds the texture object array to the type
// テクスチャをバインドする
void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

// Unbinds the texture object array to the type
// テクスチャのバインドを解除する
void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Deletes the texture
// テクスチャを削除する
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}