#!/usr/bin/env python3
"""
Linear 12-bit Brute Force Generator for Australian 433.92 MHz Roller Doors
Flipper Zero Sub-GHz Playlist Compatible

Generates individual .sub files for each Linear 12-bit code (0x000-0xFFF)
and a playlist file to cycle through them on the Flipper.

Linear protocol: 12-bit fixed code, TE ~500us, OOK modulation
Common on older AU/NZ roller doors and garage door openers (Linear/Multi-Code)
Total code space: 4096 codes (2^12)

Usage:
    python linear_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/linear_bf/
    python linear_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/linear_bf/ --range 0x000 0x1FF
    python linear_bruteforce_gen.py --playlist-only --output-dir ../../car_hacks_au/gates/linear_bf/
"""

import argparse
import os
from pathlib import Path


LINEAR_BIT = 12
LINEAR_TE = 500
LINEAR_FREQ = 433920000
LINEAR_PRESET = "FuriHalSubGhzPresetOok650Async"


def int_to_linear_key(code: int) -> str:
    """Convert a 12-bit Linear code to 8-byte hex key format."""
    if code < 0 or code > 0xFFF:
        raise ValueError(f"Linear code must be 0x000-0xFFF, got 0x{code:03X}")
    # 12-bit code right-aligned in 8 bytes
    return f"00 00 00 00 00 00 {(code >> 4) & 0xFF:02X} {(code & 0xF) << 4 | ((code >> 8) & 0xF):02X}"


def generate_linear_sub(code: int, output_path: Path):
    """Generate a single Linear .sub Key File."""
    key_hex = int_to_linear_key(code)
    content = f"""Filetype: Flipper SubGhz Key File
Version: 1
Frequency: {LINEAR_FREQ}
Preset: {LINEAR_PRESET}
Protocol: Linear
Bit: {LINEAR_BIT}
Key: {key_hex}
TE: {LINEAR_TE}
"""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")


def generate_playlist(output_dir: Path, start: int, end: int):
    """Generate a playlist .txt pointing to all generated .sub files."""
    playlist_path = output_dir / "linear_bruteforce_playlist.txt"
    lines = [
        "# Linear 12-bit Brute Force Playlist",
        f"# Code range: 0x{start:03X} - 0x{end:03X}",
        f"# Total files: {end - start + 1}",
        f"# Frequency: {LINEAR_FREQ} (433.92 MHz AU)",
        f"# Protocol: Linear, Bit: {LINEAR_BIT}, TE: {LINEAR_TE}",
        "#",
    ]
    for code in range(start, end + 1):
        sd_path = f"/ext/subghz/car_hacks_au/gates/linear_bf/linear_{code:03X}.sub"
        lines.append(sd_path)

    playlist_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[+] Playlist -> {playlist_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Linear 12-bit Brute Force Generator for AU 433.92 MHz"
    )
    parser.add_argument(
        "--output-dir", type=str, default="linear_bf",
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
        print(f"[*] Generating {total} Linear .sub files (0x{start:03X} - 0x{end:03X})...")
        print(f"[*] Output: {output_dir.resolve()}")

        for code in range(start, end + 1):
            # Batch into subdirectories of batch_size
            batch = code // args.batch_size
            batch_dir = output_dir / f"batch_{batch:02X}"
            filename = f"linear_{code:03X}.sub"
            generate_linear_sub(code, batch_dir / filename)

            if (code - start + 1) % 500 == 0:
                print(f"  ... {code - start + 1}/{total} files generated")

        print(f"[+] Generated {total} .sub files")

    generate_playlist(output_dir, start, end)


if __name__ == "__main__":
    main()
