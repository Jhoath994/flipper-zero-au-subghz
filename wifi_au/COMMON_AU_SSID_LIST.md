# Common Australian WiFi SSIDs & Captive Portal Targets (2026)

## High Value / Common SSIDs
- Telstra
- Telstra_XXXX
- Optus_XXXX
- Vodafone_XXXX
- NBN-XXXX
- Aussie Broadband
- TPG
- iiNet
- BigPond
- "Free WiFi" at shopping centres (Westfield, etc.)
- Hotel / Motel guest networks (often very weak)
- Caravan park / campground WiFi
- "PublicWiFi" at airports and train stations

## Captive Portal Notes
Many Australian home routers and ISP-provided gateways still have weak or easily bypassable captive portals.

NBN connection boxes and some Telstra/Optus gateways are particularly soft targets when combined with good social engineering ("NBN technician" or "speed test required" pretexts).

## Recommended Tooling (2026)
- Flipper + WiFi Dev Board (or Marauder)
- ESP32-based Evil Portals (cheaper and more powerful for long-term use)
- Good cloned login pages for the major ISPs

## Field Reality
A well-made "NBN Outage - Reconnect" or "Aussie Broadband Speed Test" portal still catches a surprising number of people, especially tradies and older users.

## Legal
Setting up Evil Portals in public or on networks you don't own is illegal. Only use in authorised assessments with written scope.
