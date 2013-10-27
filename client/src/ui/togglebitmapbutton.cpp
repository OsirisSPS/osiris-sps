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
#include "togglebitmapbutton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ToggleBitmapButton::ToggleBitmapButton(wxWindow *parent, wxWindowID id, const wxBitmap &bitmap, const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator, const wxString &name) : ButtonBase(parent, id, bitmap, pos, size, style, validator, name),
																																																		  m_autoToggle(true),
																																																		  m_checked(false)
{
	loadImages();

	Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToggleBitmapButton::onClicked));
}

ToggleBitmapButton::~ToggleBitmapButton()
{

}

const wxImage & ToggleBitmapButton::getImageNormalChecked() const
{
	return m_buttonImageNormalChecked;
}

void ToggleBitmapButton::setImageNormalChecked(const wxImage &imageNormalChecked, bool deriveDisabled)
{
	m_buttonImageNormalChecked = imageNormalChecked;
	if(deriveDisabled)
		m_buttonImageDisabledChecked = imageNormalChecked.ConvertToGreyscale();		
	else
		m_buttonImageDisabledChecked = imageNormalChecked;

	updateImages(getChecked());
}

const wxImage & ToggleBitmapButton::getImageDownChecked() const
{
	return m_buttonImageDownChecked;
}

void ToggleBitmapButton::setImageDownChecked(const wxImage &imageDownChecked)
{
	m_buttonImageDownChecked = imageDownChecked;
	updateImages(getChecked());
}

const wxImage & ToggleBitmapButton::getImageFocusChecked() const
{
	return m_buttonImageFocusChecked;
}

void ToggleBitmapButton::setImageFocusChecked(const wxImage &imageFocusChecked)
{
	m_buttonImageFocusChecked = imageFocusChecked;
	updateImages(getChecked());
}

const wxImage & ToggleBitmapButton::getImageDisabledChecked() const
{
	return m_buttonImageDisabledChecked;
}

void ToggleBitmapButton::setImageDisabledChecked(const wxImage &imageDisabledChecked)
{
	m_buttonImageDisabledChecked = imageDisabledChecked;
	updateImages(getChecked());
}

const wxImage & ToggleBitmapButton::getImageOverChecked() const
{
	return m_buttonImageOverChecked;
}

void ToggleBitmapButton::setImageOverChecked(const wxImage &imageOverChecked)
{
	m_buttonImageOverChecked = imageOverChecked;
	updateImages(getChecked());
}

bool ToggleBitmapButton::getChecked() const
{
	return m_checked;
}

void ToggleBitmapButton::setChecked(bool checked)
{
	if(m_checked == checked)
		return;

	m_checked = checked;
	updateImages(true);
}

bool ToggleBitmapButton::getAutoToggle() const
{
	return m_autoToggle;
}

void ToggleBitmapButton::setAutoToggle(bool autoToggle)
{
	m_autoToggle = autoToggle;
}

void ToggleBitmapButton::loadImages()
{
	m_buttonImageNormal = ButtonBase::m_imageNormal;
	m_buttonImageDown = ButtonBase::m_imageDown;
	m_buttonImageFocus = ButtonBase::m_imageFocus;
	m_buttonImageDisabled = ButtonBase::m_imageDisabled;
	m_buttonImageOver = ButtonBase::m_imageOver;
}

void ToggleBitmapButton::updateImages(bool notify)
{
	if(m_checked)
	{
		ButtonBase::m_imageNormal = m_buttonImageNormalChecked;
		ButtonBase::m_imageDown = m_buttonImageDownChecked;
		ButtonBase::m_imageFocus = m_buttonImageFocusChecked;
		ButtonBase::m_imageDisabled = m_buttonImageDisabledChecked;
		ButtonBase::m_imageOver = m_buttonImageOverChecked;
	}
	else
	{
		ButtonBase::m_imageNormal = m_buttonImageNormal;
		ButtonBase::m_imageDown = m_buttonImageDown;
		ButtonBase::m_imageFocus = m_buttonImageFocus;
		ButtonBase::m_imageDisabled = m_buttonImageDisabled;
		ButtonBase::m_imageOver = m_buttonImageOver;
	}

	if(notify)
		ButtonBase::OnSetBitmap();
}

void ToggleBitmapButton::onClicked(wxCommandEvent &e)
{
	e.Skip();

	if(m_autoToggle)
		setChecked(getChecked() == false);
}

void ToggleBitmapButton::OnSetBitmap()
{
	loadImages();

	ButtonBase::OnSetBitmap();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
