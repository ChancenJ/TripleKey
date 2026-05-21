"""
TripleKey 固件合并脚本
将 bootloader、partitions、boot_app0、firmware、littlefs 合并为完整固件

使用方法:
  python scripts/merge_firmware.py                 # 合并默认环境
  python scripts/merge_firmware.py -e NV3023_8MB   # 合并指定环境
  python scripts/merge_firmware.py -e NV3023_8MB -e NV3023_16MB  # 合并多个环境
  python scripts/merge_firmware.py --all            # 合并全部环境
"""
import argparse
import glob
import os
import shutil
import subprocess
import sys

# ========== 路径常量 ==========
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(SCRIPT_DIR)                       # Software/
BUILD_DIR = os.path.join(PROJECT_DIR, ".pio", "build")          # PlatformIO 构建输出目录
TEMP_DIR = os.path.join(SCRIPT_DIR, "temp")                     # 临时文件目录，存放各环境合并用的 bin
OUTPUT_DIR = os.path.join(PROJECT_DIR, "temp_firmware")               # 最终合并固件输出目录

BOOT_APP0_SRC = os.path.join(PROJECT_DIR, "build_notes", "bin", "boot_app0.bin")  # OTA 初始数据，非构建产物

# ========== 环境配置 ==========
# flash_size: 由硬件 flash 容量决定，传给 esptool 的 --flash_size 参数
ENVS = {
    "NV3023_16MB":  {"flash_size": "16MB"},
    "NV3023_8MB":   {"flash_size": "8MB"},
    "GC9107_16MB":  {"flash_size": "16MB"},
    "GC9107_8MB":   {"flash_size": "8MB"},
}

# ========== 固件组件及其 flash 地址 ==========
# 地址来源: custom_8MB.csv / custom_16MB.csv 分区表
#   0x0      - bootloader (ESP32-S3 ROM 固定)
#   0x8000   - partition table (ESP-IDF 固定)
#   0xe000   - otadata 分区 (boot_app0.bin)
#   0x10000  - app0 分区 (firmware.bin)
#   0x410000 - spiffs 分区 (littlefs.bin)，两个分区表中地址一致
COMPONENTS = [
    ("0x0",      "bootloader.bin"),
    ("0x8000",   "partitions.bin"),
    ("0xe000",   "boot_app0.bin"),
    ("0x10000",  "firmware.bin"),
    ("0x410000", "littlefs.bin"),
]


def find_pio_python():
    """查找 PlatformIO 自带的 Python 解释器路径。
    需要用 PlatformIO 的 Python 而非系统 Python，因为它自带 pyserial 等依赖。
    优先使用 penv 虚拟环境中的 Python，其次用 python3 目录下的。"""
    pio_home = os.path.join(os.path.expanduser("~"), ".platformio")
    for rel in ("penv/Scripts/python.exe", "penv/bin/python", "python3/python.exe", "python3/python"):
        p = os.path.join(pio_home, rel)
        if os.path.exists(p):
            return p
    return None


def find_esptool():
    """查找 PlatformIO 安装的 esptool.py 脚本路径。
    esptool 位于 ~/.platformio/packages/tool-esptoolpy*/ 目录下。"""
    pattern = os.path.join(os.path.expanduser("~"), ".platformio", "packages", "tool-esptoolpy*", "esptool.py")
    matches = glob.glob(pattern)
    if matches:
        return matches[0]
    return None


def get_version():
    """从项目根目录的 VERSION 文件读取版本号，用于输出文件命名。"""
    try:
        with open(os.path.join(PROJECT_DIR, "VERSION"), "r") as f:
            return f.read().strip()
    except FileNotFoundError:
        return "unknown"


