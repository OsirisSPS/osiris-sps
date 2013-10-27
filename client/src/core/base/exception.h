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

#ifndef _OS_CORE_EXCEPTION_H
#define _OS_CORE_EXCEPTION_H

//////////////////////////////////////////////////////////////////////

#define OS_EXCEPTION(desc)					throw(OS_NAMESPACE_NAME::Exception(desc, OS_CURRENT_FILE, OS_CURRENT_LINE, OS_CURRENT_FUNCTION))
#define OS_EXCEPT_IF(cond, desc)			if(cond) OS_EXCEPTION(desc)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport Exception : public std::exception
{
	typedef std::exception ExceptionBase;

// Construction
public:
	Exception(const std::string &description, const char *file, uint32 line, const char *function);
    Exception(const Exception &second);
	virtual ~Exception() throw();

// Attributes
public:
	inline const std::string & getDescription() const;
    inline const std::string & getFile() const;
	inline uint32 getLine() const;
	inline const std::string & getFunction() const;	

// std::exception overrides
public:
	virtual const char * what() const throw();

// Operators
public:
	void operator =(const Exception &second);

protected:
    std::string m_description;
    std::string m_file;
	uint32 m_line;
	std::string m_function;	
};

//////////////////////////////////////////////////////////////////////

inline const std::string & Exception::getDescription() const { return m_description; }
inline const std::string & Exception::getFile() const { return m_file; }
inline uint32 Exception::getLine() const { return m_line; }
inline const std::string & Exception::getFunction() const { return m_function; }	

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_EXCEPTION_H
