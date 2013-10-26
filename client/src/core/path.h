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

#ifndef _OS_CORE_PATH_H
#define _OS_CORE_PATH_H

//////////////////////////////////////////////////////////////////////

#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport Path
{
// Construction
public:
	Path();
	Path(const String &src);
	Path(const Path &second);
	~Path();

private:
	template <typename T>
	Path(const T &v);

// Attributes
public:
	bool empty() const;

	Path branch_path() const;
	Path relative_path() const;
	Path root_path() const;

	String path() const;
	String leaf() const;

// Operations
public:
	void clear();

// Operators
public:
	operator String() const;

	Path & operator=(const Path &second);

	bool operator==(const Path &second) const;
    bool operator!=(const Path &second) const;

private:
	struct impl;
	pimpl_ptr<impl> m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_PATH_H
