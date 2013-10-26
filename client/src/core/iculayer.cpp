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
#include "iculayer.h"

#include "allocators.h"
#include "boost/scoped_array.hpp"
#include "unicode/ustdio.h"
#include "unicode/ustring.h"
#include "singleton.h"

////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

////////////////////////////////////////////////////////////////////

OS_STATIC_ASSERT(sizeof(uchar) == sizeof(UChar));

////////////////////////////////////////////////////////////////////

class AsciiConverter : public StaticSingleton<AsciiConverter>
{
// Construction
protected:
	AsciiConverter()
	{
		UErrorCode status = U_ZERO_ERROR;
		m_converter = ucnv_open("ASCII", &status);
		OS_ASSERT(m_converter != null);
		OS_ASSERT(U_SUCCESS(status));
	}

public:
	~AsciiConverter()
	{
		if(m_converter != null)
		{
			ucnv_close(m_converter);
			m_converter = null;
		}
	}

// Operations
public:
	std::ustring fromAscii(const achar *str, icu_size length)
	{
		OS_ASSERT(m_converter != null);

		std::ustring result;
		if(str == null)
			return result;

		UErrorCode status = U_ZERO_ERROR;
		icu_size size = ucnv_toUChars(m_converter, null, 0, str, length, &status);
		if(U_SUCCESS(status))
		{
			OS_ASSERT(size == 0);
			return result;
		}

		if(status != U_BUFFER_OVERFLOW_ERROR)
		{
			OS_ASSERTFALSE();
			return result;
		}

		status = U_ZERO_ERROR;

		scoped_array<uchar, os_deallocate_t> buffer(OS_ALLOCATE_T(uchar, size));
		size = ucnv_toUChars(m_converter, static_cast<UChar *>(buffer.get()), size, str, length, &status);
		if(U_SUCCESS(status))
		{
			result.assign(buffer.get(), size);
		}
		else
		{
			OS_ASSERTFALSE();
		}

		return result;
	}

	std::string toAscii(const std::ustring &str)
	{
		OS_ASSERT(m_converter != null);

		std::string result;

		UErrorCode status = U_ZERO_ERROR;
		icu_size size = ucnv_fromUChars(m_converter, null, 0, str.data(), static_cast<icu_size>(str.size()), &status);
		if(U_SUCCESS(status))
		{
			OS_ASSERT(size == 0);
			return result;
		}

		if(status != U_BUFFER_OVERFLOW_ERROR)
		{
			OS_ASSERTFALSE();
			return result;
		}

		status = U_ZERO_ERROR;

		scoped_array<achar, os_deallocate_t> buffer(OS_ALLOCATE_T(achar, size));
		size = ucnv_fromUChars(m_converter, buffer.get(), size, str.data(), static_cast<icu_size>(str.size()), &status);
		if(U_SUCCESS(status))
		{
			result.assign(buffer.get(), size);
		}
		else
		{
			OS_ASSERTFALSE();
		}

		return result;
	}

private:
	UConverter *m_converter;
};

////////////////////////////////////////////////////////////////////

static std::ustring icu_transform(const std::ustring &str, bool lower)
{
	std::ustring result;
	icu_size size = 0;
	UErrorCode status = U_ZERO_ERROR;
	if(lower)
		size = u_strToLower(null, 0, static_cast<const UChar *>(str.data()), static_cast<icu_size>(str.size()), null, &status);
	else
		size = u_strToUpper(null, 0, static_cast<const UChar *>(str.data()), static_cast<icu_size>(str.size()), null, &status);

	if(U_SUCCESS(status))
	{
		OS_ASSERT(size == 0);
		return result;
	}

    if(status != U_BUFFER_OVERFLOW_ERROR)
    {
        OS_ASSERTFALSE();
        return result;
    }

    status = U_ZERO_ERROR;
	scoped_array<uchar, os_deallocate_t> buffer(OS_ALLOCATE_T(uchar, size));
	if(lower)
		size = u_strToLower(static_cast<UChar *>(buffer.get()), size, static_cast<const UChar *>(str.data()), static_cast<icu_size>(str.size()), null, &status);
	else
		size = u_strToUpper(static_cast<UChar *>(buffer.get()), size, static_cast<const UChar *>(str.data()), static_cast<icu_size>(str.size()), null, &status);

	if(U_SUCCESS(status))
    {
		result.assign(buffer.get(), size);
    }
	else
	{
		OS_ASSERTFALSE();
	}

	return result;
}

////////////////////////////////////////////////////////////////////

std::ustring icu_asciitou(const achar *str, icu_size length)
{
	return AsciiConverter::instance()->fromAscii(str, length);
}

std::ustring icu_asciitou(const std::string &str)
{
	return icu_asciitou(str.data(), static_cast<icu_size>(str.size()));
}

