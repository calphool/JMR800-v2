import subprocess
import time
import os
import sys

MAX_RETRIES = 10
RETRY_DELAY_SECONDS = 5

firmware_path = sys.argv[1]
mcu_type = "TEENSY41"

def upload_with_retry():
    command = [
        "teensy_loader_cli",
        f"--mcu={mcu_type}",
        "-w",  # wait for device
        "-s",  # soft reboot
        "-v",  # verbose
        firmware_path
    ]

    for attempt in range(1, MAX_RETRIES + 1):
        print(f"\nAttempt {attempt} of {MAX_RETRIES}: Uploading firmware...\n")
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

        print(result.stdout)

        if result.returncode == 0:
            print("Upload succeeded.")
            return
        else:
            print(f"Upload failed (attempt {attempt}).")
            if attempt < MAX_RETRIES:
                print(f"Retrying in {RETRY_DELAY_SECONDS} seconds...")
                time.sleep(RETRY_DELAY_SECONDS)
            else:
                print("All upload attempts failed.")
                raise Exception("Firmware upload failed after multiple attempts.")

upload_with_retry()
