//==============================================================================
// Filename: Gfx_dx12.cpp
// Description: DirectX12の処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Gfx_dx12.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== クラス実装 =====
GfxDX12::GfxDX12(WinWindow& window) : GfxMain(window),
m_pDevice(), m_pCmdAllocator(), m_pCmdList(), m_pCmdQueue(), m_pSwapChain(), m_pBackBuffers(0), m_pHeapsRTV(), m_rtvIncSize(0),
m_pDepthBuffer(), m_pHeapDSV(), m_pFence(), m_fenceVal(0), m_fenceEvent(), m_resourceBarrier(),
m_pVertexBuffer(), m_viewVB(), m_pIndexBuffer(), m_viewIB(), m_pBlobVS(), m_pBlobPS(), m_pLayouts(0),
m_pRootSignature(), m_pGfxPipelineState(), m_viewport(), m_scissorRect(),
m_pTextureBuffer(), m_pBufferHeaps(), m_pConstBuffer(), m_aMatrix(1)
{
    //ウィンドウ情報取得
    auto [winWidth, winHeight] = m_window.GetSize();

    //エラーハンドル
    HRESULT hr{};

#ifdef _DEBUG

    //デバッグレイヤ初期化
    ID3D12Debug* pDebugLayer = nullptr;
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugLayer));
    ERROR_DX(hr);
    pDebugLayer->EnableDebugLayer();
    pDebugLayer->Release();

#endif // _DEBUG

    //FeatureLevel設定
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
    };

    //デバイス作成
    hr = D3D12CreateDevice(m_pAdapter.Get(), featureLevels[0], IID_PPV_ARGS(&m_pDevice));
    if (hr != S_OK)
        hr = D3D12CreateDevice(m_pAdapter.Get(), featureLevels[1], IID_PPV_ARGS(&m_pDevice));
    if (hr != S_OK)
        throw ERROR_EX2(S_OK, "GPUはDX12非対応です。");

    //コマンドアロケータ・リスト作成
    hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAllocator));
    ERROR_DX(hr);
    hr = m_pDevice->CreateCommandList(0u,                       //nodeMask(GPU識別用)
        D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAllocator.Get(),
        nullptr,                                                //ID3D12PipelineState(?)
        IID_PPV_ARGS(&m_pCmdList));
    ERROR_DX(hr);

    //コマンドキュー作成
    D3D12_COMMAND_QUEUE_DESC cqd{};
    cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;                              //コマンドリストと同じ
    cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;                     //特に指定なし
    cqd.NodeMask = 0u;
    cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;                              //タイムアウトなし
    hr = m_pDevice->CreateCommandQueue(&cqd, IID_PPV_ARGS(&m_pCmdQueue));
    ERROR_DX(hr);

    //スワップチェーン作成
    DXGI_SWAP_CHAIN_DESC1 scd{};
    scd.Width = static_cast<UINT>(winWidth);
    scd.Height = static_cast<UINT>(winHeight);
    scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                //ピクセルフォーマット
    scd.Stereo = false;                                     //3Dディスプレイのステレオモード
    scd.SampleDesc.Count = 1u;
    scd.SampleDesc.Quality = 0u;                            //マルチサンプリング設定
    scd.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    scd.BufferCount = 2u;                                   //ダブルバッファなので2
    scd.Scaling = DXGI_SCALING_STRETCH;                     //バッファ伸び縮み可能
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;         //フリップ後速やかに破棄
    scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;            //指定なし
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     //ウィンドウモード切替可能
    wrl::ComPtr<IDXGISwapChain1> m_pSwapChain1;
    hr = m_pFactory->CreateSwapChainForHwnd(m_pCmdQueue.Get(), m_window.GetHandle(), &scd,
        nullptr,                                                                            //DXGI_SWAP_CHAIN_FULLSCREEN_DESC(?)
        nullptr,                                                                            //IDXGIOutput(?)
        &m_pSwapChain1);
    ERROR_DX(hr);
    hr = m_pSwapChain1->QueryInterface(IID_PPV_ARGS(&m_pSwapChain));                        //インターフェース継承
    ERROR_DX(hr);
    m_pBackBuffers.resize(scd.BufferCount);                                                 //バックバッファのポインタ配列を確保

    //RTV初期化
    D3D12_DESCRIPTOR_HEAP_DESC dhd{};
    dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    dhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;                                            //shaderから参照なし（SRVやCBVの場合はあり）
    dhd.NumDescriptors = scd.BufferCount;                                                   //スワップチェーンと同じ
    dhd.NodeMask = 0u;
    hr = m_pDevice->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_pHeapsRTV));                 //ディスクリプタヒープ作成
    ERROR_DX(hr);
    D3D12_CPU_DESCRIPTOR_HANDLE cdh = m_pHeapsRTV->GetCPUDescriptorHandleForHeapStart();            //RTVヒープのポインタ
    m_rtvIncSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);     //RTVヒープのインクリメントサイズ取得
    for (size_t i = 0; i < scd.BufferCount; i++) {
        hr = m_pSwapChain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&m_pBackBuffers[i]));       //ポインタ取得
        ERROR_DX(hr);

        //RTV作成(スワップチェーンと関連付け)
        m_pDevice->CreateRenderTargetView(m_pBackBuffers[i].Get(),
            nullptr,                                                //ミップマップ設定
            cdh);

        //次のヒープ
        cdh.ptr += m_rtvIncSize;
    }

    //深度バッファ作成
    D3D12_HEAP_PROPERTIES hpDsv{};                              //頂点ヒープ
    hpDsv.Type = D3D12_HEAP_TYPE_DEFAULT;                       //ヒープ種別
    hpDsv.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;    //CPUのページング設定
    hpDsv.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;     //メモリプール設定(CPU)
    hpDsv.CreationNodeMask = 0u;
    hpDsv.VisibleNodeMask = 0u;                                 //アダプター関連
    D3D12_RESOURCE_DESC rdDsv{};                                //リソース設定
    rdDsv.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;       //バッファかテクスチャか
    rdDsv.Alignment = 0;                                        //アラインメント
    rdDsv.Width = scd.Width;                                    //データサイズ
    rdDsv.Height = scd.Height;
    rdDsv.DepthOrArraySize = 1;                                 //配列要素数
    rdDsv.MipLevels = 1;                                        //ミップマップ設定
    rdDsv.Format = DXGI_FORMAT_D32_FLOAT;                       //深度値用フォーマット(ステンシル値不要の場合)
    rdDsv.SampleDesc.Count = 1u;
    rdDsv.SampleDesc.Quality = 0u;                              //マルチサンプリング設定
    rdDsv.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    rdDsv.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;      //(?)
    D3D12_CLEAR_VALUE cv{};
    cv.Format = rdDsv.Format;
    cv.DepthStencil.Depth = 1.0f;                                           //深さの最大値
    hr = m_pDevice->CreateCommittedResource(&hpDsv, D3D12_HEAP_FLAG_NONE,   //指定なし
        &rdDsv, D3D12_RESOURCE_STATE_DEPTH_WRITE,                           //深度値用
        &cv,                                                                //クリア値
        IID_PPV_ARGS(&m_pDepthBuffer));
    ERROR_DX(hr);

    //DSV作成
    D3D12_DESCRIPTOR_HEAP_DESC dhdDSV{};
    dhdDSV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dhdDSV.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dhdDSV.NumDescriptors = 1u;
    dhdDSV.NodeMask = 0u;
    hr = m_pDevice->CreateDescriptorHeap(&dhdDSV, IID_PPV_ARGS(&m_pHeapDSV));   //ディスクリプタヒープ作成
    ERROR_DX(hr);
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvd{};
    dsvd.Format = rdDsv.Format;
    dsvd.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dsvd.Flags = D3D12_DSV_FLAG_NONE;
    m_pDevice->CreateDepthStencilView(m_pDepthBuffer.Get(), &dsvd, m_pHeapDSV->GetCPUDescriptorHandleForHeapStart());

    //フェンス作成
    hr = m_pDevice->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
    ERROR_DX(hr);
    m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);         //イベントハンドル取得

    //リソースバリア作成(状態遷移)
    m_resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    m_resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;         //指定なし
    m_resourceBarrier.Transition.Subresource = 0u;                      //配列番号



    //=============================================================================

    //BindVertexBuffer();
    //BindIndexBuffer();
    //BindVS();
    //BindPS();
    //BindLayout();
    //BindRootSignature();
    //BindPipelineState();
    //BindBufferHeaps();
    //BindTextureBuffer();
    //BindConstBuffer();

    //ビューポート設定（表示サイズ）
    m_viewport.Width = 1600.0f;;
    m_viewport.Height = 900.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.MinDepth = 0.0f;

    //シザー矩形設定（切り抜き範囲）
    m_scissorRect.top = 0;
    m_scissorRect.left = 0;
    m_scissorRect.right = 1600;
    m_scissorRect.bottom = 900;

    //=============================================================================