std::string icu_utoascii(const std::ustring &str)
{
	return AsciiConverter::instance()->toAscii(str);
}

std::ustring icu_utf8tou(const achar *str, icu_size length)
{
	std::ustring result;
    if(str == null)
        return result;

    UErrorCode status = U_ZERO_ERROR;
    icu_size size = 0;
    u_strFromUTF8(null, 0, &size, str, length, &status);
	if(U_SUCCESS(status))
	{
		OS_ASSERT(size == 0);
		return result;
	}

    if(status != U_BUFFER_OVERFLOW_ERROR)
    {
        OS_ASSERTFALSE();
        return result;
    }

    status = U_ZERO_ERROR;

    scoped_array<uchar, os_deallocate_t> buffer(OS_ALLOCATE_T(uchar, size));
    u_strFromUTF8(static_cast<UChar *>(buffer.get()), size, &size, str, length, &status);
    if(U_SUCCESS(status))
    {
		result.assign(buffer.get(), size);
    }
	else
	{
		OS_ASSERTFALSE();
	}

    return result;
}

CoreExport std::ustring icu_utf8tou(const std::string &str)
{
	return icu_utf8tou(str.data(), static_cast<icu_size>(str.size()));
}

std::string icu_utoutf8(const std::ustring &str)
{
	std::string result;

    UErrorCode status = U_ZERO_ERROR;
    icu_size size = 0;
    u_strToUTF8(null, 0, &size, str.data(), static_cast<icu_size>(str.size()), &status);
	if(U_SUCCESS(status))
	{
		OS_ASSERT(size == 0);
		return result;
	}

    if(status != U_BUFFER_OVERFLOW_ERROR)
    {
        OS_ASSERTFALSE();
        return result;
    }

    status = U_ZERO_ERROR;

    scoped_array<achar, os_deallocate_t> buffer(OS_ALLOCATE_T(achar, size));
	u_strToUTF8(buffer.get(), size, &size, str.data(), static_cast<icu_size>(str.size()), &status);
    if(U_SUCCESS(status))
    {
		result.assign(buffer.get(), size);
    }
	else
	{
		OS_ASSERTFALSE();
	}

	return result;
}

std::ustring icu_wtou(const wchar *str, icu_size length)
{
	std::ustring result;
    if(str == null)
        return result;

#ifdef OS_HAS_NATIVE_UCHAR
	if(length != -1)
		result.assign(str, length);
	else
		result.assign(str);

	return result;
#else
    UErrorCode status = U_ZERO_ERROR;
    icu_size size = 0;
    u_strFromWCS(null, 0, &size, str, length, &status);
	if(U_SUCCESS(status))
	{
		OS_ASSERT(size == 0);
		return result;
	}

    if(status != U_BUFFER_OVERFLOW_ERROR)
    {
        OS_ASSERTFALSE();
        return result;
    }

    status = U_ZERO_ERROR;

    scoped_array<uchar, os_deallocate_t> buffer(OS_ALLOCATE_T(uchar, size));
    u_strFromWCS(static_cast<UChar *>(buffer.get()), size, &size, str, length, &status);
    if(U_SUCCESS(status))
    {
		result.assign(buffer.get(), size);
    }
	else
	{
		OS_ASSERTFALSE();
	}

    return result;
#endif // OS_HAS_NATIVE_UCHAR
}

CoreExport std::ustring icu_wtou(const std::wstring &str)
{
#ifdef OS_HAS_NATIVE_UCHAR
	return str;
#else
	return icu_wtou(str.data(), static_cast<icu_size>(str.size()));
#endif // OS_HAS_NATIVE_UCHAR
}

std::wstring icu_utow(const std::ustring &str)
{
#ifdef OS_HAS_NATIVE_UCHAR
	return str;
#else
	std::wstring result;

    UErrorCode status = U_ZERO_ERROR;
    icu_size size = 0;
    u_strToWCS(null, 0, &size, str.data(), static_cast<icu_size>(str.size()), &status);
	if(U_SUCCESS(status))
	{
		OS_ASSERT(size == 0);
		return result;
	}

    if(status != U_BUFFER_OVERFLOW_ERROR)
    {
        OS_ASSERTFALSE();
        return result;
    }

    status = U_ZERO_ERROR;

    scoped_array<wchar, os_deallocate_t> buffer(OS_ALLOCATE_T(wchar, size));
	u_strToWCS(buffer.get(), size, &size, str.data(), static_cast<icu_size>(str.size()), &status);
    if(U_SUCCESS(status))
    {
		result.assign(buffer.get(), size);
    }
	else
	{
		OS_ASSERTFALSE();
	}

	return result;
#endif // OS_HAS_NATIVE_UCHAR
}

