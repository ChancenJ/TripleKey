"""
TripleKey 固件发布脚本
构建固件、生成文件系统镜像、合并为完整固件，并输出到发布目录

使用方法:
  python scripts/release.py                          # 默认环境，执行全部（构建+文件系统+合并）
  python scripts/release.py -e NV3023_8MB            # 指定环境，执行全部
  python scripts/release.py -e NV3023_8MB -e GC9107_16MB  # 多个环境
  python scripts/release.py --all-envs               # 全部环境
  python scripts/release.py --firmware               # 仅构建固件
  python scripts/release.py --fs                     # 仅生成文件系统镜像
  python scripts/release.py --merge                  # 仅合并（需先构建）
  python scripts/release.py --firmware --merge       # 构建固件后合并（跳过文件系统）

输出文件（位于上级目录 Firmware/）:
  TripleKey_{env}_V{version}.bin        完整固件（bootloader+partitions+firmware+littlefs）
  TripleKey_{env}_V{version}_ota.bin    OTA 固件（仅 app 分区）
  TripleKey_{env}_V{version}_fs.bin     文件系统镜像（仅 littlefs 分区）
"""
import argparse
import glob
import os
import shutil
import subprocess
import sys

# ========== 路径常量 ==========
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_DIR = os.path.dirname(SCRIPT_DIR)                          # Software/
BUILD_DIR = os.path.join(PROJECT_DIR, ".pio", "build")             # PlatformIO 构建输出目录
OUTPUT_DIR = os.path.join(os.path.dirname(PROJECT_DIR), "Firmware")  # 上一级的 Firmware/ 目录
TEMP_DIR = os.path.join(SCRIPT_DIR, "temp")                        # 合并用临时目录

BOOT_APP0_SRC = os.path.join(PROJECT_DIR, "build_notes", "bin", "boot_app0.bin")

# ========== 环境配置 ==========
ENVS = {
    "NV3023_16MB":  {"flash_size": "16MB"},
    "NV3023_8MB":   {"flash_size": "8MB"},
    "GC9107_16MB":  {"flash_size": "16MB"},
    "GC9107_8MB":   {"flash_size": "8MB"},
}

# ========== 固件组件及其 flash 地址（来自分区表） ==========
COMPONENTS = [
    ("0x0",      "bootloader.bin"),
    ("0x8000",   "partitions.bin"),
    ("0xe000",   "boot_app0.bin"),
    ("0x10000",  "firmware.bin"),
    ("0x410000", "littlefs.bin"),
]


# ========== 工具查找 ==========

def find_pio_python():
    """查找 PlatformIO 自带的 Python 解释器路径。
    需要用 PlatformIO 的 Python 而非系统 Python，因为它自带 pyserial 等依赖。"""
    pio_home = os.path.join(os.path.expanduser("~"), ".platformio")
    for rel in ("penv/Scripts/python.exe", "penv/bin/python", "python3/python.exe", "python3/python"):
        p = os.path.join(pio_home, rel)
        if os.path.exists(p):
            return p
    return None


def find_esptool():
    """查找 PlatformIO 安装的 esptool.py 脚本路径。"""
    pattern = os.path.join(os.path.expanduser("~"), ".platformio", "packages", "tool-esptoolpy*", "esptool.py")
    matches = glob.glob(pattern)
    if matches:
        return matches[0]
    return None


# ========== 通用工具 ==========

def get_version():
    """从 VERSION 文件读取版本号。"""
    try:
        with open(os.path.join(PROJECT_DIR, "VERSION"), "r") as f:
            return f.read().strip()
    except FileNotFoundError:
        return "unknown"


def get_default_env():
    """从 platformio.ini 读取 default_envs 作为默认环境。"""
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


def run_command(cmd, label=""):
    """执行外部命令，输出直接打印到终端，失败返回 False。"""
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print(f"  [ERROR] {label} 失败")
        return False
    return True


def output_name(env, suffix=""):
    """生成输出文件名: TripleKey_{env}_V{version}{suffix}.bin"""
    version = get_version()
    return f"TripleKey_{env}_V{version}{suffix}.bin"


def copy_output(env, src_file, suffix=""):
    """将构建产物复制到输出目录，按规范重命名。"""
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    dst = os.path.join(OUTPUT_DIR, output_name(env, suffix))
    shutil.copy2(src_file, dst)
    size_kb = os.path.getsize(dst) / 1024
    print(f"  -> {dst} ({size_kb:.1f} KB)")
    return dst


# ========== 构建流程 ==========

def build_firmware(env):
    """阶段 1a: 构建固件（pio run -e {env}），生成 firmware.bin、bootloader.bin 等。"""
    print(f"  [{env}] 构建固件...")
    return run_command(["pio", "run", "-e", env], f"{env} 固件构建")


def build_filesystem(env):
    """阶段 1b: 构建文件系统镜像（pio run -e {env} -t buildfs），生成 littlefs.bin。"""
    print(f"  [{env}] 构建文件系统...")
    return run_command(["pio", "run", "-e", env, "-t", "buildfs"], f"{env} 文件系统构建")


