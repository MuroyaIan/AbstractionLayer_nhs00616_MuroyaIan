//==============================================================================
// Filename: Gfx_Topology.cpp
// Description: トポロジー処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Gfx/Binder/Gfx_Topology.h>

//===== クラス実装 =====
GfxTopology::GfxTopology(D3D_PRIMITIVE_TOPOLOGY type) noexcept :
    GfxBinder(), m_type(type)
{
}

GfxTopology::~GfxTopology() noexcept
{
}

//バインド処理
void GfxTopology::Bind(GfxMain& gfx) noexcept
{
    switch (GetApiMode()) {
        case GfxMain::API_MODE::DX_11:
            GetCommand<CmdDX11*>(&gfx)->IASetPrimitiveTopology(m_type);
            break;
        case GfxMain::API_MODE::DX_12:
            GetCommand<CmdDX12*>(&gfx)->IASetPrimitiveTopology(m_type);
            break;
        default:
            break;
    }
}
