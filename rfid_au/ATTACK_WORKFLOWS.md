# RFID 125kHz Attack Workflows (Australia)

## Basic Clone Workflow (Most Common)

1. Read the card with Flipper (125 kHz RFID app)
2. Note the format (EM4100, HID, etc.)
3. Write to a T5577 blank
4. Test on the actual reader

## Advanced / Useful Techniques

### Facility Code + Card Number Harvesting
Many Australian sites still use predictable numbering.
- Facility code often = site or company number
- Card numbers are frequently sequential or based on employee ID

If you get one card from a site, you can often guess others.

### Multi-Format Blanks
A single well-programmed T5577 can sometimes emulate multiple formats. Test this.

### Long Range Reading
Some 125kHz readers have decent range. You can sometimes read cards in wallets or pockets if the person is standing close to a reader or you get creative with antenna positioning (advanced and situational).

## Australian-Specific Notes

- **Apartment garages**: Often the easiest. Many still run pure EM4100.
- **Construction sites**: Mix of old and new. The old stuff is usually trivial.
- **Rural properties**: Gold. Many still use basic 125kHz for gates and sheds.
- **Universities & TAFEs**: Frequently still have legacy HID systems alongside newer ones.

## Recommended Tools to Carry
- Flipper Zero with good 125kHz antenna
- 10x T5577 blanks
- Small notepad for writing down facility codes + numbers when you find them

## Warning
Just because you *can* clone a card does not mean you *should*.

Only ever clone cards for systems you own or have explicit permission to test. Everything else will eventually get you in trouble.
