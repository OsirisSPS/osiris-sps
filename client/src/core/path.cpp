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
#include "path.h"

#include "boost/filesystem/operations.hpp"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

struct Path::impl
{
	inline impl() { }
	inline impl(const String &src) : path(src.to_wide(), boost::filesystem::native) { }
	inline impl(const boost::filesystem::wpath &second) : path(second) { }

    boost::filesystem::wpath path;
};

//////////////////////////////////////////////////////////////////////

Path::Path()
{

}

Path::Path(const String &src) : m_impl(src)
{

}

Path::Path(const Path &second) : m_impl(second.m_impl->path)
{

}

template <typename T>
Path::Path(const T &v) : m_impl(v)
{

}

Path::~Path()
{

}

bool Path::empty() const
{
	return m_impl->path.empty();
}

Path Path::branch_path() const
{
	return m_impl->path.branch_path();
}

Path Path::relative_path() const
{
	return m_impl->path.relative_path();
}

Path Path::root_path() const
{
	return m_impl->path.root_path();
}

String Path::path() const
{
	return m_impl->path.string();
}

String Path::leaf() const
{
	return m_impl->path.filename().string();
}

void Path::clear()
{
	m_impl->path = String::EMPTY.to_wide();
}

Path::operator String() const
{
	return path();
}

Path & Path::operator=(const Path &second)
{
	m_impl->path = second.m_impl->path;
	return *this;
}

bool Path::operator==(const Path &second) const
{
	return m_impl->path == second.m_impl->path;
}

bool Path::operator!=(const Path &second) const
{
	return m_impl->path != second.m_impl->path;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
