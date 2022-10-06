//==============================================================================
// Filename: Win_App.h
// Description: アプリケーション処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Window.h>
#include <Win/Win_TimeProc.h>
#include <Gfx/Gfx_Main.h>
#include <Draw/Draw_ShaderMgr.h>
//#include <Geometry/TextureMgr.h>
#include <Tool/Input/Tool_InputMgr.h>

#ifdef IMGUI
#
#   include <Debug/Debug_ImguiMgr.h>
#
#endif // IMGUI

//===== 構造体定義 =====
struct GfxPack                     //グラフィック処理用データ
{
    GfxMain& m_dx;                  //DXオブジェクト参照
    DrawShaderMgr& m_shaderMgr;     //シェーダマネージャ参照
    //TEXTURE_MGR& m_textureMgr;    //テクスチャマネージャ参照
};

//===== 前方宣言 =====
class DrawCameraMgr;
class DrawLightMgr;
class GfxDrawer;
class DrawDirectionalLight;

//===== クラス定義 =====

//***** ゲームアプリ（Win64） *****
class App64
{
public:

    //プロトタイプ宣言
    App64();
    ~App64();
    int Run();                                            //アプリケーション実行
    void Update();                                        //更新処理
    void Draw();                                        //描画処理

    WinWindow& GetWindowProc() noexcept                //ウィンドウ処理参照
    {
        return m_window;
    }
    WinTime& GetTimeProc() noexcept                    //時間処理参照
    {
        return m_time;
    }
    GfxPack& GetGfxPack() const noexcept                //描画データ参照
    {
        return *m_pGfx;
    }
    ToolInputMgr& GetInputMgr() const noexcept                //入力マネージャ参照
    {
        return *m_pInputMgr;
    }
    DrawCameraMgr& GetCameraMgr() const noexcept            //カメラマネージャ参照
    {
        return *m_pCameraMgr;
    }
    DrawLightMgr& GetLightMgr() const noexcept                //ライトマネージャ参照
    {
        return *m_pLightMgr;
    }

private:

    //変数宣言

#ifdef IMGUI

    DebugImguiMgr m_ImGui;                                    //ImGuiマネージャ

#endif // IMGUI

    WinWindow m_window;                                //ゲームウィンドウ
    MSG m_message;                                        //メッセージ構造体
    WinTime m_time;                                    //時間処理
    std::unique_ptr<GfxMain> m_pDX;                        //DirectX用ポインタ
    std::unique_ptr<DrawShaderMgr> m_pShaderMgr;            //シェーダマネージャ用ポインタ
    //std::unique_ptr<TEXTURE_MGR> m_pTextureMgr;            //テクスチャマネージャ用ポインタ
    std::unique_ptr<GfxPack> m_pGfx;                    //描画データ用ポインタ
    std::unique_ptr<ToolInputMgr> m_pInputMgr;                //入力マネージャ用ポインタ
    std::unique_ptr<DrawCameraMgr> m_pCameraMgr;            //カメラマネージャ
    std::unique_ptr<DrawLightMgr> m_pLightMgr;                //ライトマネージャ



    std::vector<std::unique_ptr<GfxDrawer>> m_aDrawer;        //【描画テスト】
    static constexpr int nDrawNum = 1;                //描画数
    int m_shapeID;
    std::unique_ptr<DrawDirectionalLight> m_pSunLight;        //太陽光
};
