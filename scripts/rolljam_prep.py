#!/usr/bin/env python3
"""
JESSE's RollJam Prep Helper

flipper-zero-au-subghz

This script takes TWO or MORE of YOUR OWN .sub RAW captures and spits out a
clean analysis + notes file to help you manually prepare for RollJam-style
work on hardware you own.

WHAT IT DOES:
- Parses real Flipper .sub files (Frequency, Preset, full RAW_Data timings)
- Detects approximate button-press boundaries using large timing gaps
- Calculates basic stats: total duration, pulse count, average on/off times
- Writes a timestamped prep notes file with all the raw timing data laid out
- Gives you suggested "sequence" thinking points for manual analysis

WHAT IT ABSOLUTELY DOES NOT DO:
- Transmit anything
- Jam anything
- Generate attack payloads
- Perform any radio operation whatsoever
- Create "ready to use" magic files

This is PURELY file prep and timing analysis notes. The actual RollJam technique
requires proper hardware setup (usually multiple devices or SDR), precise timing,
and is illegal as fuck in Australia unless done on your own property for
defensive research.

**READ THE REALITY CHECK**:
    docs/rolljam_reality.md

If you haven't read that document three times, close this script and go read it.

Legal reality (repeated for the slow learners):
- Jamming is illegal under the Radiocommunications Act (ACMA).
- Using this on any vehicle/gate you do not own and control = theft/break-enter charges + radio offence.
- "I was just testing" is not a defence when the owner of the Hilux comes back.
- JESSE, the repo maintainers, and every contributor take zero responsibility for you being a muppet.

Only ever use this on captures from cars/gates YOU LEGALLY OWN.

Usage:
    python scripts/rolljam_prep.py --captures capture1.sub capture2.sub capture3.sub --output my_rolljam_notes.txt
    python scripts/rolljam_prep.py --dir my_raw_captures/ --min-gap 80000

The output .txt is your working notes. Print it, scribble on it, use it with
ProtoPirate/ProtoView to dig deeper. Then decide if the technique is even worth
the risk on your particular hardware.

She'll be right — if you stay legal and on your own shit.
"""

import argparse
import re
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Optional, Tuple


# ============================================================================
# JESSE'S RAW .sub PARSER
# ============================================================================

def parse_sub_file(filepath: Path) -> Dict:
    """
    Parse a Flipper .sub RAW file and extract the important bits for timing analysis.

    Returns dict with:
        path, frequency, preset, raw_samples (list of int us durations),
        total_duration_us, pulse_count, button_boundaries (list of indices)
    """
    result = {
        "path": str(filepath),
        "frequency": None,
        "preset": None,
        "raw_samples": [],
        "total_duration_us": 0,
        "pulse_count": 0,
        "button_boundaries": [],
        "warnings": [],
    }

    if not filepath.exists():
        result["warnings"].append("File not found")
        return result

    try:
        content = filepath.read_text(encoding="utf-8", errors="ignore")
    except Exception as e:
        result["warnings"].append(f"Read error: {e}")
        return result

    # Frequency
    freq_match = re.search(r"Frequency:\s*(\d+)", content, re.IGNORECASE)
    if freq_match:
        result["frequency"] = int(freq_match.group(1))

    # Preset
    preset_match = re.search(r"Preset:\s*([A-Za-z0-9_]+)", content, re.IGNORECASE)
    if preset_match:
        result["preset"] = preset_match.group(1)

    # RAW_Data — can span multiple lines. Grab every integer (positive or negative)
    # after "RAW_Data:" until we hit a non-data line or end.
    raw_data_pattern = re.compile(r"RAW_Data:\s*(.*)", re.IGNORECASE)
    samples: List[int] = []

    # Find all RAW_Data starting points and collect following numbers
    lines = content.splitlines()
    collecting = False
    for line in lines:
        stripped = line.strip()
        if not stripped:
            continue

        if "raw_data" in stripped.lower():
            collecting = True
            # Grab numbers on the same line after the colon
            after_colon = stripped.split(":", 1)[1] if ":" in stripped else ""
            for tok in re.findall(r"-?\d+", after_colon):
                samples.append(int(tok))
            continue

        if collecting:
            # Continuation lines are usually just numbers or start with numbers
            # Stop if we hit another known header
            if re.match(r"^(Filetype|Version|Frequency|Protocol|Preset|Protocol):", stripped, re.IGNORECASE):
                collecting = False
                continue
            # Otherwise try to pull numbers
            nums = re.findall(r"-?\d+", stripped)
            for n in nums:
                samples.append(int(n))

    result["raw_samples"] = samples
    result["pulse_count"] = len(samples)

    if samples:
        result["total_duration_us"] = sum(abs(s) for s in samples)

    # Detect button press boundaries using large gaps.
    # A "large gap" between a negative (off) and next positive (on) often indicates
    # the fob stopped transmitting between button presses.
    # Default threshold ~80ms (80000 us) — tune with --min-gap
    # We record the sample index where a new "press" likely starts.
    return result


