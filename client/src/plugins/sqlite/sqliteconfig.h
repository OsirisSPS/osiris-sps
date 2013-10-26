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

#ifndef _SQLITE_CONFIG_H
#define _SQLITE_CONFIG_H

//////////////////////////////////////////////////////////////////////

#include "dbdb.h"
#include "sqlitestandards.h"
#include "sqlitemacros.h"

//////////////////////////////////////////////////////////////////////

namespace sqlite {

//////////////////////////////////////////////////////////////////////

// Sqlite non definisce un tipo per l'int (a differenza di sqlite_int64) e osiris::int32 sarebbe errato nel caso di un sistema a 64 bits,
// pertanto ovunque nell'sqlite ci siano api con parametri/valori di ritorno di tipo "int" viene utilizzato "sqlite_int" (da modificare se in futuro supportato nativamente da sqlite)
typedef int sqlite_int;

//////////////////////////////////////////////////////////////////////

class Connection;
class Driver;
class Options;
class Parameter;
class Parser;
class Result;
class Statement;

//////////////////////////////////////////////////////////////////////

} // sqlite

//////////////////////////////////////////////////////////////////////

using namespace sqlite;

//////////////////////////////////////////////////////////////////////

#endif // _SQLITE_CONFIG_H
