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
#include "pythonengine.h"

#include "base/object.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/thread.hpp"
#include "engine.h"
#include "filesystem.h"
#include "ibackgroundjob.h"
#include "log.h"
#include "platformmanager.h"
#include "pythonconverters.h"
#include "pythoninterpreter.h"
#include "pythonstate.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_PYTHON_PATH_ENVIRONMENT_VARIABLE			"PYTHONPATH"

#if OS_PLATFORM	== OS_PLATFORM_WIN
	#define OS_PYTHON_PATH_DELIMITER				";"
#else
	#define OS_PYTHON_PATH_DELIMITER				":"
#endif

#define OS_PYTHON_FOLDER_LIB						"lib"

#define OS_PYTHON_FOLDER_SITE_PACKAGES				"site-packages"

// TODO: definire opzioni
#define OS_PYTHON_CACHE_SIZE						10
#define OS_PYTHON_CACHE_THRESHOLD					5
#define OS_PYTHON_CACHE_JOB_DELAY					100

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

PythonEngine::Lock::Lock()
{
	PythonEngine::instance()->ensureLock();
}

PythonEngine::Lock::~Lock()
{
	PythonEngine::instance()->releaseLock();
}

//////////////////////////////////////////////////////////////////////

class PythonEngine::ThreadsManager : public Object
{
// Internal
private:
	class ThreadData : public Object
	{
	public:
		ThreadData(bool alreadyLocked) : m_locksCount(alreadyLocked ? 1 : 0)
		{

		}

		virtual ~ThreadData()
		{
			OS_ASSERT(empty());
		}

	private:
		bool empty()
		{
			boost::mutex::scoped_lock lock(m_mutex);
			return m_locksCount == 0;
		}

	public:
		void acquireLock()
		{
			boost::mutex::scoped_lock lock(m_mutex);

			doAcquireLock();
		}

		bool releaseLock()
		{
			boost::mutex::scoped_lock lock(m_mutex);

			return doReleaseLock();
		}

		PyThreadState * saveThread()
		{
			boost::mutex::scoped_lock lock(m_mutex);

			PyThreadState *result = doSaveThread();
			doReleaseLock();
			
			return result;
		}

		void restoreThread(PyThreadState *threadState)
		{
			boost::mutex::scoped_lock lock(m_mutex);

			doAcquireLock();
			doRestoreThread(threadState);
		}

	private:
		void doAcquireLock()
		{
			OS_ASSERT(m_mutex.try_lock() == false);		// questa funzione va loccata a monte
			
			if(m_locksCount == 0)
				PyEval_AcquireLock();

			m_locksCount++;
		}

		bool doReleaseLock()
		{
			OS_ASSERT(m_mutex.try_lock() == false);		// questa funzione va loccata a monte

			OS_ASSERT(m_locksCount > 0);
			if((--m_locksCount) == 0)
			{
				PyEval_ReleaseLock();
				return true;
			}

			return false;
		}

		PyThreadState * doSaveThread()
		{			
			OS_ASSERT(m_mutex.try_lock() == false);		// questa funzione va loccata a monte

			OS_ASSERT(m_locksCount > 0);

			// N.B.: non richiamare qui PyEval_SaveThread perchè internamente fa una PyEval_ReleaseLock
			PyThreadState *savedThread = PyThreadState_Swap(null);	
			OS_ASSERT(savedThread != null);
			return savedThread;
		}

		void doRestoreThread(PyThreadState *savedThread)
		{
			OS_ASSERT(m_mutex.try_lock() == false);		// questa funzione va loccata a monte

			// N.B.: non richiamare qui PyEval_RestoreThread perchè internamente fa una PyEval_AcquireLock
			OS_ASSERT(savedThread != null);
			PyThreadState_Swap(savedThread);			
		}

	private:
		uint32 m_locksCount;
		boost::mutex m_mutex;
	};

	typedef map<boost::thread::id, shared_ptr<ThreadData> >::type ThreadsMap;

// Construction
public:
	ThreadsManager(const boost::thread::id &mainThreadID);
	virtual ~ThreadsManager();

// Attributes
public:
	bool empty();

// Operations
public:
	void ensureLock();
	void releaseLock();

