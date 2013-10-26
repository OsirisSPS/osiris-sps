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
#include "textfile.h"

#include "charset.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

TextFile::TextFile(Encoding encoding)
{
	m_encoding = encoding;
}

TextFile::TextFile(const String &filename, uint32 flags, Encoding encoding)
{
	m_encoding = encoding;

	// Richiama la open al posto di passare i parametri alla classe base (la chiamata
	// nel costruttore dalla base non chiamerebbe la virtuale)

	open(filename, flags);
}

TextFile::~TextFile()
{

}

bool TextFile::readLine(String &line)
{
	line.clear();

	switch(m_encoding)
	{
	case feAscii:		return _readLineAscii(line);

	case feUtf8:		return _readLineUtf8(line);

	case feUnicode:		return _readLineUnicode(line);
	}

	return false;
}

bool TextFile::writeLine(const String &line)
{
	switch(m_encoding)
	{
	case feAscii:		return _writeLineAscii(line.to_ascii());

	case feUtf8:		return _writeLineUtf8(line.to_utf8());

	case feUnicode:		return _writeLineUnicode(line);
	}

	return false;
}

bool TextFile::readChar(char &c)
{
	return read(&c, sizeof(char)) == sizeof(char);
}

bool TextFile::writeChar(char c)
{
	return write(&c, sizeof(char)) == sizeof(char);
}

bool TextFile::readChar(wchar &c)
{
	return read(&c, sizeof(wchar)) == sizeof(wchar);
}

bool TextFile::writeChar(wchar c)
{
	return write(&c, sizeof(wchar)) == sizeof(wchar);
}

String TextFile::readFile(const String &filename)
{
	String out;
	if(readFile(filename, out))
		return out;
	else
		return String::EMPTY;
}

bool TextFile::readFile(const String &filename, String &str)
{
	str.clear();

	shared_ptr<TextFile> file(OS_NEW TextFile());
	if(file->open(filename, TextFile::ofRead) == false)
		return false;
	
	String line;
	while(file->readLine(line))
	{
		str.append(line);
		str.append(_S("\r\n"));
	}

	return true;
}

bool TextFile::_readLineAscii(String &line)
{
	bool done = false;

	std::string str;
	char c = 0;
	while(readChar(c))
	{
		done = true;

		if(c == '\n')
			break;

		if(c != '\r')
			str += c;
	}

	line.from_ascii(str);
	return done;
}

bool TextFile::_writeLineAscii(const std::string &line)
{
	uint32 size = static_cast<uint32>(line.size());
	if(write(line.data(), size) != size)
		return false;

	if(writeChar('\r') == false)
		return false;
	if(writeChar('\n') == false)
		return false;

	return true;
}

bool TextFile::_readLineUtf8(String &line)
{
	bool done = false;

	std::string str;
	char c = 0;
	while(readChar(c))
	{
		done = true;

		if(c == '\n')
			break;

		if(c != '\r')
			str += c;
	}

	line.from_utf8(str);
	return done;
}

bool TextFile::_writeLineUtf8(const std::string &line)
{
	uint32 size = static_cast<uint32>(line.size());
	if(write(line.data(), size) != size)
		return false;

	if(writeChar('\r') == false)
		return false;
	if(writeChar('\n') == false)
		return false;

	return true;
}

bool TextFile::_readLineUnicode(String &line)
{
	bool done = false;

	std::wstring str;
	wchar c = 0;
	while(readChar(c))
	{
		done = true;

		if(c == chLF)
			break;

		if(c != chCR)
			str += c;
	}

	line.from_wide(str);
	return done;
}

bool TextFile::_writeLineUnicode(const String &line)
{
	if(write(line.buffer(), static_cast<uint32>(line.buffer_size())) != static_cast<uint32>(line.buffer_size()))
		return false;

    static String endline = _S("\r\n");
	return write(endline.buffer(), static_cast<uint32>(endline.buffer_size())) == static_cast<uint32>(endline.buffer_size());
}

bool TextFile::open(const String &filename, uint32 flags)
{
	if(FileBase::open(filename, flags) == false)
		return false;

	if(flags & ofWrite)
	{
		if(empty())
		{
			if(writeBOM() == false)
				return false;
		}
		else if((flags & ofRead) != ofRead)
		{
			// Non può determinare l'encoding

			close();
			return false;
		}
	}

	if(flags & ofRead)
	{
		if(readBOM() == false)
			return false;
	}

	return true;
}

bool TextFile::readBOM()
{
	uint64 pos = position();
	if(pos != 0)
		seekToBegin();

	byte header[3];

	bool done = false;

	uint32 length = read(header, 2);
	if(length == 2)
	{
		if(header[0] == 0xEF && header[1] == 0xBB)
		{
			if(read(&header[2], 1) == 1)
			{
				if(header[2] == 0xBF)
				{
					m_encoding = feUtf8;
					done = true;
				}
			}
		}
		else if(header[0] == 0xFF && header[1] == 0xFE)
		{
			m_encoding = feUnicode;
			done = true;
		}
		else if(header[0] == 0xFE && header[1] == 0xFF)
		{
			// Il big endian non è supportato

			OS_ASSERTFALSE();
			return false;
		}
	}

	if(done == false)
	{
		// Nel caso non sia stato trovato alcun header assume che il file sia in ascii

		m_encoding = feAscii;
		seekToBegin();
	}

	if(pos != 0)
	{
		// Ripristina la posizione solo se non era quella iniziale, altrimenti
		// mantiene quella corrente (successiva all'header se esiste)

		seek(pos, seekBegin);
	}

	return true;
}

bool TextFile::writeBOM()
{
	if(m_encoding == feAscii)
		return true;

	uint64 pos = position();
	seekToBegin();

	switch(m_encoding)
	{
	case feUtf8:		{
							static const byte tag_utf8[3] = { 0xEF, 0xBB, 0xBF };
							write(tag_utf8, sizeof(tag_utf8));
						}

						break;

	case feUnicode:		{
							static const byte tag_unicode[2] = { 0xFF, 0xFE };
							write(tag_unicode, sizeof(tag_unicode));
						}

						break;

	default:			OS_EXCEPTION("Invalid file encoding");
						break;
	}

	if(pos != 0)
	{
		// Ripristina la posizione solo se non era quella iniziale, altrimenti
		// mantiene quella corrente (successiva all'header se esiste)

		seek(pos, seekBegin);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
