"""Fail if a firmware image contains any part of the radar flash loader.

usage: check_no_loader.py <image.bin|.elf> [loader.bin]

Run after every OPEN and protected DEV-KIT build (the top-level CMakeLists does it). With the private
loader at hand it looks for every 24-byte window of it (step 8) and for its text strings; without it
(e.g. public CI) it looks for the loader's banner text only.
"""
import re
import sys
from pathlib import Path

MARKERS = [b"[KYM] flash loader", b"xmodem-crc to SPI flash", b"send the app image (with its 16-byte flash header)"]


def strings(b, n=10):
    return set(m.group() for m in re.finditer(rb"[\x20-\x7e]{%d,}" % n, b))


def main():
    img = Path(sys.argv[1]).read_bytes()
    hits = [m.decode() for m in MARKERS if m in img]
    if len(sys.argv) > 2 and Path(sys.argv[2]).exists():
        ldr = Path(sys.argv[2]).read_bytes()
        win = 24
        for i in range(0, len(ldr) - win, 8):
            w = ldr[i:i + win]
            if len(set(w)) > 4 and w in img:   # skip runs of padding / zeros
                hits.append(f"loader bytes at loader offset {i}")
                break
        hits += [s.decode() for s in strings(ldr) if s in img]
    if hits:
        print(f"check_no_loader: FAIL, {sys.argv[1]} contains radar loader content: {hits[:5]}")
        sys.exit(1)
    print(f"check_no_loader: OK, no radar loader content in {Path(sys.argv[1]).name}")


if __name__ == "__main__":
    main()
