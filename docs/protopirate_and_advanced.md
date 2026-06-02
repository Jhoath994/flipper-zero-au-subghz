# ProtoPirate and Advanced Protocol Tools

**How to Actually Use Your Captures from This Repo with Serious Analysis Gear**

This collection gives you verified starting data, clean templates, and the professional `car_hacks_au/` structure. The real power comes when you feed **your own high-quality RAW captures** (made using the distance technique) into proper analysis tools.

ProtoPirate and its cousins are excellent for this. They are **not** magic "open any car" apps. They are analysis and research tools. Garbage captures still produce garbage analysis.

---

## ProtoPirate — The Current Heavyweight for Automotive Sub-GHz on Flipper

**Project**: ProtoPirate (also called Flipper Zero Protocol Pirate)

- Primary home: https://protopirate.net/ProtoPirate (check here first for latest builds and security notes)
- GitHub mirror: https://github.com/RocketGod-git/ProtoPirate

**What it actually does well**:
- Loads and analyses existing `.sub` files via "Sub Decode" (point it at files in your `/ext/subghz/car_hacks_au/` folders).
- Real-time protocol receiver with animated display and frequency hopping support.
- Strong decoder coverage for many families relevant to AU vehicles:
  - Kia (multiple versions — directly relevant to Hyundai/Kia entries in AU_VEHICLES)
  - Ford (relevant to Ranger)
  - Subaru, Suzuki, Mazda, Mitsubishi (all appear in the 10 brands)
  - VAG group elements (relevant to VW Amarok)
  - KeeLoq, AES128, CRC variants, Manchester, PWM, etc.
- Timing Tuner tool — compares your real captured pulse durations/jitter against protocol definitions and suggests tweaks.
- Transmission is **disabled by default** in many builds precisely to avoid accidental rolling code desync on your own vehicles.

**How to use it with this repo**:

1. Capture high-quality RAW files using the methods in `capture_techniques.md` (5-15m, multiple presses with 2-3s gaps, correct preset from AU_VEHICLES, immediate verification on your car).
2. Organise them into `car_hacks_au/<brand>/` with descriptive names.
3. Install ProtoPirate `.fap` (commonly available in RogueMaster, Momentum, and other custom firmware plugin bundles).
4. Run ProtoPirate → Sub Decode → navigate to your capture.
5. Look at matches against Kia/Ford/Subaru/Mitsubishi etc. families.
6. Use the timing tools on files that partially decode or look promising.
7. Export insights or edited versions if the tool supports it, then re-test replay on your actual vehicle.

**Important notes**:
- It will not turn a saturated close-range mess into a working attack. Good distance captures are still required.
- Many rolling code systems (especially post-2015 Hyundai/Kia, later Rangers, etc.) will show partial or no clean decodes even with excellent captures — that's the reality of modern implementations.
- Always final-verify any analysis or modified files by replaying on hardware **you own**.

---

## Complementary On-Device Tools

- **ProtoView** (by antirez): https://github.com/antirez/protoview
  - Excellent general-purpose Sub-GHz visualisation, oscilloscope-style pulse view, protocol decoding (including KeeLoq and many others), message editing, and custom decoder framework.
  - Often bundled alongside or instead of ProtoPirate in serious custom firmwares.
  - Perfect companion for inspecting the RAW_Data sections of your captures before or after ProtoPirate.

- **RocketGods-SubGHz-Toolkit** (same author ecosystem): https://github.com/RocketGod-git/RocketGods-SubGHz-Toolkit
  - Lower-level tools and C code for reverse-engineering Sub-GHz protocols and KeeLoq manufacturer codes on Flipper.

- Custom firmware Sub-GHz enhancements (RogueMaster, Momentum, Unleashed, and forks like Flipper-ARF/D4C1-Labs):
  - Better frequency lists, custom presets (exactly the OOK650 / 2FSKDev476 recommendations from the AU generator), improved RAW handling, and often include the above apps out of the box.

---

## Desktop and Offline Analysis Tools (Where the Deep Work Happens)

Your best captures deserve more than on-device tools.

