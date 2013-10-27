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

#ifndef _OS_LANG_LANGUAGEFOLDER_H
#define _OS_LANG_LANGUAGEFOLDER_H

#include "base/object.h"
#include "boost/thread/mutex.hpp"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LanguageVocabolary;

//////////////////////////////////////////////////////////////////////

class LangExport LanguageFolder : public enable_this_ptr<LanguageFolder>,
								  public Object
{
	typedef unordered_map<String, shared_ptr<LanguageVocabolary> >::type Vocabolaries;	

// Construction
public:
	LanguageFolder();
	virtual ~LanguageFolder();

// Attributes
public:

// Operations
public:
	void clear();

	void addPath(const String &path);
	
	String findText(const String &culture, const String &name, bool searchingInAlternative) const;

private:	
	void addPath(const String &culture, const String &path);	

	void notifyKey(const String &culture, const String &name, const String &value) const;
	void notifyUnknownKey(const String &culture, const String &name) const;

protected:	
	weak_ptr<LanguageFolder> m_parent;
	mutable Vocabolaries m_vocabolaries;	

protected:
	static const String UNKNOWNTEXT;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_LANG_LANGUAGEFOLDER_H
