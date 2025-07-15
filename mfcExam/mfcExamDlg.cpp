
// mfcExamDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "mfcExam.h"
#include "mfcExamDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

void processRows(unsigned char* fm, int nPitch, int startRow, int endRow, double dcenterX, double dcenterY, int dRadius, int m_dThickness)
{
	for (int j = startRow; j < endRow; ++j) 
	{
		for (int i = 0; i < NWIDTH; ++i) 
		{
			//cout << "i : " << i << " , " << "j : " << j << endl;

			double distanceToCenter = sqrt((i - dcenterX) * (i - dcenterX) + (j - dcenterY) * (j - dcenterY));
			if (fabs(distanceToCenter - dRadius) <= m_dThickness) 
			{
				fm[j * nPitch + i] = 255;
			}

			/*double dist2 = (i - dcenterX) * (i - dcenterX) + (j - dcenterY) * (j - dcenterY);
			double rmin2 = (dRadius - m_dThickness) * (dRadius - m_dThickness);
			double rmax2 = (dRadius + m_dThickness) * (dRadius + m_dThickness);

			if (dist2 >= rmin2 && dist2 <= rmax2) {
				fm[j * nPitch + i] = 255;
			}*/
		}
	}
}

void ThreadFunc(CmfcExamDlg* pDlg)
{

	for (int i = 0; i < 10; i++)
	{
		CPoint tempPoints[3];

		pDlg->getimage().Destroy();
		pDlg->inItBackground();
		unsigned char* fm = (unsigned char*)pDlg->getimage().GetBits();


		for (int j = 0; j < 3; j++)
		{
			tempPoints[j].x = rand() % (NWIDTH - 2 * pDlg->getRadius()) + pDlg->getRadius();
			tempPoints[j].y = rand() % (BACKGROUND_HEIGHT - 2 * pDlg->getRadius()) + pDlg->getRadius();
			pDlg->makeCircle(fm, tempPoints[j].x, tempPoints[j].y, pDlg->getRadius());
		}
		pDlg->makeGarden(tempPoints, fm);

		Sleep(500);
	}
}


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CmfcExamDlg 대화 상자



CmfcExamDlg::CmfcExamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCEXAM_DIALOG, pParent)

	, m_nRadius(10)
	, m_dThickness(1.0)


{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmfcExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nRadius);
	DDX_Text(pDX, IDC_EDIT_RADIUS2, m_dThickness);


	
}

BEGIN_MESSAGE_MAP(CmfcExamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CmfcExamDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_MOVERANDOM, &CmfcExamDlg::OnBnClickedBtnMoverandom)
	ON_MESSAGE(CUSTOM_UPDATEDATA, ForCustomMessageFromThread)
END_MESSAGE_MAP()


// CmfcExamDlg 메시지 처리기

BOOL CmfcExamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	int nBpp = 8;
	SetDlgItemText(IDC_STATIC_CENTERXY, _T("0"));

	MoveWindow(0, 0, NWIDTH, NHEIGHT);
	

	// 멤버 변수 초기화
	m_nClickCount = 0;
	m_bIsDragging = false;
	m_nDragIndex = -1;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CmfcExamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CmfcExamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmfcExamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmfcExamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonDown(nFlags, point);

	//AfxMessageBox(_T("Test"));

	UpdateData(TRUE);

	if (point.x > 0 && point.x < NWIDTH && point.y > 0 && point.y < BACKGROUND_HEIGHT)
	{
		if (m_nClickCount < 3)
		{
			if (m_nClickCount == 0)
			{
				inItBackground();
			}

			unsigned char* fm = (unsigned char*)m_image.GetBits();
			makeCircle(fm, point.x, point.y, m_nRadius);
			
			m_nPoints[m_nClickCount].x = point.x + m_nRadius;
			m_nPoints[m_nClickCount].y = point.y + m_nRadius;


			CString str;
			str.Format(_T("%d , %d"), m_nPoints[m_nClickCount].x, m_nPoints[m_nClickCount].y);
			SetDlgItemText(IDC_STATIC_CENTERXY, str);

			UpdateData(FALSE);

			m_nClickCount++;

			if (m_nClickCount == 3)
			{
				cout << m_nClickCount << endl;
				makeGarden(m_nPoints, fm);
			}
		}
		
		if (m_nClickCount == 3)
		{
			for (int i = 0; i < 3; ++i)
			{
				int dx = point.x - m_nPoints[i].x;
				int dy = point.y - m_nPoints[i].y;
				if (dx * dx + dy * dy <= m_nRadius * m_nRadius)
				{
					m_bIsDragging = true;
					m_nDragIndex = i;
					m_ptDragOffset = CPoint(dx, dy);
					break;
				}
			}
		}
	}
	else
	{
		AfxMessageBox(_T("영역 안에서 클릭해주세요."));
	}
	

}

