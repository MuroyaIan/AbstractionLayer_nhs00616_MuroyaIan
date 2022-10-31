//==============================================================================
// Filename: Win_Main.cpp
// Description: メインファイル（エントリポイント）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

// インクルード部
#include <Win/Win_Main.h>
#include <Win/Win_VerCheckWindow.h>
#include <Win/Win_App.h>



//OpenGL
#include <3rd_Party/glad/gl.h>
#include <3rd_Party/glfw/glfw3.h>
#define GLM_FORCE_RADIANS                   //ラジアン角度に設定
#define GLM_ENABLE_EXPERIMENTAL             //GLM_GTX_transform使用
#include <3rd_Party/glm/glm.hpp>
//#pragma warning(disable: 4201)
#include <3rd_Party/glm/gtc/type_ptr.hpp>   //glm::vec型用
#include <3rd_Party/glm/gtx/transform.hpp>  //行列変換

//追加ライブラリ
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glm_static.lib")

void TestRun();
int InitSystem(GLFWwindow** ppWindow);

float lastX = 0.0f;
float lastY = 0.0f;
float pitch = 0.0f;
float yaw = -90.0f;
bool firstMouse = true;
void Callback_MouseDelta(GLFWwindow* window, double xpos, double ypos);     //マウス座標取得
void Callback_ResizeWindow(GLFWwindow* window, int width, int height);      //ビューポート設定用CALLBACK関数

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
void InputProc(GLFWwindow* window);     //入力処理

void InitGfx();                         //GfxAPI更新
void Update();
void Draw();

void BindVertexIndexBuffer();
void BindShader();
void BindTexture();

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;
unsigned int instanceVBO;
unsigned int shaderProgram;
unsigned int texture;

struct Vertex
{
	struct Pos
	{
		float x, y, z;
	};

	struct Tex
	{
		float u, v;
	};

	Pos pos;
	Tex uv;
};
std::vector<Vertex> MakeBox();



//エントリーポイント
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    try
    {
        //前処理
        UNREFERENCED_PARAMETER(hInstance);
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(lpCmdLine);
        UNREFERENCED_PARAMETER(nCmdShow);

#ifdef _DEBUG

        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   //メモリリーク検出

#endif // _DEBUG

        //グラフィックスAPI設定
        WinCheckGfx Window;
        int wParam = Window.Update();
        if (wParam != 0 || GfxMain::CheckApiVer(GfxMain::API_MODE::NONE)
            || GfxMain::CheckApiVer(GfxMain::API_MODE::VULKAN))
            return wParam;

        //アプリケーション実行
        if (GfxMain::CheckApiVer(GfxMain::API_MODE::OPEN_GL))
            TestRun();
        else {
            while (!GfxMain::CheckApiVer(GfxMain::API_MODE::NONE))
                wParam = App64{}.Run();
        }

#ifdef _DEBUG

        //エラー処理
        Microsoft::WRL::ComPtr<IDXGIDebug1> pDebugDxgi;
        if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&pDebugDxgi))))
            pDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);

