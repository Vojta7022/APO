## Build, Installation and Execution Instructions

### Requirements

- Cross-compilation tools for ARM architecture:
  - `arm-linux-gnueabihf-gcc`
- `make`
- SSH access with the correct private key to the target board (mzapo-root-key)

To get the key, it has to be copied from `postel.felk.cvut.cz`:
```bash
scp cvut_login@postel.felk.cvut.cz:/opt/zynq/ssh-connect/mzapo-root-key ~/.ssh/
chmod 600 ~/.ssh/mzapo-root-key
```

To get the IP address of the board, it has to be turned on and then black squares will be displayed, telling you the IP address, in binary.

Then, change the `TARGET_IP` in `Makefile` to the IP you read from the board's display.

Also, you have to change this line in the Makefile, and replace the existing username with yours.
`SSH_OPTIONS=-i /home/ponrtvoj/mzapo-root-key -o 'ProxyJump=ponrtvoj@postel.felk.cvut.cz'`

### Compilation

To compile the project, simply run:

```bash
make
```
This builds the executable tileman using all .c source files in the current directory. The binary is statically linked and compiled for the ARM architecture.

### Execution on Target
To build and run the program directly on the target device:

```bash
make run TARGET_IP=<target-ip-address>
```
This will:

- Kill any running gdbserver processes.
- Upload the tileman binary to the remote device.
- Execute it remotely over SSH.

### Cleanup
To remove all compiled binaries and object files:

```bash
make clean
```