def get_default_env():
    """从 platformio.ini 读取 default_envs 配置，作为未指定环境时的默认值。"""
    ini_path = os.path.join(PROJECT_DIR, "platformio.ini")
    try:
        with open(ini_path, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line.startswith("default_envs"):
                    return line.split("=")[1].strip()
    except FileNotFoundError:
        pass
    return "NV3023_16MB"


def prepare_temp(env):
    """为指定环境准备合并所需的 bin 文件。
    将 .pio/build/{env}/ 中的构建产物和 build_notes/bin/boot_app0.bin
    复制到 scripts/temp/{env}/ 目录，供 esptool merge_bin 使用。"""
    temp_env = os.path.join(TEMP_DIR, env)
    # 清空旧的临时目录，确保干净
    if os.path.exists(temp_env):
        shutil.rmtree(temp_env)
    os.makedirs(temp_env, exist_ok=True)

    build_env = os.path.join(BUILD_DIR, env)
    for _, fname in COMPONENTS:
        # boot_app0.bin 来自 build_notes，其余来自构建输出
        if fname == "boot_app0.bin":
            src = BOOT_APP0_SRC
        else:
            src = os.path.join(build_env, fname)
        if not os.path.exists(src):
            print(f"  [ERROR] 缺少 {fname}，请先运行: pio run -e {env}")
            return None
        shutil.copy2(src, os.path.join(temp_env, fname))

    return temp_env


def merge_env(env, pio_python, esptool):
    """合并单个环境的全部 bin 为一个完整固件。
    流程: 准备临时文件 -> 调用 esptool merge_bin -> 输出到 temp_firmware/ 目录。"""
    cfg = ENVS[env]

    # 阶段 1: 复制 bin 到临时目录
    print(f"\n[{env}] 准备文件...")
    temp_env = prepare_temp(env)
    if temp_env is None:
        return False

    # 阶段 2: 构建 esptool merge_bin 命令
    # esptool merge_bin 会按地址偏移将多个 bin 拼接为一个完整 flash 镜像
    cmd = [
        pio_python, esptool,
        "--chip", "esp32s3",
        "merge_bin",
        "--flash_mode", "qio",              # 板子定义 esp32-s3-devkitc-1.json 中的 flash_mode
        "--flash_size", cfg["flash_size"],   # 8MB 或 16MB，由环境 ENVS 配置决定
        "--flash_freq", "80m",               # 板子定义中的 f_flash: 80000000L
        "-o", os.path.join(temp_env, "merged.bin"),
    ]
    # 按 COMPONENTS 中定义的地址顺序拼接各 bin
    for addr, fname in COMPONENTS:
        cmd += [addr, os.path.join(temp_env, fname)]

    # 阶段 3: 执行合并
    print(f"[{env}] 合并中 (flash_size={cfg['flash_size']})...")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  [ERROR] esptool 失败:\n{result.stderr}")
        return False

    # 阶段 4: 复制合并结果到输出目录并重命名
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    version = get_version()
    out_name = f"TripleKey_{env}_V{version}_merged.bin"
    dst = os.path.join(OUTPUT_DIR, out_name)
    shutil.copy2(os.path.join(temp_env, "merged.bin"), dst)
    size_mb = os.path.getsize(dst) / (1024 * 1024)
    print(f"[{env}] 完成: {dst} ({size_mb:.1f} MB)")
    return True


def main():
    # 阶段 1: 解析命令行参数，确定要合并的环境列表
    parser = argparse.ArgumentParser(description="TripleKey 固件合并工具")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-e", "--env", action="append", dest="envs",
                       help="指定要合并的环境，可多次使用")
    group.add_argument("--all", action="store_true", help="合并全部环境")
    args = parser.parse_args()

    if args.all:
        targets = list(ENVS.keys())
    elif args.envs:
        for e in args.envs:
            if e not in ENVS:
                print(f"未知环境: {e}")
                print(f"可选: {', '.join(ENVS.keys())}")
                sys.exit(1)
        targets = args.envs
    else:
        targets = [get_default_env()]

    # 阶段 2: 定位 PlatformIO Python 和 esptool
    pio_python = find_pio_python()
    if pio_python is None:
        print("[ERROR] 未找到 PlatformIO Python，请先运行: pio run")
        sys.exit(1)

    esptool = find_esptool()
    if esptool is None:
        print("[ERROR] 未找到 esptool.py，请先运行: pio run")
        sys.exit(1)

    # 阶段 3: 逐个环境执行合并
    print(f"目标环境: {', '.join(targets)}")

    ok, fail = 0, 0
    for env in targets:
        if merge_env(env, pio_python, esptool):
            ok += 1
        else:
            fail += 1

    # 阶段 4: 清理临时目录
    if os.path.exists(TEMP_DIR):
        shutil.rmtree(TEMP_DIR)

    print(f"\n完成: {ok} 成功, {fail} 失败")


if __name__ == "__main__":
    main()
