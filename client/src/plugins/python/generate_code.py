# <osiris_sps_source_header>
# This file is part of Osiris Serverless Portal System.
# Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http:#www.osiris-sps.org )
#
# Osiris Serverless Portal System is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Osiris Serverless Portal System is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Osiris Serverless Portal System.  If not, see <http:#www.gnu.org/licenses/>.
# </osiris_sps_source_header>

#! /usr/bin/python



#winpdb
#import rpdb2; rpdb2.start_embedded_debugger("osiris")

import os
import sys
import time

from environment import settings
from pygccxml import declarations
from pygccxml import parser
from pygccxml import utils
from pyplusplus import code_creators
from pyplusplus import creators_factory
from pyplusplus import messages
from pyplusplus import module_builder
import pyplusplus.code_repository.convenience
import pyplusplus.code_repository.indexing_suite.pair_header
from pyplusplus import decl_wrappers
import pyplusplus.file_writers.multiple_files
from pyplusplus.function_transformers import templates
from pyplusplus.function_transformers import transformer
from pyplusplus.module_builder import call_policies
import pair_header_patch

####################################################################################################

startTime = time.clock()

####################################################################################################

# Da abilitare solo se necessario
messages.disable(messages.W1023)		# Py++ will generate class wrapper - there are few functions that should be redefined in class wrapper.
# Necessario per funzioni virtuali protette
messages.disable(messages.W1031)		# Py++ will generate class wrapper - user asked to expose non - public member function <name>
# Dalla versione 1.40 del boost i duplicati sono supportati
messages.disable(messages.W1032)		# Boost.Python library does not support enums with duplicate values
# Da abilitare solo se necessario
messages.disable(messages.W1041)		# Property <x> could not be created. There is another exposed declaration with the same name( alias )." The property will make it inaccessible.
# Some "declaration is unexposed" is a false warning.
messages.disable(messages.W1040)	  	# execution error W1040: The declaration is unexposed, but there are other declarations, which refer to it. This could cause "no to_python converter found" run time error. 

####################################################################################################

logger = utils._create_logger_(settings.module_name)

####################################################################################################

# Applica la patch a pair.hpp per evitare registrazioni multiple di std::pair identici (vedere costruttore pair_exposer_t)
# N.B.: implementato un fix nell'svn Py++ del 13/01/10  (rimuovere la patch se si aggiorna)
pyplusplus.code_repository.indexing_suite.pair_header.code = pair_header_patch.code

####################################################################################################

# Fixa i problemi nell'ordine di registrazione dei metodi http://www.language-binding.net/pyplusplus/documentation/functions/registration_order.html
pyplusplus.creators_factory.sort_algorithms.USE_CALLDEF_ORGANIZER = True

####################################################################################################

def override_create_identifier(creator, full_name ):
	if full_name == "::boost::python::override":
		return "::osiris::PythonOverride"	
	
	if full_name == "::boost::python::pure_virtual":
		return ""	# Disabilita boost::python::pure_virtual non essendo supportato nelle funzioni virtuali pure protette

	if full_name == "::boost::python::wrapper":
		return "::osiris::PythonWrapper"
							
	return full_name
	
pyplusplus.code_creators.algorithm.create_identifier = override_create_identifier

####################################################################################################

# Converte tutti gli includes in minuscolo
@staticmethod
def override_normalize_include(path):
	#return os.path.normpath( os.path.normcase( path ) )
	return os.path.normpath(path.lower())
	
pyplusplus.code_creators.include_directories_t.normalize = override_normalize_include

####################################################################################################

# N.B.: la nuova versione svn dovrebbe supportare questa modifica, in tal caso eliminare l'override

'''
constructor_t.does_define_implicit_conversion non supporta casi come:

class Foo
{
public:
	Foo(int v, bool type = true);
};

nel caso di un solo parametro crea correttamente la conversione implicita, ma con parametri > 1 (con valori di default) no
'''

def override_does_define_implicit_conversion(self):
	if self.parent.is_abstract:
		return False
		
	if self.is_copy_constructor:
		return False
	
	## modifica rispetto alla base #####################################
	args = len(self.arguments)
	if args != 1:
		# Qui l'implementazione di base fa una "return False" secca, la modifica controlla se i parametri dal secondo in poi hanno tutti valori di default
		if args > 1:
			for arg in self.arguments[1::]:
				if not arg.default_value:
					return False
		else:
			return False
	#########################################################
	
	if self.parent.find_out_member_access_type(self) != declarations.ACCESS_TYPES.PUBLIC:
		return False
		
	return True
	
decl_wrappers.calldef_wrapper.constructor_t.does_define_implicit_conversion = override_does_define_implicit_conversion

####################################################################################################
	
# risolve il bug nel GCC per i parametri template di default http://stackoverflow.com/questions/851584/default-template-class-argument-confuses-g
	
def customize_default_args(args):
	for arg in args:		
		if arg.default_value:
			if not declarations.is_fundamental(arg.type):
				arg.default_value = "(" + arg.default_value + ")"

####################################################################################################
	
class allow_threading_transformer_t(transformer.transformer_t):
	def __init__(self, function):
		transformer.transformer_t.__init__(self, function)		
				
	def __configure_sealed(self, controller):
		controller.add_pre_call_code("::osiris::PythonThreadSaver __pythreadSaver;")		
		controller.add_post_call_code("__pythreadSaver.restore();")		#Necessario per la corretta conversione in caso di valore di ritorno			
		customize_default_args(controller.wrapper_args)
		
	def configure_mem_fun(self, controller):
		self.__configure_sealed(controller)

	def configure_free_fun(self, controller):
		self.__configure_sealed(controller)

	def configure_virtual_mem_fun(self, controller):
		empty=[]		
		
	def required_headers(self):
		return []
		
def allow_threading_transformer_creator():
	def creator(function):
		return allow_threading_transformer_t(function)
		
	return creator
	
####################################################################################################

