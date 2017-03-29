// ComboBoxNoWheel.cpp : 实现文件
//

#include "stdafx.h" 
#include "ComboBoxNoWheel.h"


// CComboBoxNoWheel

IMPLEMENT_DYNAMIC(CComboBoxNoWheel, CComboBox)

CComboBoxNoWheel::CComboBoxNoWheel()
{

}

CComboBoxNoWheel::~CComboBoxNoWheel()
{
}


BEGIN_MESSAGE_MAP(CComboBoxNoWheel, CComboBox)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CComboBoxNoWheel 消息处理程序




BOOL CComboBoxNoWheel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CComboBox::OnMouseWheel(nFlags, zDelta, pt);
}
