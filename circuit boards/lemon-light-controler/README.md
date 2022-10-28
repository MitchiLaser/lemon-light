Main Controller Design
===
- parts used
  - node MCU
  - small buck converter breakout board
  - SN74HCT245N (although a one channel version of it would be better)
  - screw terminals for power supply and TX
- design
  - soldered on a breadboard
  - the buck converter provides 5V power supply
  - the RX pin is shifted with the SN74HCT245N from the 3.3V to the 5V level
- notes
  - make sure not to pull the TX of the node MCU low during boot
    (if you use a different level shifter)
  - during boot some data is output which causes the display to show random stuff
    - could be avoided by connecting the output enable pin of the SN74HCT245N
      to some GPIO of the node mcu

