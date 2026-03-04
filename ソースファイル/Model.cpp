#include "Model.h"

Model::Model(const char* file)
{
	// Read the entire file contents into a string
	// ファイル全体の内容を文字列として読み込む
	std::string text = get_file_contents(file);

	// Parse the JSON structure from the glTF file
	// glTFファイルのJSON構造を解析する
	JSON = json::parse(text);

	// Store the file path for later use (e.g., loading textures and buffers)
	// 後でテクスチャやバッファを読み込むためにファイルパスを保存する
	Model::file = file;

	// Load the binary buffer data referenced by the glTF file
	// glTFファイルが参照しているバイナリバッファデータを読み込む
	data = getData();

	// Begin traversing the scene graph starting from the root node (index 0)
	// ルートノード（インデックス0）からシーングラフの探索を開始する
	traverseNode(0);
}

void Model::Draw(Shader& shader, Camera& camera)
{
	// Loop through all meshes stored in this model
	// このモデルに保存されているすべてのメッシュをループする
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		// Draw each mesh using the provided shader, camera,
		// and its corresponding transformation matrix
		// 渡されたシェーダーとカメラ、
		// そして対応する変換行列を使用して各メッシュを描画する
		meshes[i].Mesh::Draw(shader, camera, matricesMeshes[i]);
	}
}

void Model::loadMesh(unsigned int indMesh)
{
	// Get accessor index for vertex positions from the glTF JSON
	// glTFのJSONから頂点位置のアクセサインデックスを取得する
	unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];

	// Get accessor index for vertex normals
	// 頂点法線のアクセサインデックスを取得する
	unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];

	// Get accessor index for texture coordinates (UVs)
	// テクスチャ座標（UV）のアクセサインデックスを取得する
	unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];

	// Get accessor index for mesh indices
	// メッシュのインデックスのアクセサインデックスを取得する
	unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

	// Retrieve raw float data for positions from buffer
	// バッファから頂点位置の生のfloatデータを取得する
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);

	// Group position floats into vec3 structures
	// 位置のfloatデータをvec3にまとめる
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

	// Retrieve raw float data for normals
	// 法線の生のfloatデータを取得する
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);

	// Group normal floats into vec3 structures
	// 法線のfloatデータをvec3にまとめる
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);

	// Retrieve raw float data for texture coordinates
	// テクスチャ座標の生のfloatデータを取得する
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);

	// Group texture coordinate floats into vec2 structures
	// テクスチャ座標のfloatデータをvec2にまとめる
	std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

	// Combine positions, normals, and UVs into Vertex structures
	// 位置・法線・UVをVertex構造体にまとめる
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);

	// Retrieve index data from buffer
	// バッファからインデックスデータを取得する
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);

	// Load associated textures for this mesh
	// このメッシュに関連するテクスチャを読み込む
	std::vector<Texture> textures = getTextures();

	// Create a Mesh object and store it in the meshes vector
	// Meshオブジェクトを作成し、meshesベクターに追加する
	meshes.push_back(Mesh(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
{
	// Get the current node from the glTF JSON scene graph
	// glTFのシーングラフから現在のノードを取得する
	json node = JSON["nodes"][nextNode];

	// Initialize translation with default value (no movement)
	// translationをデフォルト値（移動なし）で初期化する
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);

	// If the node contains translation data, read it
	// ノードにtranslationが存在する場合は読み込む
	if (node.find("translation") != node.end())
	{
		float transValues[3];

		// Copy translation values from JSON
		// JSONからtranslation値をコピーする
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			transValues[i] = (node["translation"][i]);

		// Convert float array to glm::vec3
		// float配列をglm::vec3に変換する
		translation = glm::make_vec3(transValues);
	}

	// Initialize rotation as identity quaternion (no rotation)
	// rotationを単位クォータニオン（回転なし）で初期化する
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	// If the node contains rotation data, read it
	// ノードにrotationが存在する場合は読み込む
	if (node.find("rotation") != node.end())
	{
		// glTF stores quaternion as (x, y, z, w),
		// but glm expects (w, x, y, z)
		// glTFは(x, y, z, w)形式、
		// glmは(w, x, y, z)形式を使用する
		float rotValues[4] =
		{
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2],
		};

		// Convert float array to glm quaternion
		// float配列をglmのクォータニオンに変換する
		rotation = glm::make_quat(rotValues);
	}

	// Initialize scale with default value (1,1,1)
	// scaleをデフォルト値（1,1,1）で初期化する
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	// If the node contains scale data, read it
	// ノードにscaleが存在する場合は読み込む
	if (node.find("scale") != node.end())
	{
		float scaleValues[3];

		// Copy scale values from JSON
		// JSONからscale値をコピーする
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);

		// Convert float array to glm::vec3
		// float配列をglm::vec3に変換する
		scale = glm::make_vec3(scaleValues);
	}

	// Initialize transformation matrix for the node
	// ノードの変換行列を初期化する
	glm::mat4 matNode = glm::mat4(1.0f);

	// If a full transformation matrix is provided, use it
	// 完全な変換行列が指定されている場合はそれを使用する
	if (node.find("matrix") != node.end())
	{
		float matValues[16];

		// Copy matrix values from JSON
		// JSONから行列の値をコピーする
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			matValues[i] = (node["matrix"][i]);

		// Convert float array to glm::mat4
		// float配列をglm::mat4に変換する
		matNode = glm::make_mat4(matValues);
	}

	// Initialize separate TRS matrices
	// 平行移動・回転・拡大縮小用の行列を初期化する
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Convert translation vector into matrix
	// translationベクトルを行列に変換する
	trans = glm::translate(trans, translation);

	// Convert quaternion into rotation matrix
	// クォータニオンを回転行列に変換する
	rot = glm::mat4_cast(rotation);

	// Convert scale vector into scale matrix
	// scaleベクトルを拡大縮小行列に変換する
	sca = glm::scale(sca, scale);

	// Combine parent matrix with current node transformations
	// 親ノードの行列と現在のノードの変換を合成する
	glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

	// If this node contains a mesh, store its transform and load it
	// このノードにメッシュがある場合は変換情報を保存し、読み込む
	if (node.find("mesh") != node.end())
	{
		translationsMeshes.push_back(translation);
		rotationsMeshes.push_back(rotation);
		scalesMeshes.push_back(scale);
		matricesMeshes.push_back(matNextNode);

		loadMesh(node["mesh"]);
	}

	// If this node has children, recursively traverse them
	// このノードに子ノードがある場合は再帰的に探索する
	if (node.find("children") != node.end())
	{
		for (unsigned int i = 0; i < node["children"].size(); i++)
			traverseNode(node["children"][i], matNextNode);
	}
}

