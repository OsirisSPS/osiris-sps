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

#ifndef _OS_CORE_ALGORITHMS_H
#define _OS_CORE_ALGORITHMS_H

#include "boost/algorithm/string/case_conv.hpp"
#include "boost/algorithm/string/compare.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/algorithm/string/erase.hpp"
#include "boost/algorithm/string/join.hpp"
#include "boost/algorithm/string/regex.hpp"
#include "boost/algorithm/string/replace.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/trim.hpp"
#include "boost/type_traits/is_base_of.hpp"
#include "mpl.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((algorithms))

//////////////////////////////////////////////////////////////////////

enum case_type
{
	case_sensitive		= 0,
	case_insensitive	= 1,
};

//////////////////////////////////////////////////////////////////////

template <typename T, bool>
struct algorithms_type_checker : public boost::true_type {};

template <typename T>
struct algorithms_type_checker<T, false> : boost::integral_constant<bool, boost::is_base_of<std::basic_string<typename T::value_type>, T>::value> {};

#define OS_BOOST_ALGORITHM_STATIC_ASSERT(T)		OS_STATIC_ASSERT((algorithms_type_checker<T, boost::is_pod<T>::value>::value))

//////////////////////////////////////////////////////////////////////

template <typename T>
inline T left(const T &str, const typename T::size_type &count = T::npos)
{
	return str.substr(0, count);
}

template <typename T>
inline T mid(const T &str, const typename T::size_type &offset = 0, const typename T::size_type &count = T::npos)
{
	return str.substr(offset, count);
}

template <typename T>
inline T right(const T &str, const typename T::size_type &count = T::npos)
{
	typename T::size_type size = str.size();
	return str.substr(count < size ? size - count : 0, count);
}

//////////////////////////////////////////////////////////////////////

// N.B.: per le varianti di "copia" viene utilizzata una versione meno restrittiva sul tipo di dato dell'equilvalente boost (si aspetta ad esempio l'implementazione di iteratori)
// in modo da poter specializzare solo la versione per riferimento nel caso di implementazioni ad hoc

template <typename T>
inline void to_lower(T &v)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::to_lower(v);
}

template <typename T>
inline T to_lower_copy(const T &v)
{
	T copy(v);
	algorithms::to_lower(copy);
	return copy;
}

template <typename T>
inline void to_upper(T &v)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::to_upper(v);
}

template <typename T>
inline T to_upper_copy(const T &v)
{
	T copy(v);
	algorithms::to_upper(copy);
	return copy;
}

//////////////////////////////////////////////////////////////////////

template <typename T, typename P1, typename P2>
inline void replace_all(T &v, const P1 &search, const P2 &format, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P1);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P2);

	if(mode == case_insensitive)
		boost::algorithm::ireplace_all(v, search, format);
	else
		boost::algorithm::replace_all(v, search, format);
}

template <typename T, typename P1, typename P2>
inline T replace_all_copy(const T &v, const P1 &search, const P2 &format, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::replace_all(copy, search, format, mode);
	return copy;
}

template <typename T, typename P>
inline void replace_first(T &v, const P &search, const P &format, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		boost::algorithm::ireplace_first(v, search, format);
	else
		boost::algorithm::replace_first(v, search, format);
}

template <typename T, typename P>
inline T replace_first_copy(const T &v, const P &search, const P &format, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::replace_first(copy, search, format, mode);
	return copy;
}

template <typename T, typename P>
inline void replace_last(T &v, const P &search, const P &format, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		boost::algorithm::ireplace_last(v, search, format);
	else
		boost::algorithm::replace_last(v, search, format);
}

template <typename T, typename P>
inline T replace_last_copy(const T &v, const P &search, const P &format, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::replace_last(copy, search, format, mode);
	return copy;
}

//////////////////////////////////////////////////////////////////////

template <typename T, typename P>
inline void erase_all(T &v, const P &search, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		boost::algorithm::ierase_all(v, search);
	else
		boost::algorithm::erase_all(v, search);
}

template <typename T, typename P>
inline T erase_all_copy(const T &v, const P &search, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::erase_all(copy, search, mode);
	return copy;
}

