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

#ifndef _OS_PYTHON_PYTHONENGINE_H
#define _OS_PYTHON_PYTHONENGINE_H

#include "boost/thread/mutex.hpp"
#include "boost/thread/shared_mutex.hpp"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

#define OS_PYTHON_MAIN_MODULE_NAME			"__main__"
#define OS_PYTHON_MODULE_DICTIONARY			"__dict__"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IJob;
class PythonInterpreter;

//////////////////////////////////////////////////////////////////////

class PythonEngine : public StaticSingleton<PythonEngine>
{
	typedef unordered_map<std::string, boost::python::object>::type ModulesMap;
	typedef deque<shared_ptr<PythonInterpreter> >::type Interpreters;
	typedef map<PyThreadState *, shared_ptr<boost::recursive_mutex> >::type InterpretersLocks;

public:
	class Lock
	{
	// Construction
	public:
		Lock();
		~Lock();
	};

// Internal
private:	
	class ThreadsManager;
	class InterpretersJob;

// Construction
protected:
	PythonEngine();

public:
	virtual ~PythonEngine();

// Attributes
public:
	inline boost::shared_mutex & getEvalCS();

	boost::recursive_mutex & getInterpreterMutex(PyThreadState *state);

	bool requiresInterpreters() const;

	static bool getLastError(std::string &type, std::string &message, std::string &callstack, bool reset);
	static bool getLastError(std::string &error, bool reset);

// Operations
public:
	bool init(const String &modulesPath, const String &homePath);
	void cleanup();

	void run();

	bool loadModule(const char *module_name, void (*module_function)(void));
	void logLastError(bool reset);
	
	void ensureLock();
	void releaseLock();

	PyThreadState * saveThread();
	void restoreThread(PyThreadState *threadState);

	shared_ptr<PythonInterpreter> createInterpreter();
	bool cacheInterpreter();

private:
	shared_ptr<PythonInterpreter> allocateInterpreter(bool force);
	void initInterpreterLock(PyThreadState *state);
	void releaseInterpreterLock(PyThreadState *state);
	void deallocateInterpreter(PythonInterpreter *interpreter);
	void queueInterpretersJob();	

	static bool invokeCallback(boost::python::detail::exception_handler const &, boost::function0<void> const &callback);

private:
	PyThreadState *m_globalState;
	scoped_ptr<ThreadsManager> m_threadsManager;
	std::string m_homePath;
	ModulesMap m_modulesMap;	
	scoped_ptr<boost::python::detail::exception_handler, os_delete_t> m_exceptionHandler;
	mutable boost::mutex m_interpretersCS;
	Interpreters m_interpreters;	
	boost::shared_mutex m_evalCS;
	weak_ptr<IJob> m_interpretersJob;
	bool m_initialized;
	InterpretersLocks m_interpretersLock;
	boost::mutex m_interpretersLockMutex;
};

//////////////////////////////////////////////////////////////////////

inline boost::shared_mutex & PythonEngine::getEvalCS() { return m_evalCS; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONENGINE_H
