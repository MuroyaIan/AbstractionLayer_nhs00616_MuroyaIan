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
//#include <Draw/Draw_ShaderMgr.h>
#include <Tool/Input/Tool_InputMgr.h>

#ifdef IMGUI
#
#   include <Debug/Debug_ImguiMgr.h>
#
#endif // IMGUI

//===== 構造体定義 =====
struct GfxPack              //グラフィック処理用データ
{
    //--------------------------------------------------------------------------
    //GfxMain& m_dx;
    //DrawShaderMgr& m_shaderMgr;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_dx            DXオブジェクト参照
    /// m_shaderMgr     シェーダマネージャ参照
    /// </summary>
};

//===== 前方宣言 =====
//class DrawCameraMgr;
//class DrawLightMgr;
//class GfxDrawer;
//class DrawDirectionalLight;

//===== クラス定義 =====

//***** ゲームアプリ（Win64） *****
class App64
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    App64();

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~App64();

    //--------------------------------------------------------------------------
    /// アプリケーション実行
    ///
    /// \return エラーコード
    //--------------------------------------------------------------------------
    int Run();

    //--------------------------------------------------------------------------
    /// 更新処理
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Update();

    //--------------------------------------------------------------------------
    /// 描画処理
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Draw();

    //--------------------------------------------------------------------------
    /// ウィンドウ処理参照
    ///
    /// \return ウィンドウ処理の参照先
    //--------------------------------------------------------------------------
    WinWindow& GetWindowProc() noexcept
    {
        return m_window;
    }

    //--------------------------------------------------------------------------
    /// 時間処理参照
    ///
    /// \return 時間処理の参照先
    //--------------------------------------------------------------------------
    WinTime& GetTimeProc() noexcept
    {
        return m_time;
    }

    //--------------------------------------------------------------------------
    /// 描画データ参照
    ///
    /// \return 描画データの参照先
    //--------------------------------------------------------------------------
    //GfxPack& GetGfxPack() const noexcept
    //{
    //    return *m_pGfx;
    //}

    //--------------------------------------------------------------------------
    /// 入力マネージャ参照
    ///
    /// \return 入力マネージャの参照先
    //--------------------------------------------------------------------------
    ToolInputMgr& GetInputMgr() const noexcept
    {
        return *m_pInputMgr;
    }

    //--------------------------------------------------------------------------
    /// カメラマネージャ参照
    ///
    /// \return カメラマネージャの参照先
    //--------------------------------------------------------------------------
    //DrawCameraMgr& GetCameraMgr() const noexcept
    //{
    //    return *m_pCameraMgr;
    //}

    //--------------------------------------------------------------------------
    /// ライトマネージャ参照
    ///
    /// \return ライトマネージャの参照先
    //--------------------------------------------------------------------------
    //DrawLightMgr& GetLightMgr() const noexcept
    //{
    //    return *m_pLightMgr;
    //}

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------

#ifdef IMGUI

    DebugImguiMgr m_imGui;

#endif // IMGUI

    WinWindow m_window;
    MSG m_message;
    WinTime m_time;
    std::unique_ptr<GfxMain> m_pDX;
    //std::unique_ptr<DrawShaderMgr> m_pShaderMgr;
    //std::unique_ptr<GfxPack> m_pGfx;
    std::unique_ptr<ToolInputMgr> m_pInputMgr;
    //std::unique_ptr<DrawCameraMgr> m_pCameraMgr;
    //std::unique_ptr<DrawLightMgr> m_pLightMgr;
    //std::vector<std::unique_ptr<GfxDrawer>> m_aDrawer;
    int m_shapeID;
    //std::unique_ptr<DrawDirectionalLight> m_pSunLight;
    DirectX::XMINT2 m_rot;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_imGui         ImGuiマネージャ
    /// m_window        ゲームウィンドウ
    /// m_message       メッセージ構造体
    /// m_time          時間処理
    /// m_pDX           DirectX用ポインタ
    /// m_pShaderMgr    シェーダマネージャ用ポインタ
    /// m_pGfx          描画データ用ポインタ
    /// m_pInputMgr     入力マネージャ用ポインタ
    /// m_pCameraMgr    カメラマネージャ
    /// m_pLightMgr     ライトマネージャ
    /// m_aDrawer       テスト描画用ドロワー
    /// m_shapeID       形状番号
    /// m_pSunLight     太陽光
    /// m_rot           モデル回転量
    /// </summary>
};