def detect_button_boundaries(samples: List[int], min_gap_us: int = 80000) -> List[int]:
    """
    Heuristic: look for big silence gaps between transmissions.
    Returns list of starting indices in the samples list for each detected press.
    """
    if not samples:
        return []

    boundaries = [0]  # first press always starts at 0
    current_pos = 0

    for i, val in enumerate(samples):
        if val < 0:  # off period
            gap = abs(val)
            if gap >= min_gap_us:
                # Next positive sample (if any) is start of new press
                # Find the next on-sample after this big off
                for j in range(i + 1, len(samples)):
                    if samples[j] > 0:
                        if j not in boundaries:
                            boundaries.append(j)
                        break
    return sorted(set(boundaries))


def format_duration_us(us: int) -> str:
    """Human readable us/ms/s"""
    if us < 1000:
        return f"{us} us"
    elif us < 1_000_000:
        return f"{us/1000:.1f} ms"
    else:
        return f"{us/1_000_000:.2f} s"


def generate_prep_notes(
    captures: List[Dict],
    min_gap_us: int,
    output_path: Path,
    extra_notes: Optional[str] = None,
) -> str:
    """
    Build the full notes content. Returns the string (also writes to disk).
    """
    now = datetime.now().isoformat()
    lines = []

    # ==================== MASSIVE WARNING HEADER ====================
    lines.append("=" * 72)
    lines.append("JESSE'S ROLLJAM PREP NOTES — FOR YOUR OWN HARDWARE ONLY")
    lines.append("=" * 72)
    lines.append("")
    lines.append("GENERATED: " + now)
    lines.append("TOOL: scripts/rolljam_prep.py (file analysis only — ZERO radio capability)")
    lines.append("")
    lines.append("!!! READ THIS OR GET FUCKED, MATE !!!")
    lines.append("")
    lines.append("This file contains timing analysis of RAW captures YOU made from")
    lines.append("hardware YOU LEGALLY OWN. It is notes. Nothing more.")
    lines.append("")
    lines.append("JAMMING IS ILLEGAL IN AUSTRALIA.")
    lines.append("Using Sub-GHz jamming, replay outside your own property, or any")
    lines.append("RollJam-style attack on vehicles/gates you do not control will get")
    lines.append("you charged under Criminal Code + ACMA radiocommunications laws.")
    lines.append("")
    lines.append("This script and these notes give you ZERO excuse.")
    lines.append("The repo maintainers and JESSE accept ZERO responsibility.")
    lines.append("")
    lines.append("Full reality check and legal warnings live here:")
    lines.append("    docs/rolljam_reality.md")
    lines.append("")
    lines.append("If you have not read that document, close this file and go read it.")
    lines.append("If your plan involves anyone else's car: stop. Delete these files.")
    lines.append("If you're on your own property testing your own 2014 Hilux for")
    lines.append("defensive research: proceed with eyes open. Success is not guaranteed.")
    lines.append("")
    lines.append("=" * 72)
    lines.append("")

    # Input summary
    lines.append("CAPTURES ANALYSED:")
    for i, cap in enumerate(captures, 1):
        lines.append(f"  [{i}] {cap['path']}")
        lines.append(f"      Freq: {cap.get('frequency', 'UNKNOWN')} Hz")
        lines.append(f"      Preset: {cap.get('preset', 'UNKNOWN')}")
        if cap["warnings"]:
            lines.append(f"      WARNINGS: {', '.join(cap['warnings'])}")
    lines.append("")

    lines.append(f"MINIMUM GAP USED FOR BUTTON DETECTION: {min_gap_us} us ({format_duration_us(min_gap_us)})")
    lines.append("")

    # Per-capture detailed analysis
    for idx, cap in enumerate(captures, 1):
        samples = cap["raw_samples"]
        boundaries = detect_button_boundaries(samples, min_gap_us)

        lines.append("-" * 60)
        lines.append(f"CAPTURE #{idx}: {Path(cap['path']).name}")
        lines.append("-" * 60)
        lines.append(f"Frequency: {cap.get('frequency')}")
        lines.append(f"Preset:    {cap.get('preset')}")
        lines.append(f"Total RAW samples: {cap['pulse_count']}")
        lines.append(f"Total duration:    {format_duration_us(cap['total_duration_us'])}")

        if boundaries:
            lines.append(f"Detected button-press boundaries (sample indices): {boundaries}")
            lines.append("  (These are approximate. Large silence gaps between presses.)")
            # Rough per-press durations
            for b_idx, start_idx in enumerate(boundaries):
                end_idx = boundaries[b_idx + 1] if b_idx + 1 < len(boundaries) else len(samples)
                press_samples = samples[start_idx:end_idx]
                press_dur = sum(abs(s) for s in press_samples)
                lines.append(f"  Press {b_idx+1}: ~{format_duration_us(press_dur)} ({len(press_samples)} samples)")
        else:
            lines.append("No clear multi-press boundaries detected with current gap threshold.")
            lines.append("Consider re-capturing with deliberate 2-3 second gaps between button presses.")

        lines.append("")
        lines.append("First 40 raw timing samples (us, + = on / TX, - = off / silence):")
        sample_preview = samples[:40] if len(samples) > 40 else samples
        lines.append("  " + " ".join(str(s) for s in sample_preview))
        if len(samples) > 40:
            lines.append(f"  ... ({len(samples) - 40} more samples)")

        lines.append("")

    # Suggested manual workflow notes (educational only)
    lines.append("=" * 72)
    lines.append("SUGGESTED MANUAL SEQUENCE THINKING (YOUR OWN HARDWARE ONLY)")
    lines.append("=" * 72)
    lines.append("")
    lines.append("This is NOT instructions for an attack. This is how some people")
    lines.append("organise their own research notes when investigating rolling code")
    lines.append("behaviour on hardware they control.")
    lines.append("")
    lines.append("Typical (very simplified) flow people discuss for manual prep:")
    lines.append("1. Capture a sequence of legitimate button presses with good gaps.")
    lines.append("2. Use the first capture(s) to understand the code the car saw.")
    lines.append("3. For classic RollJam-style research you would need simultaneous")
    lines.append("   jam + capture hardware (one Flipper alone is usually insufficient).")
    lines.append("4. Later replay attempts on the SAME vehicle you own, under controlled")
    lines.append("   conditions, to observe whether the rolling window accepted it.")
    lines.append("")
    lines.append("On modern AU vehicles (most 2018+ Rangers, Hyundai/Kia, VW, etc.):")
    lines.append("  - Rolling windows are tiny.")
    lines.append("  - Many use additional crypto/challenge layers.")
    lines.append("  - Public success rates with Flipper-class hardware are near zero.")
    lines.append("")
    lines.append("See docs/rolljam_reality.md for the full unvarnished truth.")
    lines.append("")
    lines.append("If after reading that you still want to experiment on YOUR OWN car:")
    lines.append("- Work at home on your own driveway.")
    lines.append("- Document EVERYTHING (distance, battery state, temperature, exact firmware).")
    lines.append("- Expect most attempts to teach you why it doesn't work rather than")
    lines.append("  giving you a working 'open sesame'.")
    lines.append("")
    lines.append("Better use of your time for 95% of people:")
    lines.append("  High-quality 5-15m distance RAW captures + multiple presses +")
    lines.append("  building a personal verified library. See capture_techniques.md")
    lines.append("")

    if extra_notes:
        lines.append("USER NOTES:")
        lines.append(extra_notes)
        lines.append("")

    lines.append("=" * 72)
    lines.append("END OF PREP NOTES")
    lines.append("Generated by JESSE's rolljam_prep.py — file analysis only.")
    lines.append("Stay legal. Stay on your own hardware. Build real skills.")
    lines.append("=" * 72)

    content = "\n".join(lines)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(content, encoding="utf-8")
    return content


