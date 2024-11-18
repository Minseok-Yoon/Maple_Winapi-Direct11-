#include "CIndexBuffer.h"

CIndexBuffer::CIndexBuffer() :
    m_iIndexCount(0)
{
}

CIndexBuffer::~CIndexBuffer()
{
}

bool CIndexBuffer::Create(const vector<UINT>& _vecIndices)
{
    m_iIndexCount = (UINT)_vecIndices.size();

    desc.ByteWidth = sizeof(UINT) * (UINT)_vecIndices.size();
    desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sub = {};
    sub.pSysMem = _vecIndices.data();

    if (!GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf()))
        assert(NULL && "Indices buffer create fail!");

    return true;
}

void CIndexBuffer::Bind() const
{
    GetDevice()->BindIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