#endif // _DEBUG

        //終了処理
        return wParam;
    }
    catch (const WinErrorProc& e)
    {
        //Windowエラー
        MessageBoxA(nullptr, e.what(), e.GetErrorType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        //標準エラー
        MessageBoxA(nullptr, e.what(), "標準的例外（エラー）", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        //未定義エラー
        MessageBoxA(nullptr, "詳細不明です。", "不明のエラー", MB_OK | MB_ICONEXCLAMATION);
    }

    //異常終了
    return -1;
}



void TestRun()
{
    //glfw初期化
    GLFWwindow* window = nullptr;
    int result = InitSystem(&window);
    if (result)
        return;

    //Gfx初期化
    InitGfx();

    //実行ループ
    while (glfwWindowShouldClose(window) == GL_FALSE) {

        //更新処理
        InputProc(window);  //入力処理
        Update();

        //描画
        Draw();

        //更新処理
        glfwSwapBuffers(window);    //バッファ切替
        glfwPollEvents();           //イベント実行
    }

    //glfw終了
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
}

//OpenGL初期化
int InitSystem(GLFWwindow** ppWindow)
{
    //glfw初期化
    if (glfwInit() == GL_FALSE) {

#ifdef _DEBUG

        PrintD(L"Error : glfw 3.3.8 Init Failed!\n");

#endif // _DEBUG

        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  //バージョン
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  //プロファイル
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //Windows以外のOSは必須
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);                     //windowサイズ変更禁止

    //Window作成
    *ppWindow = glfwCreateWindow(1600, 900, "OpenGL_Base",
        nullptr,                                            //フルスクリーンモード
        nullptr);                                           //複数ウィンドウの使用
    if (!*ppWindow) {

#ifdef _DEBUG

        PrintD(L"Error : Window Creation Failed!\n");

#endif // _DEBUG

        glfwTerminate();    //強制終了
        return -1;
    }
    glfwSetWindowPos(*ppWindow, 160, 80);                               //ウィンドウ初期位置
    glfwMakeContextCurrent(*ppWindow);                                  //コンテキスト作成

    //マウス設定
    glfwSetInputMode(*ppWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     //非表示
    glfwSetCursorPosCallback(*ppWindow, Callback_MouseDelta);           //座標取得

    //OpenGL関数のロード
    const int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {

#ifdef _DEBUG

        PrintD(L"Error : glad2 Init Failed!\n");

#endif // _DEBUG

        return -1;
    }

#ifdef _DEBUG

    //OpenGLバージョン出力
    std::wostringstream oss;
    oss << "Info : Load OpenGL : v" << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;
    PrintD(oss.str().c_str());

    //最大頂点属性出力
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::wostringstream oss2;
    oss2 << "Info : Max Vertex Attributes Supported : " << nrAttributes << std::endl;
    PrintD(oss2.str().c_str());

#endif // _DEBUG

    //ビューポート設定
    glfwSetFramebufferSizeCallback(*ppWindow, Callback_ResizeWindow);

    return 0;
}

//マウス座標取得
void Callback_MouseDelta(GLFWwindow* window, double xpos, double ypos)
{
    //初回処理
    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    //位置更新
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    //オフセット更新
    float sensitivity = 0.2f;    //感度
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //ピッチ角・ヨー角更新
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front{};
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    (void)window;
}

//windowサイズ変更時の更新処理
void Callback_ResizeWindow(GLFWwindow* window, int width, int height)
{
    //ビューポート更新
    glViewport(0, 0, width, height);
    (void)window;
}

//Gfx初期化
void InitGfx()
{
    //深度バッファオン
    glEnable(GL_DEPTH_TEST);

    //RTV背景色設定
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//入力処理
void InputProc(GLFWwindow* window)
{
    //ウィンドウ終了
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //カメラ制御
    float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

//更新処理
void Update()
{
}

//描画処理
void Draw()
{
    //バッファクリア
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//頂点・インデックスバッファ設定
	BindVertexIndexBuffer();

	//シェーダ設定
	BindShader();

	//テクスチャ設定
	BindTexture();

	//テクスチャセット
	glActiveTexture(GL_TEXTURE0);				//テクスチャセット（ユニットは最大16個）
	glBindTexture(GL_TEXTURE_2D, texture);

	//シェーダプログラムセット
	glUseProgram(shaderProgram);



	//変換行列処理(mtxW,V,P)
	glm::mat4 model(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	glm::mat4 view(1.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f, 100.0f);

	int modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	modelLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));
	modelLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));



	//描画処理
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 100);
	glBindVertexArray(0);

	//終了処理
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

//頂点・インデックスバッファ設定
void BindVertexIndexBuffer()
{
	//頂点配列Obj作成
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	std::vector<Vertex> data = MakeBox();
	unsigned int indices[]{
		 0,  2,  1,		 2,  3,  1,
		 4,  6,  5,		 6,  7,  5,
		 8, 10,  9,		10, 11,  9,
		12, 14, 13,		14, 15, 13,
		16, 18, 17,		18, 19, 17,
		20, 22, 21,		22, 23, 21
	};
	glGenBuffers(1, &VBO);														//作成
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											//バインド
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), GL_STATIC_DRAW);	//更新

	//インデックスバッファ作成
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//(直前にバインドしたVBO情報とリンク)
	//IL設定(頂点位置)
	glVertexAttribPointer(0,		//レイアウト位置(location)
		3,							//頂点ごとのデータ数
		GL_FLOAT,					//データの型
		GL_FALSE,					//正規化するかどうか
		5 * sizeof(float),			//頂点ごとのデータサイズ(stride)
		(void*)0);					//データの読込開始位置
	glEnableVertexAttribArray(0);	//有効化

	//IL設定(UV座標)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//インスタンスバッファ作成
	struct InstancePack
	{
		glm::vec2 translations;
		glm::mat4 models;
	};
	InstancePack pack[100];
	int index = 0;
	float offset = 2.0f;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			glm::vec2 translation{};
			translation.x = (float)x * offset;
			translation.y = (float)y * offset;
			pack[index].translations = translation;
			pack[index].models = glm::mat4(1.0f);
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			if (index % 3 == 0)
				r = 1.0f;
			else if (index % 3 == 1)
				g = 1.0f;
			else if (index % 3 == 2)
				b = 1.0f;
			pack[index].models = glm::rotate(pack[index].models, glm::radians(360.0f * index / 99.0f), glm::vec3(r, g, b));
			index++;
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstancePack) * 100, &pack[0], GL_STATIC_DRAW);

	//IL設定(インスタンス情報)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 18 * sizeof(float), (void*)(14 * sizeof(float)));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, 1);



	//バインド解除（念の為）
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	//VAOより先にバインド解除はNG
}

