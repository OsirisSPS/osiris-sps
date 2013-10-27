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
#include "soundmanager.h"

#include "conversions.h"
#include "filesystem.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

SoundManager::SoundManager() : m_enabled(true)
{

}

SoundManager::~SoundManager()
{
	clear();
}

bool SoundManager::getEnabled() const
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);
	return m_enabled;
}

void SoundManager::setEnabled(bool enabled)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	m_enabled = enabled;
	if(m_enabled == false)
		clear();
}

shared_ptr<wxSound> SoundManager::loadSound(const String &filename)
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);

	if(m_enabled == false)
		return null;

	String standardisedPath = utils::standardisePath(filename, false);
	OS_ASSERT(FileSystem::instance()->isRelativePath(standardisedPath) == false);

	Sounds::const_iterator i = m_sounds.find(standardisedPath);
	if(i != m_sounds.end())
		return i->second;

	shared_ptr<wxSound> sound(new wxSound());
	if(sound->Create(conversions::from_utf16<wxString>(standardisedPath)) == false)
	{
		OS_ASSERTFALSE();
		return null;
	}

	m_sounds[standardisedPath] = sound;
	return sound;
}

shared_ptr<wxSound> SoundManager::playSound(const String &filename, uint32 mode)
{
	shared_ptr<wxSound> sound = loadSound(filename);
	if(sound == null)
		return null;

	sound->Play(mode);
	return sound;
}

void SoundManager::stopAllSounds()
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);
	for(Sounds::const_iterator i = m_sounds.begin(); i != m_sounds.end(); i->second->Stop(), ++i);		
}

void SoundManager::clear()
{
	boost::recursive_mutex::scoped_lock lock(m_mutex);
	m_sounds.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
