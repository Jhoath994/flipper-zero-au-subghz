# car_hacks_au/ — YOUR Captures Only

**flipper-zero-au-subghz**

This is the professional, scalable folder structure for Australian 433.92 MHz Sub-GHz work on Flipper Zero.

**Every single file that ends up in these brand folders must be a capture you made yourself from hardware you legally own and control.**

No exceptions. No "I found this on Discord". No Telegram "universal" files. No YouTube magic downloads.

If it isn't yours and verified on the actual vehicle/gate it came from, it does not belong here.

---

## Folder Structure

```
car_hacks_au/
├── toyota/              # Hilux (B41TA/B42TA), Camry, Corolla, RAV4 etc.
├── ford_ranger/         # Ranger PX/PX2/PX3 + Everest shared platforms
├── holden_commodore/    # VF Commodore / Calais / SS
├── mazda/               # 3 (BM/BN), CX-5, 6, BT-50 some
├── hyundai_kia/         # i30 (GD/PD), Tucson, Sportage, Sorento
├── mitsubishi/          # Triton MQ/MR, ASX, Outlander some
├── nissan/              # Navara D23 (NP300), X-Trail, Qashqai some
├── subaru/              # Forester (SJ/SK), XV, Outback AU variants
├── isuzu_dmax/          # D-Max (RG/RT), MU-X
├── vw_amarok/           # Amarok (2H)
└── gates/               # ATA, B&D, Centurion/Elsema, Nice, generics etc.
    └── (your garage remote captures live here)
```

Each brand folder contains its own `README.md` with the verified details pulled from `AU_VEHICLES` in `fcc_research/fcc_au_car_data.py` (frequency, recommended preset, FCC refs, capture tips, quirks).

---

## Recommended File Naming (Use This)

Good names are searchable, sortable, and tell the story at a glance:

- `toyota_2012_hilux_lock_10m_ook650_2026-05-31.sub`
- `ford_ranger_2021_unlock_8m_2fskdev476_2026-04-12.sub`
- `gates_ata_2019_open_raw_2026-03-20.sub`
- `holden_vf_2015_boot_sequence_12m_2026-05-28.sub`

The `scripts/capture_helper.py` tool will do most of this for you automatically.

---

## How These Folders Are Meant to Be Used

1. Capture high-quality RAW files from **your own** vehicles using the techniques in `docs/capture_techniques.md` (5-15 m, multiple presses with gaps, RAW first, verify immediately).
2. Use `scripts/capture_helper.py` (or manual rename) to drop them into the correct `car_hacks_au/<brand>/` folder with proper names.
3. Copy the matching `_au_playlist.txt` from `fcc_research/generated/` into your Flipper's `/ext/subghz/playlists/` (or wherever your firmware looks).
4. Edit the playlist so every line points at your real files on the SD card.
5. Load the playlist on the Flipper and go range-test or sequence-test your own captures.

This is exactly how the serious collections (Flipper Zero community style) stay manageable when they grow to hundreds of files.

---

## What Does NOT Belong Here

- Any .sub file you did not capture yourself
- Files from other people's cars "for testing"
- "Universal" or "master code" claims from the internet
- Templates with the placeholder `RAW_Data` still in them (use those as starting points only, then replace with real data)
- Anything that cannot be verified to replay on the vehicle it was taken from

The `.gitignore` at repo root deliberately makes it hard to accidentally commit real captures. Respect that.

---

## Legal Reality (Repeated Because People Are Thick)

**Everything in these folders is for educational and defensive research on hardware YOU LEGALLY OWN AND CONTROL ONLY.**

Using any technique, file, frequency, or knowledge from this repository against a vehicle or gate you do not own is illegal in Australia. Criminal Code. ACMA radiocommunications offences. Theft/break-and-enter statutes.

The repo, the generator, the scripts, Jesse, and every contributor accept zero responsibility for you misusing this information.
If you're here to steal cars: leave now. This structure is not for you.

If you own the utes and gates and want to understand exactly how their 433.92 MHz systems behave so you can protect your own material — welcome. Use the tools. Capture properly. Build your own library.

---

## Quick Start with the Helper Scripts

```bash
# Organise one capture
python scripts/capture_helper.py --input my_hilux_lock.sub --brand toyota --year 2012 --button lock --distance 10

# Batch a whole folder of today's captures
python scripts/capture_helper.py --batch-dir today_captures/ --brand ford_ranger --year 2021 --button unlock --dry-run

# After you have several good captures of your own car, prepare timing notes
python scripts/rolljam_prep.py --captures cap1.sub cap2.sub cap3.sub --output my_notes.txt
```

See the individual script headers and `docs/` for full details.

---

## The Generator Connection

All the brand `README.md` files inside the subfolders are derived from the same `AU_VEHICLES` database that powers `fcc_research/fcc_au_car_data.py`.

When you run the generator (`--brand`, `--playlist all`, `--generate-settings`), you get fresh, consistent skeletons that match the structure you see here.

Regenerate whenever you extend the database with new verified models.

---

## Final Jesse Words on This Folder
This `car_hacks_au/` layout is the single best thing you can do for your own long-term sanity once you start taking Sub-GHz seriously in Australia.

It forces you to be honest: "Is this file actually mine and verified?"

If the answer is no, it doesn't go in the folder.

Do that consistently and you'll end up with a personal collection that is actually useful — instead of a pile of mystery .sub files that do jack material when you need them.

Now stop reading the root README and go capture something from your own Hilux or Ranger at 10 metres with proper gaps.

Multiple presses. RAW. Verify on the vehicle. Organise with the helper.

**you will be fine.**

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

