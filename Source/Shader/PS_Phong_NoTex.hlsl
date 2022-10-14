//==============================================================================
// Filename: PS_Phong_NoTex.hlsl
// Description: Phong���f���p�s�N�Z���V�F�[�_�i�e�N�X�`���Ȃ��j
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

////���͗p�\����
//struct PS_IN
//{
//    //--------------------------------------------------------------------------
//    float3 posWV : POSITION;
//    float2 tex : TEXCOORD;
//    float3 normalWV : NORMAL;
//    matrix mtxView : MTX_V;
//    //--------------------------------------------------------------------------

//    /// <summary>
//    /// posWV       ���_���W�i�r���[�ϊ���j
//    /// tex         UV���W
//    /// normalWV    ���_�@���i�r���[�ϊ���j
//    /// mtxView     �r���[�s��
//    /// </summary>
//};

////�\���̐錾
//struct LightDirectional     //���s����
//{
//    //--------------------------------------------------------------------------
//    float4 Pos;
//    float4 Color_D;
//    //--------------------------------------------------------------------------

//    /// <summary>
//    /// Pos         ���[���h���W
//    /// Color_D     ���̐F�i�g�U�F, a�͋��x�j
//    /// </summary>
//};

////�萔�o�b�t�@�i���C�g���j
//cbuffer CB_LIGHT : register(b1)
//{
//    LightDirectional DirectionalLight;  //���s����
//    float4 AmbientLight;                //����
//}

////�萔�o�b�t�@�i�}�e���A���j
//cbuffer CB_MATERIAL : register(b2)
//{
//    float4 mAmbient;        //����
//    float4 mDiffuse;        //�g�U���ˌ�
//    float4 mEmissive;       //���ˌ�
//    float4 mTransparent;    //���ߓx
//    float4 mSpecular;       //���ʔ��ˌ�
//    float mShininess;       //����
//    float mPad1;
//    float mPad2;
//    float mPad3;            //���u��
//};

////------------------------------------------------------------------------------
///// ���s�����̌v�Z
/////
///// \param[in] psi          ���͗p�\����
///// \param[in] ModelNormal  ���_�@��
/////
///// \return void
////------------------------------------------------------------------------------
//float3 CalcDirectionalLight(
//    /*[in]*/ PS_IN psi,
//    /*[in]*/ float3 ModelNormal);

////------------------------------------------------------------------------------
///// �G���g���[�|�C���g
/////
///// \param[in] psi  ���͗p�\����
/////
///// \return �F���
////------------------------------------------------------------------------------
//float4 main(PS_IN psi) : SV_Target
//{
//    //�@���v�Z
//    const float3 vNor_Model = normalize(psi.normalWV);

//    //���s�����̌v�Z
//    const float3 Directional = CalcDirectionalLight(psi, vNor_Model);

//    //�O���[�o�������̌v�Z
//    const float3 gAmbient = AmbientLight.rgb * AmbientLight.a;

//    //�ŏI�̏o�͐F�v�Z
//    return float4(saturate(Directional + gAmbient), 1.0f);
//}

////���s�����̌v�Z
//float3 CalcDirectionalLight(PS_IN psi, float3 ModelNormal)
//{
//    //���ւ̒P�ʃx�N�g��
//    const float3 vToLight = mul(DirectionalLight.Pos.xyz, (float3x3) psi.mtxView);
//    const float3 vNor_ToLight = normalize(vToLight);

//    //���s�����̐F
//    const float3 Light = DirectionalLight.Color_D.rgb * DirectionalLight.Color_D.a;

//    //�g�U�F�Z�o
//    const float3 Diffuse = Light * max(0.0f, dot(vNor_ToLight, ModelNormal))
//        * mDiffuse.rgb;

//    //���ʔ��ːF�Z�o
//    const float3 vRef = ModelNormal * dot(vToLight, ModelNormal) * 2.0f - vToLight;
//                                                            //���ʔ��˃x�N�g��
//    const float3 Specular = Light
//        * pow(max(0.0f, dot(normalize(vRef), normalize(-psi.posWV))), mShininess)
//        * mSpecular.rgb;

//    //�����̌v�Z
//    const float3 Ambient = Light * mAmbient.rgb;

//    //�ŏI�̏o�͐F�v�Z
//    return Diffuse + Specular + Ambient;
//}

#include "BaseShader.hlsli"

float4 main(OutputVS input) : SV_Target
{
    return float4(Tex.Sample(Sampler, input.uv));
}
