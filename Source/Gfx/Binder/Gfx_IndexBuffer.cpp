//==============================================================================
// Filename: Gfx_IndexBuffer.cpp
// Description: インデックスバッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_IndexBuffer.h>

//===== クラス実装 =====
GfxIndexBuffer::GfxIndexBuffer(GfxMain& gfx, const std::vector<unsigned short>& indices) :
    GfxBinder(), m_count(static_cast<UINT>(indices.size())), m_pIndexBuffer()
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
    hr = GetDevice(gfx)->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
    ERROR_DX(hr);
}

GfxIndexBuffer::~GfxIndexBuffer() noexcept
{
}

//バインド処理
void GfxIndexBuffer::Bind(GfxMain& gfx) noexcept
{
    GetContext(gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