#ifdef IMGUI

    //IMGUI初期化
    if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()))
        throw ERROR_EX2(S_OK, "IMGUI初期化失敗");

    //フラグ初期化
    m_bDrawImGui = true;

#endif // IMGUI

}

GfxDX12::~GfxDX12() noexcept
{
    //フェンス用イベント終了
    CloseHandle(m_fenceEvent);  //ハンドルを閉じる

#ifdef IMGUI

    //IMGUI終了
    ImGui_ImplDX11_Shutdown();

#endif // IMGUI

}

//フレーム開始
void GfxDX12::BeginFrame(float r, float g, float b) noexcept
{
    //リソースバリア更新
    UINT bufferIdx = m_pSwapChain->GetCurrentBackBufferIndex();                             //現在のバックバッファのインデックス
    m_resourceBarrier.Transition.pResource = m_pBackBuffers[bufferIdx].Get();
    m_resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    m_resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_pCmdList->ResourceBarrier(1u, &m_resourceBarrier);

    //バッファクリア
    D3D12_CPU_DESCRIPTOR_HANDLE cdh = m_pHeapsRTV->GetCPUDescriptorHandleForHeapStart();
    cdh.ptr += m_rtvIncSize * static_cast<unsigned long long>(bufferIdx);                   //次フレームのバックバッファのRTVヒープ用ポインタ
    D3D12_CPU_DESCRIPTOR_HANDLE cdhDSV = m_pHeapDSV->GetCPUDescriptorHandleForHeapStart();

    m_pCmdList->OMSetRenderTargets(1u, &cdh,
        false,                                  //マルチRT設定
        &cdhDSV);                               //DSVのハンドル
    const float color[] = { r, g, b, 1.0f };                            //背景色
    m_pCmdList->ClearRenderTargetView(cdh, color, 0u, nullptr);         //第3, 4引数⇒クリア範囲（全範囲）
    m_pCmdList->ClearDepthStencilView(cdhDSV, D3D12_CLEAR_FLAG_DEPTH,   //クリア先の指定
        1.0f, 0,                                                        //クリア値
        0u, nullptr);                                                   //クリア範囲

#ifdef IMGUI

    //IMGUI用フレーム書込開始
    if (m_bDrawImGui) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

#endif // IMGUI

}

