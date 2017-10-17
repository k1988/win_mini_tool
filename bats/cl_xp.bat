@rem 在vs2012及以上平台上，编译以xp为目标的情况
@rem SET CL=/D_USING_V140_SDK71_;%CL% 
@rem SET PATH=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Bin;%PATH%  
@rem SET INCLUDE=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Include;%INCLUDE%  
@rem SET LIB=C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib;%LIB% 

@rem 如果是控制台，需要将/subsystem:window,5.01 改成 /subsystem:console,5.01
cl /D_USING_V140_SDK71_ /link /subsystem:window,5.01  %1 %2 %3 %4 %5 %6 %7 %8 %9