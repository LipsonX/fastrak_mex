# Polhemus Fastrak matlab mex

This is a solution of mex for [Polhemus Fastrak](http://polhemus.com/motion-tracking/all-trackers/fastrak/ "Polhemus Fastrak"). the code just subject to Li,Ru's research, a sensor uesed. you can download and modify any where, just thanks.

## usage ##
- git clone
- modify project properties
	- modify VC++ Directories adjust to your install of matlab include and library
	- modify output in Linker->General->Output File(optional)
- select Debug/Release and x64 try to release or debug

## platform ##
- at least Windows 7 SP1, x64
- Matlab 2016a
- VS2012
- Polhemus SDK with VS2012 install

## Debug usage(optional) ##
- ctrl + atl + p attach to matlab.exe
- set a break point in code
- build
- open matlab and change dir into project dir
- tab >>fastrak('test')
- clean mex
- rebuild and try your new code

it may be help in [Mex文件在VS2010中调试方法](http://www.cnblogs.com/lukylu/p/4042306.html)