//描画処理
void GfxDX12::DrawIndexed(UINT indexNum) const noexcept
{
    (void)indexNum;

#ifdef _DEBUG

    PrintD(L"Error : DirectX12ではインスタンシング描画のみできます。\n");

#endif // _DEBUG

}

//インスタンシング描画
void GfxDX12::DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept
{
    static float fAngle = 0.0f;
    fAngle += 0.025f;
    if (fAngle > 3.14f)
        fAngle -= 6.28f;
    else if (fAngle < -3.14f)
        fAngle += 6.28f;

    dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(fAngle, fAngle, 0.0f);
    dx::XMMATRIX mtxV = dx::XMMatrixLookAtLH(
        dx::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    dx::XMMATRIX mtxP = dx::XMMatrixPerspectiveFovLH(6.28f / 360.0f * 60.0f, 16.0f / 9.0f, 0.5f, 40.0f);
    //m_aMatrix[0] = dx::XMMatrixTranspose(mtxW * mtxV * mtxP);

    //データマップ
    dx::XMMATRIX* pMtxMap = nullptr;
    m_pConstBuffer->Map(0u,             //ミップマップ設定
        nullptr,                        //範囲指定⇒nullptr(全範囲)
        (void**)&pMtxMap);
    std::copy(std::begin(m_aMatrix), std::end(m_aMatrix), pMtxMap);
    m_pConstBuffer->Unmap(0u, nullptr);



    //バインド処理
    m_pCmdList->SetDescriptorHeaps(1u, m_pBufferHeaps.GetAddressOf());                              //ディスクリプタヒープ指定
    //m_pCmdList->SetGraphicsRootSignature(m_pRootSignature.Get());
    D3D12_GPU_DESCRIPTOR_HANDLE gdhBuffer = m_pBufferHeaps->GetGPUDescriptorHandleForHeapStart();   //ルートパラメータと関連付け
    m_pCmdList->SetGraphicsRootDescriptorTable(0u, gdhBuffer);
    gdhBuffer.ptr += m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_pCmdList->SetGraphicsRootDescriptorTable(1u, gdhBuffer);

    //m_pCmdList->SetPipelineState(m_pGfxPipelineState.Get());
    //m_pCmdList->IASetVertexBuffers(0u, 1u, &m_viewVB);
    //m_pCmdList->IASetIndexBuffer(&m_viewIB);
    //m_pCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pCmdList->RSSetViewports(1u, &m_viewport);
    m_pCmdList->RSSetScissorRects(1u, &m_scissorRect);

    //書込み処理
    m_pCmdList->DrawIndexedInstanced(indexNum, instanceNum, 0u, 0u, 0u);
}

//フレーム終了
void GfxDX12::EndFrame()
{

#ifdef IMGUI

    //IMGUI用フレーム書込終了
    if (m_bDrawImGui) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

#endif // IMGUI

    //エラーハンドル
    HRESULT hr{};

    //リソースバリア更新
    m_resourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_resourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_pCmdList->ResourceBarrier(1u, &m_resourceBarrier);

    //命令実行
    hr = m_pCmdList->Close();   //命令リストをクローズ
    ERROR_DX(hr);
    ID3D12CommandList* pCmdLists[] = { m_pCmdList.Get() };
    m_pCmdQueue->ExecuteCommandLists(1u, pCmdLists);

    //画面スワップ
    hr = m_pSwapChain->Present(1u, 0u);
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
        throw ERROR_EX(m_pDevice->GetDeviceRemovedReason());
    else
        ERROR_DX(hr);

    //フェンス更新
    hr = m_pCmdQueue->Signal(m_pFence.Get(), ++m_fenceVal);
    ERROR_DX(hr);
    if (m_pFence->GetCompletedValue() < m_fenceVal) {
        hr = m_pFence->SetEventOnCompletion(m_fenceVal, m_fenceEvent);
        ERROR_DX(hr);
        WaitForSingleObject(m_fenceEvent, INFINITE);            //命令実行終了まで待機
    }

    //命令リセット
    hr = m_pCmdAllocator->Reset();                              //CMDアロケータをリセット
    ERROR_DX(hr);
    hr = m_pCmdList->Reset(m_pCmdAllocator.Get(), nullptr);     //CMDリスクをリセット
    ERROR_DX(hr);
}

void GfxDX12::BindVertexBuffer()
{
    //Vertex vtx[]{
    //    {{ -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f} },
    //    {{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f} },
    //    {{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f} },
    //    {{  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f} }
    //};
    std::vector<Vertex> vtx = MakeBox();

    //エラーハンドル
    HRESULT hr{};

    //頂点バッファ設定
    D3D12_HEAP_PROPERTIES hpVtx{};                              //頂点ヒープ
    hpVtx.Type = D3D12_HEAP_TYPE_UPLOAD;                        //ヒープ種別
    hpVtx.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;    //CPUのページング設定
    hpVtx.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;     //メモリプール設定
    hpVtx.CreationNodeMask = 0u;
    hpVtx.VisibleNodeMask = 0u;                                 //アダプター関連
    D3D12_RESOURCE_DESC rdVtx{};                                //リソース設定
    rdVtx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;          //バッファかテクスチャか
    rdVtx.Alignment = 0;                                        //アラインメント
    rdVtx.Width = sizeof(Vertex) * vtx.size();                  //データサイズ
    rdVtx.Height = 1u;                                          //テクスチャじゃないので⇒1
    rdVtx.DepthOrArraySize = 1;                                 //配列要素数
    rdVtx.MipLevels = 1;                                        //ミップマップ設定
    rdVtx.Format = DXGI_FORMAT_UNKNOWN;                         //テクスチャフォーマット
    rdVtx.SampleDesc.Count = 1u;
    rdVtx.SampleDesc.Quality = 0u;                              //マルチサンプリング設定
    rdVtx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;              //テクスチャの場合⇒D3D12_TEXTURE_LAYOUT_UNKNOWN
    rdVtx.Flags = D3D12_RESOURCE_FLAG_NONE;                     //(?)

    //頂点バッファ作成
    hr = m_pDevice->CreateCommittedResource(&hpVtx, D3D12_HEAP_FLAG_NONE,   //指定なし
        &rdVtx, D3D12_RESOURCE_STATE_GENERIC_READ,                          //読み取り専用
        nullptr, IID_PPV_ARGS(&m_pVertexBuffer));
    ERROR_DX(hr);

    //データマップ
    Vertex* pVtxMap = nullptr;
    hr = m_pVertexBuffer->Map(0u,   //ミップマップ設定
        nullptr,                    //範囲指定⇒nullptr(全範囲)
        (void**)&pVtxMap);
    ERROR_DX(hr);
    std::copy(std::begin(vtx), std::end(vtx), pVtxMap);
    m_pVertexBuffer->Unmap(0u, nullptr);

    //VBV作成
    m_viewVB.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
    m_viewVB.SizeInBytes = static_cast<UINT>(rdVtx.Width);
    m_viewVB.StrideInBytes = sizeof(Vertex);
}

void GfxDX12::BindIndexBuffer()
{
    //unsigned short idx[]{
    //    0, 1, 2,
    //    2, 1, 3
    //};
    unsigned short idx[]{
         0,  2,  1,         2,  3,  1,
         4,  6,  5,         6,  7,  5,
         8, 10,  9,        10, 11,  9,
        12, 14, 13,        14, 15, 13,
        16, 18, 17,        18, 19, 17,
        20, 22, 21,        22, 23, 21
    };

    //エラーハンドル
    HRESULT hr{};

    //インデックスバッファ設定
    D3D12_HEAP_PROPERTIES hpIdx{};                              //インデックスヒープ
    hpIdx.Type = D3D12_HEAP_TYPE_UPLOAD;                        //ヒープ種別
    hpIdx.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;    //CPUのページング設定
    hpIdx.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;     //メモリプール設定
    hpIdx.CreationNodeMask = 0u;
    hpIdx.VisibleNodeMask = 0u;                                 //アダプター関連
    D3D12_RESOURCE_DESC rdIdx{};                                //リソース設定
    rdIdx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;          //バッファかテクスチャか
    rdIdx.Alignment = 0;                                        //アラインメント
    rdIdx.Width = sizeof(idx);                                  //データサイズ
    rdIdx.Height = 1u;                                          //テクスチャじゃないので⇒1
    rdIdx.DepthOrArraySize = 1;                                 //配列要素数
    rdIdx.MipLevels = 1;                                        //ミップマップ設定
    rdIdx.Format = DXGI_FORMAT_UNKNOWN;                         //テクスチャフォーマット
    rdIdx.SampleDesc.Count = 1u;
    rdIdx.SampleDesc.Quality = 0u;                              //マルチサンプリング設定
    rdIdx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;              //テクスチャの場合⇒D3D12_TEXTURE_LAYOUT_UNKNOWN
    rdIdx.Flags = D3D12_RESOURCE_FLAG_NONE;                     //(?)

    //インデックスバッファ作成
    hr = m_pDevice->CreateCommittedResource(&hpIdx, D3D12_HEAP_FLAG_NONE, &rdIdx, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pIndexBuffer));
    ERROR_DX(hr);

    //データマップ
    unsigned short* pIdxMap = nullptr;
    hr = m_pIndexBuffer->Map(0u, nullptr, (void**)&pIdxMap);
    ERROR_DX(hr);
    std::copy(std::begin(idx), std::end(idx), pIdxMap);
    m_pIndexBuffer->Unmap(0u, nullptr);

    //IBV作成
    m_viewIB.BufferLocation = m_pIndexBuffer->GetGPUVirtualAddress();
    m_viewIB.SizeInBytes = static_cast<UINT>(rdIdx.Width);
    m_viewIB.Format = DXGI_FORMAT_R16_UINT;
}

