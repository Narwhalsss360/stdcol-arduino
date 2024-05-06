clean_win:
	if exist test\UnitTest\*.log del /f /q test\UnitTest\*.log
	if exist test\UnitTest\x64 rmdir /s /q test\UnitTest\x64

clean_unix:
	rmdir test/UnitTest/bin

build_win: clean_win
	mkdir test\UnitTest\x64\Debug\Intermediate
	cl /Zi /EHsc /nologo /I"include" "test\UnitTest\main.cpp" "src\stdcol_index.cpp" /Fe"test\UnitTest\x64\Debug\UnitTest.exe" /Fd"test\UnitTest\x64\Debug\vc.pdb" /Fo"test\UnitTest\x64\Debug\Intermediate\\"

build_unix:
	mkdir -p test/UnitTest/bin
	g++ -I include test/UnitTest/main.cpp src/stdcol_index.cpp -o test/UnitTest/bin/a.out
