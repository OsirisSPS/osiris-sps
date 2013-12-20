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
#include "archivesziparchive.h"

#include "constants.h"
#include "iarchivesarchiveentry.h"
#include "istream.h"
#include "log.h"
#include "utils.h"
#include "zzip/zzip.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<ArchivesZipArchive> : public Object
{
	class ArchivesZipArchiveStream;
	class ArchivesZipArchiveEntry;	

// Construction
public:
	pimpl();
	virtual ~pimpl();

// Attributes
public:
	inline ZZIP_DIR * getDirectory() const;

// Operations
public:
	bool open(shared_ptr<IArchivesArchive> archive, const String &filename);
	void close();
	const IArchivesArchive::Entries & getEntries() const;

public:
	static String getZzipErrorDescription(zzip_error_t zzipError);

private:
	ZZIP_DIR *m_root;
	IArchivesArchive::Entries m_entries;
};

//////////////////////////////////////////////////////////////////////

class pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream : public IStream
{
// Construction
public:
	ArchivesZipArchiveStream(ZZIP_FILE *zzipFile, uint64 uncompressedSize);
	virtual ~ArchivesZipArchiveStream();

// IStream interface
public:
	virtual bool is_open() const;
	virtual bool close();
	virtual uint32 read(void *v, uint32 size) const;
	virtual uint32 write(const void *v, uint32 size);
	virtual bool seek(uint64 offset, SeekPosition from = seekBegin) const;
	virtual uint64 position() const;
	virtual uint64 size() const;
	virtual bool flush();
	virtual bool eof() const;

private:
	ZZIP_FILE *m_zzipFile;
	uint64 m_uncompressedSize;
};

//////////////////////////////////////////////////////////////////////

pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::ArchivesZipArchiveStream(ZZIP_FILE *zzipFile, uint64 uncompressedSize) : m_zzipFile(zzipFile),
																																m_uncompressedSize(uncompressedSize)
{
	
}

pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::~ArchivesZipArchiveStream()
{
	close();
}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::is_open() const
{
	return m_zzipFile != nullptr;
}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::close()
{
	if(m_zzipFile != nullptr)
	{
		zzip_file_close(m_zzipFile);
		m_zzipFile = nullptr;
	}

	return true;
}

uint32 pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::read(void *v, uint32 size) const
{
	if(m_zzipFile == nullptr)
	{
		OS_ASSERTFALSE();
		return 0;
	}

	zzip_ssize_t result = zzip_file_read(m_zzipFile, v, static_cast<zzip_size_t>(size));
	if(result < 0) 
	{
		OS_LOG_ERROR(zzip_strerror_of(zzip_dirhandle(m_zzipFile)));
		return 0;
	}

	return static_cast<uint32>(result);
}

uint32 pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::write(const void *v, uint32 size)
{
	// Non supportato

	OS_ASSERTFALSE();
	return 0;
}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::seek(uint64 offset, SeekPosition from) const
{
	// Non supportato

	OS_ASSERTFALSE();
	return false;
}

uint64 pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::position() const
{
	zzip_off_t pos = zzip_tell(m_zzipFile);
	if(pos < 0)
		return IStream::npos;

	return static_cast<uint64>(pos);
}

uint64 pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::size() const
{
	return m_uncompressedSize;
}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::flush()
{
	return false;
}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream::eof() const
{
	return position() >= m_uncompressedSize;
}

//////////////////////////////////////////////////////////////////////

class pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry : public IArchivesArchiveEntry
{
	typedef IArchivesArchiveEntry BaseClass;

// Construction
public:
	ArchivesZipArchiveEntry(shared_ptr<IArchivesArchive> archive, const ZZIP_DIRENT &entry);
	virtual ~ArchivesZipArchiveEntry();

// Interface
public:
	virtual bool isFolder() const;
	virtual const String & getPath() const;
	virtual const String & getName() const;
	virtual uint64 getCompressedSize() const;
	virtual uint64 getUncompressedSize() const;
	virtual shared_ptr<IStream> openStream() const;

private:
	static void splitFilename(const String &filepath, String &outBasename, String &outPath);

private:
	bool m_folder;
	String m_path;
	String m_name;
	uint64 m_compressedSize;
	uint64 m_uncompressedSize;
};

//////////////////////////////////////////////////////////////////////

pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::ArchivesZipArchiveEntry(shared_ptr<IArchivesArchive> archive, const ZZIP_DIRENT &entry) : BaseClass(archive)
{
	String fullpath(entry.d_name);

	splitFilename(fullpath, m_name, m_path);
    
    if(m_name.empty())
    {
		splitFilename(fullpath.substr(0, fullpath.length() - 1), m_name, m_path);
		
		m_folder = true;
		m_compressedSize = 0;
		m_uncompressedSize = 0;
    }
	else
	{
		m_folder = false;
		m_compressedSize = static_cast<uint64>(entry.d_csize);
		m_uncompressedSize = static_cast<uint64>(entry.st_size);
	}
}

pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::~ArchivesZipArchiveEntry()
{

}

bool pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::isFolder() const
{
	return m_folder;
}