void GfxDX12::BindVS()
{
    //エラーハンドル
    HRESULT hr{};

    //頂点シェーダ作成
    wrl::ComPtr<ID3DBlob> pBlobErrorVS;
    hr = D3DCompileFromFile(L"Source/Shader/VS_Instance_Phong.hlsl", nullptr,   //シェーダマクロオブジェクト
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                                      //インクルード設定
        "main", "vs_5_0",                                                       //エントリーポイントとShaderModel
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,                        //シェーダコンパイルオプション
        0,                                                                      //エフェクトコンパイルオプション
        &m_pBlobVS, &pBlobErrorVS);                                             //データ受取先とエラー用ポインタ
    if (hr != S_OK) {                                                           //エラーチェック
        if (pBlobErrorVS.Get() != nullptr) {
            std::string strError;
            strError.resize(pBlobErrorVS->GetBufferSize());
            std::copy(static_cast<char*>(pBlobErrorVS->GetBufferPointer()), static_cast<char*>(pBlobErrorVS->GetBufferPointer()) + pBlobErrorVS->GetBufferSize(), strError.data());
            strError = "【VSシェーダ読込エラー】 " + strError;
            throw ERROR_EX2(S_OK, strError.c_str());
        }
        else
            ERROR_DX(hr);
    }
}

