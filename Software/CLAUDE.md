# CLAUDE.md

本文件为 Claude Code (claude.ai/code) 在此代码仓库中工作时提供指导。

## 项目概述

TripleKey 是一款基于 ESP32-S3 的智能控制器固件，配备三个 128x128 OLED 显示屏、旋转编码器、物理按键，并支持智能家居集成。设备可作为蓝牙键盘、媒体控制器、智能家居中枢（米家）、天气站、股票行情器和生产力工具（计时器、番茄钟）使用。

**版本**: 1.3.0 (硬件: TRIPLEKEY_V2)
**许可证**: 署名-非商业性使用-相同方式共享 4.0 国际

## 构建命令

本项目使用 PlatformIO 多环境配置，所有硬件变体在单一分支中通过不同环境管理。所有命令应在 `Software/` 目录下运行。

```bash
# 构建默认环境 (NV3023_16MB)
pio run

# 构建指定环境
pio run -e NV3023_16MB
pio run -e NV3023_8MB
pio run -e GC9107_16MB
pio run -e GC9107_8MB

# 构建所有环境
pio run

# 上传指定环境到设备（通过USB）
pio run -e NV3023_16MB -t upload

# 上传文件系统镜像（LittleFS）
pio run -e NV3023_16MB -t uploadfs

# 串口监视器
pio device monitor

# 构建并上传指定环境（一条命令）
pio run -e NV3023_16MB -t upload && pio device monitor

# 清理构建
pio run -t clean

# 生成发布固件（构建+文件系统+合并，输出到上级 Firmware/）
python scripts/release.py

# 仅构建固件
python scripts/release.py --firmware

# 仅构建文件系统
python scripts/release.py --fs

# 仅合并（需先构建过）
python scripts/release.py --merge

# 指定环境
python scripts/release.py -e NV3023_8MB

# 全部环境
python scripts/release.py --all-envs
```

### 可用环境

| 环境名称 | 显示屏 | Flash大小 | 分区表 | 照片功能 |
|----------|--------|-----------|--------|----------|
| `NV3023_16MB` | NV3023 | 16MB | custom_16MB.csv | 有 |
| `NV3023_8MB` | NV3023 | 8MB | custom_8MB.csv | 无 |
| `GC9107_16MB` | GC9107 | 16MB | custom_16MB.csv | 有 |
| `GC9107_8MB` | GC9107 | 8MB | custom_8MB.csv | 无 |

默认环境为 `NV3023_16MB`，可在 `platformio.ini` 中修改 `default_envs`。

### 版本号管理

版本号以 `VERSION` 文件为唯一真实来源。编译时 pre-build 脚本 `scripts/generate_version.py` 自动读取该文件并生成 `src/version_generated.h`（已加入 `.gitignore`，不入库），代码中通过 `#include "version_generated.h"` 使用 `VER_SW` 宏。

**首次 clone 后需要先运行一次脚本**，否则 IDE 会因缺少 `version_generated.h` 报错：

```bash
python scripts/generate_version.py
```

发版时只需修改 `VERSION` 文件，构建脚本 `release.py` 和固件内部版本号会自动同步。

## 硬件配置

### 显示屏类型
- **SCREEN_NV3023**: NV3023 显示控制器（旋转: 2, RGB交换: 否）
- **SCREEN_GC9107**: GC9107 显示控制器（旋转: 0, RGB交换: 是）

### Flash 大小
- **16MB**: 使用 `custom_16MB.csv` 分区表（约12MB SPIFFS）
- **8MB**: 使用 `custom_8MB.csv` 分区表（约4MB SPIFFS）

### 功能标志（platformio.ini 中的 build_flags）
- `SUPPORT_AUDIO`: 通过 I2S 输出音频
- `SUPPORT_WS2812`: RGB LED 支持
- `SUPPORT_CH423S`: 用于米家集成的 I2C 扩展器
- `SUPPORT_HumanSensor`: HLK-2410C 人体存在传感器
- `SUPPORT_PHOTO`: 照片查看器功能（仅16MB环境启用，8MB空间不足）

### 条件编译

源码中使用 `#ifdef SUPPORT_PHOTO` 包裹照片相关功能，8MB环境不定义此宏，照片页面和相关Web配置将被排除。

## 架构

### 基于页面的导航系统

固件使用管理器模式的页面系统。每个页面是一个独立的模块，具有生命周期回调函数：

```cpp
typedef struct page {
    init_cb_t init;   // 初始化变量
    enter_cb_t enter; // 进入页面时调用
    exit_cb_t exit;   // 离开页面时调用
    loop_cb_t loop;   // 页面激活时的主循环
    char *title_cn;   // 中文显示名称
    const uint8_t *icon; // XBM 图标数据
    // ... 其他字段
} page_t;
```

