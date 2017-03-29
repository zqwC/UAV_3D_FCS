
// UAV_3D_FCSView.h : CUAV_3D_FCSView 类的接口
//

#pragma once


class CUAV_3D_FCSView : public CView
{
protected: // 仅从序列化创建
	CUAV_3D_FCSView();
	DECLARE_DYNCREATE(CUAV_3D_FCSView)

// 特性
public:
	CUAV_3D_FCSDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CUAV_3D_FCSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // UAV_3D_FCSView.cpp 中的调试版本
inline CUAV_3D_FCSDoc* CUAV_3D_FCSView::GetDocument() const
   { return reinterpret_cast<CUAV_3D_FCSDoc*>(m_pDocument); }
#endif

