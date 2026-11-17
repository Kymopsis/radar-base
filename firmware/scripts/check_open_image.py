"""Build gate for the OPEN firmware: fail if the image carries anything of the dev-kit radar-flash module.

usage: check_open_image.py <app.elf> <app.bin> [private_loader.bin]

Checks (any hit = exit 1):
  1. radar flash-loader content in the .bin (same rules as check_no_loader.py, incl. the private loader bytes
     when the file is given);
  2. dev-kit module symbols in the .elf symbol table (functions that back up / write the radar's flash);
  3. dev-kit strings in the .bin (sealed-loader NVS namespace and blob tag, key label, loader console);
  4. a partition table next to the image with any partition other than the open layout.
Runs at the end of every open build (top-level CMakeLists) and in the public export audit.
"""
import struct
import subprocess
import sys
from pathlib import Path

DEVKIT_SYMBOLS = [
    "fw_job_flash", "fw_job_backup", "fw_job_batch", "fw_batch_stop", "fw_batch_json", "fw_get_loader",
    "fw_loader_command", "backup_from_loader", "start_loader", "module_present", "decrypt_blob",
]
DEVKIT_STRINGS = [b"kymldr", b"KYL1", b"Kymopsis radar loader v1", b"ldr: "]   # the shared web app names the batch log
OPEN_PARTITIONS = {"nvs", "phy_init", "factory", "storage"}


def elf_symbol_hits(elf):
    data = elf.read_bytes()
    return [s for s in DEVKIT_SYMBOLS if b"\x00" + s.encode() + b"\x00" in data]


def partition_hits(bin_path):
    pt = bin_path.parent / "partition_table" / "partition-table.bin"
    if not pt.exists():
        return []
    raw, hits = pt.read_bytes(), []
    for off in range(0, len(raw) - 31, 32):
        e = raw[off:off + 32]
        if e[:2] != b"\xaa\x50":
            continue
        name = e[12:28].split(b"\x00")[0].decode(errors="replace")
        if name not in OPEN_PARTITIONS:
            hits.append(f"partition '{name}'")
    return hits


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    elf, binf = Path(sys.argv[1]), Path(sys.argv[2])
    loader = sys.argv[3] if len(sys.argv) > 3 else ""
    hits = []
    r = subprocess.run([sys.executable, str(Path(__file__).with_name("check_no_loader.py")), str(binf)]
                       + ([loader] if loader else []), capture_output=True, text=True)
    if r.returncode:
        hits.append(r.stdout.strip())
    hits += [f"symbol {s}" for s in elf_symbol_hits(elf)]
    img = binf.read_bytes()
    hits += [f"string {s.decode()!r}" for s in DEVKIT_STRINGS if s in img]
    hits += partition_hits(binf)
    if hits:
        print(f"check_open_image: FAIL, {binf.name} is not an open image: {hits[:8]}")
        sys.exit(1)
    print(f"check_open_image: OK, {binf.name} has no dev-kit module content")


if __name__ == "__main__":
    main()
