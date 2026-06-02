#!/usr/bin/env python3
"""
Jesse's Capture Helper - Practical organiser for YOUR verified Sub-GHz captures.
flipper-zero-au-subghz

genuinely, the number of people who end up with 47 files named "new.sub", "capture1.sub",
and "flipper (2).sub" on their desktop is criminal. This script fixes that.

Features:
- Rename with brand/year/button/date (and optional distance/preset notes)
- Move into the correct car_hacks_au/<brand>/ folder (creates subdir if missing)
- Basic .sub header validation (Filetype, Frequency, Preset etc.)
- Generate ready-to-paste Sub-GHz Playlist entries
- Dry-run mode so you can see exactly what will happen before it touches anything

This script is file hygiene only. It does NOT transmit, does NOT replay, does NOT
generate codes, does NOT do anything illegal. It just helps you keep your own
captures organised like a pro.

Jesse rules:- Only ever run this on captures you made yourself from hardware YOU OWN.
- Always verify replay works on the actual vehicle before trusting the file.
- If the header validation screams at you, go back and re-capture properly.

Usage examples:
    python scripts/capture_helper.py --input "my_hilux_lock.sub" --brand toyota --year 2012 --button lock
    python scripts/capture_helper.py --batch-dir "today_captures/" --brand ford_ranger --year 2021 --button unlock --dry-run
    python scripts/capture_helper.py --input "gate_ata.sub" --brand gates --year 2020 --button open --date 2026-04-15

you will be fine if you use it properly on your own gear.
"""

import argparse
import os
import re
import shutil
from datetime import datetime
from pathlib import Path
from typing import Optional, List, Dict, Tuple


# ============================================================================
# JESSE'S .sub VALIDATION
# ============================================================================
# We only check the basics that every real Flipper SubGhz RAW file must have.
# This catches the most common mistakes (empty files, wrong exports, text notes
# renamed to .sub, etc.). It is NOT a full protocol parser.

REQUIRED_HEADER_MARKERS = [
    "Filetype: Flipper SubGhz",
    "Version:",
]

RECOMMENDED_MARKERS = [
    "Frequency:",
    "Protocol:",
    "Preset:",
]


def validate_sub_file(filepath: Path, strict: bool = False) -> Tuple[bool, List[str]]:
    """
    Jesse's quick and dirty .sub header validator.
    Returns (is_valid, list_of_warnings_or_errors)

    Looks for the minimum markers a real capture from the Flipper must have.
    Does not require RAW_Data (you might be organising a template first).
    """
    warnings: List[str] = []
    errors: List[str] = []

    if not filepath.exists():
        return False, [f"File does not exist: {filepath}"]

    if filepath.suffix.lower() != ".sub":
        warnings.append("File does not end in .sub — are you sure this is a Flipper capture?")

    try:
        content = filepath.read_text(encoding="utf-8", errors="ignore")
        lines = [line.strip() for line in content.splitlines() if line.strip()]
    except Exception as e:
        return False, [f"Failed to read file: {e}"]

    if not lines:
        return False, ["File is completely empty. This is not a valid .sub."]

    # Check core Filetype marker (case-insensitive enough for our purposes)
    has_filetype = any("filetype: flipper subghz" in line.lower() for line in lines)
    if not has_filetype:
        errors.append("Missing 'Filetype: Flipper SubGhz RAW File' header. This does not look like a real Flipper .sub capture.")

    # Check for Frequency line
    has_freq = any(line.lower().startswith("frequency:") for line in lines)
    if not has_freq:
        warnings.append("No 'Frequency:' line found. Real captures should declare the exact Hz used (e.g. 433920000).")

    # Check for Preset
    has_preset = any(line.lower().startswith("preset:") for line in lines)
    if not has_preset:
        warnings.append("No 'Preset:' line found. You should know whether this was OOK650 or 2FSKDev476 etc.")

    # Optional but common sanity: look for RAW_Data or at least some data hint
    has_raw = any("raw_data" in line.lower() for line in lines)
    if not has_raw:
        warnings.append("No RAW_Data line visible. If this is supposed to be a finished capture, you may have an empty template or truncated file.")

    # Frequency sanity for AU work (warn only, don't block)
    freq_match = re.search(r"Frequency:\s*(\d+)", content, re.IGNORECASE)
    if freq_match:
        freq = int(freq_match.group(1))
        if not (433000000 <= freq <= 435000000):
            warnings.append(f"Frequency {freq} Hz looks outside the normal AU 433.92 MHz ballpark. Double-check this was captured on the right band.")

    all_issues = warnings + errors
    is_valid = len(errors) == 0

    if strict and not is_valid:
        return False, all_issues

    return is_valid, all_issues


