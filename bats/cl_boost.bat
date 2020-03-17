@rem 设置boost头文件和库目录
@echo setting boost env
SET CL=/std:c++latesd /EHsc %CL%
SET LINK=/LIBPATH:D:\vc_include\boost_1_71_0\stage\lib %LINK%
SET INCLUDE=D:\vc_include\boost_1_71_0\stage;%INCLUDE%  
SET LIB=D:\vc_include\boost_1_71_0\stage\lib;%LIB% 