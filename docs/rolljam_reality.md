# RollJam Reality: What Actually Works on Australian 433.92 MHz Cars (And What Will Get You Charged)

**Kelly's Unfiltered Breakdown — No Hype, No TikTok Bullshit**

RollJam videos look sexy. "One Flipper, jam the car, capture the code, open it later." Deadset, most of that content is either ancient vehicles with weak implementations, heavily edited lab conditions, or straight-up fantasy for clicks.

This document exists because the AU_VEHICLES database and real-world testing tell a much more boring, more honest story. Here's the truth for Australian-market cars on 433.92 MHz.

---

## What RollJam / Rollback Actually Is

Classic RollJam (popularised in public research years ago):

- You jam the car's receiver so the legitimate fob press never reaches it (the car doesn't "consume" that rolling code).
- You capture the fob signal at the same time.
- Later you replay the captured signal when the owner isn't around.

Variants include "rollback" attacks where you capture a sequence and replay an earlier code the car might still accept within a window.

This is **not** simple replay. Simple replay on modern rolling code cars usually works zero or one time before the fob and car desynchronise.

**Critical hardware reality on Flipper Zero**:
- A **single stock Flipper cannot do proper simultaneous jam + capture** reliably. The CC1101 is half-duplex in practice for this kind of work.
- Real RollJam implementations that have any success rate usually require:
  - Two devices (one dedicated to jamming/transmitting noise, one receiving the fob).
  - Or SDR hardware (HackRF, etc.) with much better timing control and full-duplex capability.
  - Or very specific custom firmware hacks with precise timing that most users never get stable.
- "One Flipper RollJam" videos are almost always either using pre-recorded signals, very old/simple systems, or not actually demonstrating a live attack on a modern rolling code car.

---

## What Actually Works on Which AU Cars / Years

Grounded in the AU_VEHICLES data and verified patterns:

**Some chance / older weaker implementations (pre ~2018-ish)**:
- Certain Toyota Hilux (2005-2015 Tokai Rika B41TA/B42TA era) — some models had simpler rolling.
- Early Ford Ranger PX (pre-2018ish) — occasional reports on weaker Continental/Hella modules.
- Holden VF Commodore (2013-2017) — more forgiving than later stuff in some cases.
- Many gates/garage openers (ATA, B&D, Centurion/Elsema generics) — these often use short or predictable rolling/fixed codes. Much higher success surface.

**Low to near-zero public success on modern AU vehicles**:
- Ford Ranger PX2/PX3 2018+ (especially post-2020).
- Hyundai/Kia (i30 GD/PD, Tucson, Sportage QL, Sorento) ~2015 onwards — heavy use of 2FSK + rolling + additional challenge-response elements. The AU_VEHICLES entry explicitly notes these are harder.
- VW Amarok (2H) — VW group FSK implementations are not pushovers.
- Later Mazda, Mitsubishi Triton MQ/MR, Nissan Navara D23, Subaru, Isuzu D-Max — progressively tighter rolling code windows. Many moved to stronger KeeLoq variants or proprietary crypto.

Even on the "easier" older cars, success is **highly variable**, depends on exact firmware in the fob and receiver, battery condition, temperature, and requires excellent timing. It is nowhere near the "works every time" fantasy sold in short videos.

The distance capture + multiple presses technique documented in `capture_techniques.md` is currently more reliable for building actual usable personal libraries than hoping RollJam works on your neighbour's 2022 Ranger.

---

## Legal Warning — Read This Twice

**Jamming is illegal in Australia.**

Under the Radiocommunications Act (administered by ACMA), deliberate interference with radio communications is prohibited. This includes jamming key fobs, garage doors, or anything else in the 433.92 MHz band — even if you think "it's just ISM, unlicensed."

Using these techniques on a vehicle, gate, or property you do **not** legally own and control is:

- Attempted theft / break and enter under state criminal codes.
- Potential fraud or property damage charges.
- Radiocommunications offence on top.

People have been charged in Australia for far less sophisticated RF mischief. "I was just testing with my Flipper" is not a legal defence when the tradie comes back to find his Hilux gone or the police explain ACMA complaints.

This repo's entire legal disclaimer (see README) exists for a reason. The maintainers and Kelly accept zero responsibility for you being a muppet.

If you're doing this on hardware you own for defensive research and education, document everything and stay on your own property. Anything else is you choosing to play stupid games.

---

## When RollJam Is Actually Useful vs When It's a Complete Waste of Time

**Potentially useful (narrow cases, your own hardware only)**:
- Educational research on your own pre-2018 vehicle with known weak rolling implementation.
- Testing gate/garage systems you control (many of these are trivial compared to cars).
- Understanding exactly why modern systems resist these attacks (the best way to learn defensive radio security).

**Almost always a waste of time on modern AU cars**:
- 2018+ Rangers, most Korean brands, VW group utes, recent Toyotas/Subarus etc.
- Any scenario where you only have one Flipper and no SDR.
- Any plan involving public roads, other people's property, or "just testing the neighbour's car."
- Expecting it to be reliable enough to be your primary method.

The honest truth: for the majority of vehicles listed in AU_VEHICLES in 2026, investing the same hours into high-quality distance RAW captures of your own fobs will produce more usable results than chasing RollJam fairy tales.

---

## Safer, More Useful Alternatives

1. **Just use your key like a normal person.** Revolutionary concept.
2. **Master the distance capture technique** (see the other doc). Build a personal verified library of your own vehicles' signals across conditions. This is actually valuable.
3. **Buy a spare genuine fob** from the dealer or a reputable locksmith if you're worried about losing the only one. Program it properly. Done.
4. **For learning**: Capture your own gates first. Then your own older vehicles. Analyse the hell out of them with ProtoPirate/ProtoView. Understand the difference between a clean capture and a saturated mess.
5. **Defensive mindset**: Use this knowledge to recognise when your own vehicles might be vulnerable (older fobs, certain brands) and take practical steps (faraday pouch for the fob when parked in high-risk areas, etc.). Knowledge, not crime.

---

## Final Kelly Sass

If the plan in your head involves "I'll just RollJam it with one Flipper from the footpath," stop. Go read `capture_techniques.md` again, walk 10 metres back from your own car, and do 12 proper presses with gaps. Save the RAW. Test the replay. Repeat until it actually works.

The public research on these attacks has been around for years. The car manufacturers didn't sit on their hands. Most modern 433.92 AU implementations (especially the FSK ones on Hyundai/Kia, later Rangers, VW) were designed with exactly these kinds of attacks in mind.

The videos that make it look trivial are lying to you or working on 2008-era hardware.

Stay legal. Stay on your own shit. Build real skills with clean captures instead of chasing unreliable illegal attacks that mostly don't work on the cars Australians actually drive in 2026.

The AU_VEHICLES list is full of real utes and cars. None of them are toys.

Do the boring, legal, effective work.

**She'll be right — if you don't do anything that gets you charged.**

— Kelly
