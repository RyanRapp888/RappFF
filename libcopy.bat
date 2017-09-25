pushd %~dp0
xcopy /Freetype_src/freetype-2.8/objs/freetype28.lib x64/Release/freetype28.lib
xcopy /Freetype_src/freetype-2.8/objs/freetype28_D.lib x64/Debug/freetype28_D.lib
popd