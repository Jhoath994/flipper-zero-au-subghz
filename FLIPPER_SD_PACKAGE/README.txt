# Flipper Zero SD Card Package - AU 433.92 MHz Sub-GHz
# Copy the contents of this folder to your Flipper Zero SD card root

## Installation
1. Remove SD card from Flipper
2. Insert into computer
3. Copy contents of FLIPPER_SD_PACKAGE/ to SD card root
4. Reinsert SD card into Flipper
5. Reboot Flipper

## Directory structure after install:
/ext/
  subghz/
    car_hacks_au/
      ford_ranger/      (4 files)
      gates/           (27 files - FIXED CODE gate openers)
      great_wall_cannon/ (36 files - likely fixed/weak KeeLoq)
      holden_commodore/  (4 files)
      honda/           (4 files - includes CVE-2022-27254 broken rolling)
      hyundai_kia/     (8 files)
      isuzu_dmax/      (1 file)
      jeep/            (3 files)
      ldv_t60/         (1 file)
      mazda/           (1 file)
      mazda_bt50/      (1 file)
      mitsubishi/      (1 file)
      nissan/          (1 file)
      subaru/          (62 files - includes FIXED CODE Forester 2002-2008)
      suzuki/          (1 file)
      toyota/          (8 files)
      vw_amarok/       (4 files)
    playlists/
      ALL_AU_MASTER_playlist.txt
      HIGH_PRIORITY_fixed_broken_rolling.txt
      gates_fixed_code_au_playlist.txt
      <brand>_au_playlist.txt (17 brand playlists)
    assets/
      keeloq_mfcodes_user  (38 manufacturer codes)
      setting_user          (AU frequencies + custom presets)

## What to try FIRST (highest success rate):

1. GATES - Load gates_fixed_code_au_playlist.txt in Sub-GHz Playlist
   - CAME 12-bit, Princeton 24-bit, PT2260, SMC5326, Linear
   - These are FIXED CODE - highest probability of direct replay

2. SUBARU Forester 2002-2008 - FIXED CODE
   - Files: subaru/forester_02_08_fixed_ook650_*.sub
   - Try both TE 400 and TE 320 variants
   - Capture from the fob first, then replay

3. HONDA 2012+ - CVE-2022-27254 broken rolling
   - Rolling code does NOT increment on some models
   - Capture once, replay forever

4. GREAT WALL pre-2020 - Likely fixed/weak KeeLoq
   - Try the fixed-code variants first
   - Then KeeLoq with mfcodes loaded

## Brute-force generators (run on PC, copy output to SD):
  python3 scripts/came_bruteforce_gen.py --output-dir /sd/subghz/car_hacks_au/gates/came_bf/
  python3 scripts/princeton_bruteforce_gen.py --output-dir /sd/subghz/car_hacks_au/gates/princeton_bf/

## KeeLoq car files require real fob capture to work
  The .sub files contain correct mfcodes and protocol settings,
  but rolling codes need a captured signal from the physical key fob.