# ============================================================================
# NAMING + ORGANISING LOGIC
# ============================================================================

def build_new_filename(
    original: Path,
    brand: str,
    year: Optional[str],
    button: Optional[str],
    date_str: Optional[str],
    distance: Optional[str] = None,
    preset: Optional[str] = None,
) -> str:
    """
    Jesse's recommended naming scheme for captures.
    Example outputs:
        toyota_2012_lock_2026-05-31.sub
        ford_ranger_2021_unlock_8m_2fskdev476_2026-05-30.sub
        gates_ata_2019_open_2026-04-12.sub

    Keeps it sortable, searchable, and obvious at a glance what the file contains.
    """
    parts = [brand.lower().replace(" ", "_")]

    if year:
        parts.append(str(year))

    if button:
        parts.append(button.lower().replace(" ", "_"))

    if distance:
        parts.append(f"{distance}m".replace(" ", ""))

    if preset:
        clean_preset = preset.lower().replace(" ", "").replace("/", "")
        parts.append(clean_preset)

    if date_str:
        parts.append(date_str)
    else:
        today = datetime.now().strftime("%Y-%m-%d")
        parts.append(today)

    # Preserve some hint of original if it had useful info, but keep clean
    new_name = "_".join(parts) + ".sub"
    # Remove any double underscores that might sneak in
    new_name = re.sub(r"_+", "_", new_name)
    return new_name


def get_brand_folder(brand: str) -> str:
    """Map common brand inputs to the actual folder names used in car_hacks_au/"""
    brand = brand.lower().strip()
    mapping = {
        "toyota": "toyota",
        "ford": "ford_ranger",
        "ford_ranger": "ford_ranger",
        "ranger": "ford_ranger",
        "holden": "holden_commodore",
        "commodore": "holden_commodore",
        "holden_commodore": "holden_commodore",
        "mazda": "mazda",
        "hyundai": "hyundai_kia",
        "kia": "hyundai_kia",
        "hyundai_kia": "hyundai_kia",
        "mitsubishi": "mitsubishi",
        "triton": "mitsubishi",
        "nissan": "nissan",
        "navara": "nissan",
        "subaru": "subaru",
        "isuzu": "isuzu_dmax",
        "dmax": "isuzu_dmax",
        "isuzu_dmax": "isuzu_dmax",
        "vw": "vw_amarok",
        "amarok": "vw_amarok",
        "vw_amarok": "vw_amarok",
        "gate": "gates",
        "gates": "gates",
        "garage": "gates",
    }
    return mapping.get(brand, brand)  # fallback to what user gave


def extract_preset_from_sub(filepath: Path) -> Optional[str]:
    """Try to pull the Preset value out of an existing .sub for better naming."""
    try:
        content = filepath.read_text(encoding="utf-8", errors="ignore")
        match = re.search(r"Preset:\s*([A-Za-z0-9_]+)", content, re.IGNORECASE)
        if match:
            return match.group(1)
    except Exception:
        pass
    return None


# ============================================================================
# MAIN WORKER
# ============================================================================

def process_capture(
    input_path: Path,
    base_output_dir: Path,
    brand: str,
    year: Optional[str],
    button: Optional[str],
    date_str: Optional[str],
    distance: Optional[str],
    dry_run: bool = False,
    strict_validation: bool = False,
) -> Optional[Dict]:
    """
    Does the actual validation, rename, move, and playlist line generation for one file.

    Returns a dict with results or None on fatal failure.
    """
    print(f"\n[Jesse] Processing: {input_path}")
    is_valid, issues = validate_sub_file(input_path, strict=strict_validation)

    for issue in issues:
        prefix = "ERROR" if "Missing" in issue or "empty" in issue.lower() or "does not exist" in issue else "WARNING"
        print(f"  [{prefix}] {issue}")

    if not is_valid and strict_validation:
        print("  [Jesse] Strict mode: refusing to touch this file until you fix the header. Re-capture it properly, mate.")        return None

    # Try to auto-detect preset for naming if user didn't supply one
    detected_preset = extract_preset_from_sub(input_path)

    target_folder_name = get_brand_folder(brand)
    target_dir = base_output_dir / target_folder_name
    target_dir.mkdir(parents=True, exist_ok=True)

    new_filename = build_new_filename(
        original=input_path,
        brand=target_folder_name,
        year=year,
        button=button,
        date_str=date_str,
        distance=distance,
        preset=detected_preset,
    )

    dest_path = target_dir / new_filename

    if dest_path.exists():
        print(f"  [WARNING] Destination already exists: {dest_path}")
        print("  [Jesse] Not overwriting. Rename the existing one first or use a more specific --button/--date.")        return None

    playlist_entry = f"/ext/subghz/car_hacks_au/{target_folder_name}/{new_filename}"

    result = {
        "original": str(input_path),
        "new_name": new_filename,
        "destination": str(dest_path),
        "brand_folder": target_folder_name,
        "playlist_entry": playlist_entry,
        "validated": is_valid,
    }

    if dry_run:
        print(f"  [DRY-RUN] Would rename/move -> {new_filename}")
        print(f"  [DRY-RUN] Would land in       -> {dest_path}")
        print(f"  [DRY-RUN] Playlist line: {playlist_entry}")
        return result

    # Real move
    try:
        shutil.move(str(input_path), str(dest_path))
        print(f"  [+] Moved to: {dest_path}")
        print(f"  [Jesse] Nice. One clean capture organised.")    except Exception as e:
        print(f"  [ERROR] Move failed: {e}")
        return None

    return result


