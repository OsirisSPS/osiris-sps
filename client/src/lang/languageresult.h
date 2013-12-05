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

#ifndef _OS_LANG_LANGUAGERESULT_H
#define _OS_LANG_LANGUAGERESULT_H

#include "base/object.h"
#include "boost/thread/mutex.hpp"
#include "collections.h"
#include "dataitem.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LangExport LanguageResult : public Object
{	
// Construction
public:
	LanguageResult();
	explicit LanguageResult(const char *id);
	explicit LanguageResult(const std::string &id);
	explicit LanguageResult(const std::string &id, const StringCollection<String> &params);
	explicit LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1);
	explicit LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1, const std::string &key2, const DataItem &val2);
	explicit LanguageResult(const std::string &id, const std::string &key1, const DataItem &val1, const std::string &key2, const DataItem &val2, const std::string &key3, const DataItem &val3);

	virtual ~LanguageResult();

// Attributes
public:

	inline bool empty() const;

	inline void setId(const std::string &id);
	inline std::string getId() const;

	inline const StringCollection<String> & getParams() const;

// Operations
public:
	void setNoTranslate(const std::string& text);
	void setParam(const std::string &key, const DataItem &val);
	void clear();
	
private:
	std::string m_id;
	StringCollection<String> m_params;
};

//////////////////////////////////////////////////////////////////////

inline bool LanguageResult::empty() const { return m_id.empty(); }

inline void LanguageResult::setId(const std::string &id) { m_id = id; }
inline std::string LanguageResult::getId() const { return m_id; }

inline const StringCollection<String> & LanguageResult::getParams() const { return m_params; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_LANG_LANGUAGERESULT_H
