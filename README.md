# PackingSystem (Qt course design)

**MileStone: Hello UI!**

一个基于 Qt 的停车场管理系统，支持车辆入库、出库、排队与历史记录，
数据以 JSON 格式持久化到本地。

## 功能

- 停车场车位可视化，点击车位查看车辆详情
- 车辆入库：自动分配空车位，满员时进入等待队列
- 车辆出库：结算费用，队列自动补位
- 三个视图：在场车辆 / 等待队列 / 历史记录
- 数据持久化：启动时导入 JSON，关闭时导出 JSON

## 开发环境
- **IDE**: Qt Creator
- **Qt 版本**： Qt 6.8.3
- **编译器**: MSVC 2022 (Microsoft Visual C++ 2022)
- **构建系统**: CMake
- **操作系统**: Windows 11

## 编译说明
使用 Qt Creator 打开 `CMakeLists.txt` 配置并构建即可。如果Qt Creator未能自动识别Kit，请前往 工具->外部->配置... ->构建套件(kit)，手动配置Qt 6.8.3与MSVC 2022编译器

## 运行
从 [Releases](../../releases) 下载最新 `PackingSystem-vX.X-win64.zip`，
解压后双击 `PackingSystem.exe` 即可运行，无需安装 Qt。

## 数据文件位置
程序运行时数据保存在：
C:\Users<用户名>\AppData\Roaming\ParkingSystem\carpark.json

## 项目结构
PackingSystem/
├── CMakeLists.txt
├── main.cpp
├── mainwindow.h / .cpp / .ui # UI 层
├── provide.h / .cpp # 业务层（parkingsystem）
└── carstore.h / .cpp # 存储层（JSON 读写）

## 进度
- [x] 搭建项目骨架 (CMake + Qt)
- [x] 完成 UI 布局 (mainwindow.ui)
- [x] 连接 UI 与业务接口 (mainwindow + provide)
- [x] 实现停车出库/入库业务逻辑
- [x] JSON 数据持久化（启动导入 / 关闭导出）