bool icu_compare_no_case(const std::ustring &first, const std::ustring &second)
{
	UErrorCode status = U_ZERO_ERROR;
	return u_strCaseCompare(static_cast<const UChar *>(first.data()), static_cast<icu_size>(first.size()), static_cast<const UChar *>(second.data()), static_cast<icu_size>(second.size()), 0, &status) == 0;
}

std::ustring icu_to_lower(const std::ustring &str)
{
	return icu_transform(str, true);
}

std::ustring icu_to_upper(const std::ustring &str)
{
	return icu_transform(str, false);
}

std::ustring icu_format_string(const uchar *pattern, va_list args)
{
    va_list argscopy;

	std::ustring formattedString;
	if(pattern != null)
	{
		OS_ASSERT(String(pattern).find(_S("%c")) == String::npos);	// %c è considerato dall'icu come carattere ASCII non UNICODE, è necessario specificare %C per i caratteri UNICODE
		OS_ASSERT(String(pattern).find(_S("%s")) == String::npos);	// %s è considerata dall'icu stringa ASCII non UNICODE, è necessario specificare %S per le stringhe UNICODE

		icu_size bufferSize = u_strlen(static_cast<const UChar *>(pattern)) * 2;
		if(bufferSize > 0)
		{
			uchar *buffer = static_cast<uchar *>(OS_ALLOCATE(bufferSize * sizeof(uchar)));
			while(buffer != null)
			{
#if OS_PLATFORM == OS_PLATFORM_WIN
				// va_copy non è implementata sotto win
				argscopy = args;
#else
			    va_copy(argscopy, args);
#endif
				int32 result = u_vsnprintf_u(static_cast<UChar *>(buffer), bufferSize, static_cast<const UChar *>(pattern), argscopy);

				va_end(argscopy);

				// In caso di errore viene restituito un valore negativo, altrimenti il valore viene troncato alla
				// dimensione del buffer specificato (se non sufficiente) per cui il modo per assicurare che il buffer sia abbastanza capiente
				// è verificare che la dimensione restituita sia inferiore a quella totale
				if(result >= 0 && result < bufferSize)
				{
					formattedString.assign(buffer, result);
					break;
				}

				// Raddoppia la dimensione del buffer se necessario
				bufferSize *= 2;
				// Rialloca il buffer
				buffer = static_cast<uchar *>(OS_REALLOCATE(buffer, bufferSize * sizeof(uchar)));
			}

			OS_DEALLOCATE(buffer);
		}
	}

	return formattedString;
}

/* Old Version
std::ustring icu_format_string(const uchar *pattern, va_list args)
{
	std::ustring formattedString;
	if(pattern != null)
	{
		OS_ASSERT(String(pattern).find(_S("%c")) == String::npos);	// %c è considerato dall'icu come carattere ASCII non UNICODE, è necessario specificare %C per i caratteri UNICODE
		OS_ASSERT(String(pattern).find(_S("%s")) == String::npos);	// %s è considerata dall'icu stringa ASCII non UNICODE, è necessario specificare %S per le stringhe UNICODE

		icu_size bufferSize = u_strlen(static_cast<const UChar *>(pattern)) * 2;
		if(bufferSize > 0)
		{
			uchar *buffer = static_cast<uchar *>(OS_ALLOCATE(bufferSize * sizeof(uchar)));
			while(buffer != null)
			{
				int32 result = u_vsnprintf_u(static_cast<UChar *>(buffer), bufferSize, static_cast<const UChar *>(pattern), args);
				// In caso di errore viene restituito un valore negativo, altrimenti il valore viene troncato alla
				// dimensione del buffer specificato (se non sufficiente) per cui il modo per assicurare che il buffer sia abbastanza capiente
				// è verificare che la dimensione restituita sia inferiore a quella totale
				if(result >= 0 && result < bufferSize)
				{
					formattedString.assign(buffer, result);
					break;
				}

				// Raddoppia la dimensione del buffer se necessario
				bufferSize *= 2;
				// Rialloca il buffer
				buffer = static_cast<uchar *>(OS_REALLOCATE(buffer, bufferSize * sizeof(uchar)));
			}

			OS_DEALLOCATE(buffer);
		}
	}

	return formattedString;
}
*/

icu_size icu_scanf(const uchar *str, const uchar *pattern, va_list args)
{
	// Il punto può generare ambiguità in alcune circostanze come "%d.%d" dove un valore 5.3 potrebbe ad esempio essere interpretato come double
	OS_ASSERT(String(pattern).find(_S(".")) == String::npos);
	return u_vsscanf_u(static_cast<const UChar *>(str), static_cast<const UChar *>(pattern), args);
}

////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

////////////////////////////////////////////////////////////////////
