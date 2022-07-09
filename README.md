# ultrasonic

## 介绍

A ultrasonic sensor library for STM32 based on HAL

基于stm32HAL库的超声波传感器库，推荐使用[HC-SR04](https://item.taobao.com/item.htm?id=606445673789)

## 使用

本仓库采用CLion+OpenOCD+HAL开发环境，具体配置可以参考[这里](https://github.com/Aiano/Clion-Stm32)

1. 将`ultrasonic`文件夹拷贝到工程目录下

2. 配置`CMakeLists_template.txt`，添加相关头文件和源文件为target

3. 打开STM32CubeMX，

   1. [配置](.\Ultrasonic超声波模块.md)
   2. 点击Generate Code重新生成代码

4. 在`main.c`中引用头文件

   ```c
   /* USER CODE BEGIN Includes */
   #include "ultrasonic.h"
   /* USER CODE END Includes */
   ```

5. `main()`函数中初始化

   ```c
   /* USER CODE BEGIN 2 */
   ULTRASONIC_init();
   static double distance;
   /* USER CODE END 2 */
   ```

6. 主循环添加

   ```c
   /* USER CODE BEGIN WHILE */
   while (1) {
   	ULTRASONIC_start();
   	HAL_Delay(200);
   	distance = ULTRASONIC_read();
   	printf("%d\n", (int) distance);
   	/* USER CODE END WHILE */
   
   	/* USER CODE BEGIN 3 */
   }
   ```

   