void GfxDX12::BindPS()
{
    //エラーハンドル
    HRESULT hr{};

    //ピクセルシェーダ作成
    wrl::ComPtr<ID3DBlob> pBlobErrorPS;
    hr = D3DCompileFromFile(L"Source/Shader/PS_Phong_NoTex.hlsl", nullptr,  //シェーダマクロオブジェクト
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                                  //インクルード設定
        "main", "ps_5_0",                                                   //エントリーポイントとShaderModel
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,                    //シェーダコンパイルオプション
        0,                                                                  //エフェクトコンパイルオプション
        &m_pBlobPS, &pBlobErrorPS);                                         //データ受取先とエラー用ポインタ
    if (hr != S_OK) {                                                       //エラーチェック
        if (pBlobErrorPS.Get() != nullptr) {
            std::string strError;
            strError.resize(pBlobErrorPS->GetBufferSize());
            std::copy(static_cast<char*>(pBlobErrorPS->GetBufferPointer()), static_cast<char*>(pBlobErrorPS->GetBufferPointer()) + pBlobErrorPS->GetBufferSize(), strError.data());
            strError = "【VSシェーダ読込エラー】 " + strError;
            throw ERROR_EX2(S_OK, strError.c_str());
        }
        else
            ERROR_DX(hr);
    }
}

