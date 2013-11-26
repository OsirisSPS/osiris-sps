#!/bin/bash

python2 generate_code.py

# Manual fixes
# Syntax:
# find . -name "*.txt" -print0 | xargs -0 sed -i '' -e 's/foo/bar/g'

# Fix 1 : Where need an explicit cast (std::string or std::wstring)
#DotReplace edit "return func_onEncodeParam( i, context, name, value );" "return func_onEncodeParam( i, context, name, value ).operator std::wstring();" "wrappers" "oml*.cpp"
#DotReplace edit "return func_getName(  );" "return func_getName(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
#DotReplace edit "return func_getDescription(  );" "return func_getDescription(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
find ./wrappers/ -name "oml*.cpp" -print0 | xargs -0 sed -i '' -e 's/return func_onEncodeParam( i, context, name, value );/return func_onEncodeParam( i, context, name, value ).operator std::wstring();/g'
find ./wrappers/ -name "idbdriver.pypp.cpp" -print0 | xargs -0 sed -i '' -e 's/return func_getName(  );/return func_getName(  ).operator std::string();/g'
find ./wrappers/ -name "idbdriver.pypp.cpp" -print0 | xargs -0 sed -i '' -e 's/return func_getDescription(  );/return func_getDescription(  ).operator std::string();/g'


# Fix 2 : I don't remember why.
#DotReplace edit "::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);" "::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);" "wrappers" "regexmanager.pypp.cpp"
find ./wrappers/ -name "regexmanager.pypp.cpp" -print0 | xargs -0 sed -i '' -e 's/::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);/::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);/g'


# Cleaning backups
rm *.*~
rm wrappers\*.*~