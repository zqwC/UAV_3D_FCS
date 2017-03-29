#pragma once


// CComboBoxNoWheel

class CComboBoxNoWheel : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxNoWheel)

public:
	CComboBoxNoWheel();
	virtual ~CComboBoxNoWheel();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


