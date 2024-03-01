# Abby Cadabby - vita fuel gauge RE
# USE AT YOUR OWN RISK. BRICKING FUEL GAUGE = BRICKING VITA FOREVER. THERE'S (almost) NO RECOVERY

## WIP

## Hardware

Vita fuel gauge (codename Abby) is an sn27520 (re-branded bq27520 with custom fw) ti fg.  
FAT seems to use die rev A3 (so chip revision G2-G3), late FAT (one with slim SoC) - die revision A7 (chip rev. G4), and slim - die revision A9 (unknown chip revision, maybe L1). Late slim uses combined charger+fg, those are unknown atm.  
Fg cpu die is based on Xemics CoolRISC816, you can find more info in `docs` folder.  
There's (not fully compatible) [simulator/disassembler](https://repo.or.cz/cr816-sim.git)

## Dumps
Contains raw binary dumps of FW and dataflash, flashstream files (those are used by vita updater and/or TI software)

## Dumpers
Contains firmware/dataflash dumpers.
* i2c_python - dumps firmware via raw i2c (even from semi-dead board, as long as there's power to fg) from RPi or any device with i2c.
* kernel_plugin - vita kernel plugin to dump firmware and raw dataflash from live vita. **do not use plugin loaders**, load only from config.txt, takes some time.

## Structs
Contains dataflash sections structures as well as mapped offsets in raw dump.

## Unsealer
Little kernel module to unseal fuel gauge in case you ever accidentaly seal it.  
Note: if you ever seal fg, it modifies FW enabling "access protection", this means that every time you exit ROM mode or read some protected registers, FG will auto-seal. The only way around it is re-flashing first page of FW.  

## Flasher
vita kernel module to re-flash first page (or whole) FW and dataflash. **do not use plugin loaders**, load only from config.txt, takes some time.  
By default it builds for pc "emu", use `-DPLATFORM=vita` cmake flag to build for vita.  
Expects `abbyfw.bin`, `abbydf.bin`, as well as safe-guard file `thats_so_magic` (removes it after flashing) in `ur0:/data`.  
PSA:
* flashing your FG is dangerous, think twice. Bricking it will require soldering for direct i2c, *if* it still boots in ROM mode. Otherwise you're fucked
* **Always** use charger while flashing.
* If vita turns off and shows blinking orange light - disconnect and reconnect battery.
* If vita shows garbage values in PSVIdent battery section - use batteryfixer app to hard-resed syscon and FG.

## Credits
* Cat
* Totally real™ sKgLeBa (pop13_13) - research
* [zecoxao](https://github.com/zecoxao) - providing flashstream files
* CBPS discord - support