template <typename T, typename P>
inline void erase_first(T &v, const P &search, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		boost::algorithm::ierase_first(v, search);
	else
		boost::algorithm::erase_first(v, search);
}

template <typename T, typename P>
inline T erase_first_copy(const T &v, const P &search, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::erase_first(copy, search, mode);
	return copy;
}

template <typename T, typename P>
inline void erase_last(T &v, const P &search, case_type mode = case_sensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		boost::algorithm::ierase_last(v, search);
	else
		boost::algorithm::erase_last(v, search);
}

template <typename T, typename P>
inline T erase_last_copy(const T &v, const P &search, case_type mode = case_sensitive)
{
	T copy(v);
	algorithms::erase_last(copy, search, mode);
	return copy;
}

template <typename T1, typename T2>
inline bool compare_no_case(const T1 &v1, const T2 &v2)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T1);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T2);

	return boost::algorithm::iequals(v1, v2);
}

template <typename T, typename P>
inline bool starts_with(const T &str, const P &pattern, case_type mode = case_insensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		return boost::algorithm::istarts_with(str, pattern);

	return boost::algorithm::starts_with(str, pattern);
}

template <typename T, typename P>
inline bool ends_with(const T &str, const P &pattern, case_type mode = case_insensitive)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(P);

	if(mode == case_insensitive)
		return boost::algorithm::iends_with(str, pattern);

	return boost::algorithm::ends_with(str, pattern);
}

//////////////////////////////////////////////////////////////////////

// Le versioni con un parametro usano come predicato di default "boost::algorithm::is_space" che effettua il trim su spazio, \r, \n, \t
// Le versioni con due parametri si aspettano un predicato, es.:
// trim(std::string("alfa,;"), boost::algorithm::is_any_of(",;"));

template <typename T>
inline void trim_left(T &v)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim_left(v);
}

template <typename T, typename P>
inline void trim_left(T &v, const P &predicate)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim_left_if(v, predicate);
}

template <typename T>
inline T trim_left_copy(const T &v)
{
	T copy(v);
	trim_left(copy);
	return copy;
}

template <typename T, typename P>
inline T trim_left_copy(const T &v, const P &predicate)
{
	T copy(v);
	trim_left(copy, predicate);
	return copy;
}

template <typename T>
inline void trim_right(T &v)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim_right(v);
}

template <typename T, typename P>
inline void trim_right(T &v, const P &predicate)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim_right_if(v, predicate);
}

template <typename T>
inline T trim_right_copy(const T &v)
{
	T copy(v);
	trim_right(copy);
	return copy;
}

template <typename T, typename P>
inline T trim_right_copy(const T &v, const P &predicate)
{
	T copy(v);
	trim_right(copy, predicate);
	return copy;
}

template <typename T>
inline void trim(T &v)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim(v);
}

template <typename T, typename P>
inline void trim(T &v, const P &predicate)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);

	boost::algorithm::trim_if(v, predicate);
}

template <typename T>
inline T trim_copy(const T &v)
{
	T copy(v);
	trim(copy);
	return copy;
}

template <typename T, typename P>
inline T trim_copy(const T &v, const P &predicate)
{
	T copy(v);
	trim(copy, predicate);
	return copy;
}

//////////////////////////////////////////////////////////////////////

// Separa la stringa in un array in base al criterio specificato (se non specificato usa boost::algorithm::is_space)
// Es.:
// 	std::string str = "alfa;beta; ;gamma";
//	std::vector<std::string> result;
//	algorithms::split(str, result, boost::algorithm::is_any_of("; "));

template <typename T, typename A>
inline A & split(const T &v, A &array, bool compress = true)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename A::value_type);

	return boost::algorithm::split(array, v, boost::algorithm::is_space(), compress ? boost::algorithm::token_compress_on : boost::algorithm::token_compress_off);
}

template <typename T, typename A, typename P>
inline A & split(const T &v, A &array, const P &predicate, bool compress = true)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename A::value_type);

	return boost::algorithm::split(array, v, predicate, compress ? boost::algorithm::token_compress_on : boost::algorithm::token_compress_off);
}

// Es.:
// 	std::string str = "alfa\r\nbeta\r\ngamma";
//	std::vector<std::string> result;
//	algorithms::split(str, result, boost::algorithm::first_finder("\r\n", boost::algorithm::is_equal()));