# Effettua il riconoscimento in automatico di funzioni get/set is/set has/set e crea in automatico i wrappers delle proprieta'
class properties_recognizer_t(decl_wrappers.name_based_recognizer_t):
	def __init__(self):
		decl_wrappers.name_based_recognizer_t.__init__(self)
		
	def prefixes( self ):
		# N.B.: la base contempla anche 'has' <-> 'set' e 'is' <-> 'set' ma questo genera problemi nel caso in cui ad esempio siano presenti contemporaneamente 'has', 'get' e 'set' (es.: getID, setID e hasID) in quando vengono generate due proprieta' (una per get<->set e l'altra per has<->set)
		return [ ('get', 'set')]
		
	def find_out_property_name( self, fget, prefix ):
		propertyName = decl_wrappers.name_based_recognizer_t.find_out_property_name(self, fget, prefix)
		# Se il nome della proprieta' e' tutto maiuscolo viene convertito in minuscolo (es.: ID -> id) altrimenti converte solo la prima lettera in minuscolo (es.: Version -> version)
		if(propertyName.isupper()):
			propertyName = propertyName.lower()
		else:
			propertyName = propertyName[0].lower() + propertyName[1:]
				
		return propertyName
		
	def is_accessor( self, mem_fun ):
		if mem_fun.ignore:
			return False
		if mem_fun.access_type != 'public':
			return False
		if mem_fun.has_static:
			return False
		# Modificato rispetto alla base, altrimenti una funzione come virtual std::string getName() const = 0 non verrebbe mappata (valutare se comporta problemi)
		#if mem_fun.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
		#	return False		
		return True	
		
	def check_type_compatibility(self, fget, fset):
		if decl_wrappers.name_based_recognizer_t.check_type_compatibility(self, fget, fset):
			return True
			
		# N.B.: la base non contempla il caso il cui la get restituisca "type" e la set abbia come parametro "const type &" (capita ad esempio in circostanze dove il valore di ritorno della get viene ottenuto tramite conversione da stringa)		
			
		t1 = fget.return_type
		t2 = fset.arguments[0].type			
			
		if declarations.is_reference(t2):
			t2 = declarations.remove_cv( declarations.remove_reference( t2 ) )
			if declarations.is_same( t1, t2 ):
				return True
				
		return False
		
####################################################################################################

def initAlreadyExposedClasses(module_builder):
	# Elenco delle classi esposte manualmente
	already_exposed_classes = ["::boost::posix_time::ptime",					       
					       "::osiris::String"]

	for c in already_exposed_classes:
		module_builder.class_(c).already_exposed = True
		
####################################################################################################
		
unexposedClasses = []
def logUnexposedClass(class_):
	ignoredDecls = ["::boost",
			      "::osiris::AllocWrapper", 				 
			      "::osiris::enable_this_ptr", 
			      "::osiris::ide::Control",
			      "::osiris::ide::PortalControl",
			      "::osiris::Object"]
	
	for ic in ignoredDecls:
		if class_.decl_string.startswith(ic):
			return
			
	global unexposedClasses
	if class_.decl_string not in unexposedClasses:
		unexposedClasses.append(class_.decl_string)
		logger.warning("Class '" + class_.decl_string + "' not exposed")

####################################################################################################

def customizeOMLEditor(class_):
	# problema boost::function: "function return function"
	class_.member_functions("getPreviewCallback").exclude()
	class_.member_functions("setPreviewCallback").exclude()	
	
def customizeContainerWrapper(class_):
	# I wrappers delle funzioni che restituiscono un iteratore non generano codice portabile (a causa delle diverse implementazioni dell'stl)
	class_.member_functions("begin").exclude()
	class_.member_functions("end").exclude()

#def customizeIExtension(class_):
#	class_.add_registration_code("::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::IExtension > >();", works_on_instance = False)		

def customizeOptions(class_):
	class_.member_functions("subscribeInitHandler").exclude()
	class_.member_functions("subscribeUpdateHandler").exclude()

'''
def customizeXMLAttributes(class_):
	customizeContainerWrapper(class_)
	
	class_.member_functions("set").exclude()
	# Seleziona solo il prototipo con primo parametro qualunque (None) e secondo const osiris::String &
	class_.member_functions("set", arg_types=[None, "::osiris::String const &"]).include()
'''
	
def customizeXMLStylesheet(class_):
	class_.member_functions("getEventError").exclude()
	
def customizeDataItem(class_):
	class_.member_functions("get").exclude()
	class_.operators().exclude()
	
####################################################################################################	
	
class ExposedClass:
	def __init__(self, mainHeader = None, relatedHeaders = None, customizeCallback = None, priority = None):
		self.mainHeader = mainHeader
		self.relatedHeaders = relatedHeaders
		self.customizeCallback = customizeCallback
		self.priority = priority
				
	def expose(self, class_):
		class_.include()
				
		if self.mainHeader:
			self.include(class_, self.mainHeader)			
		
		if self.relatedHeaders:
			for relatedHeader in self.relatedHeaders:
				self.include(class_, relatedHeader, False)
				
		if self.customizeCallback:
			self.customizeCallback(class_)
			
		try:
			class_.member_function("getRuntimeClass").exclude()
		except:
			pass	
				
		for f in class_.member_functions(allow_empty=True):	
			if f.ignore == False and f.virtuality == declarations.VIRTUALITY_TYPES.NOT_VIRTUAL:
				f.add_transformation(allow_threading_transformer_creator())
				# In caso di overloads vengono generati nomi di funzioni con hash (es.: addChild_5fb3084aa3119f828abd29b2a5f4eb5d)  quando viene aggiunto un transformer ad una funzione (la versione pulita sarebbe passare il parametro keywd a add_transformation settando come chiave "alias" e valore "f.alias")
				f.transformations[len(f.transformations) - 1].alias = f.alias
				
			customize_default_args(f.arguments)
				
		class_.add_properties(recognizer=properties_recognizer_t())
		
		for subclass in class_.classes(allow_empty=True):	
			subclass.add_properties(recognizer=properties_recognizer_t())
			
	def include(self, class_, header, check = True):
		if check and (header in class_.include_files):
			# Lancia un'eccezione per evitare che l'header sia incluso inutilmente nei files correlati della classe
			raise RuntimeError("Header file '" + header + "' already included in '" + class_.name + "' (check related header of '" + class_.name + "' and its bases classes)")
			
		class_.include_files.append(header)		
		
####################################################################################################
# Legend:
# - "ErrorG" - Error during generation of wrappers
# - "ErrorC" - Error during compiling *.pypp.cpp
# - "ErrorP" - Error during compiling on specific platform
# - "ErrorX" - Error during execution
	
