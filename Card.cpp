#include "Card.h"

// Constructor that loads the model and sets initial orientation
// モデルを読み込み、初期の向きを設定するコンストラクタ
Card::Card(const char* modelPath) : model(modelPath)
{
    // Rotate 90 degrees around Y axis
    // Y軸を中心に90度回転させる
    currentOrientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Then rotate 180 degrees around Z axis
    // その後、Z軸を中心に180度回転させる
    currentOrientation = currentOrientation *
        glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Apply rotation to transform
    // 回転をTransformに適用する
    transform.SetRot(currentOrientation);
}

// Updates card rotation animation
// カードの回転アニメーションを更新する
void Card::Update(float deltaTime)
{
    // If currently rotating, update interpolation
    // 回転中であれば補間処理を行う
    if (isRotating)
    {
        // Increase timer by frame delta time
        // フレームの経過時間をタイマーに加算する
        rotationTimer += deltaTime;

        // Calculate progress ratio
        // 進行度（0〜1）を計算する
        float progress = rotationTimer / rotationDuration;

        // Clamp progress between 0 and 1
        // 進行度を0〜1に制限する
        progress = glm::clamp(progress, 0.0f, 1.0f);

        // Spherical linear interpolation between start and target rotation
        // 開始回転と目標回転の間を球面線形補間（slerp）する
        currentOrientation =
            glm::slerp(startOrientation,
                targetOrientation,
                progress);

        // If animation finished
        // アニメーションが完了した場合
        if (progress >= 1.0f)
        {
            // Stop rotating
            // 回転を停止する
            isRotating = false;

            // Ensure final orientation is exact
            // 最終的な向きを正確に設定する
            currentOrientation = targetOrientation;
        }

        // Apply updated rotation to transform
        // 更新された回転をTransformに適用する
        transform.SetRot(currentOrientation);
    }
}

// Starts flip animation
// カードのフリップアニメーションを開始する
void Card::Flip()
{
    // Prevent flipping while already rotating
    // すでに回転中であれば処理しない
    if (isRotating) return;

    // Store current orientation as start
    // 現在の向きを開始回転として保存する
    startOrientation = currentOrientation;

    // Create 180 degree rotation around Z axis
    // Z軸を中心に180度回転するクォータニオンを作成する
    glm::quat delta =
        glm::angleAxis(glm::radians(180.0f),
            glm::vec3(0, 0, 1));

    // Target orientation is delta rotation applied to start
    // 開始回転にdelta回転を掛けたものを目標回転とする
    targetOrientation = delta * startOrientation;

    // Reset animation timer
    // アニメーションタイマーをリセットする
    rotationTimer = 0.0f;

    // Mark as rotating
    // 回転中フラグを立てる
    isRotating = true;

    // Toggle face state
    // 表裏状態を切り替える
    isFaceUp = !isFaceUp;
}

// Resets card to default state
// カードを初期状態に戻す
void Card::Reset()
{
    // If card is face up, rotate it back
    // 表向きの場合、裏向きに戻す
    if (isFaceUp) {
        currentOrientation =
            glm::angleAxis(glm::radians(180.0f),
                glm::vec3(0, 0, 1)) * currentOrientation;

        // Apply reset rotation
        // リセットした回転を適用する
        transform.SetRot(currentOrientation);
    }

    // Reset states
    // 状態をリセットする
    isFaceUp = false;
    isDestroyed = false;
}

// Draws the card
// カードを描画する
void Card::Draw(Shader& shader, Camera& camera)
{
    // Update shader with current transform
    // 現在のTransformをシェーダーに渡す
    shader.Update(transform);

    // Draw model
    // モデルを描画する
    model.Draw(shader, camera);
}

// Checks if a ray intersects the card's bounding box
// レイがカードのバウンディングボックスと交差するか判定する
bool Card::CheckRayHit(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
{
    // Get the model matrix and its inverse
    // モデル行列とその逆行列を取得する
    glm::mat4 modelMatrix = transform.GetModel();
    glm::mat4 invModel = glm::inverse(modelMatrix);

    // Transform the ray into the card's local space
    // レイをカードのローカル空間へ変換する
    glm::vec3 localOrigin = glm::vec3(invModel * glm::vec4(rayOrigin, 1.0f));
    glm::vec3 localDir = glm::normalize(glm::vec3(invModel * glm::vec4(rayDir, 0.0f)));

    // Define local-space AABB bounds
    // ローカル空間でのAABB範囲を定義する
    glm::vec3 min = -halfExtents;
    glm::vec3 max = halfExtents;

    // Initialize intersection distances
    // 交差距離の初期値を設定する
    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();

    // Perform slab intersection test
    // スラブ法による交差判定を行う
    for (int i = 0; i < 3; i++)
    {
        // If ray direction is nearly zero
        // レイ方向がほぼ0の場合
        if (abs(localDir[i]) < 1e-6f)
        {
            // Ray is parallel and outside bounds
            // レイが平行で範囲外なら交差しない
            if (localOrigin[i] < min[i] || localOrigin[i] > max[i])
                return false;
        }
        else
        {
            // Compute intersection distances
            // 交差距離を計算する
            float invD = 1.0f / localDir[i];
            float t1 = (min[i] - localOrigin[i]) * invD;
            float t2 = (max[i] - localOrigin[i]) * invD;

            // Swap if needed
            // 必要であれば入れ替える
            if (t1 > t2) std::swap(t1, t2);

            // Update min and max intersection distances
            // 最小・最大交差距離を更新する
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            // If no overlap, no intersection
            // 範囲が重ならなければ交差しない
            if (tMin > tMax)
                return false;
        }
    }

    // Ray intersects AABB
    // レイはAABBと交差している
    return true;
}