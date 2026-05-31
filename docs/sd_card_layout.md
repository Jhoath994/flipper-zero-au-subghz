# Kelly's Recommended Flipper SD Card Layout for AU Car Work

This is how I organise my shit so I can actually find things when I'm standing in a carpark at 11pm.

## Core Structure

```
/ext/subghz/
├── car_hacks_au/                 # This entire repo lives here
│   ├── toyota/
│   ├── ford_ranger/
│   ├── hyundai_kia/
│   ├── mazda_bt50/
│   ├── great_wall_cannon/
│   ├── ... (all the brands)
│   └── gates/
│
├── playlists/                    # Drop the generated *_au_playlist.txt files here
│   ├── toyota_au_playlist.txt
│   ├── ford_ranger_au_playlist.txt
│   └── ...
│
├── templates/                    # The .sub templates + full settings file
│   ├── settings_user_au_FULL.txt
│   ├── toyota_template.sub
│   └── ...
│
├── my_captures/                  # Your actual good captures (never commit these)
│   └── 2026-05-31_hilux_10m/
│
└── raw_dumps/                    # Temporary raw captures before sorting
```

## How I Actually Use It

1. **First time setup**
   - Copy the entire `car_hacks_au/` folder from this repo to your SD.
   - Copy everything from `playlists/` and `templates/`.
   - Add the frequencies + custom presets from `templates/settings_user_au_FULL.txt` into your firmware's `settings_user.txt`.

2. **When capturing**
   - Capture into `raw_dumps/`.
   - Use `scripts/capture_helper.py` to rename + move them into the correct `car_hacks_au/<brand>/` folder with good names.
   - Add the new files to the relevant playlist.

3. **Naming convention I actually use**
   ```
   toyota_hilux_2012_lock_10m_ook_2026-05-31.sub
   ford_ranger_2021_unlock_12m_fsk_2026-06-01.sub
   gate_ata_2024_open_5m_2026-05-30.sub
   ```

## Pro Tips

- Keep a `notes.txt` inside each brand folder with what worked, what preset was best, distance that gave clean captures, etc.
- The `gates/` folder is your training ground. Get good there before you go after utes.
- Never put real captures from other people's cars in this repo if you ever push it publicly.

This layout has saved me hours of swearing at my Flipper.

Now go outside and capture something properly.
