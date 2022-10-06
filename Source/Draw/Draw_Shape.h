//==============================================================================
// Filename: Draw_Shape.h
// Description: ウィンドウ処理
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

#pragma once

//===== インクルード部 =====
#include <Gfx/Drawer/Gfx_DrawerRef.h>
#include <Gfx/Drawer/Gfx_vsdRef.h>
#include <Win/Win_App.h>

//===== クラス定義 =====

//***** ジオメトリ描画（モデル表示） *****
class SHAPE_MODEL : public GfxDrawerEx<SHAPE_MODEL>
{
public:

    //プロトタイプ宣言
    SHAPE_MODEL(GfxPack& Gfx, GfxVsdMaker::Shape Type);
    ~SHAPE_MODEL() noexcept override;
    void Update() noexcept override;                                                            //更新処理
    void Draw(GfxMain& Gfx, bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;        //書込み処理
    int AddInstance() override;                                                                    //インスタンス追加

    DirectX::XMFLOAT4X4 GetTransformMtx(int InstanceIndex = 0) const noexcept override            //変形行列取得
    {
        return m_aInstanceData[InstanceIndex].mtxWorld;
    }

    UINT GetPolygonNum() const noexcept override                                                //ポリゴン数取得
    {
        return GetIndexNum() / 3 * m_InstanceNum;
    }

    static int RotX;
    static int RotY;
    static int RotZ;

private:

    //変数宣言
    GfxPack& m_Gfx;                                //描画データ参照
    GfxVsdMaker::Shape m_Type;                        //形状タイプ
    int m_InstanceNum;                                //インスタンス数
    std::vector<GfxInstanceData> m_aInstanceData;        //インスタンス情報
    GfxMaterialData m_Material;                        //マテリアル情報
};
