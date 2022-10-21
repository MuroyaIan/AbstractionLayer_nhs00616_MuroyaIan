//==============================================================================
// Filename: Gfx_VertexBuffer.cpp
// Description: 頂点バッファ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_VertexBuffer.h>

//===== クラス実装 =====
GfxVertexBuffer::~GfxVertexBuffer() noexcept
{
}

//バインド処理
void GfxVertexBuffer::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            BindDX11(gfx);
            break;
        case GfxMain::API_MODE::DX_12:
            BindDX12(gfx);
            break;
        default:
            break;
    }
}

void GfxVertexBuffer::BindDX11(GfxMain& gfx) const noexcept
{
    if (m_pVertexBuffer11[static_cast<int>(Type::INSTANCE)].Get() == nullptr) {

        //インスタンシングなし
        const UINT offset = 0u;
        GetCommand<CmdDX11*>(&gfx)->IASetVertexBuffers(0u, 1u,
            m_pVertexBuffer11[static_cast<int>(Type::VERTEX)].GetAddressOf(),
            m_aStride, &offset);
    }
    else {

        //インスタンシングあり
        ID3D11Buffer* aBufferPtr[2] = { m_pVertexBuffer11[static_cast<int>(Type::VERTEX)].Get(),
            m_pVertexBuffer11[static_cast<int>(Type::INSTANCE)].Get() };
        const UINT offset[2] = { 0u, 0u };
        GetCommand<CmdDX11*>(&gfx)->IASetVertexBuffers(0u, 2u, aBufferPtr, m_aStride, offset);
    }
}

void GfxVertexBuffer::BindDX12(GfxMain& gfx) const noexcept
{
    GetCommand<CmdDX12*>(&gfx)->IASetVertexBuffers(0u, 2u, m_viewVB);
}