	PyThreadState * saveThread();
	void restoreThread(PyThreadState *threadState);

	shared_ptr<ThreadData> getThread(const boost::thread::id &threadID, bool ensure);

private:
	boost::mutex m_mutex;
	ThreadsMap m_threadsMap;
};

//////////////////////////////////////////////////////////////////////

PythonEngine::ThreadsManager::ThreadsManager(const boost::thread::id &mainThreadID)
{
	// Il thread che chiama PyEval_InitThreads inizialmente ottiene anche la lock

	boost::mutex::scoped_lock lock(m_mutex);
	m_threadsMap[mainThreadID] = shared_ptr<ThreadData>(OS_NEW ThreadData(true));
}

PythonEngine::ThreadsManager::~ThreadsManager()
{
	OS_ASSERT(empty());
}

bool PythonEngine::ThreadsManager::empty()
{
	boost::mutex::scoped_lock lock(m_mutex);
	return m_threadsMap.empty();
}

void PythonEngine::ThreadsManager::ensureLock()
{
	// N.B.: più thread contemporaneamente devono poter chiamare PyEval_AcquireLock quindi non loccare m_mutex

	shared_ptr<ThreadData> threadData = getThread(boost::this_thread::get_id(), true);
	OS_ASSERT(threadData != null);
	threadData->acquireLock();
}

