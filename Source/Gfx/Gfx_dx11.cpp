//==============================================================================
// Filename: Gfx_dx11.cpp
// Description: DirectX11の処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Gfx_dx11.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

//===== クラス実装 =====
GfxDX11::GfxDX11(WinWindow& window) : GfxMain(window),
m_pDevice(), m_pSwapChain(), m_pContext(), m_pRTView(), m_pDSView()
{
    //ウィンドウ情報取得
    auto [winWidth, winHeight] = m_window.GetSize();

    //エラーハンドル
    HRESULT hr{};

    //FeatureLevel設定
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
    D3D_FEATURE_LEVEL featureLevel{};

    //スワップチェーン設定
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.Width = 0u;
    sd.BufferDesc.Height = 0u;
    sd.BufferDesc.RefreshRate.Numerator = 0u;
    sd.BufferDesc.RefreshRate.Denominator = 0u;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SampleDesc.Count = 1u;
    sd.SampleDesc.Quality = 0u;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1u;
    sd.OutputWindow = m_window.GetHandle();
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0u;

    //デバッグ設定
    UINT createDeviceFlag = 0u;

#ifdef _DEBUG

    createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;

#endif // _DEBUG

    //デバイス・スワップチェーン初期化
    hr = D3D11CreateDeviceAndSwapChain(
        m_pAdapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        createDeviceFlag,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pDevice,
        &featureLevel,
        &m_pContext
    );
    if (hr == E_INVALIDARG) {

        //dx11_1非対応の場合（dx11_0以下を試す）
        hr = D3D11CreateDeviceAndSwapChain(
            m_pAdapter.Get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            nullptr,
            createDeviceFlag,
            nullptr,
            0u,
            D3D11_SDK_VERSION,
            &sd,
            &m_pSwapChain,
            &m_pDevice,
            &featureLevel,
            &m_pContext
        );
    }
    if (featureLevel != D3D_FEATURE_LEVEL_11_1 && featureLevel != D3D_FEATURE_LEVEL_11_0)
        throw ERROR_EX2(S_OK, "GPUはDX11非対応です。");
    ERROR_DX(hr);

    //バックバッファアクセス取得
    wrl::ComPtr<ID3D11Resource> pBackBuffer;
    hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    ERROR_DX(hr);
    hr = m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pRTView);
    ERROR_DX(hr);

    //深度・ステンシルステート作成
    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    wrl::ComPtr<ID3D11DepthStencilState> pDSState;
    hr = m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
    ERROR_DX(hr);

    //ステートをバインド
    m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);



    //深度・ステンシルバッファ作成
    D3D11_TEXTURE2D_DESC descDepth{};
    descDepth.Width = static_cast<UINT>(winWidth);
    descDepth.Height = static_cast<UINT>(winHeight);
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    hr = m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
    ERROR_DX(hr);

    //ビュー作成
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    hr = m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSView);
    ERROR_DX(hr);

    //ビューをバインド
    m_pContext->OMSetRenderTargets(1u, m_pRTView.GetAddressOf(), m_pDSView.Get());



    //ビューポート設定（ラスタライザ）
    D3D11_VIEWPORT vp{};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = static_cast<float>(winWidth);
    vp.Height = static_cast<float>(winHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_pContext->RSSetViewports(1u, &vp);



#ifdef IMGUI

    //IMGUI初期化
    if (!ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get()))
        throw ERROR_EX2(S_OK, "IMGUI初期化失敗");

    //フラグ初期化
    m_bDrawImGui = true;

#endif // IMGUI

}

GfxDX11::~GfxDX11() noexcept
{

#ifdef IMGUI

    //IMGUI終了
    ImGui_ImplDX11_Shutdown();

#endif // IMGUI

}

//フレーム開始
void GfxDX11::BeginFrame(float r, float g, float b) noexcept
{
    //バッファクリア
    const float color[] = { r, g, b, 1.0f };
    m_pContext->ClearRenderTargetView(m_pRTView.Get(), color);
    m_pContext->ClearDepthStencilView(m_pDSView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

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
void GfxDX11::DrawIndexed(UINT indexNum) const noexcept
{
    //書込み処理
    m_pContext->DrawIndexed(indexNum, 0u, 0);
}

//インスタンシング描画
void GfxDX11::DrawInstanced(UINT indexNum, UINT instanceNum) const noexcept
{
    //書込み処理
    m_pContext->DrawIndexedInstanced(indexNum, instanceNum, 0u, 0, 0u);
}

//フレーム終了
void GfxDX11::EndFrame()
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

    //描画命令
    hr = m_pSwapChain->Present(1u, 0u);
    if (hr == DXGI_ERROR_DEVICE_REMOVED)
        throw ERROR_EX(m_pDevice->GetDeviceRemovedReason());
    else
        ERROR_DX(hr);
}
