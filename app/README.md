# Video overlays zephyr app
Copyright (c) 2022 [Antmicro](https://antmicro.com).
## Cloning the repository and building

Please refer to Zephyr's official [getting started guide](https://docs.zephyrproject.org/latest/getting_started/index.html) for detailed setup instructions.

Assuming the SDK and ``west`` are set up correctly, run the following commands to get the correct sources:
```
mkdir zephyrproject
git clone --single-branch --branch=arty_exp_board https://dev.antmicro.com/git/video-overlays-zephyr.git zephyrproject/zephyr
cd zephyrproject/zephyr
west init -l
git submodule update --init
cd ..
west update
```

Then, build the sample:

```bash
cd zephyr
west bu
