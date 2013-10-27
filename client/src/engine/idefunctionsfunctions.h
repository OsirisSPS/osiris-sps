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

#ifndef _IDE_FUNCTIONS_H
#define _IDE_FUNCTIONS_H

#include "ideide.h"
#include "ixmlfunction.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPage;
class IPortalPage;

//////////////////////////////////////////////////////////////////////

namespace ide {
namespace functions {

//////////////////////////////////////////////////////////////////////

class EngineExport IFunction : public IXMLFunction
{
	typedef IXMLFunction FunctionBase;

// Construction
public:
	IFunction(shared_ptr<IPage> page, const String &name, const String &ns, uint32 required_args = 0, uint32 optionals_args = 0);
	virtual ~IFunction();

// Attributes
public:
	inline shared_ptr<IPage> getPage() const;

protected:
	weak_ptr<IPage> m_page;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IPage> IFunction::getPage() const { return m_page.lock(); }

//////////////////////////////////////////////////////////////////////

// Effettua il confronto tra due date restituendo -1 se la prima è minore della seconda, 0 se sono uguali, 1 se la prima è maggiore della seconda

class EngineExport CompareDates : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	CompareDates(shared_ptr<IPage> page);
	virtual ~CompareDates();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport CurrentDateTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	CurrentDateTime(shared_ptr<IPage> page);
	virtual ~CurrentDateTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport FormatDate : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	FormatDate(shared_ptr<IPage> page);
	virtual ~FormatDate();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport FormatSize : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	FormatSize(shared_ptr<IPage> page);
	virtual ~FormatSize();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport ShortDate : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ShortDate(shared_ptr<IPage> page);
	virtual ~ShortDate();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport LongDate : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	LongDate(shared_ptr<IPage> page);
	virtual ~LongDate();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport ShortTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ShortTime(shared_ptr<IPage> page);
	virtual ~ShortTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport LongTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	LongTime(shared_ptr<IPage> page);
	virtual ~LongTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport ShortDateTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ShortDateTime(shared_ptr<IPage> page);
	virtual ~ShortDateTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport LongDateTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	LongDateTime(shared_ptr<IPage> page);
	virtual ~LongDateTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport UserFriendlyDateTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	UserFriendlyDateTime(shared_ptr<IPage> page);
	virtual ~UserFriendlyDateTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport RFC822DateTime : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	RFC822DateTime(shared_ptr<IPage> page);
	virtual ~RFC822DateTime();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport Parse : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	Parse(shared_ptr<IPage> page);
	virtual ~Parse();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport ParseReverse : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ParseReverse(shared_ptr<IPage> page);
	virtual ~ParseReverse();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport VersionMajor : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	VersionMajor(shared_ptr<IPage> page);
	virtual ~VersionMajor();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport VersionMinor : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	VersionMinor(shared_ptr<IPage> page);
	virtual ~VersionMinor();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport GetOption : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	GetOption(shared_ptr<IPage> page);
	virtual ~GetOption();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport HtmlEncode : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	HtmlEncode(shared_ptr<IPage> page);
	virtual ~HtmlEncode();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport HtmlDecode : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	HtmlDecode(shared_ptr<IPage> page);
	virtual ~HtmlDecode();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport JSEncode : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	JSEncode(shared_ptr<IPage> page);
	virtual ~JSEncode();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport UrlEncode : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	UrlEncode(shared_ptr<IPage> page);
	virtual ~UrlEncode();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport UrlDecode : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	UrlDecode(shared_ptr<IPage> page);
	virtual ~UrlDecode();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport HelpLink : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	HelpLink(shared_ptr<IPage> page);
	virtual ~HelpLink();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

/*
//////////////////////////////////////////////////////////////////////

class EngineExport PortalLink : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	PortalLink(shared_ptr<IPage> page);
	virtual ~PortalLink();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};
*/

//////////////////////////////////////////////////////////////////////

class EngineExport ReferenceDate : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ReferenceDate(shared_ptr<IPortalPage> page);
	virtual ~ReferenceDate();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport ResourceUrl : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	ResourceUrl(shared_ptr<IPage> page);
	virtual ~ResourceUrl();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport Replace : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	Replace(shared_ptr<IPage> page);
	virtual ~Replace();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport Text : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	Text(shared_ptr<IPage> page);
	virtual ~Text();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport EnglishName : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	EnglishName(shared_ptr<IPage> page);
	virtual ~EnglishName();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

class EngineExport NativeName : public IFunction
{
	typedef IFunction FunctionBase;

// Construction
public:
	NativeName(shared_ptr<IPage> page);
	virtual ~NativeName();

// IFunction interface
public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result);
};

//////////////////////////////////////////////////////////////////////

} // functions
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_FUNCTIONS_H
