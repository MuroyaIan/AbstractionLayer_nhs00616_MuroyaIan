//==============================================================================
// Filename: Gfx_Main.h
// Description: グラフィック処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Win/Win_Window.h>
#include <wrl.h>                    //COMポインタ
#include <d3d11.h>
#include <d3dcompiler.h>            //シェーダ読込
#include <DirectXMath.h>

//===== 定数・マクロ定義 =====
#define ERROR_DX(hr) if(hr != S_OK) throw ERROR_EX(hr)    //DirectXエラー出力用

//===== クラス定義 =====

//***** グラフィック *****
class GRAPHIC
{
public:

    //描画モード
    enum class DRAW_MODE
    {
        DRAW_2D,
        DRAW_3D
    };

    //コピーNG
    GRAPHIC(const GRAPHIC&) = delete;
    GRAPHIC& operator=(const GRAPHIC&) = delete;

    //プロトタイプ宣言
    GRAPHIC(HWND hWindow, float fWidth, float fHeight);
    ~GRAPHIC() noexcept;
    void BeginFrame(float R, float G, float B) const noexcept;                        //フレーム開始
    void DrawIndexed(UINT IndexNum) const noexcept(!IS_DEBUG);                        //フレームバッファ書込み
    void DrawInstanced(UINT IndexNum, UINT InstanceNum) const noexcept(!IS_DEBUG);    //インスタンシング描画
    void EndFrame() const;                                                            //フレーム終了⇒描画開始
    void SetDrawMode(DRAW_MODE Mode);                                                //描画モード設定

    void SetViewMtx(DirectX::XMFLOAT4X4 mtxView) noexcept            //ビュー行列へのアクセス
    {
        m_mtxView = mtxView;
    }
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }
    void SetProjectionMtx(DirectX::XMFLOAT4X4 mtxProj) noexcept        //投影行列へのアクセス
    {
        m_mtxProjection = mtxProj;
    }
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    void SetImGuiMode(const bool bEnable) noexcept                    //ImGui描画ON/OFF
    {
        m_bDrawImGui = bEnable;
    }

    bool IsImGuiEnabled() const noexcept                            //ImGui描画状態確認
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

private:

    //変数宣言
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;                    //デバイス
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;            //スワップチェーン
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;            //コンテキスト
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;    //ターゲットビュー
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;        //深度・ステンシルビュー

    DirectX::XMFLOAT4X4 m_mtxView;            //ビュー行列（カメラ）
    DirectX::XMFLOAT4X4 m_mtxProjection;    //投影行列

#ifdef IMGUI

    bool m_bDrawImGui;                        //ImGUI描画制御

#endif // IMGUI

    //権限指定
    friend class BINDER;
};
