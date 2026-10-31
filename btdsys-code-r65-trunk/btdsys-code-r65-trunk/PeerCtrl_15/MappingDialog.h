#pragma once
#include "ctrl.h"
#include "envelope.h"


// CMappingDialog dialog

class CMappingDialog : public CDialog
{
	DECLARE_DYNAMIC(CMappingDialog)

public:
	CMappingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMappingDialog();

// Dialog Data
	enum { IDD = IDD_MAPPINGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEnvelope m_envelope;
	CEnvData m_envdata;
	CEnvData* m_mapping;

	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void EnvelopeChangedMappingenvelope(long numPoints, long SustainPoint, long* Points);
};