void GfxDX12::BindLayout()
{
    //頂点レイアウト作成
    m_pLayouts.push_back({ "POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u,                           0u, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0u });
    m_pLayouts.push_back({ "TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT,    0u, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0u });
}

void GfxDX12::BindRootSignature()
{
    //エラーハンドル
    HRESULT hr{};

    //バージョン確認
    D3D12_FEATURE_DATA_ROOT_SIGNATURE fdrs{};
    fdrs.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
    hr = m_pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &fdrs, sizeof(fdrs));
    if (hr != S_OK) {
        fdrs.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        hr = m_pDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &fdrs, sizeof(fdrs));
        ERROR_DX(hr);
    }
    D3D12_VERSIONED_ROOT_SIGNATURE_DESC vrsd{};
    vrsd.Version = fdrs.HighestVersion;

    //ルートパラメータ設定
    D3D12_DESCRIPTOR_RANGE1 dr1[2]{};
    D3D12_ROOT_PARAMETER1 rp1[2]{};
    dr1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    dr1[0].NumDescriptors = 1u;
    dr1[0].BaseShaderRegister = 0u;     //最初の入力スロット
    dr1[0].RegisterSpace = 0u;          //合わせるためのスペース(?)
    //dr1[0].Flags;
    dr1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    dr1[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    dr1[1].NumDescriptors = 1u;
    dr1[1].BaseShaderRegister = 0u;
    dr1[1].RegisterSpace = 0u;
    dr1[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    rp1[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;  //タイプ
    rp1[0].DescriptorTable.pDescriptorRanges = &dr1[0];                 //ディスクリプタレンジのポインタ
    rp1[0].DescriptorTable.NumDescriptorRanges = 1u;                    //ディスクリプタレンジ数
    rp1[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;            //PSシェーダ利用許可
    rp1[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;  //タイプ
    rp1[1].DescriptorTable.pDescriptorRanges = &dr1[1];                 //ディスクリプタレンジのポインタ
    rp1[1].DescriptorTable.NumDescriptorRanges = 1u;                    //ディスクリプタレンジ数
    rp1[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;           //VSシェーダ利用許可

    D3D12_DESCRIPTOR_RANGE dr[2]{};
    D3D12_ROOT_PARAMETER rp[2]{};
    dr[0].RangeType = dr1[0].RangeType;
    dr[0].NumDescriptors = dr1[0].NumDescriptors;
    dr[0].BaseShaderRegister = dr1[0].BaseShaderRegister;
    dr[0].RegisterSpace = dr1[0].RegisterSpace;
    dr[0].OffsetInDescriptorsFromTableStart = dr1[0].OffsetInDescriptorsFromTableStart;
    dr[1].RangeType = dr1[1].RangeType;
    dr[1].NumDescriptors = dr1[1].NumDescriptors;
    dr[1].BaseShaderRegister = dr1[1].BaseShaderRegister;
    dr[1].RegisterSpace = dr1[1].RegisterSpace;
    dr[1].OffsetInDescriptorsFromTableStart = dr1[1].OffsetInDescriptorsFromTableStart;

    rp[0].ParameterType = rp1[0].ParameterType;
    rp[0].DescriptorTable.pDescriptorRanges = &dr[0];
    rp[0].DescriptorTable.NumDescriptorRanges = rp1[0].DescriptorTable.NumDescriptorRanges;
    rp[0].ShaderVisibility = rp1[0].ShaderVisibility;
    rp[1].ParameterType = rp1[1].ParameterType;
    rp[1].DescriptorTable.pDescriptorRanges = &dr[1];
    rp[1].DescriptorTable.NumDescriptorRanges = rp1[1].DescriptorTable.NumDescriptorRanges;
    rp[1].ShaderVisibility = rp1[1].ShaderVisibility;

    //サンプラー設定
    D3D12_STATIC_SAMPLER_DESC ssd{};
    ssd.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                //補間方法
    ssd.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;             //アドレッシングモード（繰り返し）
    ssd.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    ssd.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    //ssd.MipLODBias;                                             //計算結果ミップマップレベルからのオフセット(?)
    //ssd.MaxAnisotropy;                                          //異方性フィルタの最大値
    ssd.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;           //比較関数（サンプリングなし）
    ssd.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;   //端の色
    ssd.MinLOD = 0.0f;
    ssd.MaxLOD = D3D12_FLOAT32_MAX;                             //ミップマップの最大値・最小値
    ssd.ShaderRegister = 0u;                                    //入力スロット
    ssd.RegisterSpace = 0u;
    ssd.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;       //PSシェーダ利用許可

    //ルールシグネチャ作成
    wrl::ComPtr<ID3DBlob> pBlobRS;                                                              //バイナリコード作成
    wrl::ComPtr<ID3DBlob> pBlobErrorRS;
    if (vrsd.Version == D3D_ROOT_SIGNATURE_VERSION_1_1) {
        vrsd.Desc_1_1.NumParameters = 2u;                                                       //ルートパラメータ
        vrsd.Desc_1_1.pParameters = rp1;
        vrsd.Desc_1_1.NumStaticSamplers = 1u;                                                   //サンプラー
        vrsd.Desc_1_1.pStaticSamplers = &ssd;
        vrsd.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;     //IA許可
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    else {
        vrsd.Desc_1_0.NumParameters = 2u;
        vrsd.Desc_1_0.pParameters = rp;
        vrsd.Desc_1_0.NumStaticSamplers = 1u;
        vrsd.Desc_1_0.pStaticSamplers = &ssd;
        vrsd.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        hr = D3D12SerializeVersionedRootSignature(&vrsd, &pBlobRS, &pBlobErrorRS);
    }
    if (hr != S_OK) {                                                                           //エラーチェック
        if (pBlobErrorRS.Get() != nullptr) {
            std::string strError;
            strError.resize(pBlobErrorRS->GetBufferSize());
            std::copy(static_cast<char*>(pBlobErrorRS->GetBufferPointer()), static_cast<char*>(pBlobErrorRS->GetBufferPointer()) + pBlobErrorRS->GetBufferSize(), strError.data());
            strError = "【VSシェーダ読込エラー】 " + strError;
            throw ERROR_EX2(S_OK, strError.c_str());
        }
        else
            ERROR_DX(hr);
    }
    hr = m_pDevice->CreateRootSignature(0u, pBlobRS->GetBufferPointer(), pBlobRS->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature));
    ERROR_DX(hr);
}

void GfxDX12::BindPipelineState()
{
    //エラーハンドル
    HRESULT hr{};

    //GFXパイプラインステート設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd{};
    gpsd.pRootSignature = m_pRootSignature.Get();               //ルールシグネチャ
    gpsd.VS.pShaderBytecode = m_pBlobVS->GetBufferPointer();    //頂点シェーダ
    gpsd.VS.BytecodeLength = m_pBlobVS->GetBufferSize();
    gpsd.PS.pShaderBytecode = m_pBlobPS->GetBufferPointer();    //ピクセルシェーダ
    gpsd.PS.BytecodeLength = m_pBlobPS->GetBufferSize();
    //gpsd.DS;                                                  //ドメインシェーダ
    //gpsd.HS;                                                  //ハルシェーダ
    //gpsd.GS;                                                  //ジオメトリシェーダ
    //gpsd.StreamOutput;                                        //ストリーム出力バッファ設定

    gpsd.BlendState.AlphaToCoverageEnable = false;                                          //アルファブレンディング（AA用）
    gpsd.BlendState.IndependentBlendEnable = false;                                         //8個のRTそれぞれ個別にブレンドステートを設定するかどうか
    gpsd.BlendState.RenderTarget[0].BlendEnable = false;                                    //ブレンド演算
    gpsd.BlendState.RenderTarget[0].LogicOpEnable = false;                                  //論理演算
    //gpsd.BlendState.RenderTarget[0].SrcBlend;
    //gpsd.BlendState.RenderTarget[0].DestBlend;
    //gpsd.BlendState.RenderTarget[0].BlendOp;
    //gpsd.BlendState.RenderTarget[0].SrcBlendAlpha;
    //gpsd.BlendState.RenderTarget[0].DestBlendAlpha;
    //gpsd.BlendState.RenderTarget[0].BlendOpAlpha;
    //gpsd.BlendState.RenderTarget[0].LogicOp;
    gpsd.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;   //色チャンネル書込み指定

    gpsd.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;            //サンプルマスク
    gpsd.RasterizerState.MultisampleEnable = false;         //AAなし
    gpsd.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;   //カリングモード
    gpsd.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;  //中身を塗りつぶす
    gpsd.RasterizerState.DepthClipEnable = true;            //深度方向のクリッピング有効

    gpsd.DepthStencilState.DepthEnable = true;                              //深度設定
    gpsd.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;     //書込む
    gpsd.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;          //小さい方を取る
    gpsd.DepthStencilState.StencilEnable = false;                           //ステンシル設定
    gpsd.InputLayout.pInputElementDescs = m_pLayouts.data();                //インプットレイアウト
    gpsd.InputLayout.NumElements = static_cast<UINT>(m_pLayouts.size());
    gpsd.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;     //ストリップ描画時の頂点カット設定
    gpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;    //トポロジー設定

    gpsd.NumRenderTargets = 1u;                         //RT数
    gpsd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;    //RTVフォーマット(RTの数だけ設定)
    gpsd.DSVFormat = DXGI_FORMAT_D32_FLOAT;             //DSVフォーマット
    gpsd.SampleDesc.Count = 1u;                         //AA設定
    gpsd.SampleDesc.Quality = 0u;
    gpsd.NodeMask = 0u;
    //gpsd.CachedPSO;                                   //高速化設定
    //gpsd.Flags;                                       //ツールデバッグ設定

    //GFXパイプラインステート作成
    hr = m_pDevice->CreateGraphicsPipelineState(&gpsd, IID_PPV_ARGS(&m_pGfxPipelineState));
    ERROR_DX(hr);
}

void GfxDX12::BindBufferHeaps()
{
    //エラーハンドル
    HRESULT hr{};

    //ディスクリプタヒープ作成
    D3D12_DESCRIPTOR_HEAP_DESC dhd{};
    dhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    dhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;  //shaderから参照あり（SRVやCBVの場合はあり）
    dhd.NumDescriptors = 2u;                                //バッファの数
    dhd.NodeMask = 0u;
    hr = m_pDevice->CreateDescriptorHeap(&dhd, IID_PPV_ARGS(&m_pBufferHeaps));
    ERROR_DX(hr);
}

void GfxDX12::BindTextureBuffer()
{
    //テクスチャ作成
    struct Tex
    {
        unsigned char R, G, B, A;
    };
    int imageW = 4;
    int imageH = 4;
    std::vector<Tex> texData(imageW * static_cast<long long>(imageH));
    for (auto& d : texData) {
        d.R = rand() % 256;
        d.G = rand() % 256;
        d.B = rand() % 256;
        d.A = 255;
    }

    //エラーハンドル
    HRESULT hr{};

    //テクスチャバッファ設定
    D3D12_HEAP_PROPERTIES hpTex{};                                  //頂点ヒープ
    hpTex.Type = D3D12_HEAP_TYPE_CUSTOM;                            //ヒープ種別
    hpTex.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;     //CPUのページング設定
    hpTex.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;              //メモリプール設定(CPU)
    hpTex.CreationNodeMask = 0u;
    hpTex.VisibleNodeMask = 0u;                                     //アダプター関連
    D3D12_RESOURCE_DESC rdTex{};                                    //リソース設定
    rdTex.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;           //バッファかテクスチャか
    rdTex.Alignment = 0;                                            //アラインメント
    rdTex.Width = imageW;                                           //データサイズ
    rdTex.Height = imageH;
    rdTex.DepthOrArraySize = 1;                                     //配列要素数
    rdTex.MipLevels = 1;                                            //ミップマップ設定
    rdTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                      //テクスチャフォーマット
    rdTex.SampleDesc.Count = 1u;
    rdTex.SampleDesc.Quality = 0u;                                  //マルチサンプリング設定
    rdTex.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    rdTex.Flags = D3D12_RESOURCE_FLAG_NONE;                         //(?)

    //テクスチャバッファ作成
    hr = m_pDevice->CreateCommittedResource(&hpTex, D3D12_HEAP_FLAG_NONE,   //指定なし
        &rdTex, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,                 //テクスチャ用
        nullptr, IID_PPV_ARGS(&m_pTextureBuffer));
    ERROR_DX(hr);

    //データマップ
    hr = m_pTextureBuffer->WriteToSubresource(0u, nullptr,              //書込み領域設定（全部）
        texData.data(), static_cast<UINT>(sizeof(Tex) * rdTex.Width),   //1行当たりのサイズ
        static_cast<UINT>(sizeof(Tex) * texData.size()));               //スライス当たりのサイズ
    ERROR_DX(hr);

    //SRV作成
    D3D12_SHADER_RESOURCE_VIEW_DESC srvd{};
    srvd.Format = rdTex.Format;
    srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;    //データの順序通りに割当て
    srvd.Texture2D.MipLevels = 1u;                                              //ミップマップなし
    m_pDevice->CreateShaderResourceView(m_pTextureBuffer.Get(), &srvd, m_pBufferHeaps->GetCPUDescriptorHandleForHeapStart());
}

void GfxDX12::BindConstBuffer()
{
    //行列作成
    dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
    dx::XMMATRIX mtxV = dx::XMMatrixLookAtLH(
        dx::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        dx::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
    dx::XMMATRIX mtxP = dx::XMMatrixPerspectiveFovLH(6.28f / 360.0f * 60.0f, 16.0f / 9.0f, 0.5f, 40.0f);
    m_aMatrix[0] = mtxW * mtxV * mtxP;

    //エラーハンドル
    HRESULT hr{};

    //定数バッファ設定
    D3D12_HEAP_PROPERTIES hpMtx{};                                      //頂点ヒープ
    hpMtx.Type = D3D12_HEAP_TYPE_UPLOAD;                                //ヒープ種別
    hpMtx.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;            //CPUのページング設定
    hpMtx.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;             //メモリプール設定(CPU)
    hpMtx.CreationNodeMask = 0u;
    hpMtx.VisibleNodeMask = 0u;                                         //アダプター関連
    D3D12_RESOURCE_DESC rdMtx{};                                        //リソース設定
    rdMtx.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;                  //バッファかテクスチャか
    rdMtx.Alignment = 0;                                                //アラインメント
    rdMtx.Width = ALIGN256(sizeof(dx::XMMATRIX) * m_aMatrix.size());    //データサイズ
    rdMtx.Height = 1u;
    rdMtx.DepthOrArraySize = 1;                                         //配列要素数
    rdMtx.MipLevels = 1;                                                //ミップマップ設定
    rdMtx.Format = DXGI_FORMAT_UNKNOWN;                                 //テクスチャフォーマット
    rdMtx.SampleDesc.Count = 1u;
    rdMtx.SampleDesc.Quality = 0u;                                      //マルチサンプリング設定
    rdMtx.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    rdMtx.Flags = D3D12_RESOURCE_FLAG_NONE;                             //(?)

    //定数バッファ作成
    hr = m_pDevice->CreateCommittedResource(&hpMtx, D3D12_HEAP_FLAG_NONE,   //指定なし
        &rdMtx, D3D12_RESOURCE_STATE_GENERIC_READ,                          //読み取り専用
        nullptr, IID_PPV_ARGS(&m_pConstBuffer));
    ERROR_DX(hr);

    //データマップ
    dx::XMMATRIX* pMtxMap = nullptr;
    hr = m_pConstBuffer->Map(0u,        //ミップマップ設定
        nullptr,                        //範囲指定⇒nullptr(全範囲)
        (void**)&pMtxMap);
    ERROR_DX(hr);
    std::copy(std::begin(m_aMatrix), std::end(m_aMatrix), pMtxMap);
    m_pConstBuffer->Unmap(0u, nullptr);

    //CBV作成
    D3D12_CPU_DESCRIPTOR_HANDLE cdhCB = m_pBufferHeaps->GetCPUDescriptorHandleForHeapStart();
    cdhCB.ptr += m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{};
    cbvd.BufferLocation = m_pConstBuffer->GetGPUVirtualAddress();   //バッファアドレス
    cbvd.SizeInBytes = static_cast<UINT>(rdMtx.Width);              //バッファサイズ
    m_pDevice->CreateConstantBufferView(&cbvd, cdhCB);
}

std::vector<GfxDX12::Vertex> GfxDX12::MakeBox()
{
    //長さ定義
    constexpr float size = 1.0f * 0.5f;         //ボックスの辺の長さ(標準1.0f)

    //頂点作成
    std::vector<Vertex> aData(24);
    aData[0].pos  = { -size, -size, -size };
    aData[1].pos  = { size, -size, -size };
    aData[2].pos  = { -size,  size, -size };
    aData[3].pos  = { size,  size, -size };     //near
    aData[4].pos  = { size, -size,  size };
    aData[5].pos  = { -size, -size,  size };
    aData[6].pos  = { size,  size,  size };
    aData[7].pos  = { -size,  size,  size };    //far
    aData[8].pos  = { -size, -size,  size };
    aData[9].pos  = { -size, -size, -size };
    aData[10].pos = { -size,  size,  size };
    aData[11].pos = { -size,  size, -size };    //left
    aData[12].pos = { size, -size, -size };
    aData[13].pos = { size, -size,  size };
    aData[14].pos = { size,  size, -size };
    aData[15].pos = { size,  size,  size };     //right
    aData[16].pos = { -size, -size,  size };
    aData[17].pos = { size, -size,  size };
    aData[18].pos = { -size, -size, -size };
    aData[19].pos = { size, -size, -size };     //bottom
    aData[20].pos = { -size,  size, -size };
    aData[21].pos = { size,  size, -size };
    aData[22].pos = { -size,  size,  size };
    aData[23].pos = { size,  size,  size };     //top
    aData[0].uv  = { 0.0f, 1.0f };
    aData[1].uv  = { 1.0f, 1.0f };
    aData[2].uv  = { 0.0f, 0.0f };
    aData[3].uv  = { 1.0f, 0.0f };              //near
    aData[4].uv  = { 0.0f, 1.0f };
    aData[5].uv  = { 1.0f, 1.0f };
    aData[6].uv  = { 0.0f, 0.0f };
    aData[7].uv  = { 1.0f, 0.0f };              //far
    aData[8].uv  = { 0.0f, 1.0f };
    aData[9].uv  = { 1.0f, 1.0f };
    aData[10].uv = { 0.0f, 0.0f };
    aData[11].uv = { 1.0f, 0.0f };              //left
    aData[12].uv = { 0.0f, 1.0f };
    aData[13].uv = { 1.0f, 1.0f };
    aData[14].uv = { 0.0f, 0.0f };
    aData[15].uv = { 1.0f, 0.0f };              //right
    aData[16].uv = { 0.0f, 1.0f };
    aData[17].uv = { 1.0f, 1.0f };
    aData[18].uv = { 0.0f, 0.0f };
    aData[19].uv = { 1.0f, 0.0f };              //bottom
    aData[20].uv = { 0.0f, 1.0f };
    aData[21].uv = { 1.0f, 1.0f };
    aData[22].uv = { 0.0f, 0.0f };
    aData[23].uv = { 1.0f, 0.0f };              //top

    return aData;
}
