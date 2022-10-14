//==============================================================================
// Filename: PS_Phong_NoTex.hlsl
// Description: Phongモデル用ピクセルシェーダ（テクスチャなし）
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

////入力用構造体
//struct PS_IN
//{
//    //--------------------------------------------------------------------------
//    float3 posWV : POSITION;
//    float2 tex : TEXCOORD;
//    float3 normalWV : NORMAL;
//    matrix mtxView : MTX_V;
//    //--------------------------------------------------------------------------

//    /// <summary>
//    /// posWV       頂点座標（ビュー変換後）
//    /// tex         UV座標
//    /// normalWV    頂点法線（ビュー変換後）
//    /// mtxView     ビュー行列
//    /// </summary>
//};

////構造体宣言
//struct LightDirectional     //平行光源
//{
//    //--------------------------------------------------------------------------
//    float4 Pos;
//    float4 Color_D;
//    //--------------------------------------------------------------------------

//    /// <summary>
//    /// Pos         ワールド座標
//    /// Color_D     光の色（拡散色, aは強度）
//    /// </summary>
//};

////定数バッファ（ライト情報）
//cbuffer CB_LIGHT : register(b1)
//{
//    LightDirectional DirectionalLight;  //平行光源
//    float4 AmbientLight;                //環境光
//}

////定数バッファ（マテリアル）
//cbuffer CB_MATERIAL : register(b2)
//{
//    float4 mAmbient;        //環境光
//    float4 mDiffuse;        //拡散反射光
//    float4 mEmissive;       //発射光
//    float4 mTransparent;    //透過度
//    float4 mSpecular;       //鏡面反射光
//    float mShininess;       //光沢
//    float mPad1;
//    float mPad2;
//    float mPad3;            //仮置き
//};

////------------------------------------------------------------------------------
///// 平行光源の計算
/////
///// \param[in] psi          入力用構造体
///// \param[in] ModelNormal  頂点法線
/////
///// \return void
////------------------------------------------------------------------------------
//float3 CalcDirectionalLight(
//    /*[in]*/ PS_IN psi,
//    /*[in]*/ float3 ModelNormal);

////------------------------------------------------------------------------------
///// エントリーポイント
/////
///// \param[in] psi  入力用構造体
/////
///// \return 色情報
////------------------------------------------------------------------------------
//float4 main(PS_IN psi) : SV_Target
//{
//    //法線計算
//    const float3 vNor_Model = normalize(psi.normalWV);

//    //平行光源の計算
//    const float3 Directional = CalcDirectionalLight(psi, vNor_Model);

//    //グローバル環境光の計算
//    const float3 gAmbient = AmbientLight.rgb * AmbientLight.a;

//    //最終の出力色計算
//    return float4(saturate(Directional + gAmbient), 1.0f);
//}

////平行光源の計算
//float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal)
//{
//    //光への単位ベクトル
//    const float3 vToLight = mul(DirectionalLight.Pos.xyz, (float3x3) psi.mtxView);
//    const float3 vNor_ToLight = normalize(vToLight);

//    //平行光源の色
//    const float3 Light = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;

//    //拡散色算出
//    const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal))
//        * mDiffuse.rgb;

//    //鏡面反射色算出
//    const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;
//                                                            //鏡面反射ベクトル
//    const float3 Specular = Light
//        * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess)
//        * mSpecular.rgb;

//    //環境光の計算
//    const float3 Ambient = Light * mAmbient.rgb;

//    //最終の出力色計算
//    return Diffuse + Specular + Ambient;
//}

#include "BaseShader.hlsli"

float4 main(OutputVS input) : SV_Target
{
    return float4(Tex.Sample(Sampler, input.uv));
}
