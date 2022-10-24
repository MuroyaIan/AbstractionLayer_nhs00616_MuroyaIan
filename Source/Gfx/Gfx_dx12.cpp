//==============================================================================
// Filename: Gfx_dx12.cpp
// Description: DirectX12の処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Gfx_dx12.h>

#ifdef IMGUI
#
#   include <3rd_Party/ImGui/backends/imgui_impl_win32.h>
#   include <3rd_Party/ImGui/backends/imgui_impl_dx12.h>
#
#endif // IMGUI

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== クラス実装 =====
GfxDX12::GfxDX12(WinWindow& window) : GfxMain(window),
    m_pDevice(), m_pCmdAllocator(), m_pCmdList(), m_pCmdQueue(), m_pSwapChain(),
    m_pBackBuffers(0), m_pHeapsRTV(), m_rtvIncSize(0), m_pDepthBuffer(), m_pHeapDSV(),
    m_pFence(), m_fenceVal(0), m_fenceEvent(), m_resourceBarrier(),
    m_viewport(), m_scissorRect()
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

    //IMGUI用ヒープ生成
    D3D12_DESCRIPTOR_HEAP_DESC dhdImgui{};
    dhdImgui.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    dhdImgui.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    dhdImgui.NumDescriptors = 1u;
    dhdImgui.NodeMask = 0u;
    hr = m_pDevice->CreateDescriptorHeap(&dhdImgui, IID_PPV_ARGS(&m_pHeapImgui));
    ERROR_DX(hr);

    //IMGUI初期化
    if (!ImGui_ImplDX12_Init(m_pDevice.Get(), 1,                //num frames in flight(?)
        DXGI_FORMAT_R8G8B8A8_UNORM, m_pHeapImgui.Get(),            //Imgui用SRVヒープ
        m_pHeapImgui->GetCPUDescriptorHandleForHeapStart(),
        m_pHeapImgui->GetGPUDescriptorHandleForHeapStart())
        )
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
    ImGui_ImplDX12_Shutdown();

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
        ImGui_ImplDX12_NewFrame();
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
    //バインド処理
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
        m_pCmdList->SetDescriptorHeaps(1, m_pHeapImgui.GetAddressOf());
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_pCmdList.Get());
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