- **Universal Radio Hacker (URH / URH-NG forks)**: Industry-standard desktop tool for RF protocol reverse engineering. Excellent native or plugin support for importing Flipper `.sub` files. Auto-identifies many protocols and has deep analysis features.
- **KAT (KaraZajac)**: Rust TUI tool focused on key fob analysis. Decoders aligned with ProtoPirate reference implementations. Handles modulation metadata and exports back to `.sub` format. Strong for VAG and rolling code work.
- **flipper_toolbox** (evilpete): https://github.com/evilpete/flipper_toolbox — Python scripts for generating, modifying, and analysing Flipper Sub-GHz files.
- **Browser-based .sub viewers**:
  - SiroxCW/Flipper-SubGHz-Viewer-Trimmer and similar (search GitHub for current maintained forks).
  - Quick visual inspection, trimming, and basic decoding without needing the Flipper powered on.
- **Flipper-SubGHz-analyzer** style tools and various community parsers.

Workflow that actually scales:
1. Capture on Flipper using distance technique + correct preset (per AU_VEHICLES).
2. Quick triage on-device with ProtoPirate / ProtoView.
3. Offload the good RAW files to a computer.
4. Deep analysis in URH or KAT.
5. Iterate (new captures with adjusted timing/preset/distance based on what the desktop tools reveal).
6. Organise everything back into the `car_hacks_au/` structure with updated playlists.

---

## Integration with This Collection — Do It Properly

- The generated `*_template.sub` files already contain the brand, models, frequency, recommended preset, deviation warnings, FCC refs, and capture instructions. Use them as the header when you create your real files.
- The `_au_playlist.txt` files are designed to be edited with paths to your real analysed captures.
- When you discover a better preset, timing observation, or year-specific behaviour on your own AU vehicle, feed it back into the Python generator in `fcc_research/fcc_au_car_data.py` so the whole collection improves.
- Never drop random internet .sub files into the brand folders. Only your verified captures.

The structure exists so that when you have 50+ real files across Toyota, Ranger, Hyundai/Kia etc., everything stays findable and the playlists actually work.

---

## Real Public Project References (No Bullshit Links)

All of the following are active, real, public projects as of 2026:

- **ProtoPirate**: https://protopirate.net/ProtoPirate (primary) and https://github.com/RocketGod-git/ProtoPirate (mirror). Automotive-focused .sub analysis and decoders.
- **ProtoView**: https://github.com/antirez/protoview — visualisation and general protocol work.
- **RocketGods-SubGHz-Toolkit**: https://github.com/RocketGod-git/RocketGods-SubGHz-Toolkit
- **Flipper Zero official firmware (Sub-GHz reference)**: https://github.com/FlipperDevices/flipperzero-firmware (applications/main/subghz and docs on file format/presets).
- **UberGuidoZ Sub-GHz collection and patterns**: https://github.com/UberGuidoZ/Flipper/tree/main/Sub-GHz
- **kakuzu-f0 Automotive collection (structure inspiration)**: https://github.com/kakuzu-f0/Automotive-Sub-Ghz-Collection
- **flipper_toolbox**: https://github.com/evilpete/flipper_toolbox
- **Custom firmware homes** (for the latest bundled apps): RogueMaster, Momentum, Unleashed — follow their official channels.

For desktop: Search for current maintained URH forks and KAT releases. The space moves fast; always verify the repo is active and the `.fap` matches your firmware version.

---

## Final JESSE Reality Check

These tools will show you exactly how good (or how dogshit) your captures actually are. They will reveal pulse jitter, modulation errors, and timing problems that the stock Sub-GHz app hides.

They will **not** give you working codes for cars that aren't yours. They will not defeat modern challenge-response or well-implemented rolling codes on 2020+ AU utes.

Use them on your own verified distance captures from the brands in AU_VEHICLES. Use the insights to get better at capturing. Use the organisation this repo provides so you can actually find the good files later.

Anything else is just playing with expensive toys while posting screenshots.

Stand back from the car. Capture properly. Analyse ruthlessly. Verify on hardware you own.

That's the only pipeline that produces real results.

**Now go feed some actual good captures into ProtoPirate instead of random Telegram trash.**

— JESSE

