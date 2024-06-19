/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

// Drawing.cpp
/// \file
/// Drawing Android implementation

#include "../FreetypeRenderer/FreetypeGraphicRenderer.h"
#include "Drawing.h"
#include "Platform.h"

FreetypeGraphicRenderer* g_pFreetypeRenderer = NULL;

static AkUInt32 g_iWidth;
static AkUInt32 g_iHeight;

AkUInt32 GetWindowWidth()
{
	return g_iWidth;
}

AkUInt32 GetWindowHeight()
{
	return g_iHeight;
}

/// Initializes the system's drawing engine for usage.
/// \return True if the system has been initialized, false otherwise.
/// \sa TermDrawing()
bool InitDrawing(
	void* in_pParam,						///< - Pointer to any platform specific parameter (eg. the window handle in Windows)
	AkOSChar* in_szErrorBuffer,				///< - Buffer where error details will be written (if the function returns false)
	unsigned int in_unErrorBufferCharCount,	///< - Number of characters available in in_szErrorBuffer, including terminating NULL character
	AkUInt32 in_windowWidth,			///< - window widht in pixels
	AkUInt32 in_windowHeight			///< - window height in pixels
)
{
	g_pFreetypeRenderer = new FreetypeGraphicRenderer;

	g_iWidth = in_windowWidth;
	g_iHeight = in_windowHeight;

	return g_pFreetypeRenderer->InitDrawing(in_pParam,
			in_szErrorBuffer,
			in_unErrorBufferCharCount,
			in_windowWidth,
			in_windowHeight);
}


/// Begins a drawing sequence
/// \sa DoneDrawing()
void BeginDrawing()
{
	if( g_pFreetypeRenderer == NULL )
		return;

	g_pFreetypeRenderer->BeginDrawing();
}


/// Draws a string of text on the screen at a given point.
/// \note The coordinates (0, 0) refer to the top-left corner of the screen.
/// \warning This function must be called between calls to BeginDrawing and DoneDrawing.
/// \sa BeginDrawing(), DoneDrawing()
void DrawTextOnScreen(
	const char* in_szText,	///< - The string to draw on the screen
	int in_iXPos,			    ///< - The X value of the drawing position
	int in_iYPos,			    ///< - The Y value of the drawing position
	DrawStyle in_eDrawStylel		///< - The style with which to draw the text
)
{
	if( g_pFreetypeRenderer == NULL )
		return;

	g_pFreetypeRenderer->DrawTextOnScreen(in_szText, in_iXPos, in_iYPos, in_eDrawStylel);
}

void DrawLineOnScreen(
	int in_iXPosStart,		    ///< - The X value of the drawing position
	int in_iYPosStart,		    ///< - The Y value of the drawing position
	int in_iXPosEnd,		    ///< - The X value of the drawing position
	int in_iYPosEnd,		    ///< - The Y value of the drawing position
	DrawStyle in_eDrawStyle ///< - The style with which to draw the text
)
{
	// no impl
}

/// Ends the drawing sequence and outputs the drawing.
/// \sa BeginDrawing()
void DoneDrawing()
{
	if( g_pFreetypeRenderer == NULL )
		return;

	// On Nexus 9, font metrics actually cause the characters to exceed line height. So allow for more room than a single line height to avoid a segfault.
	int y = g_iHeight - GetLineHeight( DrawStyle_Error ) * 4;
	g_pFreetypeRenderer->DrawTextOnScreen("Please press menu to show soft keyboard", g_iWidth / 4, y, DrawStyle_Error);

	g_pFreetypeRenderer->DoneDrawing();
}


/// Closes the drawing engine and releases any used resources.
/// \sa InitDrawing()
void TermDrawing()
{
	if( g_pFreetypeRenderer != NULL )
	{
		g_pFreetypeRenderer->TermDrawing();
		delete g_pFreetypeRenderer;
		g_pFreetypeRenderer = NULL;
	}
}


/// Gets the height of a line of text drawn using the given style.
/// \return The height of a line of text, in pixels.
int GetLineHeight(
	DrawStyle in_eDrawStyle		///< - The style of the text being queried
)
{
	if( g_pFreetypeRenderer == NULL )
		return 0;

	return g_pFreetypeRenderer->GetLineHeight(in_eDrawStyle);
}

