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

#ifndef _OS_UI_WXCUSTOMCLIENTDATA_H
#define _OS_UI_WXCUSTOMCLIENTDATA_H

#include "wx/clntdata.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class wxCustomClientData : public wxClientData
{
// Construction
public:
    wxCustomClientData(); 
	wxCustomClientData(const T &value);
	virtual ~wxCustomClientData();

// Attributes
public:
	inline const T & getValue() const;
	inline void setValue(const T &value);

private:
	T m_value;
};

//////////////////////////////////////////////////////////////////////
  
template <typename T>
wxCustomClientData<T>::wxCustomClientData()
{

}

template <typename T>
wxCustomClientData<T>::wxCustomClientData(const T &value) : m_value(value)
{

}

template <typename T>
wxCustomClientData<T>::~wxCustomClientData()
{

}

template <typename T>
inline const T & wxCustomClientData<T>::getValue() const
{
	return m_value;
}

template <typename T>
inline void wxCustomClientData<T>::setValue(const T &value)
{
	m_value = value;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_WXCUSTOMCLIENTDATA_H