//シェーダ設定
void BindShader()
{
	//頂点シェーダ作成
	std::string vsSource;
	std::ifstream vss("Source/Shader/glsl/VS_Test.glsl", std::ios::in);
	if (vss.is_open()) {
		std::stringstream sstr;
		sstr << vss.rdbuf();
		vsSource = sstr.str();
		vss.close();
	}

	//シェーダObj作成
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//コンパイル
	char const* pVS = vsSource.c_str();
	glShaderSource(vertexShader,
		1,							//リソース配列の要素数
		&pVS,						//リソース配列のポインタ
		NULL);						//リソース配列のサイズ
	glCompileShader(vertexShader);

#ifdef _DEBUG

	{
		//デバッグ情報
		int  success;
		char infoLog[512]{};
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::wostringstream oss;
			oss << "Error : Vertex Shader Compilation Failed!\n" << infoLog << std::endl;
			PrintD(oss.str().c_str());
		}
	}

#endif // _DEBUG



	//ピクセルシェーダ作成
	std::string psSource;
	std::ifstream pss("Source/Shader/glsl/PS_Test.glsl", std::ios::in);
	if (pss.is_open()) {
		std::stringstream sstr;
		sstr << pss.rdbuf();
		psSource = sstr.str();
		pss.close();
	}

	//シェーダObj作成
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char const* pPS = psSource.c_str();
	glShaderSource(fragmentShader, 1, &pPS, NULL);			//コンパイル
	glCompileShader(fragmentShader);

#ifdef _DEBUG

	{
		//デバッグ情報
		int  success;
		char infoLog[512]{};
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::wostringstream oss;
			oss << "Error : Vertex Shader Compilation Failed!\n" << infoLog << std::endl;
			PrintD(oss.str().c_str());
		}
	}

#endif // _DEBUG



	//シェーダプログラム作成
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

#ifdef _DEBUG

	{
		//デバッグ情報
		int  success;
		char infoLog[512]{};
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::wostringstream oss;
			oss << "Error : Shader Program Link Failed!\n" << infoLog << std::endl;
			PrintD(oss.str().c_str());
		}
	}

#endif // _DEBUG

	glDeleteShader(vertexShader);		//シェーダObjを破棄
	glDeleteShader(fragmentShader);
}

