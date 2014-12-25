// KeyBoardWinCEDlg.h : ͷ�ļ�
//

#pragma once

// CKeyBoardWinCEDlg �Ի���
class CKeyBoardWinCEDlg : public CDialog
{
// ����
public:
	CKeyBoardWinCEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_KEYBOARDWINCE_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()

public:
	int m_key;
	unsigned int techBoxled;
	afx_msg LRESULT OnTeachBoxKeyHandler(WPARAM,LPARAM);
	void AddSystrayIcon(void);
	void DelSystrayIcon(void);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
