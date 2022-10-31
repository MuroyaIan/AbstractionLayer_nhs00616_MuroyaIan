
#version 440 core

//入力
in vec2 TexCoord;

//出力
out vec4 FragColor;

//グローバル変数
uniform sampler2D texture1;

//エントリーポイント
void main()
{
    FragColor = texture(texture1, TexCoord);
}
