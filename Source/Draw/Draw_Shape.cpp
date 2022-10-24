//==============================================================================
// Filename: Draw_Shape.cpp
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Draw/Draw_Shape.h>
#include <Gfx/Binder/Gfx_BinderRef.h>
#include <Draw/Draw_LightMgr.h>

namespace dx = DirectX;

int DrawShapeModel::m_rotX = 0;
int DrawShapeModel::m_rotY = 0;
int DrawShapeModel::m_rotZ = 0;

//===== クラス実装 =====
DrawShapeModel::DrawShapeModel(GfxPack& gfx, GfxVsdMaker::Shape type) :
    GfxDrawerEx(), m_gfx(gfx), m_type(type), m_instanceNum(0), m_aInstanceData(m_instanceNum),
    m_material()
{
    //頂点情報作成
    VsData<VertexM> model = GfxVsdMaker::MakeData_Model(m_type);
    dx::XMFLOAT4X4 mtx{};
    dx::XMStoreFloat4x4(&mtx, dx::XMMatrixScaling(10.0f, 10.0f, 10.0f));
    model.InitSize(mtx);
    AddBind(std::make_unique<GfxVertexBuffer>(m_gfx.m_dx, model.m_vertices, m_aInstanceData));

    //インデックス情報作成
    AddBind(std::make_unique<GfxIndexBuffer>(m_gfx.m_dx, model.m_indices));

    //ヒープ設定
    std::unique_ptr<GfxHeapMgr::HeapInfo> pheapInfo;
    if (m_type == GfxVsdMaker::Shape::BOX)
        pheapInfo = std::make_unique<GfxHeapMgr::HeapInfo>(1, 0, 2, 1);
    else
        pheapInfo = std::make_unique<GfxHeapMgr::HeapInfo>(1, 0, 2, 0);


    //CBV登録（カメラ）
    GfxCBuffMtxVP* pCamera = dynamic_cast<GfxCBuffMtxVP*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::CB_VS_MTX_VP).get());
    pCamera->AddViewInfo(pheapInfo.get());

    //CBV登録（ライト）
    GfxPixelCBuffer<DrawLightMgr::LightPack>* pLight = dynamic_cast<GfxPixelCBuffer<DrawLightMgr::LightPack>*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::CB_PS_LIGHT).get());
    pLight->AddViewInfo(pheapInfo.get());

    //定数バッファ作成（マテリアル）
    AddBind(std::make_unique<GfxCBuffMaterial>(m_gfx.m_dx, pheapInfo.get(), m_material));

    //テクスチャバッファ作成
    if (m_type == GfxVsdMaker::Shape::BOX) {
        unsigned char color[] = {
            255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 255, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,
            255, 000, 255, 255,     255, 000, 255, 255,     255, 000, 255, 255,     255, 255, 255, 255,
        };
        ToolTexLoader::TexData data;
        data.pImageData = color;
        data.nWidth = 8;
        data.nHeight = 8;
        AddBind(std::make_unique<GfxTexture>(m_gfx.m_dx, data, pheapInfo.get(), -1, 0));
    }

    //ディスクリプタヒープ作成
    GfxHeapMgr* pHeapMgr = nullptr;
    AddBind(std::make_unique<GfxHeapMgr>(m_gfx.m_dx, *pheapInfo, &pHeapMgr));

    //ルートシグネチャ初期化
    GfxSampler* pSampler = dynamic_cast<GfxSampler*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::SAMPLER).get());
    GfxRootSignature* pRS = nullptr;
    AddBind(std::make_unique<GfxRootSignature>(m_gfx.m_dx, *pheapInfo, *pSampler, *pHeapMgr, &pRS));

    //パイプラインステート初期化
    GfxVertexShader* pVS = dynamic_cast<GfxVertexShader*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::VS_INSTANCE_PHONG).get());
    GfxPixelShader* pPS = nullptr;
    if (m_type == GfxVsdMaker::Shape::BOX)
        pPS = dynamic_cast<GfxPixelShader*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::PS_PHONG).get());
    else
        pPS = dynamic_cast<GfxPixelShader*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::PS_PHONG_NO_TEX).get());
    GfxInputLayout* pLayout = dynamic_cast<GfxInputLayout*>(m_gfx.m_shaderMgr.GetBind(DrawShaderMgr::BinderID::IL_INSTANCE_PHONG).get());
    AddBind(std::make_unique<GfxPipelineState>(m_gfx.m_dx, *pVS->GetBytecode(), *pPS->GetBytecode(), pLayout->GetLayout(), pRS->GetRootSignature()));



    //マテリアル情報初期化
    m_material.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
    m_material.specular = { 0.6f, 0.6f, 0.6f, 1.0f };
    m_material.shininess = 30.0f;
}

DrawShapeModel::~DrawShapeModel() noexcept
{
}

//更新処理
void DrawShapeModel::Update() noexcept
{
    //ワールド行列更新
    for (int i = 0; i < m_instanceNum; i++) {
        dx::XMMATRIX mtxW = dx::XMMatrixRotationRollPitchYaw(gMath::GetRad(m_rotX),
            gMath::GetRad(m_rotY), gMath::GetRad(m_rotZ));
        dx::XMStoreFloat4x4(&m_aInstanceData[i].mtxWorld, mtxW);
    }
}

//書込み処理
void DrawShapeModel::Draw(GfxMain& gfx, bool bDrawInstance) const noexcept(!IS_DEBUG)
{
    //例外処理
    if (m_instanceNum < 1)
        return;

    //インスタンス更新
    std::vector<GfxInstanceData> aInstData = m_aInstanceData;
    for (auto& i : aInstData)
        gMath::MtxTranspose4x4_SSE(&i.mtxWorld._11);
    GetVertexBuffer().UpdateBuffer(gfx, aInstData, GfxVertexBuffer::Type::INSTANCE);

    //インスタンス描画
    if (m_type == GfxVsdMaker::Shape::BOX)
        m_gfx.m_shaderMgr.Bind_Instance_Phong();
    else
        m_gfx.m_shaderMgr.Bind_Instance_Phong_NoTex();
    (void)bDrawInstance;
    GfxDrawer::Draw(gfx, true);
}

//インスタンス追加
int DrawShapeModel::AddInstance()
{
    //配列追加
    m_instanceNum++;
    GfxInstanceData instData{};
    //InstData.MaterialColor = {
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    RAND_MAKER::MakeRand_Float(0.0f, 1.0f),
    //    1.0f
    //};
    m_aInstanceData.push_back(instData);

    //インスタンスバッファ再設定
    GetVertexBuffer().ResetInstanceBuffer(m_gfx.m_dx, m_aInstanceData);
    GetVertexBuffer().UpdateBuffer(m_gfx.m_dx, m_aInstanceData,
        GfxVertexBuffer::Type::INSTANCE);

    //インスタンス数更新
    SetInstanceNum(m_instanceNum);

    return m_instanceNum - 1;
}
