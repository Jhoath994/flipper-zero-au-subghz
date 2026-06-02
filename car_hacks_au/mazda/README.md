# Mazda — JESSE's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No bullshit, no made-up codes." — JESSE**

This folder is for **your own Mazda** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Mazda 3 (BM/BN 2014-2018 AU)
- CX-5 (KE/GH 2012-2017)
- Mazda 6, BT-50 (some variants)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. Extremely popular cars in Australia.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Mix — many ASK, some FSK

**The ASK/FSK trap applies.** Test OOK first for most Mazda 3/CX-5 of that era. Some later ones went FSK. Wrong preset = poor or zero usable results.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- Test OOK first for most Mazda 3/CX-5 of that era.
- Plenty of ASK fobs in the 2012-2018 window.
- If the signal looks weak or won't replay properly on your own car, immediately try the FSK preset.

**Alternative presets to try if OOK650 gives poor results:**
- 2FSKDev476
- Custom

**Deviation notes from the verified database:**
"Test OOK first for most Mazda 3/CX-5 of that era. Some later ones went FSK."

---

## Real Capture Advice (JESSE Method — Do This or Waste Your Time)

1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation fucks your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Mazda. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (3/CX-5/6/BT-50), year (BM/BN/KE etc critical), distance, preset used, button, conditions.

**FCC refs (traceable):** Various Mazda 433.92 modules

**Capture tips from database:** "Popular AU cars. Good for playlist building once you have your own captures."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `mazda3_bn_2017_lock_9m_ook650_2026-05-30.sub`
- `cx5_ke_2015_unlock_11m_ook650_raw.sub`
- `mazda6_2014_2fskdev476_7m.sub`
- `bt50_2016_boot_10m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `mazda_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **Mazda 3 BM/BN (2014-2018)**: Very common. Mix of ASK and some FSK. OOK650 is the right starting point for most.
- **CX-5 KE/GH (2012-2017)**: Same era, similar profile. Good solid target.
- **Later examples**: Higher chance of FSK creeping in. Have 2FSKDev476 ready as a quick switch.
- BT-50 (some): Shares platform bits with Ranger in some years — watch for FSK if it's a later one.
- Rolling codes present on most. Good platform for building serious personal playlists once you have verified captures.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Mazda you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's CX-5 or the family 3.
- Rolling codes exist specifically to stop exactly this shit. Most modern replays fail fast anyway.
- The repo maintainers and JESSE take **zero responsibility** for you being a fuckwit and getting charged.

If you're here looking to steal Mazdas: **fuck off**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. She'll be right.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["mazda"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own Mazda 3 or CX-5 properly at 10 metres. Start OOK650. Multiple presses. RAW first.

— JESSE
Verified only • Australian 433.92 specialist • No rules except the real ones

