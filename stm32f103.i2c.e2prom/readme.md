<<<<<<< HEAD
# 秉火指南者 - 工程模板

## 创建工程步骤

1. 建立好工程文件夹，在 git 命令行敲命令更快：mkdir doc driver library listing output project startup user
2. 使用 keil 新建工程，工程文件保存在 project 目录下
3. 将文件加入工程中，注意 *.s 启动代码要必须要加入工程
4. 设置链接和编译输出文件路径
5. 指定头文件查找路径
6. 设置默认开启的宏定义：STM32F10X_HD, USE_STDPERIPH_DRIVER
7. 仅需要添加使用到的库文件进工程
8. 修改 stm32f10x_conf.h 文件，只需要包含使用到的头文件
9. 新增 stm32f10x_main.c 文件，实现主函数
10. 编译工程，工程可以编译通过
11. 设置 DAP/J-LINK 下载配置，需要配置两个地方：Debug、Utilites
=======
# 秉火指南者 - 工程模板

## 创建工程步骤

1. 建立好工程文件夹，在 git 命令行敲命令更快：mkdir doc driver library listing output project startup user
2. 使用 keil 新建工程，工程文件保存在 project 目录下
3. 将文件加入工程中，注意 *.s 启动代码要必须要加入工程
4. 设置链接和编译输出文件路径
5. 指定头文件查找路径
6. 设置默认开启的宏定义：STM32F10X_HD, USE_STDPERIPH_DRIVER
7. 仅需要添加使用到的库文件进工程
8. 修改 stm32f10x_conf.h 文件，只需要包含使用到的头文件
9. 新增 stm32f10x_main.c 文件，实现主函数
10. 编译工程，工程可以编译通过
11. 设置 DAP/J-LINK 下载配置，需要配置两个地方：Debug、Utilites
>>>>>>> updata
