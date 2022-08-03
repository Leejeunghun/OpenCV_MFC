
// OpenCV_MFCDlg.h: 헤더 파일
//

#pragma once

#include "pch.h"
#include "CLASS_LOG.h"
#include "OpencvMat.h"
// COpenCVMFCDlg 대화 상자
class COpenCVMFCDlg : public CDialogEx
{
// 생성입니다.
public:
	COpenCVMFCDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCV_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//Log 관련
	afx_msg void OnEnChangeEdLog();
	CEdit m_ed_log;
	CString m_log;
	CLASS_LOG log_data;
	afx_msg void OnBnClickedBtnLog();
	afx_msg void OnBnClickedButton2();

	//OpenCV 관련
	class_OpenCV_cusmtom OpenCV_Al;
	CStatic m_Picture_1;
};
