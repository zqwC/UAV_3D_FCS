#pragma once


// CSplashWnd

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplashWnd)

	typedef BOOL(WINAPI *lpfnSetLayeredWindowAttributes)
		(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;
public:
	CSplashWnd();
	CSplashWnd(UINT dwFileName, UINT dwFile2Name, COLORREF colTrans);
	virtual ~CSplashWnd();
	CBitmap m_bitmap;
	CBitmap m_bitmap2;
	UINT m_SplashBack;
	UINT m_SplaskBack2;
	COLORREF m_colTrans;
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
protected:
	BOOL Create(CWnd* pParentWnd = NULL);
	void Init();
	bool MakeTransparent();
	static CSplashWnd* c_pSplashWnd;
	UINT m_Process;
	CString m_Text;
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