exposed_classes =  {
				   #"Foo" : ExposedClass("wrappers.h"),	# Classe per effettuare tests				   	   				   				   				   
				   			   
				   # ---------------------------------------------------------------------------------
				   # Archives - Status: All, Untested
				   # One of this give thic #ErrorC: // #ErrorC: error C2678: binary '==' : no operator found which takes a left-hand operand of type 'osiris::IArchivesArchiveEntry' (or there is no acceptable conversion)	o:\osiris\client\src\plugins\python\wrappers\indexing_suite\value_traits.hpp	65	
				   #"IArchivesArchive" : ExposedClass("iarchivesarchive.h", ["archivesziparchive.h"]), 
				   #"IArchivesArchiveEntry" : ExposedClass("iarchivesarchiveentry.h"), 
				   #"IArchivesArchiveFactory" : ExposedClass("iarchivesarchivefactory.h"), 
				   #"ArchivesManager" : ExposedClass("archivesmanager.h"), 
				   #"ArchivesZipArchive" : ExposedClass("archivesziparchive.h"), #ErrorC:  error C2248: 'boost::noncopyable_::noncopyable::noncopyable' : cannot access private member declared in class 'boost::noncopyable_::noncopyable'
				   #"ArchivesZipArchiveFactory" : ExposedClass("archivesziparchivefactory.h"), 
				   
				   # ---------------------------------------------------------------------------------
				   # Core - Status: Partial. Last: IOptionValidator
				   
				   "IEvent" : ExposedClass("ievent.h"),
				   "IEventCallback" : ExposedClass("ieventcallback.h", ["eventcallback.h", "ievent.h"]),				   				   
				   "IJob" : ExposedClass("ijob.h"),		   				   
				   "IOptionValidator" : ExposedClass("ioptionvalidator.h"), 
				   
				   "Buffer" : ExposedClass("buffer.h"),
				   "DataItem" : ExposedClass("dataitem.h", [], customizeDataItem),
				   #"DataTree" : ExposedClass("datatree.h"), #ErrorC: error LNK2019: unresolved external symbol "public: __thiscall osiris::DataTree::HEADER::HEADER(void)" (??0HEADER@DataTree@osiris@@QAE@XZ) referenced in function "public: __thiscall boost::python::objects::value_holder<struct osiris::DataTree::HEADER>::value_holder<struct osiris::DataTree::HEADER>(struct _object *)" (??0?$value_holder@UHEADER@DataTree@osiris@@@objects@python@boost@@QAE@PAU_object@@@Z)
				   "DataTable" : ExposedClass("datatable.h"),
				   "CryptManager" : ExposedClass("cryptmanager.h", ["cryptkey.h"]), "StaticSingleton<osiris::CryptManager, true>" : ExposedClass(), 
				   "DateTime" : ExposedClass("datetime.h"),				   				   
				   "TimeDuration" : ExposedClass("datetime.h"),				   				   
				   
				   "TextFile" : ExposedClass("textfile.h"),
				   "ThirdPartyLibrary" : ExposedClass("thirdpartylibrary.h"),
				   "ThirdPartyLibrariesReporter" : ExposedClass("thirdpartylibrariesreporter.h"), "StaticSingleton<osiris::ThirdPartyLibrariesReporter, true>" : ExposedClass(),
				   
				   
				   #"EventCallback" : ExposedClass("eventcallback.h", ["ievent.h"]), #ErrorG: RuntimeError: Header file 'eventcallback.h' already included in 'IEventCallback' (check related header of 'IEventCallback' and its bases classes)
				   "EventSource" : ExposedClass("eventsource.h", ["ievent.h"]),				   
				   "EventsSource<osiris::String, osiris::IEvent>" : ExposedClass("eventssource.h", ["ievent.h"]),
				   "File" : ExposedClass("file.h"),
				   "FileLogger" : ExposedClass("filelogger.h"), 				   
				   "FileSystem" : ExposedClass("filesystem.h"), "StaticSingleton<osiris::FileSystem, true>" : ExposedClass(),				   
				   "Hash" : ExposedClass("hash.h"), 
				   "Image" : ExposedClass("image.h"), 
				   "RegexManager" : ExposedClass("regexmanager.h"), "StaticSingleton<osiris::RegexManager, true>" : ExposedClass(),
				   "UniqueID" : ExposedClass("uniqueid.h"),
				   "Version" : ExposedClass("version.h"),
				   
				   # ---------------------------------------------------------------------------------
				   # DB - Status: Partial
				   
				   "IDbConnection" : ExposedClass("idbconnection.h", ["idboptions.h", "idbresult.h", "idbstatement.h"]),
				   "IDbDriver" : ExposedClass("idbdriver.h", ["idbconnection.h", "idboptions.h"]), #Error VC10: error C2668: 'std::basic_string<_Elem,_Traits,_Ax>::basic_string' : ambiguous call to overloaded function	o:\osiris\client\src\plugins\python\wrappers\omlimg.pypp.cpp	45	1	Python
				   "IDbOptions" : ExposedClass("idboptions.h"),
				   "IDbParameter" : ExposedClass("idbparameter.h"),
				   "IDbResult" : ExposedClass("idbresult.h"),
				   "IDbStatement" : ExposedClass("idbstatement.h"),				   
				   
				   # ---------------------------------------------------------------------------------
				   # Engine - Status: Partial
				   
				   # Engine - ID (ids.h) - Status: All
				   
				   #"EntityID" : ExposedClass(),				   
				   #"PortalID" : ExposedClass(),				   
				   #"PovID" : ExposedClass(),				   
				   #"ExtensionID" : ExposedClass(),
				   #"ExtensionsModuleID" : ExposedClass(),
				   #"ExtensionsComponentID" : ExposedClass(),
				   #"SkinID" : ExposedClass(),				   
				   
				   # Engine - Data - Status: Partial
				   
				   "DataAccount" : ExposedClass("dataaccount.h"),				   
				   
				   # Engine - Extensions - Status: All
				   
				   "IExtensionsExtension" : ExposedClass("iextensionsextension.h", ["uniqueid.h", "version.h"]),				   
				   "IExtensionsModule" : ExposedClass("iextensionsmodule.h", ["ids.h", "extensionsmoduleeditor.h", "extensionsmoduleviewer.h"]),				   
				   "IExtensionsComponent" : ExposedClass("iextensionscomponent.h", ["ids.h", "extensionsmoduleeditor.h", "extensionsmoduleviewer.h", "extensionscomponenteditor.h", "extensionscomponentviewer.h"]),   
				   "ExtensionsComponentControl" : ExposedClass("extensionscomponentcontrol.h", ["xmldocument.h"]),
				   "ExtensionsComponentEditor" : ExposedClass("extensionscomponenteditor.h"),
				   #"ExtensionsComponentID" : ExposedClass("extensionscomponentid.h"),
				   "ExtensionsComponentViewer" : ExposedClass("extensionscomponentviewer.h"),				   
				   "ExtensionsExtension" : ExposedClass("extensionsextension.h", ["languagefolder.h"]),				   
				   "ExtensionsInvalidComponent" : ExposedClass("extensionsinvalidcomponent.h"),
				   "ExtensionsInvalidModule" : ExposedClass("extensionsinvalidmodule.h"),
				   "ExtensionsModuleControl" : ExposedClass("extensionsmodulecontrol.h", ["xmldocument.h"]),
				   "ExtensionsModuleEditor" : ExposedClass("extensionsmoduleeditor.h"),
				   #"ExtensionsModuleID" : ExposedClass("extensionsmoduleid.h"), #toclean
				   "ExtensionsModuleViewer" : ExposedClass("extensionsmoduleviewer.h"),				   
				   "ExtensionsModuleViewerHidden" : ExposedClass("extensionsmoduleviewerhidden.h"),				   
				   "ExtensionsSystem" : ExposedClass("extensionssystem.h", ["extensionscomponenteditor.h", "extensionscomponentviewer.h", "extensionsmoduleeditor.h", "extensionsmoduleviewer.h"]), "DynamicSingleton<osiris::ExtensionsSystem>" : ExposedClass(),
				   				   
				   # Engine - OML - Status: All
				   
				   "IOMLCode" : ExposedClass("iomlcode.h"),
				   "IOMLXsl" : ExposedClass("iomlxsl.h"),
				   "OMLA" : ExposedClass("omla.h"), #Error VC10: error C2668: 'std::basic_string<_Elem,_Traits,_Ax>::basic_string' : ambiguous call to overloaded function	o:\osiris\client\src\plugins\python\wrappers\omlimg.pypp.cpp	45	1	Python
				   "OMLAnchor" : ExposedClass("omlanchor.h"),
				   "OMLCode" : ExposedClass("omlcode.h"),
				   "OMLContext" : ExposedClass("omlcontext.h", ["iideportalpage.h"]),
				   "OMLDate" : ExposedClass("omldate.h"),
				   "OMLDebug" : ExposedClass("omldebug.h"),
				   "OMLEMail" : ExposedClass("omlemail.h"),
				   "OMLEmoticon" : ExposedClass("omlemoticon.h"),
				   "OMLGoto" : ExposedClass("omlgoto.h"),
				   "OMLHtml" : ExposedClass("omlhtml.h"),
				   "OMLHtmlWrapper" : ExposedClass("omlhtmlwrapper.h"), #Error VC10: error C2668: 'std::basic_string<_Elem,_Traits,_Ax>::basic_string' : ambiguous call to overloaded function	o:\osiris\client\src\plugins\python\wrappers\omlimg.pypp.cpp	45	1	Python
				   "OMLIf" : ExposedClass("omlif.h"),
				   "OMLImg" : ExposedClass("omlimg.h"), #Error VC10: error C2668: 'std::basic_string<_Elem,_Traits,_Ax>::basic_string' : ambiguous call to overloaded function	o:\osiris\client\src\plugins\python\wrappers\omlimg.pypp.cpp	45	1	Python
				   "OMLItem" : ExposedClass("omlitem.h"),
				   "OMLLang" : ExposedClass("omllang.h"),
				   "OMLLess" : ExposedClass("omlless.h"),
				   "OMLList" : ExposedClass("omllist.h"),
				   "OMLManager" : ExposedClass("omlmanager.h"), "StaticSingleton<osiris::OMLManager, true>" : ExposedClass(),
				   "OMLMore" : ExposedClass("omlmore.h"),
				   "OMLNoBR" : ExposedClass("omlnobr.h"),
				   "OMLNoParse" : ExposedClass("omlnoparse.h"),
				   "OMLObject" : ExposedClass("omlobject.h"),
				   "OMLPopup" : ExposedClass("omlpopup.h"),
				   "OMLQuote" : ExposedClass("omlquote.h"),
				   "OMLReplace" : ExposedClass("omlreplace.h"),
				   "OMLSize" : ExposedClass("omlsize.h"),
				   "OMLSystem" : ExposedClass("omlsystem.h"),
				   "OMLTab" : ExposedClass("omltab.h"),
				   "OMLText" : ExposedClass("omltext.h"),
				   "OMLUrl" : ExposedClass("omlurl.h"),
				   "OMLUser" : ExposedClass("omluser.h"),
				   "OMLView" : ExposedClass("omlview.h"),
				   
				   # Engine - Pickers
				   "IdePickerBool" : ExposedClass("idepickerbool.h"),
				   "IdePickerColor" : ExposedClass("idepickercolor.h"),
				   "IdePickerComponent" : ExposedClass("idepickercomponent.h"),				   
				   "IdePickerCulture" : ExposedClass("idepickerculture.h"),				   
				   "IdePickerDateTime" : ExposedClass("idepickerdatetime.h"),
				   "IdePickerDateTimeSpecial" : ExposedClass("idepickerdatetimespecial.h"),
				   "IdePickerNumber" : ExposedClass("idepickernumber.h"),
				   "IdePickerObject" : ExposedClass("idepickerobject.h"),
				   "IdePickerObjectEx" : ExposedClass("idepickerobjectex.h"),
				   "IdePickerSections" : ExposedClass("idepickersections.h"),
				   "IdePickerSelect" : ExposedClass("idepickerselect.h"),
				   "IdePickerSkin" : ExposedClass("idepickerskin.h"),
				   
				   # Engine - Misc
				   
				   "CacheSystem" : ExposedClass("cachesystem.h"), "DynamicSingleton<osiris::CacheSystem>" : ExposedClass(),
				   "IsisSystem" : ExposedClass("isissystem.h"), "DynamicSingleton<osiris::IsisSystem>" : ExposedClass(),
				   #"ReputationsFeedback" : ExposedClass("reputationsfeedback.h"), # TOFIX: Error under linux. We need to remove iterator?
				   "ReputationsManager" : ExposedClass("reputationsmanager.h"),
				   "ReputationsScore" : ExposedClass("reputationsscore.h"),
				   
				   # Engine - Search - Status: All
				   
				   "ISearchOptions" : ExposedClass("isearchoptions.h", ["idesearchparam.h"]),
				   "ISearchResults" : ExposedClass("isearchresults.h", ["searchresult.h"]),
				   "SearchAttachmentOptions" : ExposedClass("searchattachmentoptions.h"),
				   "SearchCalendarEventOptions" : ExposedClass("searchcalendareventoptions.h"),
				   "SearchFileOptions" : ExposedClass("searchfileoptions.h"),
				   "SearchInstanceOptions" : ExposedClass("searchinstanceoptions.h"),				   
				   "SearchModelOptions" : ExposedClass("searchmodeloptions.h"),
				   "SearchPollOptionOptions" : ExposedClass("searchpolloptionoptions.h"),
				   "SearchPollOptions" : ExposedClass("searchpolloptions.h"),
				   "SearchPostOptions" : ExposedClass("searchpostoptions.h"),
				   "SearchQuery" : ExposedClass("searchquery.h",["isearchoptions.h"]),
				   "SearchResult" : ExposedClass("searchresult.h"),
				   "SearchResultsDatabase" : ExposedClass("searchresultsdatabase.h"),
				   "SearchResultsLucene" : ExposedClass("searchresultslucene.h"),
				   "SearchSectionOptions" : ExposedClass("searchsectionoptions.h"),
				   "SearchTagOptions" : ExposedClass("searchtagoptions.h"),
				   "SearchTextOptions" : ExposedClass("searchtextoptions.h"),
				   
				   # ---------------------------------------------------------------------------------
				   # HTTP - Status: Ok, All
				   
				   "IHttpClient" : ExposedClass("ihttpclient.h", ["httprequest.h", "httpresponse.h", "httpurl.h"]),
				   "IHttpDirectory" : ExposedClass("ihttpdirectory.h", ["datetime.h", "httppath.h", "httpsessionstate.h", "httpenumerateddirectorycallback.h", "httpdirectorycallback.h"]),
				   "IHttpDirectoryHandler" : ExposedClass("ihttpdirectoryhandler.h", ["httppath.h"]),
				   "IHttpDirectoryRegexHandler" : ExposedClass("ihttpdirectoryregexhandler.h"),
				   "IHttpMethod" : ExposedClass("ihttpmethod.h", ["httpcontent.h", "httpdata.h"]),
				   "IHttpServer" : ExposedClass("ihttpserver.h", ["httppath.h", "httpsessionstate.h", "ihttpdirectory.h"]),
				   "HttpClient" : ExposedClass("httpclient.h"),
				   "HttpContent" : ExposedClass("httpcontent.h"),				   
				   "HttpCookie" : ExposedClass("httpcookie.h"), 
				   "HttpData" : ExposedClass("httpdata.h"), 
				   #"HttpDirectoryCallback" : ExposedClass("httpdirectorycallback.h"), #ErrorC: error C2091: function returns function
				   #"HttpEnumeratedDirectoryCallback" : ExposedClass("httpenumerateddirectorycallback.h"), #ErrorC: error C2091: function returns function
				   "HttpParser" : ExposedClass("httpparser.h"), 
				   "HttpPath" : ExposedClass("httppath.h"),
				   "HttpPhysicalDirectory" : ExposedClass("httpphysicaldirectory.h"),
				   "HttpPostParam" : ExposedClass("httppostparam.h"),
				   "HttpProxy" : ExposedClass("httpproxy.h"), 
				   "HttpRequest" : ExposedClass("httprequest.h", ["buffer.h", "httpurl.h"]),
				   "HttpResponse" : ExposedClass("httpresponse.h", ["buffer.h", "istream.h"]),
				   "HttpRootDirectory" : ExposedClass("httprootdirectory.h"), 
				   "HttpSession" : ExposedClass("httpsession.h", ["httprequest.h", "httpresponse.h", "httpsessionstate.h", "ihttpserver.h", "tcpsocket.h"]),
				   "HttpSessionState" : ExposedClass("httpsessionstate.h"),
				   #"HttpSocketProxy" : ExposedClass("httpsocketproxy.h"), #Error: Is a template class, maybe require a specific alias. Maybe unwrapped.
				   "HttpUrl" : ExposedClass("httpurl.h"),
				   "HttpVirtualDirectory" : ExposedClass("httpvirtualdirectory.h"),
				   
				   
				   # ---------------------------------------------------------------------------------
				   # IRC - Status: Ok, don't need wrappers.
				   
				   # ---------------------------------------------------------------------------------
				   # Lang - Status: Ok, All
				   
				   "LanguageCulture" : ExposedClass("languageculture.h"),
				   "LanguageFolder" : ExposedClass("languagefolder.h"),
				   "LanguageFolderListener" : ExposedClass("languagefolderlistener.h"),
				   "LanguageManager" : ExposedClass("languagemanager.h", ["languageculture.h", "languagefolder.h"]), "StaticSingleton<osiris::LanguageManager, true>" : ExposedClass(),
				   "LanguageResult" : ExposedClass("languageresult.h"),
				   "LanguageVocabolary" : ExposedClass("languagevocabolary.h"),
				   
				   # ---------------------------------------------------------------------------------
				   # Net - Status: Ok, Partial, don't need wrappers.
				   
				   #"ConnectionsManager" : ExposedClass("connectionsmanager.h"),   #error C2091: function returns function
				   "IPAddress" : ExposedClass("ipaddress.h"),
				   "NetworkSystem" : ExposedClass("networksystem.h"), "StaticSingleton<osiris::NetworkSystem, true>" : ExposedClass(),
				   
				   # ---------------------------------------------------------------------------------
				   # SDK - Status: Ok, don't need wrappers.
				   
				   # ---------------------------------------------------------------------------------
				   # UI - Status: Ok, don't need wrappers.
				   
				   # ---------------------------------------------------------------------------------
				   # UPNP - Status: Ok, don't need wrappers.
				   
				   #"UPnPManager" : ExposedClass("upnpmanager.h"),
				   #"UPnPServer" : ExposedClass("upnpserver.h"),
				   #"UPnPService" : ExposedClass("upnpservice.h"),
				   
				   # ---------------------------------------------------------------------------------
				   # Web - Status: Ok, All	
				   			   
				   "IHtmlControl" : ExposedClass("ihtmlcontrol.h", ["datatree.h", "htmlattributes.h", "htmlevent.h", "eventssource.h", "htmlvirtualpage.h", "htmlwriter.h", "httprequest.h", "httpresponse.h", "httpsession.h"]),
				   "IHtmlInput" : ExposedClass("ihtmlinput.h"),
				   "IHtmlSelect" : ExposedClass("ihtmlselect.h", ["htmloption.h"]),
				   "HtmlAttribute" : ExposedClass("htmlattribute.h", ["ihtmlcontrol.h"]),
				   "HtmlAttributes" : ExposedClass("htmlattributes.h", ["ihtmlcontrol.h"], customizeContainerWrapper),
				   "HtmlBR" : ExposedClass("htmlbr.h"), 
				   "HtmlButton" : ExposedClass("htmlbutton.h"),
				   "HtmlCheckBox" : ExposedClass("htmlcheckbox.h"),
				   "HtmlComboBox" : ExposedClass("htmlcombobox.h"),
				   "HtmlControls" : ExposedClass("htmlcontrols.h",  ["ihtmlcontrol.h"], customizeContainerWrapper),
				   "HtmlDiv" : ExposedClass("htmldiv.h", ["ideportalcontrol.h"]),
				   "HtmlEvent" : ExposedClass("htmlevent.h"),			
				   "HtmlFileBrowser" : ExposedClass("htmlfilebrowser.h", ["buffer.h"]),				   	   				   
				   "HtmlImage" : ExposedClass("htmlimage.h"),
				   "HtmlLink" : ExposedClass("htmllink.h"),
				   "HtmlLinkButton" : ExposedClass("htmllinkbutton.h"),
				   "HtmlListBox" : ExposedClass("htmllistbox.h"),
				   "HtmlLinkTag" : ExposedClass("htmllinktag.h", ["htmlwriter.h"]),
				   "HtmlLiteral" : ExposedClass("htmlliteral.h"),				   
				   "HtmlMetaTag" : ExposedClass("htmlmetatag.h", ["htmlwriter.h"]),
				   "HtmlOption" : ExposedClass("htmloption.h"),				   
				   "HtmlOptionGroup" : ExposedClass("htmloptiongroup.h", ["htmloption.h"]), 
				   "HtmlPage" : ExposedClass("htmlpage.h"),
				   "HtmlPager" : ExposedClass("htmlpager.h"), 
				   "HtmlPanel" : ExposedClass("htmlpanel.h"), 
				   "HtmlParagraph" : ExposedClass("htmlparagraph.h"), 
				   "HtmlParser" : ExposedClass("htmlparser.h"),  "StaticSingleton<osiris::HtmlParser, true>" : ExposedClass(), 
				   "HtmlPopup" : ExposedClass("htmlpopup.h"), 
				   "HtmlRoller" : ExposedClass("htmlroller.h"), 
				   "HtmlScript" : ExposedClass("htmlscript.h"), 
				   "HtmlSpace" : ExposedClass("htmlspace.h"), 
				   "HtmlSpan" : ExposedClass("htmlspan.h"), 
				   "HtmlTab" : ExposedClass("htmltab.h"),
				   "HtmlTabCtrl" : ExposedClass("htmltabctrl.h", ["htmldiv.h", "htmltabpage.h"]),
				   "HtmlTable" : ExposedClass("htmltable.h"),
				   "HtmlTableCell" : ExposedClass("htmltablecell.h"),
				   "HtmlTableHeader" : ExposedClass("htmltableheader.h"),
				   "HtmlTableRow" : ExposedClass("htmltablerow.h"),
				   "HtmlTabPage" : ExposedClass("htmltabpage.h"),
				   "HtmlText" : ExposedClass("htmltext.h"),
				   "HtmlTextArea" : ExposedClass("htmltextarea.h"),
				   "HtmlTextBox" : ExposedClass("htmltextbox.h"),				
				   "HtmlTooltip" : ExposedClass("htmltooltip.h"),				
				   "HtmlUnit" : ExposedClass("htmlunit.h"),				  
				   "HtmlVirtualPage<osiris::IMainPage>" : ExposedClass(None, ["iidemainpage.h"]),
				   "HtmlWriter" : ExposedClass("htmlwriter.h"),
				   "HtmlXSLControl" : ExposedClass("htmlxslcontrol.h", ["xmldocument.h", "xmlstylesheet.h"]),
				   
				   # ---------------------------------------------------------------------------------
				   # XML - Status: Ok, All	
				   # Note: - Classes inside "xmldetails.h" need wrappers?
				   
				   "IXMLFunction" : ExposedClass("ixmlfunction.h"), 
				   "IXMLHandler" : ExposedClass("ixmlhandler.h", ["httpurl.h", "xmlschema.h"]),				   
				   "IXMLResolver" : ExposedClass("ixmlresolver.h"), 
				   "IXMLSerializable" : ExposedClass("ixmlserializable.h", ["xmlnode.h"]), 				   
				   "XMLAttribute" : ExposedClass("xmlattribute.h", ["xmlnode.h"]),
				   "XMLAttributes" : ExposedClass("xmlattributes.h", ["xmlnode.h"], customizeContainerWrapper),
				   "XMLDocument" : ExposedClass("xmldocument.h", ["buffer.h", "xmlnode.h"]),
				   "XMLManager" : ExposedClass("xmlmanager.h", ["httpurl.h", "xmldocument.h"]), "StaticSingleton<osiris::XMLManager, true>" : ExposedClass(), 
				   "XMLNode" : ExposedClass("xmlnode.h", ["buffer.h", "datetime.h", "xmlattributes.h", "xmldocument.h", "xmlnodes.h"]),
				   "XMLNodes" : ExposedClass("xmlnodes.h", ["datetime.h", "xmlnode.h"], customizeContainerWrapper),
				   "XMLSchema" : ExposedClass("xmlschema.h", ["buffer.h"]),
				   "XMLStylesheet" : ExposedClass("xmlstylesheet.h", ["ixmlresolver.h"], customizeXMLStylesheet),
				   "XMLWriter" : ExposedClass("xmlwriter.h"),
				   
				   
				   
				   # ---------------------------------------------------------------------------------
				   # Other (toclean)				   				   
				   
				   "DatabasesSystem" : ExposedClass("dbdatabasessystem.h", ["version.h"]), "DynamicSingleton<osiris::DatabasesSystem>" : ExposedClass(),
				   "Engine" : ExposedClass("engine.h", ["osirislink.h", "httpurl.h", "ids.h"]), "DynamicSingleton<osiris::Engine>" : ExposedClass(),				   				   				   				   				   
				   "IBackgroundJob" : ExposedClass("ibackgroundjob.h"),				   				   
				   "ID" : ExposedClass("id.h"),				   				   
				   "IdeAccount" : ExposedClass("ideaccount.h", ["dataaccount.h"]),				   				   
				   "IdeAccountsManager" : ExposedClass("ideaccountsmanager.h"), "StaticSingleton<osiris::IdeAccountsManager, true>" : ExposedClass(),
				   "IdeAssistant" : ExposedClass("ideassistant.h"),				   				   
				   "IdeBlock" : ExposedClass("ideblock.h"),				   
				   "IdeButton" : ExposedClass("idebutton.h"),				   				   
				   "IdeHelpBox" : ExposedClass("idehelpbox.h"),				   
				   "IdeOMLEditor" : ExposedClass("ideomleditor.h", [], customizeOMLEditor),
				   "IdeMailBox" : ExposedClass("idemailbox.h"),
				   "IdePathway" : ExposedClass("idepathway.h"),
				   "IdePositionEditor" : ExposedClass("idepositioneditor.h"),
				   "IdeSearchGlobalParams" : ExposedClass("idesearchglobalparams.h", ["searchquery.h"]),
				   "IdeSearchParam" : ExposedClass("idesearchparam.h"),
				   "IdeSearchQuery" : ExposedClass("idesearchquery.h", ["idesearchglobalparams.h", "searchquery.h"]),
				   "IdeSearchResults" : ExposedClass("idesearchresults.h", ["isearchresults.h", "searchquery.h"]),				   
				   "IdeSession" : ExposedClass("idesession.h", ["ideskin.h", "objectsuser.h", "portalsaccount.h", "portalsportal.h"]),
				   "IdeSessionPortal" : ExposedClass("idesessionportal.h"),
				   "IdeSkin" : ExposedClass("ideskin.h"),
				   "IdeSkinSimple" : ExposedClass("ideskinsimple.h"),
				   "IdeSystem" : ExposedClass("idesystem.h", ["ideskin.h", "ihttpserver.h"]), "DynamicSingleton<osiris::IdeSystem>" : ExposedClass(),
				   "IdeTableQuery" : ExposedClass("idetablequery.h"),
				   "IdeVoteEditor" : ExposedClass("idevoteeditor.h"),
				   "IdeXSLResolver" : ExposedClass("idexslresolver.h"),				   				   				   
				   "IIdePageable" : ExposedClass("iidepageable.h"),				   
				   "IMainPage" : ExposedClass("iidemainpage.h"),				   
				   "IPage" : ExposedClass("iidepage.h", ["htmlxslcontrol.h", "idepathway.h", "ideskin.h", "iportaldatabase.h", "omlcontext.h", "idesession.h", "portalsportal.h", "xmlstylesheet.h"]),				   
				   "IPageControl<osiris::HtmlDiv>" : ExposedClass(None, ["ideskin.h", "idesession.h"]),
				   "IPageControl<osiris::IHtmlControl>" : ExposedClass(None, ["ideskin.h", "idesession.h"]),
				   "IPortalBackgroundJob" : ExposedClass("iportalbackgroundjob.h"),				   
				   "IPortalDatabase" : ExposedClass("iportaldatabase.h", ["dataaccount.h", "datadiscussionstats.h", "dataentry.h", "datapeer.h", "datapolloptionstats.h", "datapollstats.h", "datauserstats.h", "dbbinder.h","idbconnection.h", "objectsiobject.h"]),
				   "IPortalPage" : ExposedClass("iideportalpage.h", ["ideassistant.h", "isisendpoint.h", "ids.h"]),				   
				   "IPortalPageControl<osiris::HtmlDiv>" : ExposedClass(None),				   
				   "IPortalPageControl<osiris::IHtmlControl>" : ExposedClass(None),				   				   
				   "IServer" : ExposedClass("iserver.h", ["iconnection.h"]),
				   "IStream" : ExposedClass("istream.h"),				   
				   "IsisEndpoint" : ExposedClass("isisendpoint.h", ["isisconnection.h"]),				   				   
				   "IXSLRenderer<osiris::IPageControl<osiris::IHtmlControl> >" : ExposedClass(None, ["xmlstylesheet.h"]),				   
				   "IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >" : ExposedClass(None, ["xmlstylesheet.h"]),				   				   				   				   				   
				   "LogManager" : ExposedClass("logmanager.h"), "StaticSingleton<osiris::LogManager, true>" : ExposedClass(),
				   "MainAddonsJob" : ExposedClass("mainaddonsjob.h"),				   
				   #"ObjectID" : ExposedClass("objectsobjectid.h"),				   
				   "Options" : ExposedClass("options.h", [], customizeOptions), "StaticSingleton<osiris::Options, true>" : ExposedClass(),				   
				   "Portal" : ExposedClass("portalsportal.h", ["portalspeersmanager.h", "ids.h", "portalsportaloptions.h", "reputationsmanager.h"]),
				   #"PortalsExporterJob" : ExposedClass("portalsexporterjob.h"), #Error: Strange, seem to work, but under Linux ICU throw an exception about this.
				   "PortalsImporterJob" : ExposedClass("portalsimporterjob.h"),
				   #"PortalID" : ExposedClass("portalsportalid.h"),
				   "PortalOptions" : ExposedClass("portalsportaloptions.h", ["datetime.h", "idboptions.h"]),
				   "PortalsOptionsShared" : ExposedClass("portalsoptionsshared.h", ["datetime.h", "idboptions.h"]),
				   "PortalsPeer" : ExposedClass("portalspeer.h"),
				   "PortalsPeersManager" : ExposedClass("portalspeersmanager.h"),
				   "PortalsSystem" : ExposedClass("portalsportalssystem.h", ["datetime.h", "httpenumerateddirectorycallback.h", "httpvirtualdirectory.h", "isisservice.h", "ids.h"]), "DynamicSingleton<osiris::PortalsSystem>" : ExposedClass(),
				   "PortalsStabilityJob" : ExposedClass("portalsstabilityjob.h"),
				   "PortalsTransaction" : ExposedClass("portalstransaction.h"),				   				   				   
				   }
				   
