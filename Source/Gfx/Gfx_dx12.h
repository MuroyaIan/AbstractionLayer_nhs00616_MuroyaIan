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
    GfxDX12(WinWindow& window);
    ~GfxDX12() noexcept override;
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



    Microsoft::WRL::ComPtr<ID3D12Resource> m_pVertexBuffer;                 //頂点バッファ
    D3D12_VERTEX_BUFFER_VIEW m_viewVB;                                      //頂点バッファビュー
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pIndexBuffer;                  //インデックスバッファ
    D3D12_INDEX_BUFFER_VIEW m_viewIB;                                       //インデックスバッファビュー
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobVS;                             //VSバイナリ
    Microsoft::WRL::ComPtr<ID3DBlob> m_pBlobPS;                             //PSバイナリ
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_pLayouts;                       //インプットレイアウト
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;           //ルールシグネチャ
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pGfxPipelineState;        //GFXパイプラインステート
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_pTextureBuffer;                //テクスチャバッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pConstBuffer;                  //定数バッファ（変換行列）
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pBufferHeaps;            //バッファ用ヒープ
    std::vector<DirectX::XMMATRIX> m_aMatrix;                               //変換行列

    //プロトタイプ宣言
    void BindVertexBuffer();
    void BindIndexBuffer();
    void BindVS();
    void BindPS();
    void BindLayout();
    void BindRootSignature();
    void BindPipelineState();

    void BindBufferHeaps();
    void BindTextureBuffer();
    void BindConstBuffer();
    std::vector<Vertex> MakeBox();
};
