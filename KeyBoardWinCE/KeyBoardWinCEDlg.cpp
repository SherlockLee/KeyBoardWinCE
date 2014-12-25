// KeyBoardWinCEDlg.cpp : ʵ���ļ�
//


#include "stdafx.h"
#include "KeyBoardWinCE.h"
#include "KeyBoardWinCEDlg.h"

#include "modbustcp.h"

#include "TechBoxLib.h"
#pragma comment(lib,"TeachBox1_2.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NOTIFYICONDATA  KeyBoardND;


//////////////////////////////////////////////////////////////////////////
// Modbus TCP�õ���ȫ�ֱ���
//////////////////////////////////////////////////////////////////////////
USHORT   usRegInputStart = REG_INPUT_START;
USHORT   usRegInputBuf[REG_INPUT_NREGS];

USHORT   usRegHoldingStart = REG_HOLDING_START;
USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

USHORT   usInputStatusStart = INPUT_STATUS_START;
UCHAR	ucInputStatusBuf[INPUT_STATUS_NREGS];

USHORT   usInputCoilStart = INPUT_COIL_START;
UCHAR	ucInputCoilBuf[INPUT_COIL_NREGS];
//////////////////////////////////////////////////////////////////////////

// CKeyBoardWinCEDlg �Ի���

CKeyBoardWinCEDlg::CKeyBoardWinCEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardWinCEDlg::IDD, pParent)
	, m_key(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	techBoxled=0;
}

void CKeyBoardWinCEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyBoardWinCEDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_TECHBOX_KEY_CHANGE,OnTeachBoxKeyHandler)

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CKeyBoardWinCEDlg::OnBnClickedButtonMin)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CKeyBoardWinCEDlg ��Ϣ�������

BOOL CKeyBoardWinCEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if(TECHBOX_NOERR!=TechBoxLibInit(this->m_hWnd,WM_TECHBOX_KEY_CHANGE,WM_TECHBOX_MPG))
	{
		MessageBox(_T("TeachBox init error!"));
	}
	else
	{

	}	
	AddSystrayIcon();
	//::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 100, 100, 300, 300, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	SetTimer(1, 1, NULL);

	//////////////////////////////////////////////////////////////////////////
	//	ucInputStatusBuf[3] ��Ӧ23~30
	//  ֵ			λֵ
	//	32			25
	//	16			26
	//	8			27
	//	4			28
	//	2			29
	//	1			30
	//	ucInputStatusBuf[4] ��Ӧ31~38
	//  ֵ			λֵ
	//	128			31
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// ����Modbus ��վ
	//////////////////////////////////////////////////////////////////////////
	StartModbusTcp();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CKeyBoardWinCEDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_KEYBOARDWINCE_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_KEYBOARDWINCE_DIALOG));
	}
}
#endif

LRESULT CKeyBoardWinCEDlg::OnTeachBoxKeyHandler(WPARAM w,LPARAM l)  //w:key1  l:key2
{
	m_key=w;

	if(0!=m_key)
	{
		techBoxled|=TEACHBOX_BUZZER;
		switch(m_key)
		{
		case 0x0001:	// ��ʱ 31//�޸�130��ֵΪ1
			ucInputStatusBuf[4] = 1;
			ucInputStatusBuf[3] = 0;	
			break;
		case 0x0002:	// ��ʱ 30
			ucInputStatusBuf[3] = 128;
			ucInputStatusBuf[4] = 0;
			break;
		case 0x0004:	// ��ʱ 29
			ucInputStatusBuf[3] = 64;
			ucInputStatusBuf[4] = 0;
			break;
		case 0x0008:	// ��ʱ 28
			ucInputStatusBuf[3] = 32;
			ucInputStatusBuf[4] = 0;
			break;
		case 0x0010:	// ��ʱ 27
			ucInputStatusBuf[3] = 16;
			ucInputStatusBuf[4] = 0;
			break;
		case 0x0020:	// ��ʱ 26
			ucInputStatusBuf[3] = 8;
			ucInputStatusBuf[4] = 0;
			break;
		case 0x0040:	// ��ʱ 25
			ucInputStatusBuf[3] = 4;
			ucInputStatusBuf[4] = 0;
			break;
		}
		//MessageBox(_T("LED3 on"));
	}
	else
	{
		techBoxled&=~TEACHBOX_BUZZER;
		//MessageBox(_T("LED3 off"));
	}
	SetOutputData1(techBoxled) ;

	UpdateData(FALSE);
	if (m_key == 0x0000)
	{
		//ucInputStatusBuf[3] = 0;
		//ucInputStatusBuf[4] = 0;
		//GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T("Buttons Up"));
	}

	return 0; 
}
void CKeyBoardWinCEDlg::AddSystrayIcon(void)
{
	// ��ͼ�����ϵͳ����
	KeyBoardND.cbSize = sizeof (NOTIFYICONDATA);
	KeyBoardND.hWnd = m_hWnd;
	KeyBoardND.uID = IDR_MAINFRAME;
	KeyBoardND.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	KeyBoardND.uCallbackMessage= WM_KEYBOARD_NOTIFYICON;
	KeyBoardND.hIcon = m_hIcon;
	wcscpy(KeyBoardND.szTip, _T("KeyBoard Control"));
	Shell_NotifyIcon(NIM_ADD, &KeyBoardND);
}

void CKeyBoardWinCEDlg::DelSystrayIcon(void)
{
	Shell_NotifyIcon(NIM_DELETE, &KeyBoardND);
}

LRESULT CKeyBoardWinCEDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_KEYBOARD_NOTIFYICON:
		if(lParam==WM_LBUTTONDBLCLK)
		{
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		}
		else if(lParam==WM_RBUTTONDOWN)
		{
#if 0
			CMenu menu; 
			//�������ȶ����ѡ��
			menu.LoadMenu(IDR_TRADEMEMU); 
			CMenu*pMenu=menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos); 
			//����SetForegroundWindow��Ŀ��Ϊʹ�û���˵�֮��ʱ�˵�������ʧ
			::SetForegroundWindow(m_hWnd);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd()); 
#endif
		}
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CKeyBoardWinCEDlg::OnDestroy()
{
	CDialog::OnDestroy();
	DelSystrayIcon( );

	//////////////////////////////////////////////////////////////////////////
	// ֹͣModbus ��վ
	//////////////////////////////////////////////////////////////////////////
	StopModbusTcp();
	//////////////////////////////////////////////////////////////////////////
}

void CKeyBoardWinCEDlg::OnBnClickedButtonMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if 0
	HWND hWnd=::FindWindow(NULL, TEXT("KeyBoardWinCE"));
	if(hWnd)
	{
		if (::ShowWindow(hWnd, SW_MINIMIZE))
		{
			::SetForegroundWindow(hWnd);
		}		
	}
	this->ShowWindow(SW_MINIMIZE);
#endif	
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 210, 60, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	//::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 500, 500, SWP_HIDEWINDOW|SWP_NOACTIVATE);
}

void CKeyBoardWinCEDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 210, 60, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	//::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 500, 500, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	KillTimer(1);
	CDialog::OnTimer(nIDEvent);
}

#if 0
void CKeyBoardWinCEDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CHAR cKeyValueBuf[64];
	CString csKeyValue(_T(""));
	GetDlgItem(IDC_EDIT_KEY_VALUE)->GetWindowText(csKeyValue);
	strcpy(cKeyValueBuf, CT2CA(csKeyValue));
	ucInputStatusBuf[3] = atoi(cKeyValueBuf);
}
#endif
