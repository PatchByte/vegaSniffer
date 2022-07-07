# Vega Sniffer

Just a project for fun! Made for rooted android phones!

## Setup
- Android NDK (Min: Api Version: 24)
- make
- bash (git bash on windows with cmake)

## Build

```sh
Follow: https://developer.android.com/ndk/guides/other_build_systems

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