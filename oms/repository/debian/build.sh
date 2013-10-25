dpkg-scanpackages dists/stable/non-free/binary-i386 /dev/null | gzip -9c > dists/stable/non-free/binary-i386/Packages.gz
dpkg-scanpackages dists/testing/non-free/binary-i386 /dev/null | gzip -9c > dists/testing/non-free/binary-i386/Packages.gz
#dpkg-scansources source /dev/null | gzip -9c > source/Sources.gz