// Load the binary buffer data referenced by the glTF file
// glTFファイルで参照されているバイナリバッファデータを読み込む
std::vector<unsigned char> Model::getData()
{
	std::string bytesText;

	// Get the URI of the first buffer defined in the glTF file
	// glTFファイルで定義されている最初のバッファのURIを取得する
	std::string uri = JSON["buffers"][0]["uri"];

	// Convert the model file path to a string
	// モデルのファイルパスを文字列に変換する
	std::string fileStr = std::string(file);

	// Extract the directory path from the full file path
	// フルパスからディレクトリ部分を取得する
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Read the buffer file contents as raw bytes
	// バッファファイルの内容を生バイトとして読み込む
	bytesText = get_file_contents((fileDirectory + uri).c_str());

	// Convert the byte string into a vector of unsigned char
	// バイト列をunsigned char型のベクターに変換する
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());

	return data;
}


// Extract floating-point data from the buffer using accessor information
// アクセサ情報を使用してバッファから浮動小数点データを取得する
std::vector<float> Model::getFloats(json accessor)
{
	std::vector<float> floatVec;

	// Get the bufferView index from the accessor
	// アクセサからbufferViewのインデックスを取得する
	unsigned int buffViewInd = accessor.value("bufferView", 1);

	// Get the number of elements defined by the accessor
	// アクセサで定義されている要素数を取得する
	unsigned int count = accessor["count"];

	// Get the byte offset inside the accessor (if any)
	// アクセサ内のバイトオフセットを取得する（存在する場合）
	unsigned int accByteOffset = accessor.value("byteOffset", 0);

	// Get the type of data (SCALAR, VEC2, VEC3, VEC4)
	// データの型（SCALAR, VEC2, VEC3, VEC4）を取得する
	std::string type = accessor["type"];

	// Retrieve the corresponding bufferView object
	// 対応するbufferViewオブジェクトを取得する
	json bufferView = JSON["bufferViews"][buffViewInd];

	// Get the starting byte offset inside the buffer
	// バッファ内の開始バイト位置を取得する
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;

	// Determine how many floats make up one element
	// 1要素あたりのfloat数を決定する
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4");

	// Calculate the absolute starting position of the data
	// データの絶対開始位置を計算する
	unsigned int beginningOfData = byteOffset + accByteOffset;

	// Calculate total byte length of the data (4 bytes per float)
	// データ全体のバイト長を計算する（floatは4バイト）
	unsigned int lengthOfData = count * 4 * numPerVert;

	// Read float values from the raw byte buffer
	// 生バイトバッファからfloat値を読み取る
	for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
	{
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };

		float value;

		// Copy 4 bytes into a float variable
		// 4バイトをfloat変数にコピーする
		std::memcpy(&value, bytes, sizeof(float));

		floatVec.push_back(value);
	}

	return floatVec;
}

