#include "..\Public\VIBuffer.h"
#include "GameInstance.h"
#include "Mesh.h"

CVIBuffer::CVIBuffer(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext)
	: CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_BufferDesc(rhs.m_BufferDesc)
	, m_SubResourceData(rhs.m_SubResourceData)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_iNumIndices(rhs.m_iNumIndices)
	, m_iIndexStride(rhs.m_iIndexStride)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	//추가
	, m_vecVertexInfo(rhs.m_vecVertexInfo)
	, m_vecIndexInfo(rhs.m_vecIndexInfo)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);

}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (m_pContext != nullptr)
		m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

void CVIBuffer::Update(_cref_time fTimeDelta)
{
}

HRESULT CVIBuffer::Bind_VIBuffers()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		RETURN_EFAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
	};

	_uint				iStrides[] = {
		m_iStride,
	};

	_uint				iOffsets[] = {
		0,
	};


	/* 어떤 버텍스 버퍼들을 이용할거다. */
	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 어떤 인덱스 버퍼를 이용할거다. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 정점을 어떤식으로 이어서 그릴거다. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	///* 장치에게 내가 그릴려고하는 정점의 구성정보를 모두 보여준다. */
	///* DX9기준, 정점의 변환을 장치가 알아서 수행한다(고정기능렌더링파이플아ㅣㄴ)(.*/
	//m_pContext->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);




	return S_OK;
}

HRESULT CVIBuffer::Create_ModelBuffer(_Inout_ ID3D11Buffer** ppBuffer)
{
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, ppBuffer);
}

_bool CVIBuffer::Intersect_MousePos(SPACETYPE eSpacetype, _float3* pOut, _matrix matWorld, _float* pLengthOut)
{
	if (typeid(*this) == typeid(CMesh))
	{
		if (static_cast<CMesh*>(this)->Get_ModelType() == MODELTYPE::ANIM)
		{
			D3D11_MAPPED_SUBRESOURCE	SubResource;
			if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
				return false;

			for (_uint i = 0; i < m_vecVertexInfo.size(); i++) {
				m_vecVertexInfo[i] = ((VTXMESH*)(SubResource.pData))[i].vPosition;
			}

			m_pContext->Unmap(m_pVB, 0);
		}
	}

	if (m_vecIndexInfo.size() == 0)
		return false;

	_float fMinLength = INFINITY;
	_float3 vOut = {};
	_bool  bCheck = false;
	for (_uint i = 0; i < m_iNumIndices / 3.f; i++) {
		_uint3 iIndices = m_vecIndexInfo[i];

		_vector vVec1, vVec2, vVec3;

		vVec1 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iX]);
		vVec2 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iY]);
		vVec3 = XMLoadFloat3(&m_vecVertexInfo[iIndices.iZ]);

		if (m_pGameInstance->Mouse_RayIntersect(eSpacetype, pOut, vVec1, vVec2, vVec3, matWorld, pLengthOut))
		{
			if (*pLengthOut < fMinLength)
			{
				fMinLength = *pLengthOut;
				vOut = *pOut;
				bCheck = true;
			}
		}
	}

	*pOut = vOut;
	*pLengthOut = fMinLength;

	return bCheck;
}


void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
