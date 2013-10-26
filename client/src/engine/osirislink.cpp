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
#include "osirislink.h"

#include "algorithms.h"
#include "httpparser.h"
#include "log.h"
#include "osiriscommon.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

OsirisLink::OsirisLink(const std::string &link)
{
	parse(link);
}

OsirisLink::~OsirisLink()
{

}

bool OsirisLink::empty() const
{
	return m_params.empty();
}

ordered_map<std::wstring, std::wstring> OsirisLink::getParams2() const
{
	ordered_map<std::wstring, std::wstring> params;

	for(OsirisLinkParams::const_iterator i = m_params.begin(); i != m_params.end(); ++i)
	{
		std::wstring key = utils::atow(i->first);
		std::wstring value = i->second;

		//params[key] = HttpParser::urlEncode2(value);		
		params[key] = value;
	}

	return params;
}

bool OsirisLink::hasParam(const std::string &name) const
{
	OsirisLinkParams::const_iterator e = m_params.find(name);
	return(e != m_params.end());
}

std::wstring OsirisLink::getParam(const std::string &name) const
{
	OsirisLinkParams::const_iterator e = m_params.find(name);
	if(e != m_params.end())
		return e->second;
	else
		return _W("");
}

void OsirisLink::setParam(const std::string &name, const std::wstring &value)
{
	if(value != _W(""))
		m_params[name] = value;
}

void OsirisLink::setParam(const std::string &name, const std::string &value)
{
	if(value != "")
		m_params[name] = utils::atow(value);
}

OsirisLink::LinkType OsirisLink::getType() const
{
	if(getParam("type") == OS_OSIRISLINK_TYPE_PORTAL)
		return linkPortal;
	else if(getParam("type") == OS_OSIRISLINK_TYPE_ISIS)
		return linkIsis;
	else if(getParam("type") == OS_OSIRISLINK_TYPE_URL)
		return linkUrl;
	else if(getParam("type") == OS_OSIRISLINK_TYPE_FILE)
		return linkFile;
	else if(getParam("type") == OS_OSIRISLINK_TYPE_RES)
		return linkResource;
	else
		return linkInvalid;
}





PortalID OsirisLink::getPortal() const
{
	return utils::wtoa(getParam("portal"));
}

PovID OsirisLink::getPov() const
{
	return utils::wtoa(getParam("pov"));
}

String OsirisLink::getName() const
{
	return getParam("name");
}

String OsirisLink::getDescription() const
{
	return getParam("description");
}

void OsirisLink::clear()
{
	m_params.clear();
}

bool OsirisLink::isValid() const
{
	bool valid = true;
	
	if(getParam("type") == OS_OSIRISLINK_TYPE_PORTAL)
	{
		if(hasParam("portal") == false)
			return false;
		
		PortalID portal = static_cast<String>(getParam("portal")).to_ascii();
		if(portal.validate(false) == false)
			return false;

#ifdef OS_OSIRIS_VERSION_RAZOR
		if(hasParam("pov") == false)
			return false;

		PovID pov = static_cast<String>(getParam("pov")).to_ascii();
		if(pov.empty())
			return false;		
#endif
	}
	else if(getParam("type") == OS_OSIRISLINK_TYPE_ISIS)
	{
		if(hasParam("portal") == false)
			return false;
		
		PortalID portal = static_cast<String>(getParam("portal")).to_ascii();
		if(portal.validate(false) == false)
			return false;

#ifdef OS_OSIRIS_VERSION_RAZOR
		if(hasParam("pov") == false)
			return false;

		PovID pov = static_cast<String>(getParam("pov")).to_ascii();
		if(pov.empty())
			return false;		
#endif

		if(hasParam("name") == false)
			return false;		

		if(hasParam("url") == false)
			return false;

		// TODO: check if is a url.
	}
	else if(getParam("type") == OS_OSIRISLINK_TYPE_FILE)
	{
		if(hasParam("id") == false)
			return false;		

		EntityID id = static_cast<String>(getParam("id")).to_ascii();
		if(id.empty())
			return false;
				
		if(hasParam("portal"))
		{
			PortalID portal = static_cast<String>(getParam("portal")).to_ascii();
			if(portal.validate(false) == false)
				return false;
		}

#ifdef OS_OSIRIS_VERSION_RAZOR
		if(hasParam("pov"))
		{
			PovID pov = static_cast<String>(getParam("pov")).to_ascii();
			if(pov.empty())
				return false;		
		}
#endif	
	}
	else if(getParam("type") == OS_OSIRISLINK_TYPE_URL)
	{
		if(hasParam("url") == false)
			return false;		

		// TODO: check if is a url	
	}
	else if(getParam("type") == OS_OSIRISLINK_TYPE_RES)
	{
		if(hasParam("path") == false)
			return false;

		// We don't check if is a path.
	}
	else
		return false;

	return true;
}

