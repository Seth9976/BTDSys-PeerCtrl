/*
Copyright (c) 2003-2006, Edward Powley
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of BTDSys nor the names of its contributors may be used
      to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "stdafx.h"
#include "groupie.h"
#include "MacView.h"
#include "BuzzMI.h"
#include "MacViewFrame.h"
#include "AddMachineDlg.h"

void CMacView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect r;
	GetClientRect(&r);

	switch (m_nDragMode)
	{
	case move:
		{
			float dx = (float)(point.x - m_ptDrag.x) / (float)r.Width() * 2.0f;
			float dy = (float)(point.y - m_ptDrag.y) / (float)r.Height() * 2.0f;
			for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
				 pp < m_pFrame->pmi->vpMacs.end(); pp++)
			{
				CMachine *p = *pp;
				if (p->bSelected)
				{
					p->fX += dx;
					p->fY += dy;
				}
			}

			RedrawWindow();
			m_ptDrag = point;
		}
		break;

	case conn:
		m_ptDrag = point;
		RedrawWindow();
		break;

	case connvol:
		m_pContextMenuTarget.c->fAmp = m_fDragOrigAmp - (float)(point.y - m_ptDrag.y) / (float)AMPPANBOX_H;
		if (m_pContextMenuTarget.c->fAmp < 0.0f) m_pContextMenuTarget.c->fAmp = 0.0f;
		if (m_pContextMenuTarget.c->fAmp > 1.0f) m_pContextMenuTarget.c->fAmp = 1.0f;

		CMachine *ptarg = m_pContextMenuTarget.c->pMac2;
		if (ptarg->bSpecial || (ptarg->IsMDK() && ptarg->bStereoOut))
		{
			m_pContextMenuTarget.c->fPan = m_fDragOrigPan + (float)(point.x - m_ptDrag.x) / (float)AMPPANBOX_W * 2.0f;
			if (m_pContextMenuTarget.c->fPan < 0.0f) m_pContextMenuTarget.c->fPan = 0.0f;
			if (m_pContextMenuTarget.c->fPan > 2.0f) m_pContextMenuTarget.c->fPan = 2.0f;
		}
		else m_pContextMenuTarget.c->fPan = 1.0f;

		if (nFlags & MK_SHIFT) m_pContextMenuTarget.c->fPan = m_fDragOrigPan;
		if (nFlags & MK_CONTROL) m_pContextMenuTarget.c->fAmp = m_fDragOrigAmp;

		RedrawWindow();
		break;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMacView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMachine *pMac = NULL;

	for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
		 pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *p = *pp;
		if (p->rect.PtInRect(point))
			pMac = p;
	}

	if (pMac)
	{
		if (nFlags & MK_SHIFT)
		{
			m_nDragMode = conn;
			m_pDragConnMac = pMac;
			m_ptDrag = point;

			RedrawWindow();
		}
		else
		{
			if (nFlags & MK_CONTROL)
				pMac->bSelected = true;

			if (!pMac->bSelected)
			{
				for (pp = m_pFrame->pmi->vpMacs.begin(); 
					 pp < m_pFrame->pmi->vpMacs.end(); pp++)
				{
					(*pp)->bSelected = false;
				}

				pMac->bSelected = true;
			}

			m_ptDrag = point;
			m_nDragMode = move;

			RedrawWindow();
		}
	}
	else
	{
		for (pp = m_pFrame->pmi->vpMacs.begin(); 
			 pp < m_pFrame->pmi->vpMacs.end(); pp++)
		{
			(*pp)->bSelected = false;
		}

		CConnection *pCon = NULL;
		for (vector<CConnection>::iterator p = m_pFrame->pmi->vConns.begin();
			 p < m_pFrame->pmi->vConns.end(); p++)
		{
			if (p->rect.PtInRect(point))
				pCon = p;
		}

		if (pCon)
		{
			m_pContextMenuTarget.c = pCon;
			m_fDragOrigAmp = pCon->fAmp;
			m_fDragOrigPan = pCon->fPan;
			m_ptDrag = point;
			m_nDragMode = connvol;
			RedrawWindow();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMacView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch (m_nDragMode)
	{
	case conn:
		{
			CMachine *pMac = NULL;

			for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
				 pp < m_pFrame->pmi->vpMacs.end(); pp++)
			{
				CMachine *p = *pp;
				if (p->rect.PtInRect(point))
					pMac = p;
			}

			if (pMac)
			{
				if (!m_pFrame->pmi->TryConnectMachines(m_pDragConnMac, pMac))
				{
					MessageBox("No connection could be made");
				}
			}
		}
		break;
	}

	m_nDragMode = none;
	RedrawWindow();

	CWnd::OnLButtonUp(nFlags, point);
}

void CMacView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CMachine *pMac = NULL;

	for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
		 pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *p = *pp;
		if (p->rect.PtInRect(point))
			pMac = p;
	}

	if (!pMac)
	{
		CAddMachineDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_szMacName[0])
			{
				CMachine *pm = m_pFrame->pmi->TryAddMachine(dlg.m_szMacName);
				if (pm)
				{
					CRect r;
					GetClientRect(&r);

					pm->fX = ((float)point.x / (float)r.Width()  - 0.5f) * 2.0f;
					pm->fY = ((float)point.y / (float)r.Height() - 0.5f) * 2.0f;

					RedrawWindow();
				}				
				else
					MessageBox("Failed to add the machine");
			}
		}
	}
	else
	{
		m_pContextMenuTarget.m = pMac;
		OnMachineParameters();
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CMacView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMachine *pMac = NULL;

	for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
		 pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *p = *pp;
		if (p->rect.PtInRect(point))
			pMac = p;
	}

	if (pMac)
	{
		if (!pMac->bSelected)
		{
			for (pp = m_pFrame->pmi->vpMacs.begin(); 
				 pp < m_pFrame->pmi->vpMacs.end(); pp++)
			{
				(*pp)->bSelected = false;
			}

			pMac->bSelected = true;
		}

		RedrawWindow();

		m_pContextMenuTarget.m = pMac;
		CMenu mPopup;
		mPopup.LoadMenu(IDR_POPUPS);

		HMENU hMenu = GetSubMenu(mPopup.m_hMenu, 0);

		if (!pMac->IsMDK())
			EnableMenuItem(hMenu,6,MF_BYPOSITION | MF_GRAYED);
		else
		{
			EnableMenuItem(hMenu,6,MF_BYPOSITION | MF_ENABLED);
			HMENU hMode = GetSubMenu(hMenu,6);
			CheckMenuItem(hMode,ID_MACHINE_MDKWORKMODE_MONO,
				MF_BYCOMMAND | (pMac->bStereoOut ? MF_UNCHECKED : MF_CHECKED));
			CheckMenuItem(hMode,ID_MACHINE_MDKWORKMODE_STEREO,
				MF_BYCOMMAND | (pMac->bStereoOut ? MF_CHECKED : MF_UNCHECKED));
		}

		//custom context menu items
		char menuitem[256];
		char *q = menuitem;
		char const *p = pMac->pInfo->Commands;
		int id = ID_MACHINE_MENUITEM_ZERO;
		int submenunum = 1;
		while (p && *p)
		{
			if (*p == '\n')
			{
				*q = 0;
				if (menuitem[0] != '/')
				{
					AppendMenu(hMenu, MF_STRING, id, menuitem);
				}
				else
				{
					HMENU hSub = CreateMenu();
					pMac->GetSubMenuItems(hSub,id - ID_MACHINE_MENUITEM_ZERO, ID_MACHINE_MENUITEM_ZERO+submenunum*256);
					submenunum++;
					AppendMenu(hMenu, MF_STRING | MF_POPUP, (int)hSub, menuitem+1);
				}

				id++;
				q = menuitem;
			}
			else
				*q++ = *p;

			p++;
		}
		if (q != menuitem)
		{
			*q = 0;
			AppendMenu(hMenu, MF_STRING, id, menuitem);
		}

		if (pMac->bSpecial && !pMac->IsThruMachine()) //some actions don't apply to special machines
		{
			EnableMenuItem(hMenu,ID_MACHINE_DELETE,MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu,ID_MACHINE_RENAME,MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu,ID_MACHINE_PARAMETERS,MF_BYCOMMAND | MF_GRAYED);
			EnableMenuItem(hMenu,ID_MACHINE_ATTRIBUTES,MF_BYCOMMAND | MF_GRAYED);
		}


		ClientToScreen(&point);
		TrackPopupMenu(hMenu,
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			point.x, point.y, 0,
			m_hWnd,
			NULL);
	}
	else
	{
		CConnection *pCon = NULL;
		for (vector<CConnection>::iterator p = m_pFrame->pmi->vConns.begin();
			 p < m_pFrame->pmi->vConns.end(); p++)
		{
			if (p->rect.PtInRect(point))
				pCon = p;
		}

		if (pCon)
		{
			m_pContextMenuTarget.c = pCon;
			CMenu mPopup;
			mPopup.LoadMenu(IDR_POPUPS);

			ClientToScreen(&point);
			TrackPopupMenu(GetSubMenu(mPopup.m_hMenu, 1),
				TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				point.x, point.y, 0,
				m_hWnd,
				NULL);
		}
		else
		{
			OnLButtonDblClk(nFlags,point);
//			MessageBox("Please right-click a machine or connection for a context menu, or doubleclick empty space "
//				"to add a machine! (An index.txt style menu will be added later)");
		}
	}
	
	CWnd::OnRButtonUp(nFlags, point);
}