def main():
    parser = argparse.ArgumentParser(
        description="Jesse's Capture Helper - Rename, validate, and organise your own Flipper Sub-GHz captures into car_hacks_au/",
        epilog="Only use on captures from hardware you legally own. This tool touches files only. No radio unverified claims."    )
    parser.add_argument("--input", type=str, help="Single .sub file to process")
    parser.add_argument("--batch-dir", type=str, help="Directory of .sub files to process in batch")
    parser.add_argument("--brand", type=str, required=True, help="Brand key (toyota, ford_ranger, hyundai_kia, gates, etc.)")
    parser.add_argument("--year", type=str, default=None, help="Model year (e.g. 2012, 2021)")
    parser.add_argument("--button", type=str, default=None, help="Button/action (lock, unlock, boot, open, etc.)")
    parser.add_argument("--date", type=str, default=None, help="Date string for filename (YYYY-MM-DD). Defaults to today.")
    parser.add_argument("--distance", type=str, default=None, help="Distance in metres (e.g. 10) for naming")
    parser.add_argument("--output-dir", type=str, default="car_hacks_au", help="Base output directory (default: car_hacks_au)")
    parser.add_argument("--dry-run", action="store_true", help="Show what would happen without moving anything")
    parser.add_argument("--strict", action="store_true", help="Fail hard on validation warnings (recommended for batch)")
    parser.add_argument("--playlist-file", type=str, default=None, help="Optional: append generated playlist entries to this file")

    args = parser.parse_args()

    if not args.input and not args.batch_dir:
        parser.error("You must supply either --input <file.sub> or --batch-dir <dir/>")

    base_output = Path(args.output_dir).resolve()
    base_output.mkdir(parents=True, exist_ok=True)

    files_to_process: List[Path] = []

    if args.input:
        p = Path(args.input).resolve()
        if not p.exists():
            print(f"[Jesse] ERROR: Input file not found: {p}")            return
        files_to_process.append(p)

    if args.batch_dir:
        d = Path(args.batch_dir).resolve()
        if not d.is_dir():
            print(f"[Jesse] ERROR: Batch dir not found or not a directory: {d}")
            return
        sub_files = sorted(d.glob("*.sub"))
        if not sub_files:
            print(f"[Jesse] No .sub files found in {d}")
            return
        print(f"[Jesse] Found {len(sub_files)} .sub files in batch dir.")        files_to_process.extend(sub_files)

    results = []
    for f in files_to_process:
        res = process_capture(
            input_path=f,
            base_output_dir=base_output,
            brand=args.brand,
            year=args.year,
            button=args.button,
            date_str=args.date,
            distance=args.distance,
            dry_run=args.dry_run,
            strict_validation=args.strict,
        )
        if res:
            results.append(res)

    # Summary + optional playlist file append
    if results:
        print("\n" + "=" * 60)
        print("[Jesse] SUMMARY — Playlist entries ready to copy:")        print("=" * 60)
        for r in results:
            print(r["playlist_entry"])

        if args.playlist_file:
            pl_path = Path(args.playlist_file)
            pl_path.parent.mkdir(parents=True, exist_ok=True)
            with open(pl_path, "a", encoding="utf-8") as plf:
                plf.write(f"\n# Added by capture_helper.py on {datetime.now().isoformat()}\n")
                for r in results:
                    plf.write(r["playlist_entry"] + "\n")
            print(f"\n[+] Appended {len(results)} entries to {pl_path}")

        print("\n[Jesse] All done, legend. Now go edit your actual playlist .txt and test replay on your own car.")
        print("Remember: if it didn't work on the vehicle you captured it from, the file is not ready for anything else.")
    else:
        print("\n[Jesse] Nothing was processed successfully. Fix the issues above and try again.")

if __name__ == "__main__":
    main()

