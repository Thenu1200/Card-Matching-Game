#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

// Alias nlohmann::json to json for convenience
// 利便性のために nlohmann::json を json という別名で使用する
using json = nlohmann::json;

// Model class responsible for loading and rendering a 3D model
// 3Dモデルの読み込みと描画を担当するModelクラス
class Model
{
public:

	// Constructor that loads a model from file path
	// ファイルパスからモデルを読み込むコンストラクタ
	Model(const char* file);

	// Draws the model using the provided shader and camera
	// 指定されたシェーダーとカメラを使ってモデルを描画する
	void Draw(Shader& shader, Camera& camera);

private:

	// Path to the model file
	// モデルファイルへのパス
	const char* file;

	// Raw binary buffer data from the model file
	// モデルファイルから取得した生のバイナリバッファデータ
	std::vector<unsigned char> data;

	// Parsed JSON structure of the glTF file
	// glTFファイルを解析したJSON構造
	json JSON;

	// Collection of meshes that make up the model
	// モデルを構成するMeshの集合
	std::vector<Mesh> meshes;

	// Per-mesh translation vectors
	// 各メッシュごとの平行移動ベクトル
	std::vector<glm::vec3> translationsMeshes;

	// Per-mesh rotation quaternions
	// 各メッシュごとの回転（クォータニオン）
	std::vector<glm::quat> rotationsMeshes;

	// Per-mesh scale vectors
	// 各メッシュごとのスケールベクトル
	std::vector<glm::vec3> scalesMeshes;

	// Final transformation matrices for each mesh
	// 各メッシュの最終的な変換行列
	std::vector<glm::mat4> matricesMeshes;

	// Names of textures that have already been loaded (to avoid duplicates)
	// 重複を防ぐため、すでに読み込まれたテクスチャ名
	std::vector<std::string> loadedTexName;

	// Loaded texture objects
	// 読み込まれたテクスチャオブジェクト
	std::vector<Texture> loadedTex;

	// Loads a mesh from the JSON using its index
	// インデックスを使ってJSONからメッシュを読み込む
	void loadMesh(unsigned int indMesh);

	// Recursively traverses a node in the scene graph and applies transformations
	// シーングラフ内のノードを再帰的に巡回し、変換を適用する
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Retrieves raw binary buffer data from the model file
	// モデルファイルから生のバイナリバッファデータを取得する
	std::vector<unsigned char> getData();

	// Extracts floating-point attribute data from an accessor
	// アクセサから浮動小数点の属性データを抽出する
	std::vector<float> getFloats(json accessor);

	// Extracts index data from an accessor
	// アクセサからインデックスデータを抽出する
	std::vector<GLuint> getIndices(json accessor);

	// Loads textures defined in the model file
	// モデルファイルで定義されたテクスチャを読み込む
	std::vector<Texture> getTextures();

	// Combines positions, normals, and UVs into Vertex objects
	// 位置・法線・UVを組み合わせてVertexオブジェクトを作成する
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Groups a flat float array into vec2 structures
	// フラットなfloat配列をvec2構造にまとめる
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);

	// Groups a flat float array into vec3 structures
	// フラットなfloat配列をvec3構造にまとめる
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);

	// Groups a flat float array into vec4 structures
	// フラットなfloat配列をvec4構造にまとめる
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif