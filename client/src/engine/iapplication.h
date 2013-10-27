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

#ifndef _OS_ENGINE_IAPPLICATION_H
#define _OS_ENGINE_IAPPLICATION_H

#include "base/object.h"
#include "irunnable.h"
#include "platformmanager.h"

//////////////////////////////////////////////////////////////////////

#define OS_IAPPLICATION_COMMANDS_SEPARATOR		"\r\n"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IApplication : public Object,
								  public IRunnable
{
	class NotifyListener;
	struct ProcessData;

// Construction
public:
	IApplication(PlatformManager::ApplicationType applicationType);
	virtual ~IApplication();

// Attributes
public:
	inline bool getRecoveryMode() const;
	inline void setRecoveryMode(bool recoveryMode);

	inline bool getVerbose() const;
	inline void setVerbose(bool verbose);

	bool isPrimaryProcess() const;

// IRunnable interface
public:
	virtual bool start();
	virtual void stop();

// Operations
public:
    bool init(int argc, char *argv[]);
	bool init(int argc, wchar_t *argv[]);
	bool init(const String &rootPath);
	void cleanup();

protected:
	void addProcessCommand(const String &command);

// Interface
protected:
	virtual void handleProcessCommands(const std::string &commands) = 0;

private:
	void initProcess();
	void cleanupProcess();
	bool registerAsPrimaryProcess();
	bool queryPrimaryProcessID(uint32 currentProcessID, uint32 &primaryProcessID) const;
	bool postPrimaryProcessCommand(uint32 processID, std::string command);
	bool peekProcessData(std::string &data);
	void handleProcessCommandsCallback();
	
private:
	String m_commands;
	bool m_recoveryMode;
	bool m_verbose;
	uint32 m_processID;
	uint32 m_primaryProcessID;
	scoped_ptr<NotifyListener> m_notifyListener;
};

//////////////////////////////////////////////////////////////////////

inline bool IApplication::getRecoveryMode() const { return m_recoveryMode; }
inline void IApplication::setRecoveryMode(bool recoveryMode) { m_recoveryMode = recoveryMode; }

inline bool IApplication::getVerbose() const { return m_verbose; }
inline void IApplication::setVerbose(bool verbose) { m_verbose = verbose; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_IAPPLICATION_H
