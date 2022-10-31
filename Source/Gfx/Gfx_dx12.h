//==============================================================================
// Filename: Gfx_dx12.h
// Description: DirectX12の処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Gfx_Main.h>

//===== クラス定義 =====
class GfxDX12 : public GfxMain
{
public:

    struct Vertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 uv;
    };

    //プロトタイプ宣言
    GfxDX12(WinDirectX& window);
    ~GfxDX12() noexcept(!IS_DEBUG) override;
    void BeginFrame(float r, float g, float b) noexcept override;                   //フレーム開始
    void DrawIndexed(UINT indexNum) const noexcept override;                        //描画処理(DX12では廃止)
    void DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept override;    //インスタンシング描画
    void EndFrame() override;                                                       //フレーム終了

private:

    //変数宣言
    Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice;                         //デバイス
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCmdAllocator;         //コマンドアロケータ
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCmdList;           //コマンドリスト
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCmdQueue;                 //コマンドキュー
    Microsoft::WRL::ComPtr<IDXGISwapChain4> m_pSwapChain;                   //スワップチェーン
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_pBackBuffers;     //バックバッファのポインタ配列
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapsRTV;               //RTV用ヒープ
    UINT m_rtvIncSize;                                                      //RTV用ヒープのインクリメントサイズ
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pDepthBuffer;                  //深度バッファ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapDSV;                //DSV用ヒープ
    Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;                           //フェンス
    UINT64 m_fenceVal;                                                      //フェンスの実行値
    HANDLE m_fenceEvent;                                                    //フェンス用イベント
    D3D12_RESOURCE_BARRIER m_resourceBarrier;                               //リソースバリア

#ifdef IMGUI

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeapImgui;

#endif // IMGUI

    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;

    //権限指定
    friend class GfxBinder;
};
