Import("env")
import os

firmware_path = env.get("BUILD_DIR") + "/firmware.hex"
cmd = f"teensy_loader_cli --mcu=imxrt1062 -w -s -v {firmware_path}"

env.Replace(UPLOADCMD=cmd)
