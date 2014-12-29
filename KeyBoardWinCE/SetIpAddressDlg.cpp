// SetIpAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KeyBoardWinCE.h"
#include "SetIpAddressDlg.h"

#include <ndis.h>

#include <sip.h> 

// CSetIpAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CSetIpAddressDlg, CDialog)

CSetIpAddressDlg::CSetIpAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetIpAddressDlg::IDD, pParent)
{

}

CSetIpAddressDlg::~CSetIpAddressDlg()
{
}

void CSetIpAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//
BEGIN_MESSAGE_MAP(CSetIpAddressDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetIpAddressDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetIpAddressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetIpAddressDlg ��Ϣ�������

BOOL CSetIpAddressDlg::SetIpAddress(CString m_strIp , CString m_strMask , CString m_strGateWay)
{
	// ����������豸��
	WCHAR Names[50];
	DWORD bytes;
	HANDLE m_hFileHandle = CreateFile(_T("NDS0:"), 0, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		(HANDLE) INVALID_HANDLE_VALUE);

	if( m_hFileHandle == INVALID_HANDLE_VALUE )
	{
		MessageBox(_T("���������ʱ���豸����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}

	// Get list of adapter names
	if (!DeviceIoControl(m_hFileHandle, 
		IOCTL_NDIS_GET_ADAPTER_NAMES,
		NULL, 0,
		Names, MAX_PATH, &bytes, NULL))
	{
		MessageBox(_T("�������������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}
	DWORD len = wcslen(Names);
	Names[len] = 0;
	Names[len+1] = 0;
	CString strKeyName;

	strKeyName.Format(_T("Comm\\%s\\Parms\\TCPIP"), Names);


	//��ע��������IP��Ϣ��Ӧ�ӽ������޸�
	HKEY hkey; 
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyName, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("��ע������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}
	DWORD value;
	value = 0;
	WCHAR buffer[50];

	//set EnableDHCP   
	if(RegSetValueEx(hkey, TEXT("EnableDHCP"), 0, REG_DWORD, (const unsigned char *)&value, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("�ر��Զ����IP����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}		
	//set ip address
	memset(buffer,0,100);
	memcpy(buffer, m_strIp.GetBuffer(0), m_strIp.GetLength()*2);
	if(RegSetValueEx(hkey, TEXT("IpAddress"), 0, REG_MULTI_SZ, (const unsigned char *)buffer, m_strIp.GetLength()*2+2) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("����IP����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}
	//set subnet mask
	memset(buffer, 0, 100);
	memcpy(buffer, m_strMask.GetBuffer(0), m_strMask.GetLength()*2);
	if(RegSetValueEx(hkey, TEXT("SubnetMask"), 0, REG_MULTI_SZ, (const unsigned char *)buffer, m_strMask.GetLength()*2+2) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("���������������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}		
	//set gateway
	memset(buffer, 0, 100);
	memcpy(buffer, m_strGateWay.GetBuffer(0), m_strGateWay.GetLength()*2);
	if(RegSetValueEx(hkey, TEXT("DefaultGateway"), 0, REG_MULTI_SZ, (const unsigned char *)buffer, m_strGateWay.GetLength()*2+2) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("�������ش���"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}
	RegFlushKey(hkey);
	RegCloseKey(hkey);

	// �������������û���������
	HANDLE hNdis = CreateFile(_T("NDS0:"), 0, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		(HANDLE) INVALID_HANDLE_VALUE);
	if( hNdis == INVALID_HANDLE_VALUE )
	{
		MessageBox(_T( "������������ʱ���豸����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}

	// Send the device command.
	if (!DeviceIoControl( hNdis, IOCTL_NDIS_REBIND_ADAPTER,
		Names, _tcslen( Names) + sizeof( TCHAR ),    // buf contains the name of the
		NULL, 0, NULL, NULL ) )
	{
		MessageBox(_T( "����������������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR); 
	}

	CloseHandle(hNdis);
	return TRUE;
}

BOOL CSetIpAddressDlg::GetIpAddress(CString & m_strIp, CString & m_strMask, CString & m_strGateWay)
{
	// ����������豸��
	WCHAR Names[50];
	DWORD bytes;
	HANDLE m_hFileHandle = CreateFile(_T("NDS0:"), 0, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		(HANDLE) INVALID_HANDLE_VALUE);

	if( m_hFileHandle == INVALID_HANDLE_VALUE )
	{
		MessageBox(_T("���������ʱ���豸����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}

	// Get list of adapter names
	if (!DeviceIoControl(m_hFileHandle, 
		IOCTL_NDIS_GET_ADAPTER_NAMES,
		NULL, 0,
		Names, MAX_PATH, &bytes, NULL))
	{
		MessageBox(_T("�������������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}
	DWORD len = wcslen(Names);
	Names[len] = 0;
	Names[len+1] = 0;
	CString strKeyName;

	strKeyName.Format(_T("Comm\\%s\\Parms\\TCPIP"), Names);

	//��ע��������IP��Ϣ��Ӧ�ӽ������޸�
	HKEY hkey; 
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKeyName, 0, KEY_WRITE, &hkey) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("��ע������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
		return false;
	}
	WCHAR buffer[100];
	DWORD lpType = REG_MULTI_SZ, lpcbData = 200;

	//get gateway
	memset(buffer, 0, lpcbData);
	if(RegQueryValueEx(hkey, TEXT("DefaultGateway"), 0, &lpType, (LPBYTE)buffer, &lpcbData) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("��ȡ���ش���"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}
	m_strGateWay.Format(_T("%s"), buffer);

	//get ip address
	memset(buffer, 0, lpcbData);
	if(RegQueryValueEx(hkey, TEXT("IpAddress"), 0, &lpType, (LPBYTE)buffer, &lpcbData) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("��ȡIP����"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}
	m_strIp.Format(_T("%s"), buffer);

	//get subnet mask
	memset(buffer, 0, lpcbData);
	if(RegQueryValueEx(hkey, TEXT("SubnetMask"), 0, &lpType, (LPBYTE)buffer, &lpcbData) != ERROR_SUCCESS)
	{
		MessageBox(TEXT("��ȡ�����������"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONERROR);
	}		
	m_strMask.Format(_T("%s"), buffer);

	RegFlushKey(hkey);
	RegCloseKey(hkey);

	return TRUE;
}

void CSetIpAddressDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csIpAddress(_T("")), csMask(_T("")), csGateWay(_T(""));
	GetDlgItem(IDC_EDIT_IPADDRESS)->GetWindowText(csIpAddress);
	GetDlgItem(IDC_EDIT_MASK)->GetWindowText(csMask);
	GetDlgItem(IDC_EDIT_GATEWAY)->GetWindowText(csGateWay);
	
	if ((csIpAddress.GetLength() != 0) && (csMask.GetLength() != 0) && (csGateWay.GetLength() != 0))
	{
		if (SetIpAddress(csIpAddress, csMask, csGateWay))
		{
			MessageBox(_T("���óɹ���"), _T("��Ϣ��ʾ"), MB_OK|MB_ICONINFORMATION);
		}
	}
	SHSipPreference(this->m_hWnd, SIP_DOWN);
	OnOK();
}

void CSetIpAddressDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
	SHSipPreference(this->m_hWnd, SIP_DOWN);
}

BOOL CSetIpAddressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString csIP(_T("")), csMask(_T("")), csGateWay(_T(""));
	if (GetIpAddress(csIP, csMask, csGateWay))
	{
		GetDlgItem(IDC_EDIT_IPADDRESS)->SetWindowText(csIP);
		GetDlgItem(IDC_EDIT_MASK)->SetWindowText(csMask);
		GetDlgItem(IDC_EDIT_GATEWAY)->SetWindowText(csGateWay);
	}

	SHSipPreference(this->m_hWnd, SIP_UP);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CSetIpAddressDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	SHSipPreference(this->m_hWnd, SIP_FORCEDOWN);
	return CDialog::DestroyWindow();
}
