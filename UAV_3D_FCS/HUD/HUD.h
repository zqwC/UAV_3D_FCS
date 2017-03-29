#pragma once
#include "afxwin.h"


#include "../protocol/FCToPCProtocolConvertToRealData.h"

#include <cmath>
#include "../MemoryDC.h"

//- [3/26/2017 张群伟]
# define PI		(3.1415926)
# define ON		1
# define OFF	0

class CHUD :
	public CWnd
{
public:
	CHUD();
	~CHUD();

private:
	TSPIDC m_TSPIDC;			//数据包

	//用于存放控件区域矩形大小的变量
	CRect       m_rectCtrl;

	//缓冲区内绘制背景的虚拟DC
	CDC			m_dcPlate;

	//在缓冲区中将图形绘制到此位图中
	CBitmap		m_bitmapPlate;

	//旧的位置指针
	CBitmap     *m_pbitmapOldPlate;

	//是否需要重绘静态背景
	bool m_reqRedrawStaticBgd = true;

//- [3/26/2017 张群伟]
//-	提示：显示的大小和位置可在DrawStaticBackground(CDC *pDC)函数中通过圆弧半径和控件中心点进行设置
//-       所有显示的部分都以圆弧半径和控件中心点为参考进行绘制
private:  
	// 圆弧半径
	double m_dRadius;
	// 控件的中心点
	CPoint m_CenterPoint;	
	// 三角形的三个点
	CPoint ptTriangle[3];
	// 绘图画刷
	CBrush currentBrush;
	// 绘图画笔
	CPen currentPen;
	// 字体
	CFont currentFont;
	TEXTMETRIC tm;

public:
	//更新控件数据
	void SetCurrentData(const TSPIDC& tspidc);
	//为绘制控件做准备
	void DrawStaticPrepare(CDC* p_dc);
	//绘制静态背景
	void DrawStaticBackground(CDC *pDC);
	//绘制动态背景
	void DrawDynamicBackground(CDC *pDC);
	
//- [3/26/2017 张群伟]
public:
	// 绘制刻度和圆
	void DrawTickCircle(CDC *pDC);
	// 绘制天空和大地
	void DrawSkyGround(CDC *pDC);
	// 绘制飞机
	void DrawAerofoil(CDC *pDC);
	// 绘制中央水平线
	void DrawCenterHorizontal(CDC *pDC);
	// 绘制两个三角形
	void DrawTriangle(CDC *pDC);


	DECLARE_MESSAGE_MAP()
	//绘制控件
	afx_msg void OnPaint();	
};