template <typename T, typename A, typename P>
inline A & iter_split(const T &v, A &array, const P &predicate)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename A::value_type);

	return boost::algorithm::iter_split(array, v, predicate);
}

// Es.:
// 	std::string str = "alfa&amp;beta&amp;gamma";
//	std::vector<std::string> result;
//	algorithms::split_regex(str, result, boost::regex("&amp;"));

template <typename T, typename A, typename RC, typename RT>
inline A & split_regex(const T &v, A &array, const boost::basic_regex<RC, RT> &regex)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(T);
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename A::value_type);

	return boost::algorithm::split_regex(array, v, regex);
}

// Concatena l'array in una stringa (se non specificato usa un separatore vuoto)
// Es.:
//	std::vector<std::string> vector;
//	vector.push_back("alfa");
//	vector.push_back("beta");
//	algorithms::join(vector);		// "alfabeta"
//	algorithms::join(vector, ";");	// "alfa;beta"

template <typename T>
inline typename boost::range_value<T>::type join(const T &array)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename T::value_type);

	return boost::algorithm::join(array, typename boost::range_value<T>::type());
}

template <typename T>
inline typename boost::range_value<T>::type join(const T &array, const typename boost::range_value<T>::type &separator)
{
	OS_BOOST_ALGORITHM_STATIC_ASSERT(typename T::value_type);

	return boost::algorithm::join(array, separator);
}

//////////////////////////////////////////////////////////////////////

template <typename T1, typename T2, bool>
struct tokenize_deduce_length
{
	inline static size_t length(const T2 &str)
	{
		return str.size();
	}
};

template <typename T1, typename T2>
struct tokenize_deduce_length<T1, T2, false>
{
	inline static size_t length(const T2 &str)
	{
		return T1::traits_type::length(str);
	}
};

template <typename T, typename P>
T tokenize_next(const T &str, const P &search, typename T::size_type &pos)
{
	T result;
	if(pos != T::npos)
	{
		size_t token = str.find(search, pos);
		if(token != T::npos)
		{
			size_t len = token - pos;
			if(len > 0)
				result = mid(str, pos, len);

			pos += len + tokenize_deduce_length<T, P, has_const_iterator<P>::value>::length(search);
			OS_ASSERT(pos <= str.length());
			if(pos == str.length())
				pos = T::npos;
		}
		else
		{
			result = mid(str, pos);
			pos = T::npos;
		}
	}

	return result;
}

template <typename T>
inline T combine(const T &first, const T &second, const T &separator)
{
	if(first.empty())
		return second;

	if(second.empty())
		return first;

	return first + separator + second;
}

template <typename S, typename M>
void encode_map(S &str, const M &map, const S &keys_separator, const S &values_separator)
{
	str.clear();
	for(typename M::const_iterator i = map.begin(); i != map.end(); ++i)
	{
		if(i != map.begin())
			str += keys_separator;

		str += i->first + values_separator + i->second;
	}
}

template <typename S, typename M>
void decode_map(M &map, const S &str, const S &keys_separator, const S &values_separator, bool trim = true)
{
	map.clear();

	typename vector<S>::type keys;
	algorithms::split(str, keys, boost::algorithm::is_any_of(keys_separator.c_str()), true);

	for(typename vector<S>::type::iterator i = keys.begin(); i != keys.end(); ++i)
	{
		S &str = *i;

		typename S::size_type pos = str.find(values_separator);
		if(pos != S::npos)
		{
			S name = algorithms::left(str, pos);
			S value = algorithms::mid(str, pos + values_separator.length());

			if(trim)
			{
				algorithms::trim(name);
				algorithms::trim(value);
			}

			map[name] = value;
		}
	}
}

//////////////////////////////////////////////////////////////////////

template<>
CoreExport void to_lower<String>(String &v);

template<>
CoreExport void to_upper<String>(String &v);

template <>
CoreExport void trim_left<String>(String &v);

template <>
CoreExport void trim_right<String>(String &v);

template <>
CoreExport void trim<String>(String &v);

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((algorithms))

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ALGORITHMS_H
