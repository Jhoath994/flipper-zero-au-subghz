# Isuzu D-Max — Jesse's Verified AU 433.92 MHz Notes

**"If it ain't in the AU_VEHICLES database from fcc_au_car_data.py, it ain't verified. No unverified claims, no fabricated codes." — Jesse**
This folder is for **your own Isuzu D-Max** captures only. Nothing else belongs here.

---

## Verified Models Covered (Direct from AU_VEHICLES)

- D-Max (RG 2020+, earlier RT)
- MU-X

These are the ones with confirmed 433.92 MHz presence in Australian market data from FCC filings, teardowns, and community verification. Another absolute unit in the Australian dual-cab market.

---

## Frequency + Modulation Reality

- **Frequency**: 433920000 Hz (433.92 MHz ISM band — THE frequency that matters for AU cars)
- **Common modulation**: ASK/OOK reported on many

Similar capture profile to Triton/Hilux. Very popular — your neighbours probably have one.

---

## Best Starting Preset + Why

**Recommended preset: OOK650**

**Why OOK650 first?**
- ASK/OOK reported on many.
- Similar capture profile to Triton and Hilux — OOK650 is the proven starting point for these big utes.
- Good results expected with proper technique.

**Alternative presets to try if OOK650 gives poor results:**
- Custom

**Deviation notes from the verified database:**
"D-Max is another huge AU dual-cab. Similar capture profile to Triton/Hilux."

---

## Real Capture Advice (Jesse Method — Do This or Waste Your Time)
1. **Stand 5-15 metres away** from the vehicle. Closer is often worse — signal saturation degrades your capture.
2. **Multiple button presses with proper gaps**: Lock, wait 2-3 seconds, lock again. Do 8-12 presses minimum. For rolling code vehicles you want a sequence of consecutive codes.
3. **Capture as RAW first, every time.** Don't trust the live decoder until you have good raw data. Wrong preset lies to you.
4. **Verify immediately** on *your own* D-Max. If the replay doesn't control the vehicle you captured it from, the file is not useful yet.
5. Document: model (D-Max RG/RT / MU-X), year, distance, preset used, button, conditions.

**FCC refs (traceable):** Isuzu 433.92

**Capture tips from database:** "Very popular. Add to your personal collection."

---

## What Users Should Put in This Folder

**Only your own verified captures.**

Good naming convention (steal this):
- `dmax_rg_2021_lock_10m_ook650_2026-05-30.sub`
- `dmax_rt_2018_unlock_9m_ook650_raw.sub`
- `mux_2020_boot_11m_ook650.sub`

Drop your .sub files here (RAW preferred). Then edit a copy of the generated `isuzu_dmax_au_playlist.txt` (from `fcc_research/generated/`) so the paths point to these real files on your Flipper SD card.

This folder is **not** a dumping ground for Telegram/YouTube garbage files with zero provenance.

---

## Known Year / Model Quirks (From the Research — No More, No Less)

- **D-Max RG (2020+)** and earlier RT: Another absolute unit in the Australian dual-cab market. Similar capture profile to the Triton and Hilux.
- MU-X: Shares the platform — same rules.
- Rolling codes present on modern examples. Add your verified captures to the folder. The structure is waiting.
- Very popular — high chance your local tradies are running these.

---

## ⚠️ HEAVY LEGAL WARNING — DO NOT BE A CUNT

**THIS FOLDER AND ALL CONTENT IS FOR EDUCATIONAL AND DEFENSIVE RESEARCH ON HARDWARE YOU LEGALLY OWN AND CONTROL ONLY.**

- Using any Sub-GHz technique, replay, or knowledge from here against a D-Max you do **not** own is **illegal** in Australia.
- Criminal Code, ACMA radiocommunications laws, theft/break-and-enter statutes all apply.
- "I was just testing with my Flipper" is not a defence when it's your neighbour's D-Max or the work ute.
- Rolling codes exist specifically to prevent exactly this type of attack. Most modern replays fail fast anyway.
- The repo maintainers and Jesse take **zero responsibility** for you misusing this information and getting charged.
If you're here looking to steal Isuzus: **leave now**. Close the folder. This is not for you.

If you own the vehicle, test only on it, stay legal, and learn how the real systems actually work — welcome. you will be fine.

**Verified only. Legal only. Your own property only.**

---

**Sources**: Everything here pulled strictly from `AU_VEHICLES["isuzu_dmax"]` in `fcc_research/fcc_au_car_data.py` + the main repo README. No hallucinations.

Now stop reading and go capture your own D-Max properly at 10 metres. OOK650 is the move here, same as the other big utes. Multiple presses. RAW first.

— JesseVerified only • Australian 433.92 specialist • No rules except the real ones

