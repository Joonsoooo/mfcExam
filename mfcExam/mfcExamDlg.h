
//mfcExamDlg.h: 헤더 파일
//

#pragma once

#define NWIDTH 1280
#define NHEIGHT 800
#define BACKGROUND_HEIGHT (NHEIGHT - 100)
#define CUSTOM_UPDATEDATA WM_USER

// CmfcExamDlg 대화 상자
class CmfcExamDlg : public CDialogEx
{
// 생성입니다.
public:
	CmfcExamDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCEXAM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
private: // 멤버 변수
	int m_nClickCount;
	int m_nRadius;
	int m_nDragIndex;


	double m_dThickness;

	CImage m_image;
	
	CPoint m_nPoints[3];
	CPoint m_ptDragOffset;
	
	bool m_bIsDragging;


protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void makeCircle(unsigned char* fm, int x, int y, int nRadius);
	bool isInCircle(int x, int y, int nCenterX, int nCenterY, int nRadius);
	void inItBackground();
	void UpdateDisplay();
	void makeGarden(CPoint Points[3], unsigned char* fm);
	void makeAll();
	void clearAll();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnMoverandom();
	CImage& getimage();
	int& getRadius();
	afx_msg LRESULT ForCustomMessageFromThread(WPARAM  wParam, LPARAM lParam);

};