def copy_firmware(env):
    """阶段 2a: 将 firmware.bin 复制到输出目录，后缀 _ota。"""
    src = os.path.join(BUILD_DIR, env, "firmware.bin")
    if not os.path.exists(src):
        print(f"  [ERROR] 缺少 firmware.bin，请先构建")
        return False
    copy_output(env, src, "_ota")
    return True


def copy_filesystem(env):
    """阶段 2b: 将 littlefs.bin 复制到输出目录，后缀 _fs。"""
    src = os.path.join(BUILD_DIR, env, "littlefs.bin")
    if not os.path.exists(src):
        print(f"  [ERROR] 缺少 littlefs.bin，请先构建文件系统")
        return False
    copy_output(env, src, "_fs")
    return True


def merge_firmware(env, pio_python, esptool):
    """阶段 3: 合并全部 bin 为完整固件镜像，无后缀。
    将 bootloader、partitions、boot_app0、firmware、littlefs 按地址拼接。"""
    cfg = ENVS[env]

    # 准备临时目录，收集所有 bin
    temp_env = os.path.join(TEMP_DIR, env)
    if os.path.exists(temp_env):
        shutil.rmtree(temp_env)
    os.makedirs(temp_env, exist_ok=True)

    build_env = os.path.join(BUILD_DIR, env)
    for _, fname in COMPONENTS:
        src = BOOT_APP0_SRC if fname == "boot_app0.bin" else os.path.join(build_env, fname)
        if not os.path.exists(src):
            print(f"  [ERROR] 缺少 {fname}，请先构建")
            return False
        shutil.copy2(src, os.path.join(temp_env, fname))

    # 调用 esptool merge_bin 按地址拼接为完整 flash 镜像
    merged = os.path.join(temp_env, "merged.bin")
    cmd = [
        pio_python, esptool,
        "--chip", "esp32s3",
        "merge_bin",
        "--flash_mode", "qio",              # 板子定义中的 flash_mode
        "--flash_size", cfg["flash_size"],   # 8MB 或 16MB
        "--flash_freq", "80m",               # 板子定义中的 f_flash
        "-o", merged,
    ]
    for addr, fname in COMPONENTS:
        cmd += [addr, os.path.join(temp_env, fname)]

    print(f"  [{env}] 合并完整固件 (flash_size={cfg['flash_size']})...")
    if not run_command(cmd, "esptool merge_bin"):
        return False

    copy_output(env, merged)
    return True


# ========== 主流程 ==========

def main():
    # 阶段 1: 解析命令行参数
    parser = argparse.ArgumentParser(description="TripleKey 固件发布工具")
    env_group = parser.add_mutually_exclusive_group()
    env_group.add_argument("-e", "--env", action="append", dest="envs",
                           help="指定环境，可多次使用")
    env_group.add_argument("--all-envs", action="store_true",
                           help="全部环境")

    mode_group = parser.add_argument_group("构建模式（不指定则执行全部）")
    mode_group.add_argument("--firmware", action="store_true", help="构建固件")
    mode_group.add_argument("--fs", action="store_true", help="构建文件系统镜像")
    mode_group.add_argument("--merge", action="store_true", help="合并为完整固件")
    args = parser.parse_args()

    # 确定目标环境
    if args.all_envs:
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

    # 未指定任何模式时，默认执行全部
    do_firmware = args.firmware
    do_fs = args.fs
    do_merge = args.merge
    if not (do_firmware or do_fs or do_merge):
        do_firmware = do_fs = do_merge = True

    # 阶段 2: 定位工具
    pio_python = None
    esptool = None
    if do_merge:
        pio_python = find_pio_python()
        if pio_python is None:
            print("[ERROR] 未找到 PlatformIO Python，请先运行: pio run")
            sys.exit(1)
        esptool = find_esptool()
        if esptool is None:
            print("[ERROR] 未找到 esptool.py，请先运行: pio run")
            sys.exit(1)

    # 阶段 3: 逐环境执行构建和输出
    print(f"目标环境: {', '.join(targets)}")
    print(f"输出目录: {OUTPUT_DIR}")
    modes = []
    if do_firmware: modes.append("固件")
    if do_fs: modes.append("文件系统")
    if do_merge: modes.append("合并")
    print(f"执行模式: {' + '.join(modes)}\n")

    ok, fail = 0, 0
    for env in targets:
        print(f"=== {env} ===")

        # 3a: 构建固件
        if do_firmware:
            if not build_firmware(env):
                fail += 1
                continue
            copy_firmware(env)

        # 3b: 构建文件系统
        if do_fs:
            if not build_filesystem(env):
                fail += 1
                continue
            copy_filesystem(env)

        # 3c: 合并完整固件
        if do_merge:
            if not merge_firmware(env, pio_python, esptool):
                fail += 1
                continue

        ok += 1

        # 清理该环境的临时目录
        temp_env = os.path.join(TEMP_DIR, env)
        if os.path.exists(temp_env):
            shutil.rmtree(temp_env)

    print(f"\n完成: {ok} 成功, {fail} 失败")


if __name__ == "__main__":
    main()