const String & pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::getPath() const
{
	return m_path;
}

const String & pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::getName() const
{
	return m_name;
}

uint64 pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::getCompressedSize() const
{
	return m_compressedSize;
}

uint64 pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::getUncompressedSize() const
{
	return m_uncompressedSize;
}

shared_ptr<IStream> pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::openStream() const
{
	OS_ASSERT(isFolder() == false);

	if(m_name.empty())
		return nullptr;

	shared_ptr<ArchivesZipArchive> zipArchive = boost::dynamic_pointer_cast<ArchivesZipArchive>(getArchive());
	if(zipArchive == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	ZZIP_DIR *zzipDirectory = zipArchive->getImpl()->getDirectory();
	if(zzipDirectory == nullptr)
	{
		OS_ASSERTFALSE();
		return nullptr;
	}

	// VERYURGENT: verificare zzip unicode

	// N.B.: non usare qui utils::makeFilePath perchè internamente nella zzip i path devono essere sempre separati da '/'
	ZZIP_FILE *zzipFile = zzip_file_open(zzipDirectory, (m_path + m_name).to_ascii().c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
    if(zzipFile == nullptr)
	{
		String error = "Cannot open '" + m_name + "'";
		String message = pimpl<ArchivesZipArchive>::getZzipErrorDescription(static_cast<zzip_error_t>(zzip_error(zzipDirectory)));
		if(message.empty() == false)
			error += ": " + message;

		OS_LOG_ERROR(error);
		return nullptr;
	}

	return shared_ptr<IStream>(OS_NEW pimpl<ArchivesZipArchive>::ArchivesZipArchiveStream(zzipFile, m_uncompressedSize));
}

void pimpl<ArchivesZipArchive>::ArchivesZipArchiveEntry::splitFilename(const String &filepath, String &outBasename, String &outPath)
{
    String path = filepath;
	path.replace_all("\\", "/");
    size_t i = path.find_last_of("/");
    if(i == String::npos)
    {
        outPath.clear();
		outBasename = filepath;
    }
    else
    {
        outBasename = path.substr(i + 1, path.size() - i - 1);
        outPath = path.substr(0, i + 1);
    }
}

//////////////////////////////////////////////////////////////////////

pimpl<ArchivesZipArchive>::pimpl() : m_root(nullptr)
{

}

pimpl<ArchivesZipArchive>::~pimpl()
{
	close();
}

inline ZZIP_DIR * pimpl<ArchivesZipArchive>::getDirectory() const 
{ 
	return m_root; 
}

bool pimpl<ArchivesZipArchive>::open(shared_ptr<IArchivesArchive> archive, const String &filename)
{
	close();

	OS_ASSERT(m_root == nullptr);

	// VERYURGENT: verificare zzip unicode

	zzip_error_t zzipError = ZZIP_NO_ERROR;
	m_root = zzip_dir_open(filename.to_ascii().c_str(), &zzipError);

	if(zzipError != ZZIP_NO_ERROR)
    {
		String error = "Cannot open '" + filename + "'";
		String message = getZzipErrorDescription(zzipError);
		if(message.empty() == false)
			error += ": " + message;

		OS_LOG_ERROR(error);
		return false;
	}

	ZZIP_DIRENT entry;
    while(zzip_dir_read(m_root, &entry))
	{
        m_entries.push_back(shared_ptr<ArchivesZipArchiveEntry>(OS_NEW ArchivesZipArchiveEntry(archive, entry)));
	}

	return true;
}

void pimpl<ArchivesZipArchive>::close()
{
	if(m_root != nullptr)
    {
		zzip_dir_close(m_root);
        m_root = nullptr;        
	}

	m_entries.clear();
}

const IArchivesArchive::Entries & pimpl<ArchivesZipArchive>::getEntries() const
{
	return m_entries;
}

String pimpl<ArchivesZipArchive>::getZzipErrorDescription(zzip_error_t zzipError) 
{
    String errorMsg;
    switch (zzipError)
    {
    case ZZIP_NO_ERROR:
							break;

    case ZZIP_OUTOFMEM:		errorMsg = "out of memory";
							break;            

    case ZZIP_DIR_OPEN:
    case ZZIP_DIR_STAT: 
    case ZZIP_DIR_SEEK:
    case ZZIP_DIR_READ:

							errorMsg = "unable to read zip file";
							break;            

    case ZZIP_UNSUPP_COMPR:	errorMsg = "unsupported compression format";
							break;            

    case ZZIP_CORRUPTED:	errorMsg = "corrupted archive";
							break;            

    default:				errorMsg = "unknown error";
							break;            
    }

    return errorMsg;
}

//////////////////////////////////////////////////////////////////////

ArchivesZipArchive::ArchivesZipArchive()
{

}

ArchivesZipArchive::~ArchivesZipArchive()
{

}

bool ArchivesZipArchive::open(const String &filename)
{
	return m_impl->open(get_this_ptr(), filename);
}

void ArchivesZipArchive::close()
{
	m_impl->close();
}

const IArchivesArchive::Entries & ArchivesZipArchive::getEntries() const
{
	return m_impl->getEntries();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
