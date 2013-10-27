// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#include "stdafx.h"
#include "framerenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(FrameRenderer, RendererBase)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

FrameRenderer::FrameRenderer(wxFrame *frame) : RendererBase(frame)
{

}

FrameRenderer::~FrameRenderer()
{

}

wxSize FrameRenderer::getMinSize() const
{
	wxSize minSize;
	minSize.x = m_imageTopLeft.GetWidth() + m_imageTopRight.GetWidth() + 1;
	minSize.y = m_imageTopLeft.GetHeight() + m_imageBottomLeft.GetHeight() + 1;

	return minSize;
}

void FrameRenderer::setTopLeftImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageTopLeft = image;
	if(m_imageTopLeft.HasAlpha())
		m_imageTopLeft.ConvertAlphaToMask();
}

void FrameRenderer::setTopImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageTop = image;
	if(m_imageTop.HasAlpha())
		m_imageTop.ConvertAlphaToMask();
}

void FrameRenderer::setTopRightImage(const wxImage &image)
{
	m_imageTopRight = image;
	if(m_imageTopRight.HasAlpha())
		m_imageTopRight.ConvertAlphaToMask();
}

void FrameRenderer::setBodyLeftImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBodyLeft = image;
	if(m_imageBodyLeft.HasAlpha())
		m_imageBodyLeft.ConvertAlphaToMask();
}

void FrameRenderer::setBodyImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBody = image;
	if(m_imageBody.HasAlpha())
		m_imageBody.ConvertAlphaToMask();
}

void FrameRenderer::setBodyRightImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBodyRight = image;
	if(m_imageBodyRight.HasAlpha())
		m_imageBodyRight.ConvertAlphaToMask();
}

void FrameRenderer::setBottomLeftImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBottomLeft = image;
	if(m_imageBottomLeft.HasAlpha())
		m_imageBottomLeft.ConvertAlphaToMask();
}

void FrameRenderer::setBottomImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBottom = image;
	if(m_imageBottom.HasAlpha())
		m_imageBottom.ConvertAlphaToMask();
}

void FrameRenderer::setBottomRightImage(const wxImage &image)
{
	OS_ASSERT(image.IsOk());

	m_imageBottomRight = image;
	if(m_imageBottomRight.HasAlpha())
		m_imageBottomRight.ConvertAlphaToMask();
}
/*
void FrameRenderer::refreshLayout(bool invalidate)
{
	// TODO: se invalidate == false dovrebbe effettuare delle ottimizzazioni per evitare di ricarcolare tutta la nuova area

	wxSize size = getWindow()->GetClientSize();

	wxRegion windowRegion(0, 0, size.x, size.y);

	wxRegion regionTopLeft(m_imageTopLeft, wxColour(255, 0, 255));
	regionTopLeft.Xor(regionTopLeft.GetBox());
	windowRegion.Subtract(regionTopLeft);

	wxRegion regionBottomLeft(m_imageBottomLeft, wxColour(255, 0, 255));
	regionBottomLeft.Xor(regionBottomLeft.GetBox());
	regionBottomLeft.Offset(0, size.y - m_imageBottomLeft.GetHeight());
	windowRegion.Subtract(regionBottomLeft);

	wxRegion regionTopRight(m_imageTopRight, wxColour(255, 0, 255));
	regionTopRight.Xor(regionTopRight.GetBox());
	regionTopRight.Offset(size.x - m_imageTopRight.GetWidth(), 0);
	windowRegion.Subtract(regionTopRight);

	wxRegion regionBottomRight(m_imageBottomRight, wxColour(255, 0, 255));
	regionBottomRight.Xor(regionBottomRight.GetBox());
	regionBottomRight.Offset(size.x - m_imageBottomRight.GetWidth(), size.y - m_imageBottomRight.GetHeight());
	windowRegion.Subtract(regionBottomRight);

	setWindowShape(windowRegion);

	if(invalidate)
		resizeWindow();

	getWindow()->Update();
	getWindow()->Refresh(false);
	getWindow()->Layout();
}
*/

void FrameRenderer::renderFrame(wxDC *dc)
{
	dc->SetBackgroundMode(wxTRANSPARENT);

	wxSize size = getWindow()->GetClientSize();

	int32 widthTop = size.x;
	if(m_imageTopLeft.IsOk())
		widthTop -= m_imageTopLeft.GetWidth();
	if(m_imageTopRight.IsOk())
		widthTop -= m_imageTopRight.GetWidth();
	if(widthTop <= 0)
		widthTop = 1;

	int32 widthBottom = size.x;
	if(m_imageBottomLeft.IsOk())
		widthBottom -= m_imageBottomLeft.GetWidth();
	if(m_imageBottomRight.IsOk())
		widthBottom -= m_imageBottomRight.GetWidth();
	if(widthBottom <= 0)
		widthBottom = 1;

	int32 heightLeft = size.y;
	if(m_imageTopLeft.IsOk())
		heightLeft -= m_imageTopLeft.GetHeight();
	if(m_imageBottomLeft.IsOk())
		heightLeft -= m_imageBottomLeft.GetHeight();
	if(heightLeft <= 0)
		heightLeft = 1;

	int32 heightRight = size.y;
	if(m_imageTopRight.IsOk())
		heightRight -= m_imageTopRight.GetHeight();
	if(m_imageBottomRight.IsOk())
		heightRight -= m_imageBottomRight.GetHeight();
	if(heightRight <= 0)
		heightRight = 1;

	// Top

	if(m_imageTopLeft.IsOk())
		dc->DrawBitmap(m_imageTopLeft, 0, 0, true);
	if(m_imageTop.IsOk())
		dc->DrawBitmap(m_imageTop.Scale(widthTop, m_imageTop.GetHeight()), m_imageTopLeft.GetWidth(), 0, true);
	if(m_imageTopRight.IsOk())
		dc->DrawBitmap(m_imageTopRight, widthTop + m_imageTopLeft.GetWidth(), 0, true);

	// Body

	if(m_imageBodyLeft.IsOk())
		dc->DrawBitmap(m_imageBodyLeft.Scale(m_imageBodyLeft.GetWidth(), heightLeft), 0, m_imageTopLeft.GetHeight(), true);
	if(m_imageBody.IsOk())
		dc->DrawBitmap(m_imageBody.Scale(widthTop, heightLeft), m_imageTopLeft.GetWidth(), m_imageTop.GetHeight(), true);
	if(m_imageBodyRight.IsOk())
		dc->DrawBitmap(m_imageBodyRight.Scale(m_imageBodyRight.GetWidth(), heightRight), size.x - m_imageBodyRight.GetWidth(), m_imageTopRight.GetHeight(), true);

	// Bottom

	if(m_imageBottomLeft.IsOk())
		dc->DrawBitmap(m_imageBottomLeft, 0, size.y - m_imageBottomLeft.GetHeight(), true);
	if(m_imageBottom.IsOk())
		dc->DrawBitmap(m_imageBottom.Scale(widthBottom, m_imageBottom.GetHeight()), m_imageBottomLeft.GetWidth(), size.y - m_imageBottom.GetHeight(), true);
	if(m_imageBottomRight.IsOk())
		dc->DrawBitmap(m_imageBottomRight, widthBottom + m_imageBottomLeft.GetWidth(), size.y - m_imageBottomRight.GetHeight(), true);
}

void FrameRenderer::onPaint(wxPaintEvent &e)
{
	wxBufferedPaintDC dc(getWindow());
	renderFrame(&dc);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
