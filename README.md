# PackingSystem (Qt course design)

**MileStone: Hello UI!**

## 开发环境
- **IDE**: Qt Creator
- **Qt 版本**： Qt 6.8.3
- **编译器**: MSVC 2022 (Microsoft Visual C++ 2022)
- **构建系统**: CMake
- **操作系统**: Windows 11

## 编译说明
使用 Qt Creator 打开 `CMakeLists.txt` 配置并构建即可。如果Qt Creator未能自动识别Kit，请前往 工具->外部->配置... ->构建套件(kit)，手动配置Qt 6.8.3与MSVC 2022编译器

## 当前状态
-Y 搭建项目骨架(CMake + Qt)
-Y 完成UI布局(mainwindow.ui)
-Y 连接UI与业务接口(mainwindow + provide)
-N 实现停车出库/入库业务逻辑