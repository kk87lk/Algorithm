cmake_minimum_required(VERSION 3.19)                        #要求 CMake 最低版本为 3.19（比之前的 3.16 更高）。
project(Algorithm LANGUAGES CXX)                            #定义项目名称为 Algorithm，使用 C++ 语言。

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)      #要求 Qt6 版本至少为 6.5，并依赖 Core 和 Widgets 模块。

qt_standard_project_setup()                                 #自动设置 C++ 标准、启用自动 MOC/UIC/RCC 等。
#默认使用 C++17，如需修改可添加： set(CMAKE_CXX_STANDARD 20)

qt_add_executable(Algorithm
    WIN32 MACOSX_BUNDLE                                     #​WIN32：在 Windows 上构建为 GUI 应用程序（不显示控制台窗口）。
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    common/qlogger.h common/qlogger.cpp

)

target_link_libraries(Algorithm
    PRIVATE                                                 #​PRIVATE：依赖项仅用于构建 Algorithm，不传递给其他依赖它的目标。
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)                                     #作用：引入标准安装路径变量（如 CMAKE_INSTALL_BINDIR=bin）。

install(TARGETS Algorithm
    BUNDLE  DESTINATION .                                   #macOS 应用包安装到当前目录。
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}             #Windows 的 .exe 或 Linux 的可执行文件安装到 bin。
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}             #动态库安装到 lib（通常用于 Linux 的 .so 文件）。
)

qt_generate_deploy_app_script(                              #qt_generate_deploy_app_script 适用于传统 Widgets 应用，仅处理核心 Qt 库。
                                                            #qt_generate_deploy_qml_app_script 专为 QML 应用设计，会打包 QML 模块。

    TARGET Algorithm
    OUTPUT_SCRIPT deploy_script                             #生成跨平台部署脚本，自动复制 Qt 依赖库。
    NO_UNSUPPORTED_PLATFORM_ERROR                           #允许在不支持的平台上跳过错误（如 Linux）。
)

install(SCRIPT ${deploy_script})                            #在安装过程中运行部署脚本，确保目标机器具备 Qt 运行时环境。
