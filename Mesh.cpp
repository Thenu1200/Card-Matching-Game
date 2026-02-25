#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint> indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();

	// Generates vertex buffer object and links it to vertices
	// 頂点データを格納するVBO（頂点バッファオブジェクト）を生成し、頂点情報を送る
	VBO VBO(vertices);

	// Generates element buffer object and links it to indices
	// インデックス情報を格納するEBO（エレメントバッファオブジェクト）を生成する
	EBO EBO(indices);

	// Link attributes for coordinates (layout 0)
	// 座標属性をレイアウト0にリンクする
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);

	// Link attributes for normals (layout 1)
	// 法線ベクトルをレイアウト1にリンクする
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));

	// Link attributes for colors (layout 2)
	// 頂点カラーをレイアウト2にリンクする
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(GLfloat)));

	// Link attributes for textures (layout 3)
	// テクスチャ座標をレイアウト3にリンクする
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(GLfloat)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw
(
	Shader& shader,
	Camera& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	// Bind all textures and assign them to shader uniforms
	// すべてのテクスチャをバインドし、対応するuniformに割り当てる
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}

		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Send camera position to shader
	// カメラの位置情報をシェーダーへ送る
	glUniform3f(
		glGetUniformLocation(shader.ID, "cameraPos"),
		camera.cameraPosition.x,
		camera.cameraPosition.y,
		camera.cameraPosition.z
	);

	// Send camera matrix (view * projection)
	// カメラ行列（ビュー×プロジェクション）を送信
	camera.Matrix(shader, "camMatrix");

	// Initialize transformation matrices
	// 各変換行列を初期化
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Apply translation
	// 平行移動を適用
	trans = glm::translate(trans, translation);

	// Apply rotation (quaternion → matrix)
	// クォータニオンを回転行列に変換
	rot = glm::mat4_cast(rotation);

	// Apply scaling
	// 拡大縮小を適用
	sca = glm::scale(sca, scale);

	// Send transformation matrices to shader
	// 各変換行列をシェーダーへ送信
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	// Draw the mesh using indexed drawing
	// インデックス描画でメッシュを描画する
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}