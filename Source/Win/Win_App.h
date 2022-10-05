﻿//==============================================================================
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
//#include <Tool/Input/InputMgr.h>

#ifdef IMGUI
#
#   include <Debug/Debug_ImguiMgr.h>
#
#endif // IMGUI

//===== 構造体定義 =====
struct GFX_PACK                     //描画用データ
{
    GfxMain& m_DX;                  //DXオブジェクト参照
    DrawShaderMgr& m_ShaderMgr;     //シェーダマネージャ参照
    //TEXTURE_MGR& m_TextureMgr;    //テクスチャマネージャ参照
};

//===== 前方宣言 =====
class DrawCameraMgr;
//class LIGHT_MGR;
//
//class DRAWER;
//class DIRECTIONAL_LIGHT;

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
        return m_Window;
    }
    WinTime& GetTimeProc() noexcept                    //時間処理参照
    {
        return m_Time;
    }
    GFX_PACK& GetGfxPack() const noexcept                //描画データ参照
    {
        return *m_pGfx;
    }
//    INPUT_MGR& GetInputMgr() const noexcept                //入力マネージャ参照
//    {
//        return *m_pInputMgr;
//    }
    DrawCameraMgr& GetCameraMgr() const noexcept            //カメラマネージャ参照
    {
        return *m_pCameraMgr;
    }
//    LIGHT_MGR& GetLightMgr() const noexcept                //ライトマネージャ参照
//    {
//        return *m_pLightMgr;
//    }

private:

    //変数宣言

#ifdef IMGUI

    DebugImguiMgr m_ImGui;                                    //ImGuiマネージャ

#endif // IMGUI

    WinWindow m_Window;                                //ゲームウィンドウ
    MSG m_Message;                                        //メッセージ構造体
    WinTime m_Time;                                    //時間処理
    std::unique_ptr<GfxMain> m_pDX;                        //DirectX用ポインタ
    std::unique_ptr<DrawShaderMgr> m_pShaderMgr;            //シェーダマネージャ用ポインタ
    //std::unique_ptr<TEXTURE_MGR> m_pTextureMgr;            //テクスチャマネージャ用ポインタ
    std::unique_ptr<GFX_PACK> m_pGfx;                    //描画データ用ポインタ
    //std::unique_ptr<INPUT_MGR> m_pInputMgr;                //入力マネージャ用ポインタ
    std::unique_ptr<DrawCameraMgr> m_pCameraMgr;            //カメラマネージャ
    //std::unique_ptr<LIGHT_MGR> m_pLightMgr;                //ライトマネージャ



    //std::vector<std::unique_ptr<DRAWER>> m_aDrawer;        //【描画テスト】
    //static constexpr int nDrawNum = 120;                //描画数
    //std::unique_ptr<DIRECTIONAL_LIGHT> m_pSunLight;        //太陽光
};
