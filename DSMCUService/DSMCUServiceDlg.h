
// DSMCUServiceDlg.h : header file
//

#pragma once
#include "SLABCP2112.h"
#include "afxwin.h"

#include "DSMCUService.h"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////

#define VID					0
#define PID					0


// CDSMCUServiceDlg dialog
class CDSMCUServiceDlg : public CDialogEx
{
// Construction
public:
	CDSMCUServiceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DSMCUSERVICE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
//	variables
	bool m_connected_flag;

//  SiLabs CP2112 Driver
	HID_SMBUS_DEVICE		m_hidSmbus;

//	ComboBox Element
	void UpdateDeviceList();
	BOOL GetSelectedDevice(CString& serial);
	BOOL FindDevice(CString serial, DWORD& deviceNum);

	BOOL Connect();
	BOOL Disconnect();

	void EnableDeviceCtrls(BOOL enable);
	void UpdateDeviceInformation(BOOL connected);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// string of USB vid | pid connected devices
	CComboBox m_comboDeviceList;
	afx_msg void OnDropdownComboDeviceList();
	afx_msg void OnCloseupComboDeviceList();
	afx_msg void OnSelchangeComboDeviceList();
	afx_msg void OnClickedCheckConnect();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckConnect();

	// connect status
	CStatic m_static_connect;
	afx_msg void OnStnClickedStaticConnect();
	afx_msg void OnBnClickedButtonOperator();
	afx_msg void OnBnClickedButtonAdmin();
	CStatic m_Static_Logo;
};