def main():
    parser = argparse.ArgumentParser(
        description="JESSE's RollJam Prep — timing analysis + notes generator for your own .sub captures only. READ docs/rolljam_reality.md FIRST."
    )
    parser.add_argument("--captures", nargs="+", help="Two or more .sub files (space separated)")
    parser.add_argument("--dir", type=str, help="Directory containing .sub files (will take all, requires at least 2)")
    parser.add_argument("--output", type=str, default=None, help="Output notes filename (default: rolljam_prep_notes_YYYYMMDD_HHMMSS.txt in current dir)")
    parser.add_argument("--min-gap", type=int, default=80000, help="Minimum silence gap in microseconds to consider separate button presses (default 80000 = 80ms)")
    parser.add_argument("--notes", type=str, default=None, help="Optional extra notes to include in the output file")

    args = parser.parse_args()

    print("\n[JESSE] RollJam Prep starting. Heavy legal warnings are in the output file and in docs/rolljam_reality.md")
    print("[JESSE] This tool does NOTHING on the radio. It is notes and timing stats only.\n")

    files: List[Path] = []

    if args.captures:
        files = [Path(f).resolve() for f in args.captures]

    if args.dir:
        d = Path(args.dir).resolve()
        if d.is_dir():
            files.extend(sorted(d.glob("*.sub")))
        else:
            print(f"[ERROR] --dir {args.dir} is not a directory")
            return

    # Dedup while preserving order
    seen = set()
    unique_files = []
    for f in files:
        if str(f) not in seen:
            seen.add(str(f))
            unique_files.append(f)
    files = unique_files

    if len(files) < 2:
        print("[JESSE] You need at least TWO .sub RAW captures for meaningful prep notes.")
        print("        Run with --captures file1.sub file2.sub  or  --dir some_folder/")
        return

    print(f"[JESSE] Analysing {len(files)} capture files...")

    parsed_captures = []
    for f in files:
        cap = parse_sub_file(f)
        cap["button_boundaries"] = detect_button_boundaries(cap["raw_samples"], args.min_gap)
        parsed_captures.append(cap)
        print(f"  Parsed: {f.name} | samples={cap['pulse_count']} | freq={cap.get('frequency')} | preset={cap.get('preset')}")

    # Default output name
    if not args.output:
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_path = Path(f"rolljam_prep_notes_{ts}.txt").resolve()
    else:
        output_path = Path(args.output).resolve()

    content = generate_prep_notes(
        captures=parsed_captures,
        min_gap_us=args.min_gap,
        output_path=output_path,
        extra_notes=args.notes,
    )

    print(f"\n[+] Prep notes written to: {output_path}")
    print("[JESSE] Open it. Read the warnings again. Then decide if this path is even worth it on your own car.")
    print("[JESSE] Most of the time the honest answer is: better captures + distance technique will teach you more.")
    print("\nRemember the golden rule: only ever on hardware you own and control.")


if __name__ == "__main__":
    main()

