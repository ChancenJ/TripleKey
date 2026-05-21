"""Pre-build script for 8MB environments: hide data/photo during buildfs/uploadfs."""
Import("env")
import atexit
import os
import shutil
import sys

if "buildfs" not in sys.argv and "uploadfs" not in sys.argv:
    Return()

PROJECT_DIR = env["PROJECT_DIR"]
PHOTO_DIR = os.path.join(PROJECT_DIR, "data", "photo")
TEMP_PHOTO_DIR = os.path.join(PROJECT_DIR, "scripts", "temp", "photo")


def restore_photo():
    if os.path.isdir(TEMP_PHOTO_DIR):
        shutil.move(TEMP_PHOTO_DIR, PHOTO_DIR)
        print(f"[filter_fs] scripts/temp/photo -> data/photo")


if os.path.isdir(PHOTO_DIR):
    os.makedirs(os.path.dirname(TEMP_PHOTO_DIR), exist_ok=True)
    shutil.move(PHOTO_DIR, TEMP_PHOTO_DIR)
    print(f"[filter_fs] data/photo -> scripts/temp/photo")
    atexit.register(restore_photo)
