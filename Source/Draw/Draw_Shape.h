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

#if DX_11

//***** ジオメトリ描画（モデル表示） *****
class DrawShapeModel : public GfxDrawerEx<DrawShapeModel>
{
public:

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    /// コンストラクタ
    ///
    /// \param[in] gfx      グラフィック処理の参照先
    /// \param[in] type     モデルの形状タイプ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    DrawShapeModel(
        /*[in]*/ GfxPack& gfx,
        /*[in]*/ GfxVsdMaker::Shape type);

    //--------------------------------------------------------------------------
    /// デストラクタ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    ~DrawShapeModel() noexcept override;

    //--------------------------------------------------------------------------
    /// 更新処理
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Update() noexcept override;

    //--------------------------------------------------------------------------
    /// 描画処理
    ///
    /// \param[in] gfx              グラフィック処理の参照先
    /// \param[in] bDrawInstance    インスタンス描画フラグ
    ///
    /// \return void
    //--------------------------------------------------------------------------
    void Draw(
        /*[in]*/ GfxMain& gfx,
        /*[in]*/ bool bDrawInstance = false) const noexcept(!IS_DEBUG) override;

    //--------------------------------------------------------------------------
    /// インスタンス追加
    ///
    /// \return インスタンス番号
    //--------------------------------------------------------------------------
    int AddInstance() override;

    //--------------------------------------------------------------------------
    /// 変形行列取得
    ///
    /// \param[in] instanceIndex    変形行列の取得先インスタンス番号
    ///
    /// \return 変形行列
    //--------------------------------------------------------------------------
    DirectX::XMFLOAT4X4 GetTransformMtx(
        /*[in]*/ int instanceIndex = 0) const noexcept override
    {
        return m_aInstanceData[instanceIndex].mtxWorld;
    }

    //--------------------------------------------------------------------------
    /// ポリゴン数取得
    ///
    /// \return ポリゴン数
    //--------------------------------------------------------------------------
    UINT GetPolygonNum() const noexcept override
    {
        return GetIndexNum() / 3 * m_instanceNum;
    }

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    static int m_rotX;
    static int m_rotY;
    static int m_rotZ;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_rotX  モデル回転量(x軸)
    /// m_rotY  モデル回転量(y軸)
    /// m_rotZ  モデル回転量(z軸)
    /// </summary>

private:

    //--------------------------------------------------------------------------
    GfxPack& m_gfx;
    GfxVsdMaker::Shape m_type;
    int m_instanceNum;
    std::vector<GfxInstanceData> m_aInstanceData;
    GfxMaterialData m_material;
    //--------------------------------------------------------------------------

    /// <summary>
    /// m_gfx               描画データ参照
    /// m_type              形状タイプ
    /// m_instanceNum       インスタンス数
    /// m_aInstanceData     インスタンス情報
    /// m_material          マテリアル情報
    /// </summary>
};

#elif DX_12


#endif // GFX
