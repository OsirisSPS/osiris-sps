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

#ifndef _OS_CORE_TEMPORARYFILE_H
#define _OS_CORE_TEMPORARYFILE_H

#include "file.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport TemporaryFile : public File
{
	typedef File FileBase;

// Construction
public:
	TemporaryFile(bool create, const String &filename = String::EMPTY, uint32 flags = ofWrite);	
	virtual ~TemporaryFile();

// Attributes
public:
	inline bool getAutoDelete() const;
	inline void setAutoDelete(bool autoDelete);

	inline const String & getFilename() const;

// File overrides
public:
	virtual bool open(const String &filename, uint32 flags);

protected:
	bool m_autoDelete;
	String m_filename;
};

//////////////////////////////////////////////////////////////////////

inline bool TemporaryFile::getAutoDelete() const { return m_autoDelete; }
inline void TemporaryFile::setAutoDelete(bool autoDelete) { m_autoDelete = autoDelete; }

inline const String & TemporaryFile::getFilename() const { return m_filename; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_TEMPORARYFILE_H
