# pi-app
在linux下使用C语言控制各种硬件接口的demo,基于linux自带的驱动接口

前提
======
- 需要安装wiringPi，本项目依赖wiringPi的gpio操作函数

使用
======
将项目下载到开发板上，运行以下命令，就会在所有.c文件旁边生成同名可执行文件
```
make
```