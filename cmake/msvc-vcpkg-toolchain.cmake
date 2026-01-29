# -------------------------------
# MSVC compiler paths
# -------------------------------
set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe")
set(CMAKE_CXX_COMPILER "${CMAKE_C_COMPILER}")

# -------------------------------
# Windows SDK tools
# -------------------------------
set(CMAKE_MT "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/mt.exe")
set(CMAKE_RC_COMPILER "C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64/rc.exe")

# -------------------------------
# Windows SDK include + lib paths
# -------------------------------
set(CMAKE_INCLUDE_PATH
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0"
)

set(CMAKE_LIBRARY_PATH
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/lib/x64"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64"
    "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/ucrt/x64"
)

# Force linker search paths
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LIBPATH:\"C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/lib/x64\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/ucrt/x64\"")

# -------------------------------
# vcpkg integration
# -------------------------------
set(VCPKG_TARGET_TRIPLET "x64-windows")
include("C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")