#include "MousePicker.h"
#include "GameLogic.h"



const unsigned int width = 800;
const unsigned int height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// glfw: initialize and configure
	// glfwを初期化します
	// -------------------------------------------------------------------------------
	glfwInit();

	// Tell GLFW we want to use OpenGL version 3.3 (Major.Minor)
	// GLFWにOpenGLバージョン３．３を使うと伝えます
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we want to use the core-profile so that we get a smaller subset of OpenGL features that doesnt include outdated functions
	// コアプロファイルを使います
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// glfw window creation
	// glfwのウインドウを創造します
	// -------------------------------------------------------------------------------

	// Create window object that is 800px by 800px titled Card Match
	// 800pxx800pxのウインドウオブジェクトを創造します,タイトルは「Card Match」にします
	GLFWwindow* window = glfwCreateWindow(width, height, "Card Match", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Tell GLFW to make context of window the main context on current thread
	//GLFWに、ウィンドウのコンテキストを現在のスレッドのメインコンテキストにするよう指示する
	glfwMakeContextCurrent(window); 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD (can also use gladLoadGL() instead)
	// GLADを初期化します
	// Load the address of OpenGL function pointers
	// OpenGLの関数ポインタのアドレスをロードします
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Specify the viewport of OpenGL in the Window
	// In this case, the viewport goes from x = 0, y = 0, to x = 800, y = 800
	// ウィンドウ内のOpenGLのビューポートを指定する
	// この場合、ビューポートは x = 0, y = 0 から x = 800, y = 800 までとなります。
	glViewport(0, 0, width, height);

	// build and compile shaders
	// シェーダーのビルドとコンパイル
	// -------------------------------------------------------------------------------

	// Generates Shader object using shaders default.vert and default.frag
	// デフォルトのヴァーテックスシェーダー（default.vert）とフラグメントシェーダー（default.frag）を使用してシェーダーオブジェクトを生成します。
	Shader shaderProgram("default.vert", "default.frag");


	// Make the light
	// 光を作ります
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 2.5f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enable depth test
	// デプス・テストを有効にする
	glEnable(GL_DEPTH_TEST);

	// Make the camera
	// カメラを作ります
	Camera camera(width, height, glm::vec3(0.0f, 12.0f, 12.0f));

	// Hold addresses to card models
	// カードアドレスを保持する
	const char* cardModels[] = {
		"models/card/SimplePlayingCard.gltf",
		"models/cheesegyudoncard/CheeseGyudonSimplePlayingCard.gltf",
		"models/johncard/JohnSimplePlayingCard.gltf",
		"models/sharkcard/SharkSimplePlayingCard.gltf"
	};

	// Make the board
	// ボードを作る
	Board board(cardModels);

	// Make deltaTime for loop
	// ループのdeltaTimeを設定する
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Create the MousePicker, MousePicker performs ray casting
	// MousePickerを作成するMousePickerはレイキャスティングを実行する
	MousePicker picker(camera);

	// Create the GameLogic class that handles game logic
	// ゲームロジックを処理するGameLogicクラスを作成する
	GameLogic game(board, picker, camera);

	// render loop
	// レンダリングループ
	// -------------------------------------------------------------------------------
	// loops until user closes the window
	// ユーザーがウィンドウを閉じるまでループする
	while (!glfwWindowShouldClose(window)) 
	{

		// Input
		processInput(window);


		// Specify the colour of the background to Navy Blue
		// 背景色をネイビーブルーに指定してください
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		// バックバッファをクリアし、新しい色を割り当てる
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Track the camera movement inputs
		// カメラの動きの入力を追跡する
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Update the raycaster
		// レイキャスターを更新する
		picker.Update(window);

		// Update the game logic
		// ゲームロジックを更新する
		game.Update(shaderProgram, deltaTime, window);

		// Swap back buffer with front buffer
		// バックバッファとフロントバッファを入れ替える
		glfwSwapBuffers(window);

		// check if any poll events (window appearing, being resized, etc) in GLFW are triggered
		// GLFWで発生するポールのイベント（ウィンドウの表示、サイズ変更など）がトリガーされたかどうかを確認する
		glfwPollEvents();
	}
	// Delete the shader program after use
	// 使用後にシェーダープログラムを削除する
	shaderProgram.Delete();


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

// Resizes the viewport to the window
// ビューポートをウィンドウサイズに再サイズします
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Function that handles all keyboard and other external inputs needed while the rendered frame is looping
// レンダリングされたフレームがループしている間、必要なすべてのキーボードおよびその他の外部入力を処理する関数
void processInput(GLFWwindow* window)
{
	// Close the window if the escape key is pressed
	// エスケープキーが押されたらウィンドウを閉じる
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}