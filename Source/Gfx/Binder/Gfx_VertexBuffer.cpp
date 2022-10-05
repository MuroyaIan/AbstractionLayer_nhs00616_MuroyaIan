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
    if (m_pVertexBuffer[static_cast<int>(Type::INSTANCE)].Get() == nullptr) {

        //インスタンシングなし
        const UINT stride = m_aStride[static_cast<int>(Type::VERTEX)];
        const UINT offset = 0u;
        GetContext(gfx)->IASetVertexBuffers(0u, 1u,
            m_pVertexBuffer[static_cast<int>(Type::VERTEX)].GetAddressOf(),
            &stride, &offset);
    }
    else {

        //インスタンシングあり
        ID3D11Buffer* aBufferPtr[2] = { m_pVertexBuffer[static_cast<int>(Type::VERTEX)].Get(),
            m_pVertexBuffer[static_cast<int>(Type::INSTANCE)].Get() };
        const UINT stride[2] = { m_aStride[static_cast<int>(Type::VERTEX)],
            m_aStride[static_cast<int>(Type::INSTANCE)] };
        const UINT offset[2] = { 0u, 0u };
        GetContext(gfx)->IASetVertexBuffers(0u, 2u, aBufferPtr, stride, offset);
    }
}
