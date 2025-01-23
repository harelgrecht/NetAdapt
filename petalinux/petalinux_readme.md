
# PetaLinux Step-by-Step Guide

Refer to [UG1144](https://www.xilinx.com) for more information on setting up your PetaLinux environment.

**Tested on**:  
- **Tools Version**: 2023.1  
- **Hardware Platforms**: KR260 Starter Kit  

Before starting, ensure your terminal is using `Bash` mode by running:  
`chsh -s /bin/bash`.

---

## Setting Up Your Environment

1. Open a terminal and edit the `.bashrc` file:
    ```bash
    nano ~/.bashrc
    ```

2. Add the following lines at the end of the file:
    ```bash
    sudo sysctl fs.inotify.max_user_watches=524288
    source ~/Desktop/petalinux-tool/settings.sh
    PATH="${XSCT_TOOLCHAIN}/bin:${PATH}"
    ```

3. Save and exit. This ensures every terminal you open has the PetaLinux environment.

4. Verify the setup:
    ```bash
    petalinux
    ```
    If a list of commands appears, the setup is complete.

---

## Creating a New Project

Navigate to your desired directory:
```bash
cd <directory_path>
```

To create a PetaLinux project:  
- Without BSP using a template:
    ```bash
    petalinux-create --type project --template zynqMP/zynq --name <project_name>
    ```
- With BSP:
    ```bash
    petalinux-create --type project -s <path_to_bsp> --name <project_name>
    ```

---

## Configuring the Project

Navigate to the project directory:
```bash
cd <project_name>
```

1. Configure with your `XSA` file:
    ```bash
    petalinux-config --get-hw-description=<path_to_xsa_file>
    ```

2. Reconfigure the project:
    ```bash
    petalinux-config
    ```

3. Configure a specific subsystem:
    ```bash
    petalinux-config -c <subsystem>
    ```
    Available subsystems: `rootfs`, `device-tree`, `u-boot`, `kernel`.

---

### Configuring for Offline Use

Ensure offline packages are available locally at:
```
matspen$/General/Development Kits/PetaLinux_installation
```

1. Open the config menu:
    ```bash
    petalinux-config
    ```

2. Set the following options:
    - **Yocto Settings →** `[ * ] Enable BB NO NETWORK`
    - **Yocto Settings → Network sstate feeds URL →**  
      `file:///home/user/Desktop/Petalinux-tools/OfflinePackages/aarch64`
    - **Yocto Settings → Local sstate feeds settings → Local sstate feeds URL →**  
      `file:///home/user/Desktop/Petalinux-tools/OfflinePackages/aarch64`
    - **Yocto Settings → Add pre-mirror URL →**  
      `file:///home/user/Desktop/Petalinux-tools/OfflinePackages/downloads`

3. Modify the following file:
    ```bash
    nano components/yocto/layers/meta-xilinx/meta-xilinx-core/recipes-bsp/pmu-firmware/pmu-rom-native.bb
    ```
    Update line 10 to:
    ```bash
    SRC_URI = "/home/user/Desktop/Petalinux-tools/OfflinePackages/downloads/openDownload_filename=PMU_ROM.tar.gz"
    ```

---

### Customizing the Project

1. For SD card use:
    - Change the root filesystem to `EXT4(sd/emmc)` and set the device node to `/dev/sda`.

2. Add standard libraries and compilers:
    ```bash
    petalinux-config -c rootfs
    ```
    Press `/` and search for `packages-buildessentials` and enable it.\
    Press `/` and search for `packagegroup-petalinux-kria` and disable it. (If enabled it will make problems with the FPGA)\
    Press `/` and search for `libpcap` and enable it.\
    Press `/` and search for `zlib` and enable it.
---

## Building the Project
"
1. Build the entire project:
    ```bash
    petalinux-build
    ```

2. Clean and rebuild:
    ```bash
    petalinux-build -x mrproper
    ```

3. Build a specific subsystem:
    ```bash
    petalinux-build -c <subsystem>
    ```
    Available subsystems: `rootfs`, `device-tree`, `u-boot`, `kernel`.

---

## Packaging

### Boot File

Package boot files into `BOOT.BIN`:
```bash
petalinux-package --boot --u-boot --fpga images/linux/system.bit --force
```

### Linux Image

Package the Linux image for SD/eMMC programming:

For template project:
```bash
petalinux-package --wic --bootfiles "BOOT.BIN,image.ub,system.dtb,boot.scr" --rootfs-file ./images/linux/rootfs.tar.gz
```

For BSP project:
```bash
petalinux-package --wic --images-dir images/linux/ --bootfiles "rootfs.cpio.gz.u-boot,boot.scr,Image,system.dtb,system-zynqmp-sck-kr-g-revB.dtb,system.bit" --disk-name "sda"
```

### Create BSP
```bash
petalinux-package --bsp -o <PATH-TO-BSP> -p <PATH-TO-PROJECT>
```
---

## Programming the Hardware

### KR260 Starter Kit

1. Press `FW` and reset the board to enter firmware update mode.
2. Connect via Ethernet.
3. Configure the PC IP to `192.168.0.100`.
4. Open `192.168.0.111` in a browser.
5. Choose a bootable image (A/B).
6. Upload `BOOT.BIN`.

### Zynq Flash (Arty-Z7)

1. Open Vivado and the Hardware Manager.
2. Connect to the board and select "Program Device."
3. Add the Flash device, then browse and upload:
    - `BOOT.BIN`
    - `zynq_fsbl.elf`

### SD/eMMC

- **SD Card**: Use BalenaEtcher or Rufus to flash `petalinux-sdimage.wic`.
- **eMMC**:
    1. Configure target IP: `192.168.1.2`, mask `255.255.255.0`.
    2. Configure PC IP: `192.168.1.1`, mask `255.255.255.0`.
    3. Transfer the image:
        ```bash
        gzip images/linux/petalinux-sdimage.wic
        ssh-keygen -f "/home/linux/.ssh/known_hosts" -R "192.168.1.111"
        scp -r images/linux/petalinux-sdimage.wic.gz petalinux@192.168.1.111:/tmp
        ```
    4. On the board:
        ```bash
        sudo su
        gunzip -c /tmp/petalinux-sdimage.wic.gz | dd of=/dev/mmcblk0 bs=1M status=progress
        sync
        ```

---

## Additional Notes

- **Convert DTB to DTS**:
    ```bash
    sudo apt install dtc
    dtc -I dtb -O dts -o system.dts system.dtb
    ```

- **Configure Ethernet in U-Boot**:
    ```bash
    setenv ethact <eth_interface>
    setenv ipaddr <ip_address>
    setenv netmask <netmask>
    saveenv
    ```

- **KR260 Vivado Version**: Use the default kria vivado project provided by Xilinx located at: `kr260_vivado`









# Petalinux guide to create a kernel module and using tftpboot

## how to use tftpboot
```bash
setenv ethact eth1 # make eth1 as active ethernet interface
setenv ipaddr 192.168.0.111 # set ip address of the board
setenv serverip 192.168.0.110 # set ip address of the server
tftpboot 0x10000000 Osher/image.ub # load image from server to memory
fatwrite usb 0:1 0x10000000 image.ub 0x${filesize} # write image from memory to sd card
ping 192.168.0.110 # make sure 2 devices can communicate with each other
time fatload usb 0:1 0x10000000 image.ub # load image from sd card to memory
bootm # boot the image from memory
```
## set ip on linux
```bash
sudo ifconfig eth0 192.168.0.111 # set ip address of the board
```


## copy the module from petalinux project to linux machine

```bash
scp /home/Pituah/Desktop/Osher/OsherUartOs/OsherOS/build/tmp/sysroots-components/xilinx_k26_kr/osheruart/lib/modules/6.1.5-xilinx-v2023.1/extra/osheruart.ko  osher@192.168.0.111:/home/osher # copy the module from petalinux project to linux machine
```

## modules utillities

```bash
sudo lsmod # list all the modules that are currently loaded in the kernel
sudo insmod osheruart.ko # load the module called osheruart.ko
sudo rmmod osheruart # remove the module called osheruart
sudo dmesg | tail -n 5 # pritn the last 5 lines of the kernel log
```

## general utiilites

```bash
echo -ne "\033]0;tio\007" # change terminal name to tio
```
