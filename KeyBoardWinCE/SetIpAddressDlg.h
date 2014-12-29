#pragma once


// CSetIpAddressDlg 对话框

class CSetIpAddressDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetIpAddressDlg)

public:
	CSetIpAddressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetIpAddressDlg();

// 对话框数据
	enum { IDD = IDD_SET_IPADDRESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:

	DECLARE_MESSAGE_MAP()
	BOOL SetIpAddress(CString m_strIp , CString m_strMask , CString m_strGateWay);
	BOOL GetIpAddress(CString & m_strIp, CString & m_strMask, CString & m_strGateWay);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