// Extract index data from the buffer using accessor information
// アクセサ情報を使用してバッファからインデックスデータを取得する
std::vector<GLuint> Model::getIndices(json accessor)
{
	std::vector<GLuint> indices;

	// Get bufferView index from accessor
	// アクセサからbufferViewのインデックスを取得する
	unsigned int buffViewInd = accessor.value("bufferView", 0);

	// Get number of indices
	// インデックスの数を取得する
	unsigned int count = accessor["count"];

	// Get byte offset inside accessor (if any)
	// アクセサ内のバイトオフセットを取得する（存在する場合）
	unsigned int accByteOffset = accessor.value("byteOffset", 0);

	// Get component type (data type of indices)
	// インデックスのデータ型（componentType）を取得する
	unsigned int componentType = accessor["componentType"];

	// Retrieve corresponding bufferView
	// 対応するbufferViewを取得する
	json bufferView = JSON["bufferViews"][buffViewInd];

	// Get starting byte offset in buffer
	// バッファ内の開始バイト位置を取得する
	unsigned int byteOffset = bufferView.value("byteOffset", 0);

	// Calculate absolute starting position of index data
	// インデックスデータの絶対開始位置を計算する
	unsigned int beginningOfData = byteOffset + accByteOffset;

	// If indices are unsigned int (4 bytes per index)
	// インデックスがunsigned int（4バイト）の場合
	if (componentType == 5125)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };

			unsigned int value;

			// Copy 4 bytes into unsigned int
			// 4バイトをunsigned intにコピーする
			std::memcpy(&value, bytes, sizeof(unsigned int));

			indices.push_back((GLuint)value);
		}
	}
	// If indices are unsigned short (2 bytes per index)
	// インデックスがunsigned short（2バイト）の場合
	else if (componentType == 5123)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };

			unsigned short value;

			// Copy 2 bytes into unsigned short
			// 2バイトをunsigned shortにコピーする
			std::memcpy(&value, bytes, sizeof(unsigned short));

			indices.push_back((GLuint)value);
		}
	}
	// If indices are signed short (2 bytes per index)
	// インデックスがsigned short（2バイト）の場合
	else if (componentType == 5122)
	{
		for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bytes[] = { data[i++], data[i++] };

			short value;

			// Copy 2 bytes into signed short
			// 2バイトをsigned shortにコピーする
			std::memcpy(&value, bytes, sizeof(short));

			indices.push_back((GLuint)value);
		}
	}

	return indices;
}


