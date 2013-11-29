#!/bin/bash

python2 generate_code.py

echo Manual fixes...
# Manual fixes
# C# Source of "DotReplace" are under /utils/DotReplace.
# Will be better to fix the generator.


# Fix 1 : Where need an explicit cast (std::string or std::wstring)
mono DotReplace.exe edit "return func_onEncodeParam( i, context, name, value );" "return func_onEncodeParam( i, context, name, value ).operator std::wstring();" "wrappers" "oml*.cpp"
mono DotReplace.exe edit "return func_getName(  );" "return func_getName(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
mono DotReplace.exe edit "return func_getDescription(  );" "return func_getDescription(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"


# Fix 2 : I don't remember why.
mono DotReplace.exe edit "::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);" "::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);" "wrappers" "regexmanager.pypp.cpp"


echo Cleaning backups...
rm -f *.*~
rm -f wrappers/*.*~

echo Done.
