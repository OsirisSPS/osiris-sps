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

#ifndef _OS_IRC_IRCCOMMANDMODE_H
#define _OS_IRC_IRCCOMMANDMODE_H

#include "iirccommand.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IRCExport IRCCommandMode : public IIRCCommand
{
    typedef IIRCCommand CommandBase;

// Enumerated types
public:
	typedef std::pair<IRCModeType, bool> ModeOption;		// Mode <-> enabled/disabled
	typedef list<ModeOption>::type ModeOptions;

// Construction
public:
	IRCCommandMode(shared_ptr<IRCSession> session);
	virtual ~IRCCommandMode();

// Attributes
public:
	inline const std::string & getTarget() const;
	inline void setTarget(const std::string &target);

	inline const ModeOptions & getModeOptions() const;

	inline const std::string & getArg() const;
	inline void setArg(const std::string &arg);
	inline bool hasArg() const;
	
	IRCTargetType getTargetType() const;

// Operations
public:
	void addMode(IRCModeType mode, bool enabled);

// IIRCCommand interface
public:
	virtual IRCCommandType getType() const;

// IRCCommandMode overrides
public:
	virtual std::string toString() const;

private:
	std::string m_target;
	ModeOptions m_modeOptions;
	std::string m_arg;
};

//////////////////////////////////////////////////////////////////////

inline const std::string & IRCCommandMode::getTarget() const { return m_target; }
inline void IRCCommandMode::setTarget(const std::string &target) { m_target = target; }

inline const IRCCommandMode::ModeOptions & IRCCommandMode::getModeOptions() const { return m_modeOptions; }

inline const std::string & IRCCommandMode::getArg() const { return m_arg; }
inline void IRCCommandMode::setArg(const std::string &arg) { m_arg = arg; }
inline bool IRCCommandMode::hasArg() const { return m_arg.empty() == false; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IRC_IRCCOMMANDMODE_H
