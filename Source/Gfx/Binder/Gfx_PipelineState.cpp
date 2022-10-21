//==============================================================================
// Filename: Gfx_PipelineState.cpp
// Description: パイプラインステート処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_PipelineState.h>

namespace wrl = Microsoft::WRL;

//===== クラス実装 =====
GfxPipelineState::GfxPipelineState(GfxMain& gfx, ID3DBlob& BytecodeVS, ID3DBlob& BytecodePS, std::vector<D3D12_INPUT_ELEMENT_DESC>& aLayoutDesc, ID3D12RootSignature* pRootSignature) :
    GfxBinder(), m_pGfxPipelineState()
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return;

    //エラーハンドル
    HRESULT hr{};

    //GFXパイプラインステート設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd{};
    gpsd.pRootSignature = pRootSignature;                       //ルールシグネチャ
    gpsd.VS.pShaderBytecode = BytecodeVS.GetBufferPointer();    //頂点シェーダ
    gpsd.VS.BytecodeLength = BytecodeVS.GetBufferSize();
    gpsd.PS.pShaderBytecode = BytecodePS.GetBufferPointer();    //ピクセルシェーダ
    gpsd.PS.BytecodeLength = BytecodePS.GetBufferSize();
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
    gpsd.InputLayout.pInputElementDescs = aLayoutDesc.data();                //インプットレイアウト
    gpsd.InputLayout.NumElements = static_cast<UINT>(aLayoutDesc.size());
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
    hr = GetDevice<DevDX12*>(&gfx)->CreateGraphicsPipelineState(&gpsd, IID_PPV_ARGS(&m_pGfxPipelineState));
    ERROR_DX(hr);
}

GfxPipelineState::~GfxPipelineState() noexcept
{
}

//バインド処理
void GfxPipelineState::Bind(GfxMain& gfx) noexcept
{
    //例外処理
    if (GetApiMode() != GfxMain::API_MODE::DX_12)
        return;

    GetCommand<CmdDX12*>(&gfx)->SetPipelineState(m_pGfxPipelineState.Get());
}
