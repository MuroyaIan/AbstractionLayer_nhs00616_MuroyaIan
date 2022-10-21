//==============================================================================
// Filename: Gfx_IndexBuffer.cpp
// Description: インデックスバッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_IndexBuffer.h>

//===== クラス実装 =====
GfxIndexBuffer::GfxIndexBuffer(GfxMain& gfx, const std::vector<unsigned short>& indices) :
    GfxBinder(), m_count(static_cast<UINT>(indices.size())), m_pIndexBuffer11(), m_pIndexBuffer12(), m_viewIB()
{
    //バッファ作成
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            CreateIndexBufferDX11(gfx, indices);
            break;
        case GfxMain::API_MODE::DX_12:
            CreateIndexBufferDX12(gfx, indices);
            break;
        default:
            break;
    }
}

GfxIndexBuffer::~GfxIndexBuffer() noexcept
{
}

//バインド処理
void GfxIndexBuffer::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->IASetIndexBuffer(m_pIndexBuffer11.Get(), DXGI_FORMAT_R16_UINT, 0u);
            break;
        case GfxMain::API_MODE::DX_12:
            GetCommand<CmdDX12*>(&gfx)->IASetIndexBuffer(&m_viewIB);
            break;
        default:
            break;
    }
}

//バッファ作成
void GfxIndexBuffer::CreateIndexBufferDX11(GfxMain& gfx, const std::vector<unsigned short>& indices)
{
    //エラーハンドル
    HRESULT hr{};

    //バッファ作成
    D3D11_BUFFER_DESC ibd{};
    ibd.ByteWidth = static_cast<UINT>(sizeof(unsigned short) * m_count);
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.StructureByteStride = static_cast<UINT>(sizeof(unsigned short));
    D3D11_SUBRESOURCE_DATA isd{};
    isd.pSysMem = indices.data();
    hr = GetDevice<DevDX11*>(&gfx)->CreateBuffer(&ibd, &isd, &m_pIndexBuffer11);
    ERROR_DX(hr);
}

void GfxIndexBuffer::CreateIndexBufferDX12(GfxMain& gfx, const std::vector<unsigned short>& indices)
{
    //エラーハンドル
    HRESULT hr{};

    //インデックスバッファ設定
    D3D12_HEAP_PROPERTIES hp{};                             //インデックスヒープ
    hp.Type = D3D12_HEAP_TYPE_UPLOAD;                       //ヒープ種別
    hp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;   //CPUのページング設定
    hp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;    //メモリプール設定
    hp.CreationNodeMask = 0u;
    hp.VisibleNodeMask = 0u;                                //アダプター関連
    D3D12_RESOURCE_DESC rd{};                               //リソース設定
    rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;         //バッファかテクスチャか
    rd.Alignment = 0;                                       //アラインメント
    rd.Width = sizeof(unsigned short) * indices.size();     //データサイズ
    rd.Height = 1u;                                         //テクスチャじゃないので⇒1
    rd.DepthOrArraySize = 1;                                //配列要素数
    rd.MipLevels = 1;                                       //ミップマップ設定
    rd.Format = DXGI_FORMAT_UNKNOWN;                        //テクスチャフォーマット
    rd.SampleDesc.Count = 1u;
    rd.SampleDesc.Quality = 0u;                             //マルチサンプリング設定
    rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;             //テクスチャの場合⇒D3D12_TEXTURE_LAYOUT_UNKNOWN
    rd.Flags = D3D12_RESOURCE_FLAG_NONE;                    //(?)

    //インデックスバッファ作成
    hr = GetDevice<DevDX12*>(&gfx)->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_pIndexBuffer12));
    ERROR_DX(hr);

    //データマップ
    GfxBinder::UpdateBuffer(indices, m_pIndexBuffer12.Get());

    //IBV作成
    m_viewIB.BufferLocation = m_pIndexBuffer12->GetGPUVirtualAddress();
    m_viewIB.SizeInBytes = static_cast<UINT>(rd.Width);
    m_viewIB.Format = DXGI_FORMAT_R16_UINT;
}
