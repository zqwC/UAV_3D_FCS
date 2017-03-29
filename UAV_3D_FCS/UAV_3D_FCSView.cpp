
// UAV_3D_FCSView.cpp : CUAV_3D_FCSView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "UAV_3D_FCS.h"
#endif

#include "UAV_3D_FCSDoc.h"
#include "UAV_3D_FCSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUAV_3D_FCSView

IMPLEMENT_DYNCREATE(CUAV_3D_FCSView, CView)

BEGIN_MESSAGE_MAP(CUAV_3D_FCSView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CUAV_3D_FCSView 构造/析构

CUAV_3D_FCSView::CUAV_3D_FCSView()
{
	// TODO: 在此处添加构造代码

}

CUAV_3D_FCSView::~CUAV_3D_FCSView()
{
}

BOOL CUAV_3D_FCSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CUAV_3D_FCSView 绘制

void CUAV_3D_FCSView::OnDraw(CDC* /*pDC*/)
{
	CUAV_3D_FCSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CUAV_3D_FCSView 打印

BOOL CUAV_3D_FCSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CUAV_3D_FCSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CUAV_3D_FCSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CUAV_3D_FCSView 诊断

#ifdef _DEBUG
void CUAV_3D_FCSView::AssertValid() const
{
	CView::AssertValid();
}

void CUAV_3D_FCSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUAV_3D_FCSDoc* CUAV_3D_FCSView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUAV_3D_FCSDoc)));
	return (CUAV_3D_FCSDoc*)m_pDocument;
}
#endif //_DEBUG


// CUAV_3D_FCSView 消息处理程序
