setenv MYROOT c:/devstuff/RappFF
setenv MSDEV c:/VS
setenv WINKITS "c:/PROGRA~2/WI3CF2~1/10"

setenv INCLUDE ".;${MSDEV}/VC/INCLUDE;${MSDEV}/VC/ATLMFC/INCLUDE;${WINKITS}/Include/100143~1.0/shared;${WINKITS}/Include/100143~1.0/um;${WINKITS}/Include/um;${WINKITS}/Include/100143~1.0/ucrt;${MYROOT}/include"

setenv LIB ".;${MYROOT}/lib;${WINKITS}/Lib\100143~1.0/um/x64;${MSDEV}/VC/lib/amd64;${WINKITS}/Lib\100143~1.0/ucrt/x64;${MYROOT}/lib/glfwlib-vc2015"
set path = (/cygdrive/c/VS/Common7/IDE/ /cygdrive/c/VS/VC/BIN/x86_amd64 /cygdrive/c/devstuff/cygwin1.7.34/bin /cygdrive/C/devtools/gvim7.2 )

rehash

# ${WINKITS}/Include/shared;