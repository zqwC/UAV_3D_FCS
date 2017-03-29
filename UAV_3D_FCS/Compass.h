#pragma once
#include "UavView.h"
class CCompass :
	public CUavView
{
public:
	CCompass();
	~CCompass();

	//ªÊ÷∆æ≤Ã¨±≥æ∞
	virtual void DrawStaticBackground(CDC *pDC);
	//ªÊ÷∆∂ØÃ¨±≥æ∞
	virtual void DrawDynamicBackground(CDC *pDC);

};