void PythonEngine::ThreadsManager::releaseLock()
{
	// N.B.: più thread contemporaneamente devono poter chiamare PyEval_ReleaseLock quindi non loccare m_mutex

	boost::thread::id threadID = boost::this_thread::get_id();

	shared_ptr<ThreadData> threadData = getThread(threadID, false);
	if(threadData == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	if(threadData->releaseLock())
	{
		boost::mutex::scoped_lock lock(m_mutex);
		m_threadsMap.erase(threadID);
	}
}

PyThreadState * PythonEngine::ThreadsManager::saveThread()
{
	// N.B.: più thread contemporaneamente devono poter chiamare PyEval_SaveThread quindi non loccare m_mutex

	shared_ptr<ThreadData> threadData = getThread(boost::this_thread::get_id(), true);
	OS_ASSERT(threadData != null);
	return threadData->saveThread();
}

void PythonEngine::ThreadsManager::restoreThread(PyThreadState *threadState)
{
	// N.B.: più thread contemporaneamente devono poter chiamare PyEval_RestoreThread quindi non loccare m_mutex

	shared_ptr<ThreadData> threadData = getThread(boost::this_thread::get_id(), true);
	if(threadData == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	threadData->restoreThread(threadState);
}

shared_ptr<PythonEngine::ThreadsManager::ThreadData> PythonEngine::ThreadsManager::getThread(const boost::thread::id &threadID, bool ensure)
{
	boost::mutex::scoped_lock lock(m_mutex);

	ThreadsMap::iterator i = m_threadsMap.find(threadID);
	if(i != m_threadsMap.end())
		return i->second;

	if(ensure == false)
		return null;

	shared_ptr<ThreadData> threadData(OS_NEW ThreadData(false));
	m_threadsMap[threadID] = threadData;
	return threadData;
}

//////////////////////////////////////////////////////////////////////

class PythonEngine::InterpretersJob : public IBackgroundJob
{
	typedef IBackgroundJob JobBase;

// Construction
public:
	InterpretersJob(uint32 id);
	virtual ~InterpretersJob();

// IJob interface
protected:
	virtual JobStatus run();

// IBackgroundJob interface
public:
	virtual shared_ptr<Details> getDetails() const;
};

//////////////////////////////////////////////////////////////////////

PythonEngine::InterpretersJob::InterpretersJob(uint32 id) : JobBase(false, id)
{

}

PythonEngine::InterpretersJob::~InterpretersJob()
{

}

IJob::JobStatus PythonEngine::InterpretersJob::run()
{
	while(PythonEngine::instance()->requiresInterpreters())
	{
		if(PythonEngine::instance()->cacheInterpreter() == false)
		{
			applyDelay(OS_PYTHON_CACHE_JOB_DELAY);
			return jobIncomplete;
		}
	}

	return jobComplete;
}

shared_ptr<IBackgroundJob::Details> PythonEngine::InterpretersJob::getDetails() const
{
	return shared_ptr<IBackgroundJob::Details>(OS_NEW IBackgroundJob::Details(L"python"));
}

//////////////////////////////////////////////////////////////////////

PythonEngine::PythonEngine() : m_globalState(null),
							   m_exceptionHandler(OS_NEW_T(boost::python::detail::exception_handler)(&PythonEngine::invokeCallback)),
							   m_initialized(false)
{

}

PythonEngine::~PythonEngine()
{
	cleanup();
	OS_ASSERT(m_globalState == null);
}

boost::recursive_mutex & PythonEngine::getInterpreterMutex(PyThreadState *state)
{
	boost::mutex::scoped_lock lock(m_interpretersLockMutex);

	OS_ASSERT(m_interpretersLock.find(state) != m_interpretersLock.end());
	return *(*m_interpretersLock.find(state)).second;
}

bool PythonEngine::requiresInterpreters() const
{
	boost::mutex::scoped_lock lock(m_interpretersCS);
	return m_interpreters.size() < OS_PYTHON_CACHE_SIZE;
}

static char *PyTraceback_AsString(PyObject *exc_tb)
{
	#define TRACEBACK_FETCH_ERROR(what) {errMsg = what; goto done;}

	const char *errMsg = NULL; /* holds a local error message */
	char *result = NULL; /* a valid, allocated result. */
	PyObject *modStringIO = NULL;PyObject *modTB = NULL;
	PyObject *obFuncStringIO = NULL;
	PyObject *obStringIO = NULL;
	PyObject *obFuncTB = NULL;
	PyObject *argsTB = NULL;
	PyObject *obResult = NULL;

	/* Import the modules we need - cStringIO and traceback */
	modStringIO = PyImport_ImportModule("cStringIO");
	if (modStringIO==NULL)
		TRACEBACK_FETCH_ERROR("cant import cStringIO\n");

	modTB = PyImport_ImportModule("traceback");
	if (modTB==NULL)
		TRACEBACK_FETCH_ERROR("cant import traceback\n");
	/* Construct a cStringIO object */
	obFuncStringIO = PyObject_GetAttrString(modStringIO, "StringIO");
	if (obFuncStringIO==NULL)
		TRACEBACK_FETCH_ERROR("cant find cStringIO.StringIO\n");
	obStringIO = PyObject_CallObject(obFuncStringIO, NULL);
	if (obStringIO==NULL)
		TRACEBACK_FETCH_ERROR("cStringIO.StringIO() failed\n");
	/* Get the traceback.print_exception function, and call it. */
	obFuncTB = PyObject_GetAttrString(modTB, "print_tb");
	if (obFuncTB==NULL)
		TRACEBACK_FETCH_ERROR("cant find traceback.print_tb\n");

	argsTB = Py_BuildValue("OOO",
			exc_tb  ? exc_tb  : Py_None,
			Py_None,
			obStringIO);
	if (argsTB==NULL)
		TRACEBACK_FETCH_ERROR("cant make print_tb arguments\n");

	obResult = PyObject_CallObject(obFuncTB, argsTB);
	if (obResult==NULL)
		TRACEBACK_FETCH_ERROR("traceback.print_tb() failed\n");
	/* Now call the getvalue() method in the StringIO instance */
	Py_DECREF(obFuncStringIO);
	obFuncStringIO = PyObject_GetAttrString(obStringIO, "getvalue");
	if (obFuncStringIO==NULL)
		TRACEBACK_FETCH_ERROR("cant find getvalue function\n");
	Py_DECREF(obResult);
	obResult = PyObject_CallObject(obFuncStringIO, NULL);
	if (obResult==NULL)
		TRACEBACK_FETCH_ERROR("getvalue() failed.\n");

	/* And it should be a string all ready to go - duplicate it. */
	if (!PyString_Check(obResult))
			TRACEBACK_FETCH_ERROR("getvalue() did not return a string\n");

	{ // a temp scope so I can use temp locals.
	char *tempResult = PyString_AsString(obResult);
	result = (char *)PyMem_Malloc(strlen(tempResult)+1);
	if (result==NULL)
		TRACEBACK_FETCH_ERROR("memory error duplicating the traceback string\n");

	strcpy(result, tempResult);
	} // end of temp scope.
done:
	/* All finished - first see if we encountered an error */
	if (result==NULL && errMsg != NULL) {
		result = (char *)PyMem_Malloc(strlen(errMsg)+1);
		if (result != NULL)
			/* if it does, not much we can do! */
			strcpy(result, errMsg);
	}
	Py_XDECREF(modStringIO);
	Py_XDECREF(modTB);
	Py_XDECREF(obFuncStringIO);
	Py_XDECREF(obStringIO);
	Py_XDECREF(obFuncTB);
	Py_XDECREF(argsTB);
	Py_XDECREF(obResult);
	return result;
}

static std::string getTracebackString(PyObject *exc_tb)
{
	std::string result;

	const char *szTraceback = PyTraceback_AsString(exc_tb);

	if (szTraceback == NULL)
	{
			result = "Can't get the traceback info!";
	}
	else
	{
		result += "Traceback (most recent call last):\n";
		result += szTraceback;
		PyMem_Free((void *)szTraceback);
	}

	return result;
}

bool PythonEngine::getLastError(std::string &type, std::string &message, std::string &callstack, bool reset)
{
	type.clear();
	message.clear();

	if(PyErr_Occurred() == null)
		return false;

	PyObject *ptype = null;
	PyObject *pvalue = null;
	PyObject *pcallstack = null;
    PyErr_Fetch(&ptype, &pvalue, &pcallstack);
	PyErr_NormalizeException(&ptype, &pvalue, &pcallstack);

    if(ptype != null)
	{
		PyObject *str = PyObject_Str(ptype);
        type = PyString_AsString(str);
		Py_XDECREF(str);
	}

	if(pvalue != null)
	{
		PyObject *str = PyObject_Str(pvalue);
		message = PyString_AsString(str);
		Py_XDECREF(str);
	}

	if(pcallstack != null)
		callstack = getTracebackString(pcallstack);

	if(reset)
	{
		PyErr_Clear();

		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(pcallstack);
	}
	else
	{
		// N.B.: qui non serve effettuare il decremento, la documentazione dice: "This call takes away a reference to each object: you must own a reference to each object before the call and after the call you no longer own these references"
		
		PyErr_Restore(ptype, pvalue, pcallstack);
	}	

    return true;
}

bool PythonEngine::init(const String &modulesPath, const String &homePath)
{
	if(FileSystem::instance()->directoryExists(modulesPath) == false)
	{
		OS_LOG_ERROR("Invalid Python modules directory '" + modulesPath + "'");
		return false;
	}

	if(FileSystem::instance()->directoryExists(homePath) == false)
	{
		OS_LOG_ERROR("Invalid Python home directory '" + homePath + "'");
		return false;
	}

	boost::unique_lock<boost::shared_mutex> evalLock(m_evalCS, boost::defer_lock);

	if(m_initialized)
	{
		OS_ASSERTFALSE();
		return false;
	}

	// N.B.: Py_SetPythonHome non effettua una copia della stringa passato pertanto è necessario mantenerne un riferimento fino a Py_Finalize
	m_homePath = homePath.to_utf8();
	Py_SetPythonHome(const_cast<char *>(m_homePath.c_str()));

	// N.B.: per far digerire i vari path in modo univoco sulle varie piattaforme è necessario settare sia Py_SetPythonHome che "PYTHONPATH"
	String pythonPath;
	pythonPath += utils::standardiseEnvPath(modulesPath);
	pythonPath += OS_PYTHON_PATH_DELIMITER;
	pythonPath += utils::standardiseEnvPath(utils::makeFolderPath(homePath, OS_PYTHON_FOLDER_LIB));
	pythonPath += OS_PYTHON_PATH_DELIMITER;
	pythonPath += utils::standardiseEnvPath(utils::makeFolderPath(homePath, utils::makeFolderPath(OS_PYTHON_FOLDER_LIB, OS_PYTHON_FOLDER_SITE_PACKAGES)));
    OS_LOG_DEBUG("Python path: " + pythonPath);

	if(PlatformManager::instance()->setEnv(OS_PYTHON_PATH_ENVIRONMENT_VARIABLE, pythonPath) == false)
	{
		OS_ASSERTFALSE();
		OS_LOG_ERROR("Cannot set '" OS_PYTHON_PATH_ENVIRONMENT_VARIABLE "' environment variable");
		return false;
	}

	OS_ASSERT(PlatformManager::instance()->getEnv(OS_PYTHON_PATH_ENVIRONMENT_VARIABLE) == pythonPath);

	Py_InitializeEx(0);		// Skips initialization registration of the signal handlers, which might be useful when Python is embedded
	PyEval_InitThreads();

	OS_ASSERT(m_threadsManager == null);
	m_threadsManager.reset(OS_NEW ThreadsManager(boost::this_thread::get_id()));

	m_globalState = PyThreadState_Get();
	if(m_globalState == null)
	{
		OS_ASSERTFALSE();
		return false;
	}

	initInterpreterLock(m_globalState);

	PythonConverters::init();

	releaseLock();

	m_initialized = true;

	return true;
}

void PythonEngine::cleanup()
{
	boost::unique_lock<boost::shared_mutex> evalLock(m_evalCS, boost::defer_lock);

	boost::mutex::scoped_lock lock(m_interpretersCS);
	m_interpreters.clear();

	if(m_globalState != null)
	{
		releaseInterpreterLock(m_globalState);

		Lock lock;

		PyThreadState_Swap(m_globalState);
		m_globalState = null;

		m_modulesMap.clear();

		Py_Finalize();
	}

	m_initialized = false;
}

void PythonEngine::run()
{
	queueInterpretersJob();
}

bool PythonEngine::loadModule(const char *module_name, void (*module_function)(void))
{
	PythonState state(m_globalState);

	try
	{
		OS_ASSERT(module_name != null);
		OS_ASSERT(module_function != null);

		boost::python::detail::init_module(module_name, module_function);

		// N.B.: internamente boost::python (nella versione attuale 1.38) effettua già un catch di boost::python::error_already_set,
		// pertanto qui è necessario verificare che non sia stato settato alcun errore (resettandolo in caso contrario)
		if(PyErr_Occurred() != null)
		{
			logLastError(true);
			PyErr_Clear();

			OS_ASSERTFALSE();

			return false;
		}

		m_modulesMap[module_name] = boost::python::import(module_name);
	}
	catch(const boost::python::error_already_set &)
	{
		logLastError(true);
		return false;
    }

	return true;
}

void PythonEngine::logLastError(bool reset)
{
	std::string error;
	if(getLastError(error, reset))
		OS_LOG_ERROR(error);
}

bool PythonEngine::getLastError(std::string &error, bool reset)
{
	error.clear();

	std::string type;
	std::string message;
	std::string callstack;
	if(getLastError(type, message, callstack, reset) == false)
		return false;

	error = "Python error";

	if(type.empty() == false)
	{
		error.append(" ");
		error.append(type);
	}

	if(message.empty() == false)
	{
		error.append(": ");
		error.append(message);
	}

	if(callstack.empty() == false)
	{
		error.append("\r\n");
		error.append(callstack);
	}

	return true;
}

void PythonEngine::ensureLock()
{
	m_threadsManager->ensureLock();
}

void PythonEngine::releaseLock()
{
	m_threadsManager->releaseLock();
}

PyThreadState * PythonEngine::saveThread()
{
	return m_threadsManager->saveThread();
}

void PythonEngine::restoreThread(PyThreadState *threadState)
{
	m_threadsManager->restoreThread(threadState);
}

shared_ptr<PythonInterpreter> PythonEngine::createInterpreter()
{
	// N.B.: gli interpreti non possono riutilizzati tramite cache perchè i dizionari contengono eventuali variabili globali e quindi vanno ricreati ogni volta.

	shared_ptr<PythonInterpreter> interpreter;

	bool requireInterpreters = false;

	{
		boost::mutex::scoped_lock lock(m_interpretersCS);

		if(m_interpreters.empty() == false)
			interpreter = utils::pop_front(m_interpreters);

		if(m_interpreters.size() <= OS_PYTHON_CACHE_THRESHOLD)
			requireInterpreters = true;
	}

	if(requireInterpreters)
		queueInterpretersJob();

	if(interpreter == null)
		interpreter = allocateInterpreter(true);

	OS_ASSERT(interpreter != null);
	return interpreter;
}

bool PythonEngine::cacheInterpreter()
{
	shared_ptr<PythonInterpreter> interpreter = allocateInterpreter(false);
	if(interpreter == null)
		return false;

	boost::mutex::scoped_lock lock(m_interpretersCS);
	m_interpreters.push_back(interpreter);

	return true;
}

shared_ptr<PythonInterpreter> PythonEngine::allocateInterpreter(bool force)
{
	// N.B.: per creare un interprete è necessario che non ci siano attualmente scripts in fase di esecuzione (altrimenti si genera un errore fatale di tipo "ceval: orphan tstate")

	boost::unique_lock<boost::shared_mutex> evalLock(m_evalCS, boost::defer_lock);
	if(force)
	{
		evalLock.lock();
	}
	else
	{
		if(evalLock.try_lock() == false)
			return null;
	}

	if(m_initialized == false)
		return null;		// Possibile in fase di chiusura

	PyThreadState *state = null;

	{
		Lock lock;
		state = Py_NewInterpreter();
	}

	if(state == null)
	{
		OS_ASSERTFALSE();
		return null;
	}

	initInterpreterLock(state);
	// Crea prima l'interprete in modo che in caso di errore durante il caricamento dei moduli PyThreadState venga correttamente deallocato da PythonInterpreter
	shared_ptr<PythonInterpreter> interpreter(OS_NEW PythonInterpreter(state), boost::bind(&PythonEngine::deallocateInterpreter, this, _1));	

	{
		PythonState scope(state);

		try
		{
			// Carica l'elenco dei moduli registrati nel nuovo interprete

			for(ModulesMap::const_iterator i = m_modulesMap.begin(); i != m_modulesMap.end(); ++i)
			{
				boost::python::object module(reinterpret_cast<boost::python::detail::borrowed_reference_t *>(PyImport_AddModule(i->first.c_str())));
				boost::python::object dictionary = module.attr(OS_PYTHON_MODULE_DICTIONARY);

				if(PyDict_Merge(dictionary.ptr(), static_cast<boost::python::object>(i->second.attr(OS_PYTHON_MODULE_DICTIONARY)).ptr(), 0) != 0)
				{
					OS_ASSERTFALSE();
					return null;
				}
			}
		}
		catch(const boost::python::error_already_set &)
		{
			logLastError(true);
			return null;
		}
	}

	return interpreter;
}

void PythonEngine::initInterpreterLock(PyThreadState *state)
{
	OS_ASSERT(state != null);

	boost::mutex::scoped_lock lock(m_interpretersLockMutex);

	OS_ASSERT(m_interpretersLock.find(state) == m_interpretersLock.end());
	m_interpretersLock[state] = shared_ptr<boost::recursive_mutex>(OS_NEW_T(boost::recursive_mutex), os_delete_t());
}

void PythonEngine::releaseInterpreterLock(PyThreadState *state)
{
	OS_ASSERT(state != null);

	boost::mutex::scoped_lock lock(m_interpretersLockMutex);

	OS_ASSERT(m_interpretersLock.find(state) != m_interpretersLock.end());
	m_interpretersLock.erase(state);
}

void PythonEngine::deallocateInterpreter(PythonInterpreter *interpreter)
{
	PyThreadState *state = interpreter->getThreadState();

	OS_DELETE interpreter;

	releaseInterpreterLock(state);
}

void PythonEngine::queueInterpretersJob()
{
	boost::mutex::scoped_lock lock(m_interpretersCS);

	shared_ptr<IJob> interpretersJob = m_interpretersJob.lock();
	if((interpretersJob != null) && (interpretersJob->getStatus() == IJob::jobIncomplete))
		return;

	shared_ptr<IBackgroundJob> job(OS_NEW InterpretersJob(Engine::instance()->peekBackgroundJobID()));
	m_interpretersJob = job;

	Engine::instance()->addBackgroundJob(job);
}

bool PythonEngine::invokeCallback(boost::python::detail::exception_handler const &, boost::function0<void> const &callback)
{
	// Gestire qui eventuali eccezioni

	callback();
	return false;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
