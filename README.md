# win_mini_tool

windows平台上自已随意编写的简单的一些小工具，试试github的用法，就随意传些上来

# 组织结构

* **src** c/c++格式的微代码文件，每个cpp文件都是一个单独的小工具的源码，里面大多只有一个函数。编译时会生成一个tmp目录
* **bin** src编译后的程序
* **scripts** 各种脚本(autohotkey script、python等)
* **bats** windows批处理脚本
* **build_all.bat** 编译所有src下文件
* **build_one.bat** 被build_all.bat调用的，只编译一个cpp文件，通过参数指定文件路径
* **InstallToPath.bat** 将几个常用目录加入到系统环境变量中，方便直接在win+R对话框中执行命令

# third party

 部分脚本依赖于

* http://ahkscript.org/
* https://www.python.org/