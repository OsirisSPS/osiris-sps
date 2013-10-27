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

#ifndef _OS_CORE_SAFE_BOOL_H
#define _OS_CORE_SAFE_BOOL_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class safe_bool_base
{
protected:
	typedef void (safe_bool_base::* bool_type)() const;

protected:
	safe_bool_base()
	{

	}

    safe_bool_base(const safe_bool_base &)
	{

	}

    ~safe_bool_base()
	{

	}

protected:
	void this_type_does_not_support_comparisons() const
	{

	}

	safe_bool_base & operator =(const safe_bool_base &)
	{
		return *this;
	}
};

//////////////////////////////////////////////////////////////////////

template <typename T = void>
class safe_bool : public safe_bool_base
{
protected:
	~safe_bool()
	{

	}

public:
	operator bool_type() const
	{
		return static_cast<const T *>(this)->boolean_test() ? &safe_bool<T>::this_type_does_not_support_comparisons : 0;
    }
};

//////////////////////////////////////////////////////////////////////

template<> 
class safe_bool<void> : public safe_bool_base
{
protected:
	virtual ~safe_bool()
	{

	}

public:
	operator bool_type() const
	{
		return boolean_test() == true ? &safe_bool<void>::this_type_does_not_support_comparisons : 0;
    }

protected:
    virtual bool boolean_test() const = 0;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename U>
void operator ==(const safe_bool<T> &first, const safe_bool<U> &second)
{
	first.this_type_does_not_support_comparisons();
    return false; // genera errore di compilazione in caso di utilizzo
}

template <typename T,typename U>
void operator !=(const safe_bool<T> &first, const safe_bool<U> &second)
{
	first.this_type_does_not_support_comparisons();
    return false; // genera errore di compilazione in caso di utilizzo
}

//////////////////////////////////////////////////////////////////////

// Samples

/*

class Testable_with_virtual : public safe_bool<>
{
protected:
    virtual bool boolean_test() const
	{
		// Perform Boolean logic here
    }
};

class Testable_without_virtual : public safe_bool<Testable_without_virtual>
{
public:
    bool boolean_test() const
	{
		// Perform Boolean logic here
    }
};

*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_SAFE_BOOL_H
