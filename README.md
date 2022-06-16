# Human Readabler

A program that translates a long numbers to Gigas, Megas, Kilos and etc.

## Getting the repository

```console
$ git clone https://github.com/M0x1m/humanreadabler.git
```

## Building

```console
$ cd humanreadabler
$ make
```

## Usage

```
$ <cmd> | ./humanreadabler -[kmg] [--kilo | --mega | --giga | --si | --precision <num>] [-p <num>]
```

__-k | --kilo__ &emsp; Disables kilo translates

This flag disables kilos translate. Thus, if only this flag is provided the program will cannot translate numbers from 0 to 104857. The short version of this parameter can be provided with others in one commnad line argument like this: `./humanreadabler -kg`, this command will traslate all compatible numbers from stdin to megas.

__-m | --mega__ &emsp; Disables mega translates

If only this flag is provided the program will not translates compatible numbers to megas, but will translates numbers 1024 - 1073741824 to kilos and numbers above to gigas.

__-g | --giga or other same__ &emsp; Like this two above.

__--si__ &emsp; Turns using SI instead of 2^x

If this flag is provided kilos will translates from 1000 to 1000000 instead of 1024-1048576 and etc.

__-p | --precision__ &emsp; Defines number of digits after dot in translated numbers.

#### Example

Script: Network total transfered data size monitor

```
NETDP=/sys/class/net/wlp0s2f1u10(paste your device instead of this)/statistics # path to statistics about network device

while [ 1 ]
    do echo -en "Total: $((`cat $NETDP/rx_bytes`+`cat $NETDP/tx_bytes`))\e[K\e[G" | ./humanreadabler
    sleep 1
done
```