####################################################################################################

def includeClasses(namespace):
	# N.B.: non usare use_overload_macro in quanto non verrebbe generato il wrapper per istanziare PythonThreadSaver
	
	#print "DUMP CLASSES BEGIN"
	#for tmp in namespace.classes():
#		print tmp.name		
#	print "DUMP CLASSES END"
	
	global exposed_classes
				
	for className, exposedClass in exposed_classes.items():
		c = namespace.class_(className)
		exposedClass.expose(c)			
				
		for base in c.recursive_bases:
			# Rimuove dalla classe un eventuale AllocWrapper<T, A> per evitare problemi di compilazione in seguito all'utilizzo di allocatori diversi (NedAllocator, SysAllocator, ecc...) sotto piattaforme diverse
			if "AllocWrapper" in base.related_class.name:
				c.recursive_bases.remove(base)	
				
			# Include le dipendenze delle basi nelle classi derivate
			exposedBase = exposed_classes.get(base.related_class.name, None)
			if exposedBase:
				if exposedBase.relatedHeaders:
					for rh in exposedBase.relatedHeaders:
						exposedClass.include(c, rh)
			else:
				logUnexposedClass(base.related_class)				
				
		for derived in c.recursive_derived:
			# Include nella classe l'header principale delle derivate per supportare eventuali boost::python::implicitly_convertible di quest'ultima
			exposedDerived = exposed_classes.get(derived.related_class.name, None)
			if exposedDerived:
				if exposedDerived.mainHeader:
					exposedClass.include(c, exposedDerived.mainHeader)
			else:
				logUnexposedClass(derived.related_class)
				
