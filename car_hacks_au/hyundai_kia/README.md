# Hyundai / Kia — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own Hyundai or Kia** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- Hyundai i30 (GD 2012-2017, PD 2017+)
- Tucson (LM, TL)
- Kia Sportage (QL), Sorento (UM)

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. The Korean brands went hard on FSK.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: Many 2FSK on Korean brands in this period

**The ASK/FSK trap will destroy you here.** Hyundai/Kia smart keys from ~2015+ very commonly use FSK. OOK preset will fail hard — you'll get nothing usable.

---

## Best Starting Preset + Why

**Recommended preset: 2FSKDev476**

**Why 2FSKDev476 first? (CRITICAL)**
- Hyundai/Kia smart keys from ~2015+ very commonly use FSK.
- Using an OOK preset here is a complete waste of time — you'll get nothing usable.
- Always start 2FSK or go straight to raw capture and analysis on these.

**Alternative presets to try if 2FSKDev476 gives poor results:**
- OOK650 (unlikely to help on later models but worth a quick test on very early GD i30 etc)
- Custom (with proper deviation tuning)

**Deviation notes from the verified database:**
"Hyundai/Kia smart keys from ~2015+ very commonly use FSK. OOK preset will fail hard. Use 2FSK or capture raw and analyze."

**Pro tip from Jesse**: These are harder targets overall. Many use rolling codes + challenge-response elements on top of the FSK modulation.

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you — and on these the wrong preset lies *hard*.
4. **Verify immediately** on *your own* Hyundai/Kia. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (i30/Tucson/Sportage/Sorento), exact year/generation (GD/PD/LM etc critical), distance, preset used (especially note 2FSK), button, conditions.

**FCC refs (traceable):** Hyundai/Kia 433.92 modules (various)

**Capture tips from database:** "These are harder. Many use rolling codes + challenge-response elements. Distance + timing still the best non-SDR approach."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `i30_pd_2019_lock_10m_2fskdev476_2026-05-30.sub`
- `tucson_tl_2018_unlock_12m_2fskdev476_raw.sub`
- `sportage_ql_2017_2fskdev476_8m.sub`
- `sorento_um_2016_boot_11m_2fskdev476.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `hyundai_kia_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **~2015+ smart keys**: Strong shift to 2FSK + more complex security. The preset trap is non-negotiable here.
- **Earlier GD i30 / LM Tucson**: Slightly higher chance of ASK working, but still test 2FSK.
- **PD i30, TL Tucson, QL Sportage, UM Sorento**: Very commonly FSK. Manage expectations on rolling code + challenge elements.
- These are harder than Holden VF or early Toyota/Mitsubishi. Distance + timing + multiple captures is currently the best public non-SDR approach.

No magic files exist. Anyone claiming easy unlocks on modern Korean stuff is either lying or working with stolen vehicles.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a Hyundai or Kia you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's i30 or the family Tucson.
- Rolling codes + challenge-response exist specifically to prevent exactly this type of attack. Success rates are low on modern examples anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Korean cars: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["hyundai_kia"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own i30 or Sportage properly at 10 metres. **Start with 2FSKDev476.** Multiple presses. RAW first. This one punishes the wrong preset hard.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

