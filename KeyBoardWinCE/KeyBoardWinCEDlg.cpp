// KeyBoardWinCEDlg.cpp : ʵ���ļ�
//


#include "stdafx.h"
#include "KeyBoardWinCE.h"
#include "KeyBoardWinCEDlg.h"

#include <ShellAPI.h>

#include "SetIpAddressDlg.h"
#include "modbustcp.h"

#include "TechBoxLib.h"
#pragma comment(lib,"TeachBox1_2.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

NOTIFYICONDATA  KeyBoardND;
SHELLEXECUTEINFO OpenFTPClient;

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
	ON_BN_CLICKED(IDC_BUTTON_SET_IPADDRESS, &CKeyBoardWinCEDlg::OnBnClickedButtonSetIpaddress)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FTP_TOOL, &CKeyBoardWinCEDlg::OnBnClickedButtonOpenFtpTool)
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
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	if(TECHBOX_NOERR!=TechBoxLibInit(this->m_hWnd, WM_TECHBOX_KEY_CHANGE,WM_TECHBOX_MPG))
	{
		MessageBox(_T("TeachBox init error!"));
	}
	else
	{

	}	
	AddSystrayIcon();

	SetTimer(2, 5, NULL);

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
		techBox|=TEACHBOX_BUZZER;
		switch(m_key)
		{
		case TEACHBOX_KEY1:	// �޸�130��ֵΪ1
			ucInputStatusBuf[3] = 1<<6;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY2:	// �޸�129��ֵΪ1
			ucInputStatusBuf[3] = 1<<5;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY3:	// �޸�128��ֵΪ1
			ucInputStatusBuf[3] = 1<<4;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY4:	// �޸�127��ֵΪ1
			ucInputStatusBuf[3] = 1<<3;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY5:	// �޸�126��ֵΪ1
			ucInputStatusBuf[3] = 1<<2;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY6:	// �޸�125��ֵΪ1
			ucInputStatusBuf[3] = 1<<1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY7:	// �޸�124��ֵΪ1
			ucInputStatusBuf[3] = 1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY22:	// �޸�123��ֵΪ1
			if (((ucInputStatusBuf[2]>>7) & 1) == 0)
			{
				ucInputStatusBuf[2] |= 1<<7;
			}
			else
			{
				ucInputStatusBuf[2] ^= 1<<7;
			}
			break;
		case TEACHBOX_KEY20:	// �޸�122��ֵΪ1
			ucInputStatusBuf[2] |= 1<<6;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY18:	// �޸�121��ֵΪ1
			ucInputStatusBuf[2] |= 1<<5;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY16:	// �޸�120��ֵΪ1
			 ucInputStatusBuf[2] |= 1<<4;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY14:	// �޸�119��ֵΪ1
			 ucInputStatusBuf[2] |= 1<<3;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY12:	// �޸�118��ֵΪ1
			 ucInputStatusBuf[2] |= 1<<2;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY10:	// �޸�117��ֵΪ1
			 ucInputStatusBuf[2] |= 1<<1;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY9:		// �޸�116��ֵΪ1
			 ucInputStatusBuf[2] |= 1;
			 ucInputStatusBuf[0] = 0;
			 ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY21:	// �޸�106��ֵΪ1
			ucInputStatusBuf[0] = 1<<6;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY19:	// �޸�105��ֵΪ1
			ucInputStatusBuf[0] = 1<<5;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY17:	// �޸�104��ֵΪ1
			ucInputStatusBuf[0] = 1<<4;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY15:	// �޸�103��ֵΪ1
			ucInputStatusBuf[0] = 1<<3;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY13:	// �޸�102��ֵΪ1
			ucInputStatusBuf[0] = 1<<2;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY11:	// �޸�101��ֵΪ1
			ucInputStatusBuf[0] = 1<<1;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY8:	// �޸�100��ֵΪ1
			ucInputStatusBuf[0] = 1;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY31:	// �޸�155��ֵΪ1
			ucInputStatusBuf[6] = 1<<7;
			ucInputStatusBuf[0] = 0;
			break;
		case TEACHBOX_KEY30:	// �޸�156��ֵΪ1
			ucInputStatusBuf[7] |= 1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY29:	// �޸�157��ֵΪ1
			ucInputStatusBuf[7] |= 1<<1;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY28:		// �޸�158��ֵΪ1
			ucInputStatusBuf[7] |= 1<<2;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY27:	// �޸�159��ֵΪ1
			ucInputStatusBuf[7] |= 1<<3;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY26:	// �޸�160��ֵΪ1
			ucInputStatusBuf[7] |= 1<<4;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY25:	// �޸�161��ֵΪ1
			ucInputStatusBuf[7] |= 1<<5;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY24:	// �޸�162��ֵΪ1
			ucInputStatusBuf[7] |= 1<<6;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		case TEACHBOX_KEY23:	// �޸�163��ֵΪ1
			ucInputStatusBuf[7] |= 1<<7;
			ucInputStatusBuf[0] = 0;
			ucInputStatusBuf[6] = 0;
			break;
		}
	}
	else
	{
		techBox&=~TEACHBOX_BUZZER;
	}
	SetOutputData1(techBox) ;

	UpdateData(FALSE);
	if (m_key == 0x0000)
	{
		ucInputStatusBuf[0] = 0;
		ucInputStatusBuf[2] &= 1<<7;
		ucInputStatusBuf[6] = 0;
		ucInputStatusBuf[7] = 0;
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
	DelSystrayIcon();

	//////////////////////////////////////////////////////////////////////////
	// ֹͣModbus ��վ
	//////////////////////////////////////////////////////////////////////////
	if ((GetModbusState() == MB_RUNNING) || (GetModbusState() == MB_SHUTDOWN))
	{
		StopModbusTcp();
	}	
	//////////////////////////////////////////////////////////////////////////

	if( OpenFTPClient.hProcess != NULL)
	{ 
		TerminateProcess(OpenFTPClient.hProcess, 0);
		OpenFTPClient.hProcess = NULL;
	}
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
	/*::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 210, 80, SWP_HIDEWINDOW|SWP_NOACTIVATE);*/
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOACTIVATE);
}

void CKeyBoardWinCEDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UCHAR ucBitValue = 0;
	switch(nIDEvent)
	{
	case 1:
		/*::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 210, 80, SWP_HIDEWINDOW|SWP_NOACTIVATE);*/
		KillTimer(1);
		break;
	case 2:
		//////////////////////////////////////////////////////////////////////////
		// ������ʱ����ɨ��ֵ�ı仯�����е�״̬�ĵ���
		//////////////////////////////////////////////////////////////////////////
		if ((ucInputCoilBuf[0]>>3 & 1) == 1)	// ��Ӧ�ĵ�ַΪ103����ֵΪ1	
		{
			techBox|=TEACHBOX_LED4;
			ucBitValue = ucInputCoilBuf[0]>>3;
		}
		else
		{
			techBox&=~TEACHBOX_LED4;
		}
		SetOutputData1(techBox);

		if ((ucInputCoilBuf[0]>>2 & 1) == 1)	// ��Ӧ�ĵ�ַΪ102����ֵΪ1
		{
			techBox|=TEACHBOX_LED3;
		}
		else 
		{
			techBox&=~TEACHBOX_LED3;
		}
		SetOutputData1(techBox);

		if ((ucInputCoilBuf[0]>>1 & 1) == 1)	// ��Ӧ�ĵ�ַΪ101����ֵΪ1
		{
			techBox|=TEACHBOX_LED2;
		}
		else 
		{
			techBox&=~TEACHBOX_LED2;
		}
		SetOutputData1(techBox);

		if ((ucInputCoilBuf[0]>>0 & 1) == 1)	// ��Ӧ�ĵ�ַΪ100����ֵΪ1
		{
			techBox|=TEACHBOX_LED1;			
		}
		else
		{
			techBox&=~TEACHBOX_LED1;
		}
		SetOutputData1(techBox);	
		//////////////////////////////////////////////////////////////////////////
		break;
	}
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

void CKeyBoardWinCEDlg::OnBnClickedButtonSetIpaddress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CSetIpAddressDlg SetIpAddressDlg;
	if (!SetIpAddressDlg.DoModal())
	{
		MessageBox(_T("�򿪴���ʧ�ܣ�"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}
	else
	{
		SetTimer(2, 5, NULL);

		//////////////////////////////////////////////////////////////////////////
		// ����Modbus ��վ
		//////////////////////////////////////////////////////////////////////////
		StartModbusTcp();
		//////////////////////////////////////////////////////////////////////////
	}
}

void CKeyBoardWinCEDlg::OnBnClickedButtonOpenFtpTool()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	memset(&OpenFTPClient,0x00,sizeof(SHELLEXECUTEINFO));//����ڴ��һ��Ҫ��
	OpenFTPClient.lpFile=_T("\\NandFlash\\ARMV4_FTPClient.exe");
	OpenFTPClient.lpVerb=L"open";
	OpenFTPClient.cbSize = sizeof(SHELLEXECUTEINFO);
	OpenFTPClient.fMask  = SEE_MASK_NOCLOSEPROCESS;
	OpenFTPClient.hwnd   = NULL;
	OpenFTPClient.nShow  = SW_SHOWNORMAL;

	ShellExecuteEx(&OpenFTPClient);
}
