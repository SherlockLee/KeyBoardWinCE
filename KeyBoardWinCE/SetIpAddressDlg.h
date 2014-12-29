#pragma once


// CSetIpAddressDlg �Ի���

class CSetIpAddressDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetIpAddressDlg)

public:
	CSetIpAddressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetIpAddressDlg();

// �Ի�������
	enum { IDD = IDD_SET_IPADDRESS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:

	DECLARE_MESSAGE_MAP()
	BOOL SetIpAddress(CString m_strIp , CString m_strMask , CString m_strGateWay);
	BOOL GetIpAddress(CString & m_strIp, CString & m_strMask, CString & m_strGateWay);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
};
