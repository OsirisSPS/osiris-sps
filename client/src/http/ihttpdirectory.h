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

#ifndef _OS_HTTP_IHTTPDIRECTORY_H
#define _OS_HTTP_IHTTPDIRECTORY_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

#define OS_HTTP_DIRECTORY_SESSION_COOKIE			_S("os_session")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpPath;
class HttpSession;
class HttpSessionState;
class TimeDuration;

//////////////////////////////////////////////////////////////////////

class HttpExport IHttpDirectory : public enable_this_ptr<IHttpDirectory>,
								  public Object,
								  public boost::noncopyable
{
	typedef Collections<String, IHttpDirectory>::shared_collection Directories;

// Enumerated types
public:
	enum Restrictions
	{
		drNone		= 0,		// Nessuna restrizione sulla directory
		drLocal		= 1,		// Restringe la directory alle connessioni locali
	};

// Construction
public:
    IHttpDirectory(const String &name);
	virtual ~IHttpDirectory();

// Attributes
public:
	inline bool isActive() const;
	inline void setActive(bool active);

	inline Restrictions getRestriction() const;
	inline void setRestriction(Restrictions restriction);

	inline const String & getName() const;
	inline shared_ptr<IHttpDirectory> getParent() const;

	inline bool hasDirectory(const String &name) const;

	shared_ptr<IHttpDirectory> getDirectory(const String &name) const;

	HttpPath getPath() const;

// Operations
public:
	bool addDirectory(shared_ptr<IHttpDirectory> directory);

	bool removeDirectory(shared_ptr<IHttpDirectory> directory);
	bool removeDirectory(const String &name);
	
// Overridables
public:
	virtual bool isAccessible(shared_ptr<HttpSession> session);

	virtual String getSessionCookie(shared_ptr<HttpSession> session) const;
	virtual shared_ptr<HttpSessionState> createSessionState(shared_ptr<HttpSession> session, const TimeDuration &duration) const;

	virtual bool process(shared_ptr<HttpSession> session, const HttpPath &path);

// Interface
protected:
	virtual bool handle(shared_ptr<HttpSession> session, const HttpPath &path) = 0;

protected:
	mutable boost::recursive_mutex m_cs;
	bool m_active;						// Stato della directory
	Restrictions m_restriction;			// Restrizioni sull'accesso alla directory
	const String m_name;				// Nome della directory
	weak_ptr<IHttpDirectory> m_parent;	// Directory padre
	Directories m_directories;			// Sottodirectories
};

//////////////////////////////////////////////////////////////////////

inline bool IHttpDirectory::isActive() const { return m_active; }
inline void IHttpDirectory::setActive(bool active) { m_active = active; }

inline IHttpDirectory::Restrictions IHttpDirectory::getRestriction() const { return m_restriction; }
inline void IHttpDirectory::setRestriction(Restrictions restriction) { m_restriction = restriction; }

inline const String & IHttpDirectory::getName() const { return m_name; }
inline shared_ptr<IHttpDirectory> IHttpDirectory::getParent() const { return m_parent.lock(); }

inline bool IHttpDirectory::hasDirectory(const String &name) const { return getDirectory(name) != nullptr; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_IHTTPDIRECTORY_H
