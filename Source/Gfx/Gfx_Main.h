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
class GfxMain
{

    //--------------------------------------------------------------------------
    friend class BINDER;
    //--------------------------------------------------------------------------

public:

    //描画モード
    enum class DrawMode
    {
        DRAW_2D,
        DRAW_3D
    };

    //コピーNG
    GfxMain(const GfxMain&) = delete;
    GfxMain& operator=(const GfxMain&) = delete;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] hWindow  ウィンドウハンドル
    /// \param[in] fWidth   ウィンドウ幅
    /// \param[in] fHeight  ウィンドウ高さ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    GfxMain(
        /*[in]*/ HWND hWindow,
        /*[in]*/ float fWidth,
        /*[in]*/ float fHeight);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~GfxMain() noexcept;

    //--------------------------------------------------------------------------
    /// フレーム開始
    ///
    /// \param[in] r    色のrチャンネル
    /// \param[in] g    色のgチャンネル
    /// \param[in] b    色のbチャンネル
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void BeginFrame(
        /*[in]*/ float r,
        /*[in]*/ float g,
        /*[in]*/ float b) const noexcept;

    //--------------------------------------------------------------------------
    /// フレームバッファ書込み
    ///
    /// \param[in] indexNum     描画する頂点の要素数
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawIndexed(
        /*[in]*/ UINT indexNum) const noexcept(!IS_DEBUG);

    //--------------------------------------------------------------------------
    /// インスタンシング描画
    ///
    /// \param[in] indexNum     描画する頂点の要素数
    /// \param[in] instanceNum  描画するインスタンス数
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void DrawInstanced(
        /*[in]*/ UINT indexNum,
        /*[in]*/ UINT instanceNum) const noexcept(!IS_DEBUG);

    //--------------------------------------------------------------------------
    /// フレーム終了⇒描画開始
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void EndFrame() const;

    //--------------------------------------------------------------------------
    /// 描画モード設定
    ///
    /// \param[in] mode     指定する描画モード
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetDrawMode(
        /*[in]*/ DrawMode mode);

    //--------------------------------------------------------------------------
    /// ビュー行列設定
    ///
    /// \param[in] mtxView  指定するビュー行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetViewMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxView) noexcept
    {
        m_mtxView = mtxView;
    }

    //--------------------------------------------------------------------------
    /// ビュー行列取得
    ///
    /// \return ビュー行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetViewMtx() const noexcept
    {
        return m_mtxView;
    }

    //--------------------------------------------------------------------------
    /// 投影行列設定
    ///
    /// \param[in] mtxProj  指定する投影行列
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetProjectionMtx(
        /*[in]*/ DirectX::XMFLOAT4X4 mtxProj) noexcept        //投影行列へのアクセス
    {
        m_mtxProjection = mtxProj;
    }

    //--------------------------------------------------------------------------
    /// 投影行列取得
    ///
    /// \return 投影行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetProjectionMtx() const noexcept
    {
        return m_mtxProjection;
    }

#ifdef IMGUI

    //--------------------------------------------------------------------------
    /// ImGui描画ON/OFF
    ///
    /// \param[in] bEnable  描画切替フラグ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void SetImGuiMode(
        /*[in]*/ const bool bEnable) noexcept                    //ImGui描画ON/OFF
    {
        m_bDrawImGui = bEnable;
    }

    //--------------------------------------------------------------------------
    /// ImGui描画状態確認
    ///
    /// \return bool型変数(描画中かどうか)
    //--------------------------------------------------------------------------
    bool IsImGuiEnabled() const noexcept                            //ImGui描画状態確認
    {
        return m_bDrawImGui;
    }

#endif // IMGUI

    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;
    DirectX::XMFLOAT4X4 m_mtxView;
    DirectX::XMFLOAT4X4 m_mtxProjection;

#ifdef IMGUI

    bool m_bDrawImGui;

#endif // IMGUI

    //--------------------------------------------------------------------------

    /// <summary>
    /// m_pDevice           デバイス
    /// m_pSwapChain        スワップチェーン
    /// m_pContext          コンテキスト
    /// m_pTargetView       ターゲットビュー
    /// m_pDSView           深度・ステンシルビュー
    /// m_mtxView           ビュー行列（カメラ）
    /// m_mtxProjection     投影行列
    /// m_bDrawImGui        ImGUI描画制御
    /// </summary>
};
