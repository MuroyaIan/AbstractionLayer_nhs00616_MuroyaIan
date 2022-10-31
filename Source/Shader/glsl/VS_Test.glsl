
#version 440 core

//入力
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec2 aOffset;
//layout (location = 3) in mat4 aMtx;

//出力
out vec2 TexCoord;

//変換行列
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//エントリーポイント
void main()
{
    vec4 posW = model * vec4(aPos, 1.0);
    posW.xy += aOffset;
    gl_Position = projection * view * posW;
    TexCoord = aTexCoord;
    TexCoord.y = 1.0f - TexCoord.y;     //DirectXとOpenGLのUV座標が違う為の修正
}
