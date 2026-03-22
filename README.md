# MinPci
Simple edu device
## Configuration
Follow the steps below to test this device in QEMU

1. Copy *minpci.c* file to *qemu/hw/misc* directory
2. Add this lines to *qemu/hw/misc/Kconfig*:
```
config PCI_MINPCIDEV
  bool
  default y if TEST_DEVICES
  depends on PCI
```
3. Add this line to *qemu/hw/misc/meson.build*:
`system_ss.add(when: 'CONFIG_PCI_MINPCIDEV', if_true: files('minpci.c'))`
4. Configure and build QEMU:
```
cd qemu
mkdir build && cd build
../configure --target-list=x86_64-softmmu --enable-debug --enable-debug-info
make -j$(nproc)
```
5. Verify device visibility:
`./qemu-system-x86_64 -device help | grep minpci`

## Testing device in guest 
- Run qemu in build directory (In my case, Debian 11 is installed on hd_disk.qcow2):
```
#!/bin/bash

./qemu-system-x86_64 \
	-m 4G \
	-smp 2 \
	-drive file=/home/dele/start_qemu/hd_disk.qcow2,format=qcow2 \
	-nographic \
	-device minpci \
```
- Check device with `lspci -vv` command:
```
00:04.0 Unclassified device [00ff]: Allied Telesis, Inc (Wrong ID) Device 0011
	Subsystem: Red Hat, Inc. Device 1100
	Physical Slot: 4
	Control: I/O+ Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR+ FastB2B- DisINTx-
	Status: Cap- 66MHz- UDF- FastB2B- ParErr- DEVSEL=fast >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
	Region 0: Memory at febf1000 (32-bit, non-prefetchable) [size=16]
```
- Test read/write operations with devmem command:
```
root@debian:~# busybox devmem 0xfebf1000 
VALID READ
0x00000050
root@debian:~# busybox devmem 0xfebf1000 8 10
WRITE
root@debian:~# busybox devmem 0xfebf1000 
VALID READ
0x0000000A
```




