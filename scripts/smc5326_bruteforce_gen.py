#!/usr/bin/env python3
"""
SMC5326 25-bit Brute Force Generator for Australian 433.92 MHz Gates
Flipper Zero Sub-GHz Playlist Compatible

Generates individual .sub files for SMC5326 25-bit codes and a playlist file.

SMC5326 protocol: 25-bit code, TE ~320us, OOK modulation
Similar to PT2260 but with 25-bit payload (extra address bit)
Common on generic Chinese gate remotes, older Centurion clones
Default range: 0x0000000 - 0x00FFFFF (full 25-bit space = 33,554,432 codes)
Recommended: use --range for practical subsets

Usage:
    python smc5326_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/smc5326_bf/
    python smc5326_bruteforce_gen.py --range 0x0000000 0x000FFFF
    python smc5326_bruteforce_gen.py --playlist-only --output-dir ../../car_hacks_au/gates/smc5326_bf/
"""

import argparse
import os
from pathlib import Path


SMC5326_BIT = 25
SMC5326_TE = 320
SMC5326_FREQ = 433920000
SMC5326_PRESET = "FuriHalSubGhzPresetOok650Async"


def int_to_smc5326_key(code: int) -> str:
    """Convert a 25-bit SMC5326 code to 8-byte hex key format."""
    if code < 0 or code > 0x1FFFFFF:
        raise ValueError(f"SMC5326 25-bit code must be 0x0000000-0x1FFFFFF, got 0x{code:07X}")
    # 25-bit code right-aligned in 8 bytes (last 3 bytes + top bit of byte 5)
    # Actually: store as 24-bit-ish in last 3 bytes, with bit 24 in byte 5
    # Simpler: just use the last 4 bytes, 25-bit value right-aligned
    b3 = (code >> 24) & 0x01  # bit 24 only
    b2 = (code >> 16) & 0xFF
    b1 = (code >> 8) & 0xFF
    b0 = code & 0xFF
    return f"00 00 00 00 {b3:02X} {b2:02X} {b1:02X} {b0:02X}"


def generate_smc5326_sub(code: int, output_path: Path):
    """Generate a single SMC5326 .sub Key File."""
    key_hex = int_to_smc5326_key(code)
    content = f"""Filetype: Flipper SubGhz Key File
Version: 1
Frequency: {SMC5326_FREQ}
Preset: {SMC5326_PRESET}
Protocol: SMC5326
Bit: {SMC5326_BIT}
Key: {key_hex}
TE: {SMC5326_TE}
"""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")


def generate_playlist(output_dir: Path, start: int, end: int):
    """Generate a playlist .txt pointing to all generated .sub files."""
    playlist_path = output_dir / "smc5326_bruteforce_playlist.txt"
    lines = [
        "# SMC5326 25-bit Brute Force Playlist",
        f"# Code range: 0x{start:07X} - 0x{end:07X}",
        f"# Total files: {end - start + 1}",
        f"# Frequency: {SMC5326_FREQ} (433.92 MHz AU)",
        f"# Protocol: SMC5326, Bit: {SMC5326_BIT}, TE: {SMC5326_TE}",
        "#",
    ]
    for code in range(start, end + 1):
        sd_path = f"/ext/subghz/car_hacks_au/gates/smc5326_bf/smc5326_{code:07X}.sub"
        lines.append(sd_path)

    playlist_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[+] Playlist -> {playlist_path}")


def main():
    parser = argparse.ArgumentParser(
        description="SMC5326 25-bit Brute Force Generator for AU 433.92 MHz"
    )
    parser.add_argument(
        "--output-dir", type=str, default="smc5326_bf",
        help="Output directory for generated .sub files"
    )
    parser.add_argument(
        "--range", nargs=2, type=lambda x: int(x, 0), default=[0x0000000, 0x000FFFF],
        metavar=("START", "END"),
        help="Code range in hex (default: 0x0000000 0x000FFFF = 65536 codes)"
    )
    parser.add_argument(
        "--playlist-only", action="store_true",
        help="Only generate playlist, skip .sub file generation"
    )
    parser.add_argument(
        "--batch-size", type=int, default=1024,
        help="Files per subdirectory (default: 1024)"
    )

    args = parser.parse_args()
    output_dir = Path(args.output_dir)
    start, end = args.range

    if not args.playlist_only:
        total = end - start + 1
        print(f"[*] Generating {total} SMC5326 .sub files (0x{start:07X} - 0x{end:07X})...")
        print(f"[*] Output: {output_dir.resolve()}")

        for code in range(start, end + 1):
            batch = code // args.batch_size
            batch_dir = output_dir / f"batch_{batch:04X}"
            filename = f"smc5326_{code:07X}.sub"
            generate_smc5326_sub(code, batch_dir / filename)

            if (code - start + 1) % 2000 == 0:
                print(f"  ... {code - start + 1}/{total} files generated")

        print(f"[+] Generated {total} .sub files")

    generate_playlist(output_dir, start, end)


if __name__ == "__main__":
    main()
