#!/bin/bash

# Lo script corregge il "search library order" in modo da evitare la LD_LIBRARY_PATH=./ .
# E' possibile che ci sia un modo per farlo da CodeBlock, ma non sono
# riuscito a trovarlo. LD_RUN_PATH usato in Linux non và.

# macpostbuild.sh "*.dylib" /da_dove
# Uso: "build.sh "*.dylib" " per fare anche le librerie, "build.sh "Osiris" " da mettere nel codeblock come postbuild.
# Ricordati i " sulla *.dylib, sennò processa solo il primo. Faccio questo giro complicato, xchè non posso farlo da codice: non posso andarmi a modificare i progetti delle librerie dello xalan, ad esempio. 

# Todo:
# - Fixare le liste di path sotto... è da tener aggiornato con gli eseguibili..
# - Saltare i symlink..

cd "$2"

echo Searching for $1
for i in `find $1`; 
 do 
  
  echo Updating $i;
  install_name_tool -id @executable_path/$i $i 
  for l in `find Osiris;find console;find *.dylib`;
   do
    echo Changing $l inside $i;
    install_name_tool -change ../../bin/mac32/debug/Osiris.app/Contents/MacOS/$l @executable_path/$l $i;
    install_name_tool -change ../../bin/mac32/release/Osiris.app/Contents/MacOS/$l @executable_path/$l $i;
    install_name_tool -change $l @executable_path/$l $i;
    install_name_tool -change /usr/lib/$l @executable_path/$l $i;
    install_name_tool -change /usr/local/lib/$l @executable_path/$l $i;
    install_name_tool -change /usr/local/ssl/lib/$l @executable_path/$l $i;
    
    install_name_tool -change /Users/osiris/osiris/libraries/wxwidgets/wxWidgets-2.9.0/gtk-build/debug/lib/$l @executable_path/$l $i;   
    install_name_tool -change /Users/osiris/osiris/libraries/wxwidgets/wxWidgets-2.9.0/gtk-build/release/lib/$l @executable_path/$l $i;   
done;

   otool -L $i;
 done
