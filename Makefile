
PINTOOL = ..\pintool
DLLS = pindos-x32.dll
SOURCES = $(wildcard *.cpp)

OBJECTS32 = $(patsubst %.cpp, %-x32.obj, $(SOURCES))

default: $(DLLS)

%-x32.obj: %.cpp
	cl /c /MT /EHs- /EHa- /wd4530 /DTARGET_WINDOWS /DBIGARRAY_MULTIPLIER=1 \
		/DUSING_XED /D_CRT_SECURE_NO_DEPRECATE /D_SECURE_SCL=0 /nologo /Gy \
		/O2 /DTARGET_IA32 /DHOST_IA32 /I$(PINTOOL)\source\include \
		/I$(PINTOOL)\source\include\gen /I$(PINTOOL)\source\tools\InstLib \
		/I$(PINTOOL)\extras\xed2-ia32\include \
		/I$(PINTOOL)\extras\components\include $^ /Fo$@

%-x32.dll: %-x32.obj $(OBJECTS32)
	link /DLL /EXPORT:main /NODEFAULTLIB /NOLOGO /INCREMENTAL:NO /OPT:REF \
		/MACHINE:x86 /ENTRY:Ptrace_DllMainCRTStartup@12 /BASE:0x55000000 \
		/LIBPATH:$(PINTOOL)\ia32\lib /LIBPATH:$(PINTOOL)\ia32\lib-ext \
		/LIBPATH:$(PINTOOL)\extras\xed2-ia32\lib /OUT:$@ $^ pin.lib \
		libxed.lib libcpmt.lib libcmt.lib pinvm.lib kernel32.lib ntdll-32.lib

clean:
	rm '*.dll' '*.exp' '*.lib' '*.obj'