####################################################################################################

def sortClasses(x, y):
	global exposed_classes
	
	if exposed_classes.has_key(x.name) and exposed_classes.has_key(y.name):
		exposedX = exposed_classes[x.name]
		exposedY = exposed_classes[y.name]		
				
		if exposedX.priority == exposedY.priority:
			return 0			
		elif exposedX.priority > exposedY.priority:
			return -1
		else:
			return 1			
	
	return cmp(x.decl_string, y.decl_string)
				
old_sort_classes = creators_factory.sort_algorithms.sort_classes
def override_sort_classes( classes ):
	classes = old_sort_classes(classes)
	classes.sort(sortClasses)
	return classes
	
creators_factory.sort_algorithms.sort_classes = override_sort_classes

####################################################################################################

def includeEnumerations(namespace):
	exposed_enumerations = ["BlockType",
						"CompressorType",
						"CompressionLevel",
						"RsaType",
						"HashType",
					    "HttpAuthorization",
					    "HttpMethod",
					    "HttpPageMode",
					    "HttpStatusCode",
					    "LogLevel",
					    "MessageFolder",
					    "MessageStatus",
					    "OMLRenderMode",
					    "OMLRenderUrlDestination",
					    "PageArea",
					    #"PortalAccess",
					    "PortalObjectType",
					    #"PortalType",					    
					    "SearchDateTime",
					    "SearchOrderMode",
					    "SearchOrderDirection",					    
					    "SearchTextMode",
					    "SearchTextWhere",
					    "TaskPriority"]

	for e in exposed_enumerations:
		namespace.enumerations(e).include()
		
