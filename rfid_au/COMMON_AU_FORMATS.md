# Common 125kHz RFID Formats in Australia (2026)

## Dominant Formats You'll Actually See

### EM4100 / EM4102
- By far the most common cheap 125kHz format in Australia.
- 64-bit, very easy to clone.
- Used in: apartment garages, small businesses, older carparks, storage units, rural gates.
- Flipper handles these perfectly.

### HID Prox (26-bit, 37-bit, etc.)
- Extremely common in corporate offices, universities, some government buildings, and larger apartment complexes.
- 26-bit is still surprisingly widespread.
- Flipper can read and emulate many of them (especially with the right card).

### Indala
- Less common but still exists, especially in older industrial and mining sites.

### Other Formats You Will Encounter
- AWID
- Keri
- Pyramid
- Some proprietary "Australian" rebrands of the above

## What Still Works in Australia Right Now

- **EM4100** → Trivial to clone on T5577 or EM4305 blanks. Success rate very high on older systems.
- **HID Prox 26-bit** → Still cloneable on many readers if you can get the card number + facility code.
- Many cheap Chinese access control panels installed between 2015-2024 still use these old formats because "they just work and are cheap".

## Recommended Blanks (Australia 2026)
1. **T5577** — Most flexible. Can emulate almost everything.
2. **EM4305** — Excellent for EM4100 clones.
3. **HID Prox clones** (if you can source good ones) — For corporate targets.

Buy T5577s in bulk from Australian sellers. They're cheap and reliable.

## Attack Notes from the Field
- Many apartment buildings in Sydney, Melbourne, and Brisbane still run ancient 125kHz systems from 2008-2015.
- Mining and construction sites are often even worse (older gear lasts forever out there).
- Some "upgraded" systems just added a new reader on top of the old 125kHz one — the old one often still works.

## Pro Move
Carry a small pack of T5577 blanks + your Flipper. When you see an old black or grey reader on a boom gate or door, test it. You will be surprised how often it still works.

## Legal Reminder
Cloning access cards for buildings you don't have permission to enter is a crime in every Australian state.

Only test on your own property or with explicit written authorisation.
