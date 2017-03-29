//HEAD FILES/////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "GridCtrl.h"
#include "NewCellTypes\GridCellCheck.h"
///////////////////////////////////////////////////////////////

//CLASS /////////////////////////////////////////////////////
//继承GridCtrl类，实现自定义GridCtrl
class CMyGridCtrl:public CGridCtrl
{
public:
	CMyGridCtrl();
	~CMyGridCtrl();

	void SetColumnColor(int nCol, COLORREF clr);	//设置某一列的背景颜色
	void SetRowColor(int nRow, COLORREF  clr);		//设置某一行的背景颜色
	void LoadImageList(int nImageId,int nSize);		//加载图片列表
	void SetCellCombo(int nRow, int nCol,  
		CStringArray& items);						//设置某个单元格为ComboBox
	void SetCellCheck(int nRow, int nCol, 
		bool isCheck);								//设置某个单元格为CheckBox
	void SetCellRadio(int nRow, int nCol,
		bool isRadios);
};
/////////////////////////////////////////////////////////////