void CmfcExamDlg::makeCircle(unsigned char* fm, int x, int y, int nRadius)
{
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++)
	{
		for (int i = x; i < x + nRadius * 2; i++)
		{
			if (i >= 0 && i < nPitch && j >= 0 && j < BACKGROUND_HEIGHT)
			{
				if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
				{
					fm[j * nPitch + i] = 255;
				}
			}
		}
	}

	UpdateDisplay();
}

bool CmfcExamDlg::isInCircle(int x, int y, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = x - nCenterX;
	double dY = y - nCenterY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius)
	{
		bRet = true;
	}

	return bRet;
}

void CmfcExamDlg::inItBackground()
{
	//UpdateData(TRUE);
	PostMessage(CUSTOM_UPDATEDATA, 0, 0);
	int nBpp = 8;


	m_image.Create(NWIDTH, -BACKGROUND_HEIGHT, nBpp);
	if (nBpp == 8)
	{
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0, NWIDTH * BACKGROUND_HEIGHT);

	UpdateDisplay();
}

void CmfcExamDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

void CmfcExamDlg::makeGarden(CPoint Points[3], unsigned char* fm)
{
	PostMessage(CUSTOM_UPDATEDATA, 0, 0);
	//UpdateData(TRUE);

	int nPitch = m_image.GetPitch();

	double x1 = Points[0].x, y1 = Points[0].y;
	double x2 = Points[1].x, y2 = Points[1].y;
	double x3 = Points[2].x, y3 = Points[2].y;

	cout << "Points : " << x1 << " , " << y1 << endl;
	cout << "Points : " << x2 << " , " << y2 << endl;
	cout << "Points : " << x3 << " , " << y3 << endl;


	double dDeterminant = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	double dcenterX = ((x1 * x1 + y1 * y1) * (y2 - y3) +
						(x2 * x2 + y2 * y2) * (y3 - y1) +
						(x3 * x3 + y3 * y3) * (y1 - y2)) / dDeterminant;

	double dcenterY = ((x1 * x1 + y1 * y1) * (x3 - x2) +
						(x2 * x2 + y2 * y2) * (x1 - x3) +
						(x3 * x3 + y3 * y3) * (x2 - x1)) / dDeterminant;

	double dRadius = sqrt((x1 - dcenterX) * (x1 - dcenterX) +
							(y1 - dcenterY) * (y1 - dcenterY));



	int nThreads = std::thread::hardware_concurrency();
	if (nThreads == 0) nThreads = 4;

	cout << "nThreads : " << nThreads << endl;

	int rowsPerThread = BACKGROUND_HEIGHT / nThreads;
	std::vector<std::thread> threads;

	for (int i = 0; i < nThreads; i++) 
	{
		int startRow = i * rowsPerThread;
		int endRow = (i == nThreads - 1) ? BACKGROUND_HEIGHT : startRow + rowsPerThread;
		threads.emplace_back(processRows, fm, nPitch, startRow, endRow, dcenterX, dcenterY, dRadius, m_dThickness);
	}


	for (auto& th : threads) th.join();


	UpdateDisplay();
}

void CmfcExamDlg::makeAll()
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0, NWIDTH * BACKGROUND_HEIGHT);

	for (int i = 0; i < 3; i++)
	{
		makeCircle(fm, m_nPoints[i].x, m_nPoints[i].y, m_nRadius);
	}

	makeGarden(m_nPoints, fm);

	UpdateDisplay();
}

void CmfcExamDlg::clearAll()
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	memset(fm, 255, NWIDTH * BACKGROUND_HEIGHT);
	for (int i = 0; i < 3; i++)
	{
		m_nPoints[i].x = 0;
		m_nPoints[i].y = 0;
	}

	m_nClickCount = 0;
	m_nDragIndex = -1;

	CString str;
	str.Format(_T("0 , 0"));
	SetDlgItemText(IDC_STATIC_CENTERXY, str);

	UpdateDisplay();

	m_image.Destroy();
}


void CmfcExamDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);

	m_bIsDragging = false;
	m_nDragIndex = -1;

	if (m_nClickCount > 2)
	{
		//makeAll();
	}
}

void CmfcExamDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);


	if (m_bIsDragging && m_nDragIndex != -1)
	{
		cout << "Move TRUE" << endl;

		m_nPoints[m_nDragIndex].x = point.x - m_ptDragOffset.x + m_nRadius;
		m_nPoints[m_nDragIndex].y = point.y - m_ptDragOffset.y + m_nRadius;

		makeAll();
	}

}

void CmfcExamDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	clearAll();
}

void CmfcExamDlg::OnBnClickedBtnMoverandom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nClickCount == 3)
	{

		thread t(ThreadFunc, this);
		t.detach();
	}
}

int& CmfcExamDlg::getRadius()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return m_nRadius;
}



CImage& CmfcExamDlg::getimage()
{
	return m_image;
}

LRESULT CmfcExamDlg::ForCustomMessageFromThread(WPARAM wParam, LPARAM lParam)
{
	UpdateData(true);
	return 0;
}