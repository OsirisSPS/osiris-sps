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

#ifndef _OS_CORE_TEXTFILE_H
#define _OS_CORE_TEXTFILE_H

#include "file.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport TextFile : public File
{
	typedef File FileBase;

// Enumerated types
public:
	enum Encoding
	{
		feAscii,
		feUtf8,
		feUnicode,
	};

// Construction
public:
	TextFile(Encoding encoding = feAscii);
	TextFile(const String &filename, uint32 flags, Encoding encoding = feAscii);
	virtual ~TextFile();

// Attributes
public:
	inline Encoding getEncoding() const;
	inline void setEncoding(Encoding encoding);

// Operations
public:
	bool readLine(String &line);
	bool writeLine(const String &line);

	bool readChar(char &c);
	bool writeChar(char c);

	bool readChar(wchar &c);
	bool writeChar(wchar c);

	static String readFile(const String &filename);
	static bool readFile(const String &filename, String &str);

protected:
	bool _readLineAscii(String &line);
	bool _writeLineAscii(const std::string &line);

	bool _readLineUtf8(String &line);
	bool _writeLineUtf8(const std::string &line);

	bool _readLineUnicode(String &line);
	bool _writeLineUnicode(const String &line);

// Overrides
public:
	virtual bool open(const String &filename, uint32 flags);

protected:
	bool readBOM();
	bool writeBOM();

protected:
	Encoding m_encoding;
};

//////////////////////////////////////////////////////////////////////

inline TextFile::Encoding TextFile::getEncoding() const { return m_encoding; }
inline void TextFile::setEncoding(Encoding encoding) { m_encoding = encoding; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_TEXTFILE_H
