//==============================================================================
// Filename: Gfx_Main.h
// Description: グラフィック処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Window.h>
#include <wrl.h>                //COMポインタ
#include <dxgi1_6.h>
#include <d3dcompiler.h>        //シェーダ読込
#include <d3d11.h>
#include <d3d12.h>
#include <DirectXMath.h>

#ifdef _DEBUG

#include <Initguid.h>
#include <dxgidebug.h>  //DXGIデバッグ出力

#endif // _DEBUG

//===== 定数・マクロ定義 =====
#define ERROR_DX(hr) if(hr != S_OK) throw ERROR_EX(hr)  //DirectXエラー出力用
#define ALIGN256(x) ((x + 0xff) & (~0xff))              //256アライメントに修正

//===== クラス定義 =====
class GfxMain
{
public:

    //GfxAPIモード
    enum class API_MODE
    {
        NONE,
        aDX_11,
        aDX_12,
        OPEN_GL,
        VULKAN
    };

    //描画モード
    enum class DRAW_MODE
    {
        DRAW_2D,
        DRAW_3D
    };

    //コピーNG
    GfxMain(const GfxMain&) = delete;
    GfxMain& operator=(const GfxMain&) = delete;

    //プロトタイプ宣言
    GfxMain(WinWindow& Window);
    virtual ~GfxMain() noexcept = 0;
    virtual void BeginFrame(float R, float G, float B) noexcept = 0;                    //フレーム開始
    virtual void DrawIndexed(UINT IndexNum) const noexcept = 0;                         //フレームバッファ書込み
    virtual void DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept = 0;     //インスタンシング描画
    virtual void EndFrame() = 0;                                                        //フレーム終了⇒描画開始
    void SetDrawMode(DRAW_MODE Mode) noexcept;                                          //描画モード設定

    void SetViewMtx(DirectX::XMFLOAT4X4 mtxView) noexcept           //ビュー行列へのアクセス
    {
        m_mtxView = mtxView;
    }
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }
    void SetProjectionMtx(DirectX::XMFLOAT4X4 mtxProj) noexcept     //投影行列へのアクセス
    {
        m_mtxProjection = mtxProj;
    }
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    void SetImGuiMode(const bool bEnable) noexcept                  //ImGui描画ON/OFF
    {
        m_bDrawImGui = bEnable;
    }

    bool IsImGuiEnabled() const noexcept                            //ImGui描画状態確認
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

protected:

    //変数宣言
    WinWindow& m_Window;                                //ウィンドウ情報の参照
    Microsoft::WRL::ComPtr<IDXGIFactory6> m_pFactory;   //DXGIファクトリ
    Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter;    //アダプター

private:

    //変数宣言
    DRAW_MODE m_DrawMode;                               //描画モード
    DirectX::XMFLOAT4X4 m_mtxView;                      //ビュー行列（カメラ）
    DirectX::XMFLOAT4X4 m_mtxProjection;                //投影行列

    static API_MODE m_Api;                              //GfxApiモード

#ifdef IMGUI

    bool m_bDrawImGui = true;                           //ImGUI描画制御

#endif // IMGUI

    //権限指定
    friend class GfxMgr;
    friend class BINDER;
};
