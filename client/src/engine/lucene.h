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

#ifndef _OS_ENGINE_LUCENE_H
#define _OS_ENGINE_LUCENE_H

//////////////////////////////////////////////////////////////////////

// TODO: fixme (al momento clucene viene compilato in debug solo sotto windows, sotto linux da errori di link...)
#if OS_PLATFORM != OS_PLATFORM_WIN
    #ifdef _DEBUG
        #define OS_LUCENE_RESTORE_DEBUG
        #undef _DEBUG
    #endif
#endif

//////////////////////////////////////////////////////////////////////

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(disable : 4244)		// 'conversion from <type> to <type>, possible loss of data'
	#pragma warning(disable : 4267)		// 'conversion from <type> to <type>, possible loss of data'
#endif

#include "CLucene/StdHeader.h"

#include "CLucene/analysis/Analyzers.h"
#include "CLucene/analysis/standard/StandardAnalyzer.h"
#include "CLucene/document/Document.h"
#include "CLucene/document/Field.h"
#include "CLucene/index/IndexReader.h"
#include "CLucene/index/IndexWriter.h"
#include "CLucene/index/IndexModifier.h"
#include "CLucene/queryParser/QueryParser.h"
#include "CLucene/search/IndexSearcher.h"
#include "CLucene/search/TermQuery.h"
#include "CLucene/store/FSDirectory.h"

#if OS_COMPILER	== OS_COMPILER_MSVC
	#pragma warning(default : 4244)
	#pragma warning(default : 4267)
#endif

//////////////////////////////////////////////////////////////////////

#ifdef OS_LUCENE_RESTORE_DEBUG
    #undef OS_LUCENE_RESTORE_DEBUG
    #define _DEBUG
#endif

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_LUCENE_H
