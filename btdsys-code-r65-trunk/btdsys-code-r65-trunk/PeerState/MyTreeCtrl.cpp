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

// MyTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerState.h"
#include "MyTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl

CMyTreeCtrl::CMyTreeCtrl()
{
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CMyTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl message handlers

HTREEITEM CMyTreeCtrl::InsertItem(int state,
	LPCTSTR lpszItem,
	HTREEITEM hParent /*= TVI_ROOT*/,
	HTREEITEM hInsertAfter /*= TVI_LAST*/)
{
	HTREEITEM h = CTreeCtrl::InsertItem(lpszItem, hParent, hInsertAfter);
	SetItemState(h,INDEXTOSTATEIMAGEMASK(state),TVIS_STATEIMAGEMASK);
	return h;
}

HTREEITEM CMyTreeCtrl::InsertItem(LPCTSTR lpszItem,
	HTREEITEM hParent /*= TVI_ROOT*/,
	HTREEITEM hInsertAfter /*= TVI_LAST*/)
{
	return InsertItem(MYCH_UNCHECKED,lpszItem,hParent,hInsertAfter);
}

void CMyTreeCtrl::SetState(HTREEITEM hItem, int state)
{	
	SetItemState(hItem,INDEXTOSTATEIMAGEMASK(state),TVIS_STATEIMAGEMASK);
}

int CMyTreeCtrl::GetState(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
}

void CMyTreeCtrl::SetCheck(HTREEITEM hItem, int action, bool bIgnoreParent /*= false*/)
{
	if (hItem && hItem != TVI_ROOT)
	{
		switch (action)
		{
		case MYSETCH_CHECK:
			{
				SetState(hItem, MYCH_CHECKED);
				HTREEITEM h = GetChildItem(hItem);
				while (h)
				{
					SetCheck(h,MYSETCH_CHECK,true);
					h = GetNextSiblingItem(h);
				}
				if (!bIgnoreParent)
					SetCheck(GetParentItem(hItem), MYSETCH_REFRESH);
			}
			break;

		case MYSETCH_UNCHECK:
			{
				SetState(hItem, MYCH_UNCHECKED);
				HTREEITEM h = GetChildItem(hItem);
				while (h)
				{
					SetCheck(h,MYSETCH_UNCHECK,true);
					h = GetNextSiblingItem(h);
				}
				if (!bIgnoreParent)
					SetCheck(GetParentItem(hItem), MYSETCH_REFRESH);
			}
			break;

		case MYSETCH_TOGGLE:
			switch (GetState(hItem))
			{
			case MYCH_CHECKED:
			case MYCH_GRAYED:
				SetCheck(hItem, MYSETCH_UNCHECK, bIgnoreParent);
				break;
			case MYCH_UNCHECKED:
				SetCheck(hItem, MYSETCH_CHECK, bIgnoreParent);
				break;
			}
			break;

		case MYSETCH_REFRESH:
			{
				HTREEITEM h = GetChildItem(hItem);
				if (!h) return;

				bool bHasCheckedKids = false;
				bool bAllCheckedKids = true;

				while (h)
				{
					switch (GetState(h))
					{
					case MYCH_CHECKED:
					case MYCH_GRAYED:
						bHasCheckedKids = true;
						break;
					case MYCH_UNCHECKED:
						bAllCheckedKids = false;
						break;
					}
					
					h = GetNextSiblingItem(h);
				}

				if (bAllCheckedKids)
					SetState(hItem, MYCH_CHECKED);
				else if (bHasCheckedKids)
					SetState(hItem, MYCH_GRAYED);
				else
					SetState(hItem, MYCH_UNCHECKED);
			}
			break;
		}
	}
}

void CMyTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point,&uFlags);

	if( uFlags & TVHT_ONITEMSTATEICON )
	{
		SetCheck( hItem, MYSETCH_TOGGLE );
		return;
	}
	else
		CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CMyTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE)
	{
		HTREEITEM hItem = GetSelectedItem();
		SetCheck(hItem, MYSETCH_TOGGLE);
	}
	else
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyTreeCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_imageState.Create(IDB_STATE, 11, 1, 0xFFFFFF);
	SetImageList( &m_imageState, TVSIL_STATE );

	CTreeCtrl::PreSubclassWindow();
}
