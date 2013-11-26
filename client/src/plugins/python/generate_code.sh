#!/bin/bash

python2 generate_code.py

echo Manual fixes...
# Manual fixes
# Syntax:
# find wrappers/ -name "*.txt" -print0 | xargs -0 sed -i '' -e 's/foofds9f3jf9219jg0gdg23/bar/g'

# Fix 1 : Where need an explicit cast (std::string or std::wstring)
#DotReplace edit "return func_onEncodeParam( i, context, name, value );" "return func_onEncodeParam( i, context, name, value ).operator std::wstring();" "wrappers" "oml*.cpp"
#DotReplace edit "return func_getName(  );" "return func_getName(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
#DotReplace edit "return func_getDescription(  );" "return func_getDescription(  ).operator std::string();" "wrappers" "idbdriver.pypp.cpp"
perl -e "s/FIND/REPLACE/g;" -pi $(find wrappers -type f)
perl -e "s/return func_onEncodeParam( i, context, name, value );/return func_onEncodeParam( i, context, name, value ).operator std::wstring();/g;" -pi $(find wrappers -type f)
perl -e "s/return func_getName(  );/return func_getName(  ).operator std::string();/g;" -pi $(find wrappers -type f)
perl -e "s/return func_getDescription(  );/return func_getDescription(  ).operator std::string();/g;" -pi $(find wrappers -type f)


# Fix 2 : I don't remember why.
#DotReplace edit "::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);" "::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);" "wrappers" "regexmanager.pypp.cpp"
perl -e "s/::boost::shared_ptr<boost::basic_regex<wchar_t, boost::regex_traits<wchar_t, boost::cpp_regex_traits<wchar_t> > > > result = inst.getRegex(regex, nocase);/::boost::shared_ptr<boost::wregex> result = inst.getRegex(regex, nocase);/g;" -pi $(find wrappers -type f)


echo Cleaning backups...
rm -f *.*~
rm -f wrappers/*.*~

echo Done.
