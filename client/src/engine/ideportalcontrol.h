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

#ifndef _OS_IDE_PORTALCONTROL_H
#define _OS_IDE_PORTALCONTROL_H

#include "idecontrol.h"
#include "ideide.h"
#include "iideportalpage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class IPortalPageControl : public IPageControl<T>
{
	typedef IPageControl<T> ControlBase;

// Construction
public:
	IPortalPageControl() { }
	template <typename T1>
	IPortalPageControl(const T1 &v1) : ControlBase(v1) { }
	template <typename T1, typename T2>
	IPortalPageControl(const T1 &v1, const T2 &v2) : ControlBase(v1, v2) { }
	template <typename T1, typename T2, typename T3>
	IPortalPageControl(const T1 &v1, const T2 &v2, const T3 &v3) : ControlBase(v1, v2, v3) { }

	virtual ~IPortalPageControl() { }

// Attributes
public:
	inline shared_ptr<IPortalPage> getPage() const;
	inline shared_ptr<Portal> getPortal() const;
	inline shared_ptr<IdeSession> getSessionAccount() const;	
	inline shared_ptr<IPortalDatabase> getDatabase() const;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline shared_ptr<IPortalPage> IPortalPageControl<T>::getPage() const
{
	return boost::dynamic_pointer_cast<IPortalPage>(ControlBase::getPage());
}

template <typename T>
inline shared_ptr<Portal> IPortalPageControl<T>::getPortal() const
{
	return getPage()->getPortal();
}

template <typename T>
inline shared_ptr<IdeSession> IPortalPageControl<T>::getSessionAccount() const
{
	return getPage()->getSessionAccount();
}

template <typename T>
inline shared_ptr<IPortalDatabase> IPortalPageControl<T>::getDatabase() const
{
	return getPage()->getDatabase();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_PORTALCONTROL_H
