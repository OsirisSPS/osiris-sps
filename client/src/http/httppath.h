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

#ifndef _OS_HTTP_HTTPPATH_H
#define _OS_HTTP_HTTPPATH_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HttpExport HttpPath
{
// Construction
public:
    HttpPath(const String &url);
	HttpPath(const HttpPath &second);
	virtual ~HttpPath();

// Attributes
public:
	// Restituisce l'indirizzo formattato
	inline const String & getUrl() const;

	inline bool empty() const;

	// Restituisce i tokens del path
	inline const StringVector & getTokens() const;

	// Restituisce il nome della directory di riferimento
	const String & getDirectory() const;
	// Restituisce il token specificato
	const String & getToken(uint32 index) const;

	// Indica se il path è sicuro
	bool isSafe() const;

	// Restituisce un path relativo
	String getRelativePath(uint32 index) const;
	// Restituisce il target della richiesta
	String getTarget() const;

// Operators
public:
	HttpPath & operator =(const HttpPath &second);

// Operations
public:
	void set(const String &url);

private:
	void _clear();
	void _parse(const String &url);

private:
	String m_url;
	StringVector m_tokens;
};

//////////////////////////////////////////////////////////////////////

inline const String & HttpPath::getUrl() const { return m_url; }

inline bool HttpPath::empty() const { return m_url.empty(); }

inline const StringVector & HttpPath::getTokens() const { return m_tokens; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif //_OSIRIS_HTTP_HTTPPATH_H