//テクスチャ設定
void BindTexture()
{
	//テクスチャ作成
	unsigned char color[] = {
		255, 255, 255,	255, 255, 255,	255, 255, 255,	255, 255, 255,
		255, 255, 255,	255, 255, 255,	255, 255, 255,	255, 255, 255,
		255, 255, 255,	255, 000, 255,	255, 255, 255,	255, 255, 255,
		255, 255, 255,	255, 255, 255,	255, 000, 255,	255, 255, 255,
		255, 255, 255,	255, 000, 255,	255, 000, 255,	255, 255, 255,
		255, 255, 255,	255, 000, 255,	255, 000, 255,	255, 255, 255,
		255, 000, 255,	255, 000, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 000, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 255, 255,	255, 255, 255,	255, 000, 255,
		255, 000, 255,	255, 255, 255,	255, 255, 255,	255, 000, 255,
		255, 000, 255,	255, 255, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 255, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 000, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 000, 255,	255, 000, 255,	255, 000, 255,
		255, 255, 255,	255, 000, 255,	255, 000, 255,	255, 000, 255,
		255, 000, 255,	255, 000, 255,	255, 000, 255,	255, 255, 255,
	};
	int width = 8;
	int height = 8;

	//バッファ作成
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//ラッピング設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		//リピート方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//フィルタリング設定(ミップマップ)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//ポイント方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//テクスチャ読込
	glTexImage2D(GL_TEXTURE_2D, 0,		//ミップマップレベル
		GL_RGB,							//テクスチャタイプ
		width, height, 0,				//リソースの幅と高さ
		GL_RGB, GL_UNSIGNED_BYTE,		//リソースのフォーマットと型
		color);							//リソースのポインタ
	glGenerateMipmap(GL_TEXTURE_2D);

	//バインド解除
	glBindTexture(GL_TEXTURE_2D, 0);



	//テクスチャユニット設定
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
}

std::vector<Vertex> MakeBox()
{
	//長さ定義
	constexpr float size = 1.0f * 0.5f;		//ボックスの辺の長さ(標準1.0f)

	//頂点作成
	std::vector<Vertex> aData(24);
	aData[0].pos  = { -size, -size, -size };
	aData[1].pos  = {  size, -size, -size };
	aData[2].pos  = { -size,  size, -size };
	aData[3].pos  = {  size,  size, -size };	//near
	aData[4].pos  = {  size, -size,  size };
	aData[5].pos  = { -size, -size,  size };
	aData[6].pos  = {  size,  size,  size };
	aData[7].pos  = { -size,  size,  size };    //far
	aData[8].pos  = { -size, -size,  size };
	aData[9].pos  = { -size, -size, -size };
	aData[10].pos = { -size,  size,  size };
	aData[11].pos = { -size,  size, -size };    //left
	aData[12].pos = {  size, -size, -size };
	aData[13].pos = {  size, -size,  size };
	aData[14].pos = {  size,  size, -size };
	aData[15].pos = {  size,  size,  size };    //right
	aData[16].pos = { -size, -size,  size };
	aData[17].pos = {  size, -size,  size };
	aData[18].pos = { -size, -size, -size };
	aData[19].pos = {  size, -size, -size };    //bottom
	aData[20].pos = { -size,  size, -size };
	aData[21].pos = {  size,  size, -size };
	aData[22].pos = { -size,  size,  size };
	aData[23].pos = {  size,  size,  size };    //top
	aData[0].uv  = { 0.0f, 1.0f };
	aData[1].uv  = { 1.0f, 1.0f };
	aData[2].uv  = { 0.0f, 0.0f };
	aData[3].uv  = { 1.0f, 0.0f };              //near
	aData[4].uv  = { 0.0f, 1.0f };
	aData[5].uv  = { 1.0f, 1.0f };
	aData[6].uv  = { 0.0f, 0.0f };
	aData[7].uv  = { 1.0f, 0.0f };              //far
	aData[8].uv  = { 0.0f, 1.0f };
	aData[9].uv  = { 1.0f, 1.0f };
	aData[10].uv = { 0.0f, 0.0f };
	aData[11].uv = { 1.0f, 0.0f };              //left
	aData[12].uv = { 0.0f, 1.0f };
	aData[13].uv = { 1.0f, 1.0f };
	aData[14].uv = { 0.0f, 0.0f };
	aData[15].uv = { 1.0f, 0.0f };              //right
	aData[16].uv = { 0.0f, 1.0f };
	aData[17].uv = { 1.0f, 1.0f };
	aData[18].uv = { 0.0f, 0.0f };
	aData[19].uv = { 1.0f, 0.0f };              //bottom
	aData[20].uv = { 0.0f, 1.0f };
	aData[21].uv = { 1.0f, 1.0f };
	aData[22].uv = { 0.0f, 0.0f };
	aData[23].uv = { 1.0f, 0.0f };              //top

	return aData;
}
