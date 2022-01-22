# Firmware for the slaves 
compile firmware with arduino ide and tinyMircroCore

## upload
use pyupdi

```pyupdi -d tiny406 -f firmware-slave.hex --fuses 2:0x02 6:0x04 8:0x00 -c /dev/ttyUSBx```
