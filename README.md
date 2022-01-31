# Video overlays Zephyr app
Copyright (c) 2021-2022 [Antmicro](https://antmicro.com).

## Getting started
Before getting started, make sure you have a proper Zephyr development
environment.
Please refer to Zephyr's official
[getting started guide](https://docs.zephyrproject.org/latest/getting_started/index.html)
for detailed setup instructions.

The application was created to run on LiteX firmware, which can be found [here](https://github.com/antmicro/video-overlays).

### Initialization

The first step is to initialize the workspace folder (``workspace``) where
the ``video-overlays-zephyr-app`` and all Zephyr modules will be cloned. You can do
that by running:

```shell
west init -m https://github.com/antmicro/video-overlays-zephyr-app --mr main workspace
cd workspace
west update
```

### Build & Run
The application can be built by running:

```shell
cd video-overlays-zephyr-app
west build -b litex_vexriscv -s app
```