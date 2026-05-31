# Capture Techniques: The Distance Method for 433.92 AU Rolling Codes

**Kelly's No-Bullshit Ultimate Guide**

Look, mate. If you're standing 30cm from the car mashing the fob like it's going out of style and wondering why your Flipper captures are useless garbage that won't replay, this document is for you. The "away from the car" technique — 5 to 15 metres — is the single biggest practical improvement most people can make for real 433.92 MHz Australian vehicles.

This isn't YouTube magic. This is how the actual collections (kakuzu-f0 style, UberGuidoZ patterns) get usable rolling code sequences. Everything here is grounded in the AU_VEHICLES database and real Flipper SubGhz RAW behaviour.

---

## Why 5-15 Metres Beats Standing Next to the Car

Being right on top of the vehicle is one of the most common and stupid mistakes.

- **Signal saturation**: The CC1101 in the Flipper (and the car's receiver) has automatic gain control. When the signal is stupidly strong at 0-2 metres, the front end clips or distorts. You end up with a capture that looks "strong" on the Flipper screen but bears zero resemblance to what the car actually hears from a normal key fob at 10-20m range.
- **Cleaner waveforms at distance**: At 5-15m you get the natural amplitude, pulse shapes, and timing that the vehicle's system was designed around. This is critical for rolling code cars because the receiver is tuned for real-world fob distances, not "I'm literally touching the bonnet."
- **Better rolling sequence capture**: Multiple presses with proper gaps separate cleanly. Close-range captures often smear together or have AGC pumping that makes analysis hell later in ProtoPirate or ProtoView.
- **Real-world relevance**: If your replay only works when you're doing the "Flipper dance" two metres away, it was never a good capture. The car won't see that signal in normal use.

**Sweet spot for most AU cars**: Start at 8-12 metres. Adjust based on results. Some older Toyotas and Holdens forgive closer work. Hyundai/Kia and later Fords are pickier.

---

## Step-by-Step: RAW Capture on Flipper (The Only Way That Matters)

1. **Firmware prep (do this once)**
   - Use a serious custom firmware: RogueMaster, Momentum, or Unleashed. Stock is too limited for AU work.
   - Run the generator: `python fcc_research/fcc_au_car_data.py --generate-settings`
   - Merge the output into your `settings_user.txt` (adds the proper 433.82/433.92/434.02/434.42 frequencies and documents the critical OOK vs FSK presets).
   - Reboot Flipper after changes.

2. **Know your target before you press anything**
   Use the AU_VEHICLES data:

   - **Toyota** (Hilux B41TA/B42TA 2005-2015, older Camry/Corolla): Start with `OOK650`. Test `2FSKDev476` on newer smart keys.
   - **Ford Ranger** (PX/PX2/PX3 2011-2024): `2FSKDev476` first on 2018+. Early ones sometimes OOK.
   - **Holden VF Commodore**: `OOK650` is usually solid.
   - **Hyundai/Kia** (i30, Tucson, Sportage ~2015+): `2FSKDev476` is non-negotiable. OOK preset here is a waste of time.
   - **Mazda, Mitsubishi Triton, Nissan Navara D23, Subaru Forester, Isuzu D-Max, VW Amarok**: See the per-brand notes in the generator and README. Always have both OOK650 and 2FSKDev476 ready to switch.

   Wrong preset = either nothing or fake "decoded" signals that do jack shit on replay.

3. **On the Flipper**
   - Sub-GHz app → Read (or Capture in some firmwares).
   - Set Frequency: `433920000`
   - Set Preset to the recommended one for the brand.
   - If the firmware supports it, enable RAW mode explicitly.
   - Walk 5-15m away from your vehicle. Face the car normally (no need for line-of-sight gymnastics).

4. **The actual capture session (this is the technique)**
   - Start recording.
   - Press the fob button (usually lock or unlock — test both).
   - **Wait 2-3 full seconds** between presses. The gaps matter.
   - Do this **8-15 times**. For rolling code systems you want a sequence of consecutive codes, not one lonely press.
   - Stop the capture.
   - **Save immediately** with a useful filename:
     `hilux_2012_lock_10m_ook650_2026-05-31.sub`
     `ranger_px3_unlock_12m_2fsk_2026-05-31_raw.sub`
   - Do not overwrite. New file every session or major change.

5. **Verify on the spot**
   - Replay the file from the same distance and conditions.
   - Does the car respond? Good. Try from inside the car, different angles, different buttons.
   - If it only works once or never: you have a partial or saturated capture. Go again with more distance or different preset.

6. **Organise like a professional**
   - Drop the real `.sub` files into `car_hacks_au/<brand>/` (toyota/, ford_ranger/, etc.).
   - Update the corresponding `_au_playlist.txt` (from the generator) so the Sub-GHz Playlist plugin can cycle through your verified captures automatically.
   - Keep a personal notes file with model/year, exact distance, preset, button, weather, success notes. Future you will kiss present you.

**Gates and garage doors first if you're new**: ATA, B&D, Centurion/Elsema etc. are often simpler OOK with fixed or short rolling codes. Perfect training before you touch a 2020+ Ranger or i30.

---

## Timing, Multiple Presses, and Saving Strategy

- **2-3 second gaps**: This is not arbitrary. It gives the rolling code system time to advance and lets you see individual codewords clearly in RAW_Data when you analyse later.
- **8-15 presses minimum**: One or two presses is amateur hour for rolling code work. You want a run of sequential codes.
- **Multiple sessions**: Different days, different battery levels in the fob, slightly different distances. Real collections are built over time, not in one afternoon.
- **RAW first, always**: Decode and analyse *after* you have the raw data. The live decoder on the wrong preset will lie to you cheerfully.
- **Safety copies**: As soon as a capture works on your car, copy it off the SD card. Flipper SD cards fail at the worst times.

---

## Common Mistakes (Stop Doing These)

- **Too close**: The #1 killer of good captures. "But it shows RSSI bars!" Yeah, and it's clipped to hell.
- **Wrong preset**: OOK650 on a 2FSK Hyundai fob = garbage. 2FSKDev476 on an old ASK Toyota = poor range or nothing. The AU_VEHICLES database exists for a reason.
- **Single press or no gaps**: Useless for anything beyond the most trivial fixed-code gates.
- **Using decoded files instead of RAW**: Decoded protocols are convenient but lose information. Always keep the RAW version.
- **"It decoded so it must be good"**: The Flipper can decode noise into something that looks like a protocol. Replay test on the actual car is the only truth.
- **Expecting one capture to rule them all**: Rolling codes advance. A file that works today may be useless tomorrow on many systems. Build a library.
- **Not documenting**: "hilux.sub" from three months ago tells you nothing when you need to reproduce results.
- **Ignoring the template comments**: The `_template.sub` files generated by the script contain the exact preset warnings and FCC refs for that brand. Read them.

---

## How to Analyse Your Capture

Once you have a solid RAW file:

1. **On-device**:
   - Load into ProtoPirate (if you have it on your firmware) → Sub Decode. It has strong support for Kia, Ford, Subaru, Mitsubishi and related families that match many AU vehicles.
   - Use ProtoView for waveform visualisation, pulse timing, and basic editing.
   - Stock Sub-GHz decode as a quick check (but trust it less than the dedicated tools).

2. **What you're looking for**:
   - Consistent pulse widths and gaps across presses.
   - Clear separation between individual button press events.
   - Any visible counter or rolling element (visible in some KeeLoq-style or simple Manchester encodings).
   - Whether the modulation actually matches the preset you used (OOK vs 2FSK characteristics).

3. **Desktop / advanced**:
   - Load the `.sub` into Universal Radio Hacker (URH) or dedicated parsers.
   - Tools like flipper_toolbox scripts or browser .sub viewers for quick trimming and inspection.
   - Compare timing against the brand notes in AU_VEHICLES.

4. **The only test that matters**:
   - Can you replay it and control your own vehicle under realistic conditions? Everything else is academic until that works.

**Pro move**: Name files with the analysis results too once you have them. `ranger_2019_lock_2fskdev476_8m_counterstep4.sub`

---

## Final Kelly Real Talk

The distance technique + RAW + multiple timed presses + immediate verification on your own hardware is 90% of winning at this. The other 10% is not being a muppet about presets.

Every single verified capture in serious collections started exactly like this — someone standing a sensible distance from their own car, doing the boring work of pressing the button a dozen times with gaps, saving the RAW, and testing it properly.

If it was easy, every random .sub on Telegram would actually work. They don't.

Do the work. Stand back. Capture RAW. Verify on your own shit. Build your own library using the `car_hacks_au/` structure and the generator.

The cars in the AU_VEHICLES list (Hilux B41/B42TA, Ranger PX series, VF Commodore, Korean FSK boxes, Triton, D-Max, Amarok, etc.) are all real Australian vehicles on 433.92 MHz. None of them are "press one button from the internet and done."

Now stop reading and go capture something properly.

**She'll be right — if you do it right.**

— Kelly
