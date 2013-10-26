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

#ifndef _OS_UI_SOUNDMANAGER_H
#define _OS_UI_SOUNDMANAGER_H

#include "boost/thread/recursive_mutex.hpp"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class SoundManager : public StaticSingleton<SoundManager>
{
	typedef unordered_map<String, shared_ptr<wxSound> >::type Sounds;

// Construction
public:
    SoundManager();
	virtual ~SoundManager();

// Attributes
public:
	bool getEnabled() const;
	void setEnabled(bool enabled);

// Operations
public:
	shared_ptr<wxSound> loadSound(const String &filename);
	shared_ptr<wxSound> playSound(const String &filename, uint32 mode = wxSOUND_ASYNC);

	void stopAllSounds();

	void clear();
	
private:
	mutable boost::recursive_mutex m_mutex;
	bool m_enabled;
	Sounds m_sounds;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_SOUNDMANAGER_H