**关键文件**:
- `src/manager/manager.h` - 管理器接口和 page_t 结构
- `src/manager/manager.cpp` - 页面切换逻辑
- `src/pages/pages.cpp` - 页面注册和主菜单
- `src/pages/pages.h` - 外部页面声明

### 添加新页面

1. 在 `src/pages/yourpage/` 下创建新文件夹
2. 实现 `page_yourpage`，包含 init/enter/exit/loop 回调函数
3. 在 `src/pages/pages.cpp` 中添加 include 并注册：
   ```cpp
   #include "pages/yourpage/yourpage.h"
   // 在 pages[] 数组中：
   &page_yourpage,
   ```

### 目录结构

```
src/
├── main.cpp           # 入口点（setup/loop）
├── board_def.cpp/h    # 硬件初始化，全局对象
├── manager/           # 页面导航系统
├── pages/             # 所有页面实现
│   ├── clock/         # 时间显示
│   ├── weather/       # 天气信息（和风天气API）
│   ├── stocks/        # 股票行情
│   ├── mijia/         # 米家智能家居
│   ├── keyboard/      # 蓝牙键盘
│   ├── shortcut/      # 快捷键
│   ├── web/           # 网页快捷方式
│   ├── media/         # 媒体控制
│   ├── bilibili/      # B站控制
│   ├── countdown/     # 倒计时
│   ├── pomodoro/      # 番茄钟
│   ├── photo/         # 照片查看器（仅16MB，需 SUPPORT_PHOTO）
│   └── settings/      # 网页配置
├── app/               # 核心应用模块
│   ├── app_key.cpp/h  # 按键/编码器输入处理
│   ├── app_led.cpp/h  # WS2812 LED 控制
│   ├── app_audio.cpp/h # 音频播放
│   ├── app_settings.cpp/h # 配置管理
│   ├── app_mijia.cpp/h # 米家协议
│   └── ...
└── fonts/             # 自定义字体数据（U8g2格式）
```

### 全局对象（定义在 board_def.cpp）

- `gfx[3]`: 3个显示对象的数组（Arduino_GFX）
- `bleKeyboard`: 蓝牙 HID 键盘
- `button1-4`: 物理按键（Button2库）
- `myEnc`: 旋转编码器
- `ch423`: 用于米家模块的 I2C 扩展器
- `server`: 用于配置的 AsyncWebServer

### 配置系统

配置以 JSON 文件形式存储在 LittleFS 中：
- `/config_weather.json`: 天气API设置
- `/config_encoder.json`: 旋转编码器设置
- `/config_stocks.json`: 股票代码
- `/config_mijia.json`: 米家开关映射
- `/config_web.json`: 网页快捷方式
- `/config_clock.json`: 时钟主题设置
- `/config_photo.json`: 照片轮播设置（仅16MB）

## 显示系统

三个显示屏通过独立的 CS 引脚寻址：
- `gfx[0]` (gfx1): CS1 (引脚 39) - 左侧显示屏
- `gfx[1]` (gfx2): CS2 (引脚 38) - 中间显示屏
- `gfx[2]` (gfx3): CS3 (引脚 47) - 右侧显示屏

显示分辨率: 128x128 像素（RGB565 颜色）

## 输入系统

- **旋转编码器**: 导航（引脚: EC_A=4, EC_B=7）
- **4个按键**: KEY1-4（引脚: 1, 48, 0, 9）
- **按键事件**: DOWN, UP, SHORT, DOUBLE, LONG（通过 `app_key.cpp` 处理）

## 网页配置

设备在 `http://<设备IP>/` 托管网页界面，支持：
- 天气API配置（和风天气）
- 股票代码管理
- 米家开关映射
- 网页快捷方式
- 照片上传（仅16MB）
- 编码器设置

OTA更新可通过 `http://<设备IP>/update` 访问（启动时按住右键进入）

## 固件发布

运行 `python scripts/release.py` 执行构建、文件系统生成和合并，输出到上级 `Firmware/` 目录。
每个环境生成三个文件：

```
Firmware/
├── TripleKey_NV3023_16MB_V1.3.0.bin        # 完整固件（整包烧录）
├── TripleKey_NV3023_16MB_V1.3.0_ota.bin    # OTA 固件（仅 app 分区）
├── TripleKey_NV3023_16MB_V1.3.0_fs.bin     # 文件系统镜像（仅 littlefs 分区）
└── ...
```

完整固件由 esptool merge_bin 按分区表地址拼接：bootloader(0x0) + partitions(0x8000) + boot_app0(0xe000) + firmware(0x10000) + littlefs(0x410000)。

单独合并工具 `scripts/merge_firmware.py` 可在不重新构建的情况下合并已有产物，输出到 `Software/temp_firmware/`。