bool OsirisLink::parse(const std::string &link)
{
	if(link.empty())
		return false;

	if(link.substr(0,9) == "osiris://")
	{
		// Old format

		// Compatibility problem: Firefox encode the |
		std::string link2 = link;
		algorithms::replace_all(link2, "%7C", "|");



		StringVector array1;
		utils::split(link2, _S("|"), array1);

		if(array1.size()<2)
			return false;

		std::wstring type = array1[1].to_wide();		

		if( (type == OS_OSIRISLINK_TYPE_PORTAL) || (type == OS_OSIRISLINK_TYPE_ISIS) )
		{
			if(array1.size() < 4)
				return false;

			setParam("type", type);

			setParam("portal", array1[2].to_wide());
			for(uint32 i=3;i<array1.size();i++)
			{
				std::string part = array1[i].to_ascii();
				StringVector array2;
				utils::split(part, _S("="), array2);
				
				std::string paramKey;
				std::wstring paramValue;
				if(array2.size()>=1)
					paramKey = array2[0].to_ascii();
				if(array2.size()>=2)
					paramValue = HttpParser::urlDecodeW(array2[1].to_ascii());
				
				if( (paramKey != "") && (paramValue != _W("")) )
					setParam(paramKey, paramValue);
			}
		}
		else if(type == OS_OSIRISLINK_TYPE_FILE)
		{
			if(array1.size() != 3)
				return false;

			setParam("type", OS_OSIRISLINK_TYPE_FILE);
			setParam("id", array1[2].to_wide());
		}
		else if(type == OS_OSIRISLINK_TYPE_URL)
		{
			if(array1.size() <= 2)
				return false;

			std::wstring url;
			std::wstring params;
			if(array1.size() > 2)
				url = HttpParser::urlDecodeW(array1[2].to_ascii());
			if(array1.size() > 3)
			{
				params = HttpParser::urlDecodeW(array1[3].to_ascii());


				int nParamsFound = 0;
				StringVector arrayP;
				utils::split(params,_S("&"), arrayP);

				for(uint32 a1=0; a1 < arrayP.size(); a1++)
				{
					StringVector array2;
					utils::split(arrayP[a1],_S("="), array2);
					std::wstring paramKey;
					std::wstring paramValue;
					if(array2.size()>=1)
						paramKey = HttpParser::urlDecodeW(array2[0].to_ascii());
					if(array2.size()>=2)
						paramValue = HttpParser::urlDecodeW(array2[1].to_ascii());

					if( (paramKey != "") && (paramValue != _W("")) )
					{
						if(paramKey == _W("portal"))						
						{
							setParam("portal", paramValue);
						}
						else
						{
							if(nParamsFound == 0)
								url += _W("?");
							nParamsFound++;
							url += utils::atow(HttpParser::urlEncodeW(paramKey));
							url += _W("=");
							url += utils::atow(HttpParser::urlEncodeW(paramValue));
						}
					}
				}
			}
					
			setParam("type", OS_OSIRISLINK_TYPE_URL);
			setParam("url",url);
		}
		else if(type == _W("skin"))
		{
			if(array1.size() != 3)
				return false;

			setParam("type", OS_OSIRISLINK_TYPE_RES);
			setParam("path", array1[2].to_wide());
		}		
		else
			return false;
	}
	else if(link.substr(0,8) == "osiris:?")
	{
		std::string params = link.substr(8);
		StringVector array1;
		utils::split(params,_S("&"), array1);

		for(uint32 a1=0; a1 < array1.size(); a1++)
		{
			StringVector array2;
			utils::split(array1[a1],_S("="), array2);
			std::string paramKey;
			std::wstring paramValue;
			if(array2.size()>=1)
				paramKey = array2[0].to_ascii();
			if(array2.size()>=1)
				paramValue = HttpParser::urlDecodeW(array2[1].to_ascii());

			if( (paramKey != "") && (paramValue != _W("")) )
				setParam(paramKey, paramValue);
		}
	}
	else
		return false;

	return isValid();
}

