setenv MYROOT c:/code/RappFF
setenv MSDEV c:/VS2013
setenv WINKITS "c:/PROGRA~2/WI3CF2~1/8.1"

setenv INCLUDE ".;${MSDEV}/VC/INCLUDE;${MSDEV}/VC/ATLMFC/INCLUDE;${MYROOT}/include;${WINKITS}/Include/shared;${WINKITS}/Include/um;{WINKITS}/Include/shared"
setenv LIB ".;${WINKITS}/Lib/winv6.3/um/x64;${WINKITS}/Lib/winv6.3/um/x64;${MSDEV}/VC/lib/amd64;${MYROOT}/lib;${MYROOT}/lib/glfwlib-vc2013"

set path = (/cygdrive/c/VS2013/Common7/IDE/ /cygdrive/c/VS2013/VC/BIN/amd64 /cygdrive/c/devtools/cygwin1.7.34/bin /cygdrive/C/devtools/gvim7.2 /cygdrive/c/TortoiseHG )

rehash