####################################################################################################		
		
def applyDefaultReturnPolicies(functions):
	for f in functions:
		if not f.call_policies:
			return_type = f.return_type
			if declarations.is_reference(return_type) or declarations.is_pointer(return_type):
				type_info = return_type
				type_info = declarations.remove_pointer(type_info)
				type_info = declarations.remove_reference(type_info)
				type_info = declarations.remove_const(type_info)				
				# Se il tipo non e' esposto (potrebbe essere una classe, ma non ci sono informazioni perche' la dichiarazione non e' stata incontrata), viene gestito tramite return_opaque_pointer 
				if declarations.is_class(type_info):
					f.call_policies = call_policies.return_value_policy(call_policies.reference_existing_object)
				else:
					f.call_policies = call_policies.return_value_policy(call_policies.return_opaque_pointer)
		
####################################################################################################

wrappers_file = os.path.join(settings.root_dir, "wrappers.h")
cache_file = os.path.join(settings.output_directory, "wrappers.xml")

if (len(sys.argv) > 1) and (sys.argv[1] == "enable-cache"):	
	logger.info("Cache enabled")	
else:	
	try:
		os.remove(cache_file)		
	except:
		pass
		
cached_source_fc = parser.create_cached_source_fc(wrappers_file, cache_file)

module_builder = module_builder.module_builder_t(files = [cached_source_fc], 
						    gccxml_path = settings.gccxml_path, 
						    working_directory = settings.working_directory, 
						    include_paths = settings.include_paths, 
						    define_symbols = settings.defined_symbols
						    ,indexing_suite_version = 2
						    )
						    
