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
$ <cmd> | ./humanreadabler
```

#### Example

Script: Network total transfered data size monitor

```
NETDP=/sys/class/net/wlp0s2f1u10(paste your device instead of this)/statistics # path to statistics about network device

while [ 1 ]
    do echo -en "Total: $((`cat $NETDP/rx_bytes`+`cat $NETDP/tx_bytes`))\e[K\e[G" | ./humanreadabler
    sleep 1
done
```
