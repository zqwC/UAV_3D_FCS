#include"stdafx.h"
#include "..\stdafx.h"
#include "CMyGridCtrl.h"
/***********************************************************************
* Copyright (c) 2014，南昌航空大学信息工程学院自动化系
* All rights reserved.
* 
* 文件名称：CHUD.cpp
* 摘    要：本项目是为一款基于MFC的自制表格控件
* 
* 当前版本：1.0
* 作    者：郑迪威
* 完成日期：2015年1月26日
*
* NOTE    : 未经授权请不要作为商业用途，或者如果您要作为商业
            用途，请联系作者
***********************************************************************/
//FUNCTIONS //////////////////////////////////////////////

//构造函数 //////////////////////////////////////////////////
CMyGridCtrl::CMyGridCtrl()
{
}
/////////////////////////////////////////////////////////

//析构函数 ////////////////////////////////////////////////
CMyGridCtrl::~CMyGridCtrl()
{
}
/////////////////////////////////////////////////////////

//设置某一列背景色 ////////////////////////////////////////////
void CMyGridCtrl::SetColumnColor(int nCol, COLORREF clr)
{
}
///////////////////////////////////////////////////////////

//设置某一行背景色 ///////////////////////////////////////////
void CMyGridCtrl::SetRowColor(int nRow, COLORREF  clr)
{}
//////////////////////////////////////////////////////////////

//加载图片列表 ///////////////////////////////////////////
void CMyGridCtrl::LoadImageList(int nImageId,int nSize)
{}
//////////////////////////////////////////////////////////////

//设置某个单元格为ComboBox ///////////////////////////////////////////
void CMyGridCtrl::SetCellCombo(int nRow, int nCol,  
		CStringArray& items)
{}
//////////////////////////////////////////////////////////////

//设置某个单元格为ComboBox ///////////////////////////////////////////
void CMyGridCtrl::SetCellCheck(int nRow, int nCol, 
		bool isCheck)
{
	CGridCellBase *pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL && pCell->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
	{
		//设置checkBox的状态	
		((CGridCellCheck*)pCell)->SetCheck(isCheck);
	}
}
//////////////////////////////////////////////////////////////
void CMyGridCtrl::SetCellRadio(int nRow, int nCol,
	bool isRadios)
{
	CGridCellBase *pCell = this->GetCell(nRow, nCol);
	if (pCell != NULL && pCell->IsKindOf(RUNTIME_CLASS(CGridCellCheck)))
	{
		//设置checkBox的状态	
		((CGridCellCheck*)pCell)->SetRadio(isRadios);
	}
}