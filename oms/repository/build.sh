#!/bin/bash

# TODO:
# - Ci vorrebbe un'opzione per specificare se "stable" o "testing", per ora è fisso "stable".
# - La versione dovrebbe arrivargli?

var_version="0.12.0"
var_outname="osiris-sps_"$var_version"_i386"

# Controllo se è lanciato da root
if [ "$(whoami)" != "root" ]; then
	echo "Sorry, you are not root."
	exit 1
fi

function clean {
	#echo Deleting $2 from $1
	#find $1 -name $2
	echo "1>>"	
	echo find $1 -name $2
	for i in `find $1 -name $2`; do echo rm -r $i ; rm -r $i ; done	
	echo "2>>"	
	echo find $1 -name $2
	for i in `find $1 -name $2`; do echo rm -r $i ; rm -r $i ; done	
}

function cleans {
clean $1 ".svn"
clean $1 "xx*"
clean $1 "*_work*"
clean $1 "thumbs.db"
clean $1 "machine.db"
clean $1 "*.ilk"
clean $1 "certificates"
clean $1 "mod_test"
clean $1 "00000005F81C732A927A12CB32221B84D5372CA322BB5CDC"
clean $1 "00000005F81C7322927FAACB32221BF5D5372C73229BD1CC"
clean $1 "mysql"
clean $1 "sample"
clean $1 "pluginsample"
clean $1 "stress"
clean $1 "opendht"
clean $1 "kademlia"
clean $1 "mono"
clean $1 "sample_base"
clean $1 "sample_adv"
clean $1 "testapp"
clean $1 "testapp.sh"
clean $1 "kademlia.so"
clean $1 "mysql.so"
clean $1 "opendht.so"
clean $1 "pluginsample.so"
clean $1 "stress.so"
clean $1 "console"
clean $1 "console.lib" # 20/10/2008 - Non dovrebbe servire +
clean $1 "console.exp" # 20/10/2008 - Non dovrebbe servire +
clean $1 "sampleapp"
clean $1 "libtorrent_logs" # 20/10/2008 - Non dovrebbe servire +
clean $1 "libtorrent_logs0" # 20/10/2008 - Non dovrebbe servire +
clean $1 "libtorrent_logs6881" # 20/10/2008 - Non dovrebbe servire +
clean $1 "dht.log" # 20/10/2008 - Non dovrebbe servire +
clean $1 "00000006084563A2D4B414CAE520F81430287EB9FBB2A73D" # mysql
clean $1 "sample" # sample
clean $1 "000000060E50136276A0E87CD5ADBB85D75AD3D0E0811D0D" # pluginsample
clean $1 "00000006676C8C3CD59A1233507465F82FFCF6AEFCF713AE" # stress
clean $1 "000000063DF135C9B80B2889C8061F54C758844D253E17DA" # opendht
clean $1 "0000000688736612A05E17AEF6A23F905660234E8C6E199F" # kademlia
clean $1 "000000064DB3A7FF3F31FE30C82838415F982A34C9EFB9D3" # mono
clean $1 "00000006E99862396FD9E355A6ECC5A94900924A0A973888" # wrappers
}

# ---------------------------
# Versione no-install
# ---------------------------
echo -------------------------
echo Version: no-install
echo

var_buildpath="../output/linux-i386-noinstall"
var_buildpath2="$var_buildpath/osiris-sps_$var_version"

echo Removing old path...
rm $var_buildpath -r

echo Copying releasing files...
mkdir $var_buildpath2/share -p
cp -r ../../client/share/* $var_buildpath2/share
cp -r ../../client/bin/linux-i386/release/* $var_buildpath2

echo Copying system.xml files...
cp ../data/systems/linux-i386_noinst.xml $var_buildpath2/system.xml

#echo Copying launching script...   # 20/10/2008 - Non dovrebbe servire +
#cp debian/osiris-sps_noinst $var_buildpath2/osiris-sps

echo Cleaning
cleans $var_buildpath


echo Creating .tar.gz...
# tar.gz - il 'cd' è brutto e forzato, non ho trovato alternative.
cd $var_buildpath
tar -czf ../../repository/"$var_outname".tar.gz *
cd ../../scripts

# ---------------------------
# Version: debian .deb
# ---------------------------
echo -------------------------
echo Version: debian .deb
echo

var_buildpath="../../output/linux-i386-debian"

echo Removing old path...
rm $var_buildpath -r

echo Copying releasing files...
mkdir $var_buildpath/usr/share/osiris-sps -p
cp -r ../../client/share/* $var_buildpath/usr/share/osiris-sps

mkdir $var_buildpath/usr/lib/osiris-sps -p
cp -r ../../client/bin/linux-i386/release/* $var_buildpath/usr/lib/osiris-sps

echo Copying system.xml files...
cp ../data/systems/linux-i386_inst.xml $var_buildpath/usr/lib/osiris-sps/system.xml

echo Cleaning
cleans $var_buildpath

# ... files specifici per il pacchetto .deb
echo Copying debian .deb specific files...
mkdir $var_buildpath/DEBIAN -p
cp debian/control $var_buildpath/DEBIAN

mkdir $var_buildpath/usr/bin/ -p
cp debian/osiris-sps_inst $var_buildpath/usr/bin/osiris-sps

mkdir $var_buildpath/usr/share/pixmaps -p
cp ../data/graphics/icon.png $var_buildpath/usr/share/pixmaps/osiris-sps.png

mkdir $var_buildpath/usr/share/icons -p
cp ../data/graphics/icon.png $var_buildpath/usr/share/icons/osiris-sps.png

mkdir $var_buildpath/usr/share/applications -p
cp debian/osiris-sps.desktop $var_buildpath/usr/share/applications

#mkdir ../output_debian/usr/share/doc
#mkdir ../output_debian/usr/share/doc/osiris
#cp README ../output_debian/usr/share/doc/osiris-sps
#cp copyright ../output_debian/usr/share/doc/osiris-sps
#cp changelog ../output_debian/usr/share/doc/osiris-sps

#mkdir $var_buildpath/usr/lib/osiris-sps -p
#cp osiris $var_buildpath/usr/lib/osiris-sps

echo Creating debian package...
dpkg -b $var_buildpath "../repository/"$var_outname".deb"

echo Copying in debian repository
cp "../repository/"$var_outname".deb" "../repository/debian/dists/stable/non-free/binary-i386/"$var_outname".deb"

echo Updating debian repository
cd ../repository/debian
./build.sh
cd ../../scripts

# ---------------------------
# Version: misc with "alien"
# ---------------------------
echo -------------------------
echo Version: misc with "alien"
echo

cd ../repository
debfile="../repository/"$var_outname".deb";
echo RPM
alien $debfile --to-rpm 
echo Stampede
alien $debfile --to-slp
echo LSB
alien $debfile --to-lsb
echo Slackware
alien $debfile --to-tgz 
echo Solaris
alien $debfile --to-pkg 
cd ../scripts

echo Completed.
echo ----------------

echo Clodo TEMP: Copio i file generati nel volume Windows, xchè solo da Win32 riesco ad uploadare via WINSCP.
cp --verbose * /media/sdb1/Projects/Osiris/distribute/repository
cp --verbose /home/clodo/projects/osiris/distribute/repository/debian/dists/stable/non-free/binary-i386/* /media/sdb1/Projects/Osiris/distribute/repository/debian/dists/stable/non-free/binary-i386