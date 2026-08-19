#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
public:
   typedef struct tagTrail_Desc
   {
	  _float3      vPos_0;
	  _float3      vPos_1;
	  _uint      iMaxCnt;
	  _uint		iLerpPointNum = {12};

   }VIBUFFER_TRAIL_DESC;

private:
   CVIBuffer_Trail(ComPtr<ID3D11Device> pDevice,ComPtr<ID3D11DeviceContext> pContext);
   CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
   virtual   ~CVIBuffer_Trail() = default;
public:
   virtual   HRESULT   Initialize_Prototype() override;
   virtual   HRESULT   Initialize(void* pArg) override;

   void   Update(_fmatrix matOwner,_bool bRemove = true);
   void   Reset(_fmatrix matOwner);

public:
	GETSET_EX2(VIBUFFER_TRAIL_DESC, m_Trail_Desc, Buffer_Desc, GET, SET)

private:
   VIBUFFER_TRAIL_DESC m_Trail_Desc = {};
   _uint            m_iVtxCount = { 0 };
   _uint			m_iPrimitiveNum = {};

  // _uint m_iLerpPointNum = { 12 };
   _int m_iCatMullRomIndex[4] = {0,0,0,0};

public:
   static   shared_ptr<CVIBuffer_Trail> Create(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
   virtual   shared_ptr<CComponent> Clone(void* pArg) override;
   virtual   void         Free() override;
};

END
