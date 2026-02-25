#ifndef TRANSFORM_CLASS_H
#define TRANSFORM_CLASS_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

// Transform class that stores position, rotation, and scale
// 位置・回転・スケールを保持するTransformクラス
class Transform
{
public:

	// Constructor with default values:
	// デフォルト値付きコンストラクタ：
	// position = (0,0,0)
	// 位置 = (0,0,0)
	// rotation = identity quaternion
	// 回転 = 単位クォータニオン
	// scale = (1,1,1)
	// スケール = (1,1,1)
	Transform(const glm::vec3& pos = glm::vec3(),
		const glm::quat& rot = glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {
	}

	// Returns the full model matrix (T * R * S)
	// モデル行列（T * R * S）を返す
	inline glm::mat4 GetModel() const
	{
		// Create translation matrix from position
		// 位置から平行移動行列を作成する
		glm::mat4 posMatrix = glm::translate(m_pos);

		// Create scale matrix from scale vector
		// スケールベクトルから拡大縮小行列を作成する
		glm::mat4 scaleMatrix = glm::scale(m_scale);

		// Convert quaternion rotation to matrix
		// クォータニオン回転を行列に変換する
		glm::mat4 rotMatrix = glm::mat4_cast(m_rot);

		// Combine into final model matrix (Translate * Rotate * Scale)
		// 最終的なモデル行列に結合する（平行移動 × 回転 × 拡大縮小）
		return posMatrix * rotMatrix * scaleMatrix;
	}

	// Returns reference to position vector
	// 位置ベクトルへの参照を返す
	inline glm::vec3& GetPos() { return m_pos; }

	// Returns reference to rotation quaternion
	// 回転クォータニオンへの参照を返す
	inline glm::quat& GetRot() { return m_rot; }

	// Returns reference to scale vector
	// スケールベクトルへの参照を返す
	inline glm::vec3& GetScale() { return m_scale; }

	// Sets position
	// 位置を設定する
	inline void SetPos(const glm::vec3& pos) { m_pos = pos; }

	// Sets rotation
	// 回転を設定する
	inline void SetRot(const glm::quat& rot) { m_rot = rot; }

	// Sets scale
	// スケールを設定する
	inline void SetScale(const glm::vec3& scale) { m_scale = scale; }

protected:
private:

	// Position in world space
	// ワールド空間での位置
	glm::vec3 m_pos;

	// Rotation stored as quaternion
	// クォータニオンとして保存される回転
	glm::quat m_rot;

	// Scale factors along each axis
	// 各軸方向のスケール係数
	glm::vec3 m_scale;

};

#endif