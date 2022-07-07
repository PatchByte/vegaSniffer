# Vega Sniffer

Just a project for fun! Made for rooted android phones!

## Setup
- Android NDK (Min: Api Version: 24)
- make
- bash (git bash on windows with cmake)

## Build

```sh
Follow: https://developer.android.com/ndk/guides/other_build_systems
python gencache.py
make
adb push bin/arpui /data/local/tmp
adb shell
su # rooted android required
cd /data/local/tmp
chmod +x arpui
./arpui
```

```
Open Firefox or Chrome.
Type 127.0.0.1:8090
And happy sniffing now just wait until the network does arp requests!
```

## What next?
- ARP exploits...
- Code Cleanup

## Projects I stole code from
- https://github.com/jeremycw/httpserver.h -> http server tiny c code!
- https://codepen.io/alvarotrigo/pen/YzEdrKj -> toggles for css (for the next things..)
- https://github.com/DaveGamble/cJSON -> json parser / writer!
- https://stackoverflow.com/a/426485 -> base64 decoder!
