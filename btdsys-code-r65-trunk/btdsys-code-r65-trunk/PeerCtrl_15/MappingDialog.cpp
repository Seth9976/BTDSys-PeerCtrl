// MappingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerCtrl.h"
#include "MappingDialog.h"


// CMappingDialog dialog

IMPLEMENT_DYNAMIC(CMappingDialog, CDialog)

CMappingDialog::CMappingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMappingDialog::IDD, pParent)
{

}

CMappingDialog::~CMappingDialog()
{
}

void CMappingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAPPINGENVELOPE, m_envelope);
}


BEGIN_MESSAGE_MAP(CMappingDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMappingDialog message handlers

void CMappingDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_envelope.SetWindowPos(NULL, 0,0, cx,cy, SWP_NOZORDER);

}

BOOL CMappingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_envelope.SetEnvelope(m_envdata.numpoints(), -1, &m_envdata.points[0]);

	return TRUE;
}
BEGIN_EVENTSINK_MAP(CMappingDialog, CDialog)
	ON_EVENT(CMappingDialog, IDC_MAPPINGENVELOPE, 1, CMappingDialog::EnvelopeChangedMappingenvelope, VTS_I4 VTS_I4 VTS_PI4)
END_EVENTSINK_MAP()

void CMappingDialog::EnvelopeChangedMappingenvelope(long numPoints, long SustainPoint, long* Points)
{
	m_envdata.Set(numPoints, Points);
	m_mapping->Set(numPoints, Points);
}
