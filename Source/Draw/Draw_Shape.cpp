//==============================================================================
// Filename: Draw_Shape.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_Shape.h>
#include <Gfx/Binder/Gfx_BinderRef.h>

namespace dx = DirectX;

int SHAPE_MODEL::RotX = 0;
int SHAPE_MODEL::RotY = 0;
int SHAPE_MODEL::RotZ = 0;

//===== クラス実装 =====
SHAPE_MODEL::SHAPE_MODEL(GfxPack& Gfx, GfxVsdMaker::Shape Type) :
    GfxDrawerEx(), m_Gfx(Gfx), m_Type(Type), m_InstanceNum(0), m_aInstanceData(m_InstanceNum), m_Material()
{
    //頂点情報作成
    VsData<VertexM> Model = GfxVsdMaker::MakeData_Model(m_Type);
    dx::XMFLOAT4X4 mtx{};
    dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(10.0f, 10.0f, 10.0f));
    Model.InitSize(mtx);
    AddBind(std::make_unique<GfxVertexBuffer>(m_Gfx.m_dx, Model.m_vertices, m_aInstanceData));

    //インデックス情報作成
    AddBind(std::make_unique<GfxIndexBuffer>(m_Gfx.m_dx, Model.m_indices));

    //定数バッファ作成（マテリアル）
    AddBind(std::make_unique<GfxCBuffMaterial>(m_Gfx.m_dx, m_Material));

    //テクスチャバッファ作成
    if (m_Type == GfxVsdMaker::Shape::BOX) {
        unsigned char color[] = {
            255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
        };
        ToolTexLoader::TexData data;
        data.pImageData = color;
        data.nWidth = 8;
        data.nHeight = 8;
        AddBind(std::make_unique<GfxTexture>(m_Gfx.m_dx, data));
    }

    //マテリアル情報初期化
    m_Material.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_Material.specular = { 0.6f, 0.6f, 0.6f, 1.0f };
    m_Material.shininess = 30.0f;
}

SHAPE_MODEL::~SHAPE_MODEL() noexcept
{
}

//更新処理
void SHAPE_MODEL::Update() noexcept
{
    //ワールド行列更新
    for (int i = 0; i < m_InstanceNum; i++) {
        dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(RotX), gMath::GetRad(RotY), gMath::GetRad(RotZ));
        dx::XMStoreFloat4x4(&m_aInstanceData[i].mtxWorld, mtxW);
    }
}

//書込み処理
void SHAPE_MODEL::Draw(GfxMain& Gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
    //例外処理
    if (m_InstanceNum < 1)
        return;

    //インスタンス更新
    std::vector<GfxInstanceData> aInstData = m_aInstanceData;
    for (auto& i : aInstData)
        gMath::MtxTranspose4x4_SSE(&i.mtxWorld._11);
    GetVertexBuffer().UpdateBuffer(Gfx, aInstData, GfxVertexBuffer::Type::INSTANCE);

    //インスタンス描画
    if (m_Type == GfxVsdMaker::Shape::BOX)
        m_Gfx.m_shaderMgr.Bind_Instance_Phong();
    else
        m_Gfx.m_shaderMgr.Bind_Instance_Phong_NoTex();
    (void)bDrawInstance;
    GfxDrawer::Draw(Gfx, true);
}

//インスタンス追加
int SHAPE_MODEL::AddInstance()
{
    //配列追加
    m_InstanceNum++;
    GfxInstanceData InstData{};
    //InstData.MaterialColor = {
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    1.0f
    //};
    m_aInstanceData.push_back(InstData);

    //インスタンスバッファ再設定
    GetVertexBuffer().ResetInstanceBuffer(m_Gfx.m_dx, m_aInstanceData);
    GetVertexBuffer().UpdateBuffer(m_Gfx.m_dx, m_aInstanceData, GfxVertexBuffer::Type::INSTANCE);

    //インスタンス数更新
    SetInstanceNum(m_InstanceNum);

    return m_InstanceNum - 1;
}