// Load textures defined in the glTF file
// glTFファイルで定義されているテクスチャを読み込む
std::vector<Texture> Model::getTextures()
{
	std::vector<Texture> textures;

	// Convert model file path to string
	// モデルのファイルパスを文字列に変換する
	std::string fileStr = std::string(file);

	// Extract directory path from model file
	// モデルファイルからディレクトリパスを取得する
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

	// Loop through all images defined in glTF
	// glTF内で定義されているすべての画像をループする
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		std::string texPath = JSON["images"][i]["uri"];

		bool skip = false;

		// Check if this texture was already loaded
		// このテクスチャがすでに読み込まれているか確認する
		for (unsigned int j = 0; j < loadedTexName.size(); j++)
		{
			if (loadedTexName[j] == texPath)
			{
				textures.push_back(loadedTex[j]);
				skip = true;
				break;
			}
		}

		// If texture is not loaded yet, load it
		// まだ読み込まれていない場合は読み込む
		if (!skip)
		{
			// If texture is a base color map, treat it as diffuse
			// baseColorテクスチャの場合、diffuseとして扱う
			if (texPath.find("baseColor") != std::string::npos)
			{
				Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size(), GL_LINEAR);
				textures.push_back(diffuse);
				loadedTex.push_back(diffuse);
				loadedTexName.push_back(texPath);
			}

			// If texture is metallic-roughness map, treat it as specular
			// metallicRoughnessテクスチャの場合、specularとして扱う
			if (texPath.find("metallicRoughness") != std::string::npos)
			{
				Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size(), GL_LINEAR);
				textures.push_back(specular);
				loadedTex.push_back(specular);
				loadedTexName.push_back(texPath);
			}
		}
	}

	return textures;
}

// Combines position, normal, and texture coordinate data into Vertex objects
// 位置・法線・テクスチャ座標のデータを組み合わせてVertexオブジェクトを作成する
std::vector<Vertex> Model::assembleVertices
(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texUVs
)
{
	// Container to store the final assembled vertices
	// 最終的に組み立てられた頂点を格納するコンテナ
	std::vector<Vertex> vertices;

	// Loop through each position (assumes positions, normals, and texUVs are same size)
	// 各positionをループする（positions, normals, texUVsが同じサイズであることを前提とする）
	for (int i = 0; i < positions.size(); i++)
	{
		// Create a Vertex using corresponding position, normal, default white color, and texture UV
		// 対応する位置・法線・デフォルト白色・テクスチャUVを使ってVertexを作成する
		vertices.push_back
		(
			Vertex
			{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				texUVs[i]
			}
		);
	}

	// Return the completed vertex list
	// 完成した頂点リストを返す
	return vertices;
}


// Groups a flat float array into vec2 pairs
// フラットなfloat配列をvec2のペアにまとめる
std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	// Container to store vec2 results
	// vec2の結果を格納するコンテナ
	std::vector<glm::vec2> vectors;

	// Iterate through float vector, consuming 2 floats per iteration
	// float配列を2つずつ処理しながらループする
	for (int i = 0; i < floatVec.size(); i)
	{
		// Create vec2 from two consecutive floats
		// 連続する2つのfloatからvec2を作成する
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}

	// Return grouped vec2 vector
	// まとめられたvec2配列を返す
	return vectors;
}


// Groups a flat float array into vec3 triplets
// フラットなfloat配列をvec3の3要素セットにまとめる
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	// Container to store vec3 results
	// vec3の結果を格納するコンテナ
	std::vector<glm::vec3> vectors;

	// Iterate through float vector, consuming 3 floats per iteration
	// float配列を3つずつ処理しながらループする
	for (int i = 0; i < floatVec.size(); i)
	{
		// Create vec3 from three consecutive floats
		// 連続する3つのfloatからvec3を作成する
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	// Return grouped vec3 vector
	// まとめられたvec3配列を返す
	return vectors;
}


// Groups a flat float array into vec4 quadruplets
// フラットなfloat配列をvec4の4要素セットにまとめる
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	// Container to store vec4 results
	// vec4の結果を格納するコンテナ
	std::vector<glm::vec4> vectors;

	// Iterate through float vector, consuming 4 floats per iteration
	// float配列を4つずつ処理しながらループする
	for (int i = 0; i < floatVec.size(); i)
	{
		// Create vec4 from four consecutive floats
		// 連続する4つのfloatからvec4を作成する
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	// Return grouped vec4 vector
	// まとめられたvec4配列を返す
	return vectors;
}