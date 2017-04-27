// class CGridSFF implementation file

#include "stdafx.h"
#include "GridSFF.h"

// ###################################################################
// CGridSFF GridCtrl
// ###################################################################
CGridSFF::CGridSFF()
{

}

CGridSFF::~CGridSFF()
{
}

void CGridSFF::Init()
{
	// Define Grid table common parameters 
	Config();

	// Set
	this->SetVirtualMode(m_bVirtualMode);
	this->SetEditable(m_bEditable);

	this->SetAutoSizeStyle();

	// Fill table with cells
	TRY
	{
		this->SetRowCount(m_nRows);
		this->SetColumnCount(m_nCols);
		this->SetFixedRowCount(m_nFixRows);
		this->SetFixedColumnCount(m_nFixCols);
	}
		CATCH(CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

		// > Set table appropriate size
		// header
		this->SetColumnWidth(0, 30);

	// cells
	for (int col = 1; col < this->GetColumnCount(); col++)
	{
		this->SetColumnWidth(col, 25);
	}


	// fill rows/cols with text
	for (int row = 0; row < this->GetRowCount(); row++)
	{
		for (int col = 0; col < this->GetColumnCount(); col++)
		{
			CString str;

			GV_ITEM Item;

			Item.mask = GVIF_TEXT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows)						// Title Fixed Row
			{
				if (col > 0)
				{
					str.Format(_T("%02X"), col - 1);
				}
			}
			else
			{
				if (col < m_nFixCols)					// Title Fixed Col
				{
					if (row > 0)
					{
						str.Format(_T("%02X"), row - 1);
						str.AppendChar('h');
					}
					else
					{
						str = "";
					}
				}
			}

			Item.strText = str;

			/*
			if (rand() % 10 == 1)
			{
			COLORREF clr = RGB(rand() % 128 + 128, rand() % 128 + 128, rand() % 128 + 128);
			Item.crBkClr = clr;             // or - m_Grid.SetItemBkColour(row, col, clr);
			Item.crFgClr = RGB(255, 0, 0);    // or - m_Grid.SetItemFgColour(row, col, RGB(255,0,0));
			Item.mask |= (GVIF_BKCLR | GVIF_FGCLR);
			}

			if (col < this->GetFixedColumnCount())
			{
			//Item.iImage = rand() % m_ImageList.GetImageCount();
			Item.mask |= (GVIF_IMAGE);
			}
			*/
			this->SetItem(&Item);

		}
	}
	/*
	// Having weird Autosize problems with CE 2.11 - possibly just an emulation problem
	#if !defined(_WIN32_WCE) || (_WIN32_WCE < 211)
	m_Grid.AutoSize();
	#endif
	*/
}

void CGridSFF::Config()
{
	m_OldSize = CSize(-1, -1);

	//{{AFX_DATA_INIT(CGridCtrlDemoDlg)
	m_nFixCols = 1;
	m_nFixRows = 1;
	m_nCols = 16 + 1;							// +1 is for Fixed Cell
	m_nRows = 16 + 1;							// +1 is for Fixed Cell

	m_bEditable = TRUE;
	m_bHorzLines = FALSE;
	m_bVertLines = FALSE;
	m_bListMode = TRUE;
	m_bHeaderSort = FALSE;
	m_bSingleSelMode = TRUE;
	m_bSingleColSelMode = TRUE;
	m_bSelectable = FALSE;
	m_bAllowColumnResize = FALSE;
	m_bAllowRowResize = FALSE;
	m_bItalics = FALSE;
	m_btitleTips = FALSE;
	m_bTrackFocus = FALSE;
	m_bFrameFocus = FALSE;
	m_bVirtualMode = FALSE;
	m_bCallback = TRUE;
	m_bVertical = TRUE;
	m_bExpandUseFixed = FALSE;
	m_bRejectEditAttempts = TRUE;
	m_bRejectEditChanges = FALSE;
	m_bRow2Col2Hidden = FALSE;
}

// Use: Update Grid[0xFF x 0xFF] Interface 
// FROM: startAddr TO: startAddr + count 
// with Memory Values[0xFF x 0xFF] 
// FROM: startAddr TO: startAddr + count
void CGridSFF::GridSFF_Write_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%02X"), v_ByteData[startAddr + k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

void CGridSFF::GridSFF_Write(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%02X"), v_ByteData[k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

void CGridSFF::GridSFF_Write_Byte(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// fill in table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// forming Cell Text
			CString cellText;
			cellText.Format(_T("%c"), v_ByteData[startAddr + k]);
			this->SetItemText(uRow, uCol, cellText);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

		this->Invalidate();
	} //SafeCheck
}

// Use: Update Memory Values
// FROM: startAddr TO: startAddr + count
// with Grid Values
// FROM: startAddr TO: startAddr + count
void CGridSFF::GridSFF_Read_UpdateRange(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// get from Table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// extract Cell Text
			CString cellText = this->GetItemText(uRow, uCol);
			v_ByteData[startAddr + k] = (BYTE)_tcstoul(cellText, NULL, 16);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

	}
}

// Use: simple Read Grid OP
// FROM: startAddr TO: startAddr + count
// in Memory
// FROM: 0 TO: count
void CGridSFF::GridSFF_Read(BYTE * v_ByteData, BYTE startAddr, WORD count)
{
	// Safe check
	if (startAddr + count > 256)
	{
		// out of range
		return;
	}
	else
	{
		// define table start cell
		unsigned char stRow = startAddr / 0x10;				// start Row
		unsigned char stCol = startAddr - stRow * 0x10;		// start Col

															// get from Table
		unsigned char uRow = stRow + 1;						// +1 is for Fixed Cell
		unsigned char uCol = stCol + 1;						// +1 is for Fixed Cell

		unsigned int k = 0;

		bool act = 1;
		do
		{
			// extract Cell Text
			CString cellText = this->GetItemText(uRow, uCol);
			v_ByteData[k] = (BYTE)_tcstoul(cellText, NULL, 16);

			// prepare next cell coord	/NOTE: useless at last cycle
			uCol++;

			if (uCol > 0x0F + 1)							// +1 is for Fixed Cell
			{
				uCol = 0 + 1;								// +1 is for Fixed Cell
				uRow++;										// can't exceed 0x0F val cause of #SafeCheck
			}

			k++;

		} while (k < count);

	}
}

int CGridSFF::CheckValidHex(int iRow, int iCol)
{
	// error symbol at first
	bool act = 1;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() == 2)
	{
		// check valid Symbols
		CString hexString = cellText.SpanIncluding(_T("0123456789abcdefABCDEF"));

		if (hexString.Compare(cellText) == 0)
		{
			act = 0;

			// Change to upper case
			this->SetItemText(iRow, iCol, cellText.MakeUpper());
		}
	}

	return act;
}

int CGridSFF::CheckValidASCII(int iRow, int iCol)
{
	bool act = 0;

	CString cellText = this->GetItemText(iRow, iCol);

	// check valid Length
	if (cellText.GetLength() != 1)
	{
		// error symbol
		act = 1;
	}

	return act;
}

