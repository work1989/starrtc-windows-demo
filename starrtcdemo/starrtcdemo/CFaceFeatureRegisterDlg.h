#pragma once
//#include "CFindFace.h"

// CFaceFeatureRegisterDlg 对话框

class CFaceFeatureRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFaceFeatureRegisterDlg)

public:
	CFaceFeatureRegisterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFaceFeatureRegisterDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USER_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditId;
	CEdit m_EditName;
	CEdit m_EditPath;
	int m_nId;
	CString m_strName;
	CString m_strPath;

	//CFindFace m_FindFace;

	afx_msg void OnBnClickedButtonSelectRegisterPic();
	virtual void OnOK();
};
