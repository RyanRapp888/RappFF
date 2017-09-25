pushd %~dp0
cd Freetype_src

c:/7-Zip/7z.exe x freetype-2.8.7z

cd ..

cd GLFW_src

c:/7-Zip/7z.exe x GLFW_src.7z
popd