std::string OsirisLink::generate() const
{	
#ifdef OS_OSIRIS_VERSION_RAZOR
	bool newFormat = true;
#else
	bool newFormat = false;
#endif

	if(isValid() == false)
		return "";

	if(hasParam("type") == false)
		return "";

	if(newFormat)
	{
		std::string out = "";

		for(OsirisLinkParams::const_iterator i = m_params.begin(); i != m_params.end(); ++i)
		{
			std::string key = i->first;
			std::wstring value = i->second;

			if(key != "type") // Type forced below, simply to ensure it's the first.
			{
				if(value != _W(""))
				{
					out += "&" + key + "=" + HttpParser::urlEncodeW(value);
				}
			}
		}
		return "osiris:?type=" + HttpParser::urlEncodeW(getParam("type")) + out;
	}
	else
	{
		if(getParam("type") == OS_OSIRISLINK_TYPE_PORTAL)
		{
			if(hasParam("portal") == false)
				return "";

			std::string out = "osiris://|portal|"  + HttpParser::urlEncodeW(getParam("portal")) + "|";			
			if(hasParam("name"))
				out += "name=" + HttpParser::urlEncodeW(getParam("name")) + "|";
			if(hasParam("description"))
				out += "description=" + HttpParser::urlEncodeW(getParam("description")) +  "|";
			if(hasParam("user"))
				out += "user=" + HttpParser::urlEncodeW(_W("010000002") + getParam("pov")) + "|";
			return out;
		}
		else if(getParam("type") == OS_OSIRISLINK_TYPE_ISIS)
		{
			if(hasParam("portal") == false)
				return "";

			std::string out = "osiris://|isis|"  + HttpParser::urlEncodeW(getParam("portal")) + "|";			
			
			if(hasParam("name"))
				out += "name=" + HttpParser::urlEncodeW(getParam("name")) + "|";
			if(hasParam("url"))
				out += "url=" + HttpParser::urlEncodeW(getParam("url")) +  "|";			
			return out;
		}
		else if(getParam("type") == OS_OSIRISLINK_TYPE_FILE)
		{
			std::string out = "osiris://|file|"  + HttpParser::urlEncodeW(getParam("id")) + "|";			
			return out;
		}
		else if(getParam("type") == OS_OSIRISLINK_TYPE_URL)
		{
			std::wstring url = getParam("url");
			StringVector array1;
			utils::split(url, _S("?"), array1);

			std::wstring params = _W("");
			if(array1.size()>=2)
			{
				url = array1[0].to_wide();
				params = array1[1].to_wide();
			}

			if(hasParam("portal"))
			{
				if(params.empty() == false)
					params += _W("&");
				params += _W("portal=") + getParam("portal");
			}
			
			/*
			std::string out = "osiris://|url|"  + HttpParser::urlEncodeW(url) + "|";			
			if(params != _W(""))
				out += HttpParser::urlEncodeW(params) + "|";			
			*/
			std::string out = "osiris://|url|"  + utils::wtoa(url) + "|";			
			if(params != _W(""))
				out += utils::wtoa(params) + "|";			
			return out;
		}
		else if(getParam("type") == OS_OSIRISLINK_TYPE_RES)
		{
			std::string out = "osiris://|skin|"  + HttpParser::urlEncodeW(getParam("path")) + "|";			
			return out;
		}
		else
		{
			return "";
		}
	}	
}

String OsirisLink::toString() const
{
	return generate();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
