"""
TripleKey 固件发布脚本
从 PlatformIO 构建输出中复制固件并重命名

使用方法:
  1. 先构建所有环境:  pio run
  2. 运行发布脚本:    python release.py
"""
import shutil
import os
from datetime import datetime

ENVS = ["NV3023_16MB", "NV3023_8MB", "GC9107_16MB", "GC9107_8MB"]
BUILD_DIR = ".pio/build"
FIRMWARE_DIR = "Firmware"

def get_version():
    try:
        with open("VERSION", "r") as f:
            return f.read().strip()
    except FileNotFoundError:
        print("警告: VERSION 文件未找到，使用 'unknown'")
        return "unknown"

def release():
    version = get_version()
    date = datetime.now().strftime("%Y%m%d")
    os.makedirs(FIRMWARE_DIR, exist_ok=True)

    for env in ENVS:
        src = f"{BUILD_DIR}/{env}/firmware.bin"
        if os.path.exists(src):
            dst = f"{FIRMWARE_DIR}/TripleKey_{env}_V{version}_{date}.bin"
            shutil.copy2(src, dst)
            size_kb = os.path.getsize(dst) / 1024
            print(f"已生成: {dst} ({size_kb:.1f} KB)")
        else:
            print(f"跳过: 未找到 {env} 的固件，请先运行 pio run -e {env}")

if __name__ == "__main__":
    release()
