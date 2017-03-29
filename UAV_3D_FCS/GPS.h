#pragma once
#include "afxwin.h" 

#include "UavView.h"

namespace GPSPace {
	const int title_x_offset = 40;
	const int x_offset = 5;
	const int var_x_offset = 40;
};


class CGPS :
	public CUavView
{
public:
	CGPS();
	~CGPS();  
public: 
	
	//ªÊ÷∆æ≤Ã¨±≥æ∞
	virtual void DrawStaticBackground(CDC *pDC);
	//ªÊ÷∆∂ØÃ¨±≥æ∞
	virtual void DrawDynamicBackground(CDC *pDC);

private:
	
};




