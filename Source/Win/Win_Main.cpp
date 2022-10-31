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
        IDXGIDebug1* pDebugDxgi;
        if (SUCCEEDED(DXGIGetDebugInterface1(0u, IID_PPV_ARGS(&pDebugDxgi)))) {
            pDebugDxgi->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
            pDebugDxgi->Release();
        }

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

    //描画処理

}
