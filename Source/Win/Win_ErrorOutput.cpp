//==============================================================================
// Filename: Win_ErrorOutput.cpp
// Description: エラー出力
// Copyright (C) 2022 Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

//===== インクルード部 =====
#include <Win/Win_ErrorOutput.h>

//===== クラス実装 =====
WinErrorOutput::WinErrorOutput(int nLine, const char* chFile) noexcept :
    std::exception(), m_line(nLine), m_file(chFile), m_infoBuffer()
{
}

WinErrorOutput::~WinErrorOutput() noexcept
{
}

//エラータイプ取得
const char* WinErrorOutput::GetErrorType() const noexcept
{
    return "（継承用）エラータイプ記入";
}

//エラー情報取得
std::string WinErrorOutput::GetErrorInfo() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << m_file << std::endl
        << "[Line] " << m_line;
    return std::move(oss.str());
}

//エラー出力
const char* WinErrorOutput::what() const noexcept
{
    std::ostringstream oss;
    oss << GetErrorType() << std::endl
        << GetErrorInfo();
    m_infoBuffer = oss.str();
    return m_infoBuffer.c_str();
}
