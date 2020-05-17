# Custom Boot Splash
This plugin can display a **24 bit RGB BMP** as custom boot splash on PS Vita / PS TV at boot.<br>
The boot logo that is displayed in the beginning can not be hidden with this plugin.

## Installation
Place the plugin to `ur0:tai/custom_boot_splash.skprx`<br>
Place your **24 bit RGB BMP** as `ur0:tai/boot_splash.bmp`<br>
Look for the line marked `load os0:kd/clockgen.skprx` in `ur0:tai/boot_config.txt`<br>
Add `- load ur0:tai/custom_boot_splash.skprx` one line before that.<br>

**EXAMPLE:**
```
- load ur0:tai/custom_boot_splash.skprx

load os0:kd/clockgen.skprx
#load os0:kd/syscon.skprx
#load os0:kd/rtc.skprx
```

## To Enso 3.65 users
If a boot_config.txt exists in `vs0:tai` and the plugin doesn't load even if plugin path is written to `ur0:tai/boot_config.txt`, please update Enso to the latest version:<br>
https://github.com/TheOfficialFloW/enso/releases