module_builder.calldefs().create_with_signature = True

namespace_global = module_builder.global_ns
namespace_global.exclude()

namespace_osiris = namespace_global.namespace("osiris");

mfuns = namespace_osiris.mem_funs(allow_empty=True)
mfuns.add_default_precall_code("::osiris::PythonThreadSaver __pythreadSaver;")
mfuns.add_override_precall_code("::osiris::PythonState __pystate(getPythonThreadState());")
mfuns.add_override_native_precall_code("__pystate.leave();")

initAlreadyExposedClasses(module_builder)
includeClasses(namespace_osiris)
includeEnumerations(namespace_osiris)
applyDefaultReturnPolicies(mfuns)

module_builder.build_code_creator(module_name = settings.module_name)
module_builder.code_creator.user_defined_directories.append(settings.root_dir)

module_builder.add_declaration_code('#include "pythonmanualwrappers.h"')
module_builder.add_registration_code("OS_NAMESPACE_NAME::initPythonManualWrappers();", False)

module_builder.code_creator.replace_included_headers([])		# Rimuove tutti gli headers inclusi automaticamente (wrappers.h in particolare che non deve essere incluso in ogni classe)
module_builder.code_creator.adopt_creator(code_creators.include_t("stdafx.h"), 0)
module_builder.code_creator.adopt_creator(code_creators.include_t("pypluspluscommon.h"), 1)

####################################################################################################

# I nomi dei files rispecchiano quelli delle classi, ma nell'include dei wrapper vengono generati in minuscolo da Py++ (il che crea problemi sotto linux...)
# Questa funzione fa da wrapper a quella originale passando il nome del file sempre in minuscolo
original_write_file = pyplusplus.file_writers.multiple_files.multiple_files_t.write_file
def make_lower_write_file(self, fpath, content):
	splittedPath = os.path.split(fpath)
	fpath = os.path.join(splittedPath[0], splittedPath[1].lower())	
	original_write_file(self, fpath, content)
pyplusplus.file_writers.multiple_files.multiple_files_t.write_file = make_lower_write_file

def make_lower_create_value_traits_header_name( self, value_class ):
	return ("_" + value_class.alias + "__value_traits" + self.HEADER_EXT).lower()
pyplusplus.file_writers.multiple_files.multiple_files_t.create_value_traits_header_name = make_lower_create_value_traits_header_name

####################################################################################################

module_builder.split_module(dir_name = settings.output_directory, huge_classes = [], on_unused_file_found = os.remove)

####################################################################################################

logger.info("Elapsed %.1f seconds" % (time.clock() - startTime))

####################################################################################################
