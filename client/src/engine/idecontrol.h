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

#ifndef _OS_IDE_CONTROL_H
#define _OS_IDE_CONTROL_H

#include "ideide.h"
#include "iidepage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeSkin;

//////////////////////////////////////////////////////////////////////

template <typename T>
class IPageControl : public T
{
	typedef T ControlBase;

// Construction
public:
	IPageControl() { }
	template <typename T1>
	IPageControl(const T1 &v1) : ControlBase(v1) { }
	template <typename T1, typename T2>
	IPageControl(const T1 &v1, const T2 &v2) : ControlBase(v1, v2) { }
	template <typename T1, typename T2, typename T3>
	IPageControl(const T1 &v1, const T2 &v2, const T3 &v3) : ControlBase(v1, v2, v3) { }

	virtual ~IPageControl() { }

// Attributes
public:
	inline shared_ptr<IPage> getPage() const;
	inline shared_ptr<IdeSkin> getSkin() const;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
inline shared_ptr<IPage> IPageControl<T>::getPage() const
{
	return boost::dynamic_pointer_cast<IPage>(ControlBase::getPage());
}

template <typename T>
inline shared_ptr<IdeSkin> IPageControl<T>::getSkin() const
{
	return getPage()->getSkin();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_CONTROL_H
