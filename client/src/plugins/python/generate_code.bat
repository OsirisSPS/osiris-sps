rem Remove temporary files
del wrappers\*.*~

rem Generate Python wrappers
generate_code.py

rem Overwrite automatic wrappers with manual wrappers
rem Not used, Fix 2 below.
rem copy forced\*.* wrappers

rem Custom replace, for fixing generated code that can't be compiled.
rem C# Source of "DotReplace" are under /utils/DotReplace.
rem Will be better to fix the generator.

rem - Fix 1 : Where need an explicit cast (std::string or std::wstring)
DotReplace edit "return func_onEncodeParam( i, context, name, value );" "return func_onEncodeParam( i, context, name, value ).operator std::wstring();" "wrappers" "oml*.cpp"
DotReplace edit "return func_getName(  );" "return func_getName(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
DotReplace edit "return func_getDescription(  );" "return func_getDescription(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"

rem - Fix 2 : I don't remember why.
DotReplace edit "::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);" "::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);" "wrappers" "regexmanager.pypp.cpp"
