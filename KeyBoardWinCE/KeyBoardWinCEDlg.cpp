// KeyBoardWinCEDlg.cpp : 实现文件
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
// Modbus TCP用到的全局变量
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

// CKeyBoardWinCEDlg 对话框

CKeyBoardWinCEDlg::CKeyBoardWinCEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBoardWinCEDlg::IDD, pParent)
	, m_key(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	techBox = 0;
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


// CKeyBoardWinCEDlg 消息处理程序

BOOL CKeyBoardWinCEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码
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
	SetTimer(2, 5, NULL);

	//////////////////////////////////////////////////////////////////////////
	// 开启Modbus 主站
	//////////////////////////////////////////////////////////////////////////
	StartModbusTcp();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
		techBox|=TEACHBOX_BUZZER;
		switch(m_key)
		{
		case TEACHBOX_KEY1:	// 修改130的值为1
			ucInputStatusBuf[3] = 1<<6;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY2:	// 修改129的值为1
			ucInputStatusBuf[3] = 1<<5;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY3:	// 修改128的值为1
			ucInputStatusBuf[3] = 1<<4;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY4:	// 修改127的值为1
			ucInputStatusBuf[3] = 1<<3;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY5:	// 修改126的值为1
			ucInputStatusBuf[3] = 1<<2;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY6:	// 修改125的值为1
			ucInputStatusBuf[3] = 1<<1;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY7:	// 修改124的值为1
			ucInputStatusBuf[3] = 1;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY22:	// 修改123的值为1
			ucInputStatusBuf[2] = 1<<7;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY20:	// 修改122的值为1
			ucInputStatusBuf[2] = 1<<6;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY18:	// 修改121的值为1
			ucInputStatusBuf[2] = 1<<5;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY16:	// 修改120的值为1
			 ucInputStatusBuf[2] = 1<<4;
			 ucInputStatusBuf[7] = 0;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[3] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY14:	// 修改119的值为1
			 ucInputStatusBuf[2] = 1<<3;
			 ucInputStatusBuf[7] = 0;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[3] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY12:	// 修改118的值为1
			 ucInputStatusBuf[2] = 1<<2;
			 ucInputStatusBuf[7] = 0;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[3] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY10:	// 修改117的值为1
			 ucInputStatusBuf[2] = 1<<1;
			 ucInputStatusBuf[7] = 0;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[3] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY9:	// 修改116的值为1
			 ucInputStatusBuf[2] = 1;
			 ucInputStatusBuf[7] = 0;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[3] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY21:	// 修改106的值为1
			ucInputStatusBuf[0] = 1<<6;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY19:	// 修改105的值为1
			ucInputStatusBuf[0] = 1<<5;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY17:	// 修改104的值为1
			ucInputStatusBuf[0] = 1<<4;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY15:	// 修改103的值为1
			ucInputStatusBuf[0] = 1<<3;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY13:	// 修改102的值为1
			ucInputStatusBuf[0] = 1<<2;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY11:	// 修改101的值为1
			ucInputStatusBuf[0] = 1<<1;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY8:	// 修改100的值为1
			ucInputStatusBuf[0] = 1;
			ucInputStatusBuf[7] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY31:	// 修改155的值为1
			ucInputStatusBuf[6] = 1<<7;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[7] = 0;
			break;
		case TEACHBOX_KEY30:	// 修改156的值为1
			ucInputStatusBuf[7] = 1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY29:	// 修改157的值为1
			ucInputStatusBuf[7] = 1<<1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY28:		// 修改158的值为1
			ucInputStatusBuf[7] = 1<<2;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY27:	// 修改159的值为1
			ucInputStatusBuf[7] = 1<<3;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY26:	// 修改160的值为1
			ucInputStatusBuf[7] = 1<<4;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY25:	// 修改161的值为1
			ucInputStatusBuf[7] = 1<<5;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY24:	// 修改162的值为1
			ucInputStatusBuf[7] = 1<<6;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY23:	// 修改163的值为1
			ucInputStatusBuf[7] = 1<<7;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[2] = 0;
			ucInputStatusBuf[3] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		}
		//MessageBox(_T("LED3 on"));
	}
	else
	{
		techBox&=~TEACHBOX_BUZZER;
		//MessageBox(_T("LED3 off"));
	}
	SetOutputData1(techBox) ;

	UpdateData(FALSE);
	if (m_key == 0x0000)
	{

	}

	return 0; 
}
void CKeyBoardWinCEDlg::AddSystrayIcon(void)
{
	// 将图标放入系统托盘
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
			//载入事先定义的选单
			menu.LoadMenu(IDR_TRADEMEMU); 
			CMenu*pMenu=menu.GetSubMenu(0); 
			CPoint pos; 
			GetCursorPos(&pos); 
			//加入SetForegroundWindow的目的为使用户点菜单之外时菜单可以消失
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
	// 停止Modbus 主站
	//////////////////////////////////////////////////////////////////////////
	StopModbusTcp();
	//////////////////////////////////////////////////////////////////////////
}

void CKeyBoardWinCEDlg::OnBnClickedButtonMin()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 210, 60, SWP_HIDEWINDOW|SWP_NOACTIVATE);
		KillTimer(1);
		break;
	case 2:
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	// 启动定时器，扫描值的变化，进行灯状态的调整
	//////////////////////////////////////////////////////////////////////////
	if (ucInputCoilBuf[0] != 0)		// 对应的地址为100，其值为1
	{
		techBox|=TEACHBOX_LED1;
	}
	else
	{
		techBox&=~TEACHBOX_LED1;
	}

	if (ucInputCoilBuf[0]>>1 != 0)	// 对应的地址为101，其值为1
	{
		techBox|=TEACHBOX_LED2;
	}
	else 
	{
		techBox&=~TEACHBOX_LED2;
	}

	if (ucInputCoilBuf[0]>>2 != 0)	// 对应的地址为102，其值为1
	{
		techBox|=TEACHBOX_LED3;
	}
	else 
	{
		techBox&=~TEACHBOX_LED3;
	}
	
	if (ucInputCoilBuf[0]>>3 != 0)	// 对应的地址为103，其值为1
	{
		techBox|=TEACHBOX_LED4;
	}
	else
	{
		techBox&=~TEACHBOX_LED4;
	}
	SetOutputData1(techBox);
	//////////////////////////////////////////////////////////////////////////
	CDialog::OnTimer(nIDEvent);
}

#if 0
void CKeyBoardWinCEDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CHAR cKeyValueBuf[64];
	CString csKeyValue(_T(""));
	GetDlgItem(IDC_EDIT_KEY_VALUE)->GetWindowText(csKeyValue);
	strcpy(cKeyValueBuf, CT2CA(csKeyValue));
	ucInputStatusBuf[3] = atoi(cKeyValueBuf);
}
#endif
