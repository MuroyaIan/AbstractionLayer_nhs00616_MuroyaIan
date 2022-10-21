//==============================================================================
// Filename: Gfx_dx11.h
// Description: DirectX11の処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

//===== クラス定義 =====
class GfxDX11 : public GfxMain
{
public:

    //プロトタイプ宣言
    GfxDX11(WinWindow& window);
    ~GfxDX11() noexcept override;
    void BeginFrame(float r, float g, float b) noexcept override;                   //フレーム開始
    void DrawIndexed(UINT indexNum) const noexcept override;                        //描画処理
    void DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept override;    //インスタンシング描画
    void EndFrame() override;                                                       //フレーム終了

private:

    //変数宣言
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;             //デバイス
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;        //スワップチェーン
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;     //コンテキスト
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTView;   //ターゲットビュー
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;   //深度・ステンシルビュー

    //権限指定
    friend class GfxBinder;
};
