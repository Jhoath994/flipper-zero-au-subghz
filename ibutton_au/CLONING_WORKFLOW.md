# iButton Cloning Workflow (Australian Field Guide)

## Step-by-Step (Flipper Only)

1. **Read the target**
   - Open iButton app on Flipper
   - Hold the iButton against the reader area or directly on the Flipper's iButton port
   - Press Read
   - Save the file (name it properly: `truck_isuzu_2018_ignition.sub` or similar)

2. **Verify what you got**
   - Most will show as `DS1990A` or `RW1990`
   - Note the 64-bit ID

3. **Prepare blank**
   - Have a RW1990 or DS1990A blank ready
   - Write the ID using Flipper (iButton → Write)

4. **Test emulation first (safest)**
   - Use Emulate mode on the actual reader
   - Does it unlock? Great.
   - If not, the system may be using a different family or have additional checks.

5. **Write to blank only after emulation works**
   - This saves blanks and avoids suspicion.

## Pro Tips for Australia

- Many ATA/B&D gates accept the clone instantly if it's a plain DS1990.
- Truck immobilisers are hit-and-miss — some are just ID match, some have more protection.
- Rural properties are goldmines. Old farm gates with iButton readers are often trivial.

## Common Failures & Fixes
- "Wrong family" → Try a different blank type (RW1990 vs DS1990A).
- Reader doesn't respond → Try different pressure/angle. Some readers are picky.
- Works in Emulate but not when written → The blank is material. Use better quality ones.

## Recommended Naming
`ibutton_<brand>_<year>_<purpose>_<date>.ibtn`

Example: `ibutton_ata_gate_2014_master_2026-06-01.ibtn`

## Legal (Again)
Only ever do this on systems you own or have written permission to test.

If you're cloning random depot gates at 2am, you're not a researcher — you're a irresponsible person who will eventually get caught.

Do the work properly or don't do it.
