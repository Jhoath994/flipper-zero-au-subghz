# Toyota — JESSE's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No bullshit, no made-up codes." — JESSE**

This folder is for **your own Toyota** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Hilux (2005-2015, Tokai Rika B41TA/B42TA) — **FCC gold standard reference**
- Camry (AU market ~2010-2018)
- Corolla (AU market many years)
- RAV4, Kluger (some 433.92 variants)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: ASK / OOK (older fobs); some FSK on newer smart keys / push-button start models.

**The ASK/FSK trap is 100% real here.** Older Hilux and Camry fobs are classic ASK/OOK. Newer smart key systems can flip to FSK. Using the wrong preset = you might see a signal but get zero range or zero replay success. Always test both.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- Older Hilux/Camry (especially the B41TA/B42TA Hilux) often decode cleanly on OOK650 or close custom variants.
- It's the safe starting point for the majority of documented Toyota AU fobs in this collection.
- Start here, capture RAW, then switch presets and re-capture if the signal is weak or won't replay on your own vehicle.

**Alternative presets to try immediately if OOK650 gives poor results:**
- 2FSKDev476
- Custom (tuned BW/deviation in your firmware — RogueMaster/Momentum/Unleashed users have options here)

**Deviation notes from the verified database:**
"Older Hilux/Camry often decode well on OOK650 or custom. Newer smart keys may need FSK preset. Test both."

Do **not** trust random internet .sub files claiming Toyota magic. Capture your own.

---

## Real Capture Advice (JESSE Method — Do This or Waste Your Time)

1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation fucks your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* Toyota. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model/year, distance, preset used, button (lock/unlock/boot), conditions.

**FCC refs (traceable):** MDL-B42TA, B41TA (433.92 MHz)

**Capture tips from database:** "Capture at 5-15m away from vehicle for rolling code learning. Do multiple presses with pauses. Some models use KeeLoq or similar."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `hilux_2012_lock_10m_ook650_2026-05-30.sub`
- `camry_2015_unlock_8m_ook650_raw.sub`
- `hilux_b41ta_lock_sequence_12m.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `toyota_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **2005-2015 Hilux B41TA/B42TA**: One of the best documented AU examples. Often responds well to clean OOK captures. KeeLoq-style rolling in play on many.
- **~2010-2018 Camry/Corolla**: Mix of straightforward ASK and some newer FSK smart key variants. Test OOK650 first.
- Newer Toyota smart keys/push-button start (post ~2018 in some lines): Higher chance of FSK. The preset trap bites hard.
- Rolling codes are present on most post-2010ish models. Simple replay often works once or not at all before desync.

No universal files exist for modern ones. Anyone selling you one is lying or selling stolen data.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Toyota you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's Hilux or the work ute.
- Rolling codes exist specifically to stop exactly this shit. Most modern replays fail fast anyway.
- The repo maintainers and JESSE take **zero responsibility** for you being a fuckwit and getting charged.

If you're here looking to steal cars: **fuck off**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. She'll be right.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["toyota"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations. Cross-reference fccid.io for B41TA/B42TA yourself.

Now stop reading and go capture your own Hilux properly at 10 metres. Multiple presses. RAW first.

— JESSE
Verified only • Australian 433.92 specialist • No rules except the real ones

