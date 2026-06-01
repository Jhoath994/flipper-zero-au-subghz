# Australian Vehicle Key Fob FCC Database
# Compiled from fccid.io, academic research, manufacturer specs, and community data
# Version: 2.0 | 2026-06-01
# Source: flipper-zero-au-subghz FCC research agent

## CRITICAL FINDINGS - FIXED CODE / WEAK ROLLING VEHICLES

### SUBARU A269ZUA111 - FIXED CODE (trivially replayable)
- FCC ID: A269ZUA111
- Frequency: 433 MHz
- Modulation: ASK/OOK
- Buttons: 2 (lock/unlock only, NO transponder chip)
- Coding: FIXED CODE or very weak rolling
- Vulnerability: TRIVIAL REPLAY - capture once, replay forever
- Vehicles: Subaru Forester 2002-2008, some Impreza/Liberty of same era
- This is the single most vulnerable AU-market vehicle for subghz replay

### GREAT WALL CANNON / STEED (pre-2020) - LIKELY KEELOQ CLASSIC OR FIXED CODE
- No FCC registration (Chinese domestic market only)
- Frequency: 433 MHz
- Modulation: ASK
- Chip: Likely HCS300/HCS301 (KeeLoq classic) or EV1527 (fixed code)
- Vulnerability: If KeeLoq Simple Learning = cloneable with 2 captures + known mfcodes
- If fixed code = trivial replay

### HONDA - Rolling-Pwn (CVE-2022-27254)
- Some Honda vehicles 2012+ have rolling code that does NOT increment
- Effectively FIXED CODE despite claiming rolling
- Capture one signal, replay forever on affected models

## COMPREHENSIVE FCC DATABASE

### Toyota
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| MDL-B41TA | 434 MHz | ASK | 4D67 | Rolling (KeeLoq) | KeeLoq classic attacks |
| MDL-BA2TA | 433.92 MHz | ASK | 8A/H | Rolling | RollBack/RollJam |
| HYQ14FBC | 433.92 MHz | FSK | ID71 | Rolling+AES | RollBack, relay |

### Ford Ranger
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| EB3T-15K601-BA | 433.92 MHz | ASK | ID46 Hitag2 | Rolling | RollBack, KeeLoq |
| 3M5T-15K601-DC | 433.92 MHz | ASK | ID46 | Rolling | RollBack |

### Holden
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| QQY8V00GH40001 | 433 MHz | ASK | ID46 Hitag2 | Rolling (KeeLoq) | KeeLoq classic, Hitag2 crypto |

### Hyundai
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| TQ8-FOB-4F18 | 433 MHz | ASK | ID46 | Rolling | RollBack |
| SY5DMFNA433 | 433.92 MHz | ASK/FSK | ID47 HITAG3 | Rolling+AES | RollBack, relay |

### Kia
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| SEKS-AM08FTX | 433 MHz | FSK | ID46 | Rolling (KeeLoq) | KeeLoq classic |
| FOB-4F08 | 433 MHz | ASK/FSK | ID47 HITAG3 | Rolling+AES | RollBack, relay |

### Mitsubishi
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| GHR-M003 | 433 MHz | ASK | ID46 | Rolling | RollBack |
| GHR-M004 | 433.92 MHz | FSK | ID47 HITAG3 | Rolling+AES | RollBack, relay |

### Nissan
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| CWTWB1G744 | 433 MHz | ASK | ID46 Hitag2 | Rolling | RollBack |
| CWTWB1U825 | 433 MHz | ASK/FSK | ID47 | Rolling+AES | RollBack, relay |

### Mazda
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| SKE13E-01 | 433.92 MHz | FSK | ID47 | Rolling+AES | RollBack, relay |
| TAYH-67-5DYB | 433.92 MHz | FSK | ID47 | Rolling+AES | RollBack, relay |

### Subaru
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| A269ZUA111 | 433 MHz | ASK | NONE | FIXED CODE | **TRIVIAL REPLAY** |
| HYQ14AKB | 433 MHz | FSK | ID8A | Rolling+AES | RollBack, relay |

### VW Amarok
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| 7E0837202AD | 433 MHz | ASK | ID48 Megamos | Rolling (KeeLoq) | KeeLoq classic, Megamos crypto |
| SY5DMFNA433 | 433.92 MHz | ASK/FSK | ID47 | Rolling+AES | RollBack, relay |

### Isuzu
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| ACJ932U01 | 434 MHz | ASK | ID49 | Rolling | RollBack |
| ACJ932IK4310E | 433 MHz | FSK | ID47 HITAG3 | Rolling+AES | RollBack, relay |

### Great Wall
| FCC ID | Freq | Mod | Chip | Coding | Vuln |
|--------|------|-----|------|--------|------|
| N/A (3608101XPW04A) | 433 MHz | ASK | 4A/ID47 | Rolling (KeeLoq likely) | **KeeLoq classic, possible fixed code** |

## KEELOQ ATTACK REFERENCE

| Attack | Prerequisites | Result | Applies To |
|--------|--------------|--------|------------|
| DPA on KeeLoq Decryption | Physical receiver access, ~10 power traces | Manufacturer key in <1 day | All KeeLoq Classic (HCS200/300/301) |
| Algebraic Attack | 2^32 known plaintext-ciphertext | Key recovery | All KeeLoq |
| Eavesdropping + XOR | Known mfcodes + 2 captures | Clone any device remotely | KeeLoq Simple Learning |
| RollJam | Jamming + capture hardware | Valid rolling code for later use | Most rolling code RKE |
| RollBack (Csikor 2022) | Capture + timing replay | Resync counter, replay works | Virtually ALL rolling code RKE |
| Rolling-Pwn (Honda) | Capture ONE signal | Replay same signal forever | Honda 2012+ (broken rolling code) |
| CVE-2025-6029 (Kia) | Capture ONE signal | Fixed learning codes = replay | Kia aftermarket fobs |

## PLATFORM FAMILIES

### Continental RHT433/DMFNA433 (shared by)
Ford Ranger, VW Amarok, Hyundai, Kia, Nissan
- ASK RKE: 32 kHz bandwidth, 1-1.7 kBaud
- FSK PASE: 57 kHz bandwidth, deviation ~47.6 kHz
- Same base platform = same attack surface

### Mitsubishi Electric (shared by)
Mazda, Mitsubishi, newer Toyota smart keys
- FSK at 433.92 MHz
- HITAG3/NCF2951X + AES
- More secure than KeeLoq classic

### Denso/Tokai Rika (shared by)
Toyota, Subaru (newer)
- ASK for older, FSK for newer
- Toyota B41/B42TA = KeeLoq classic
- Newer = AES-based
