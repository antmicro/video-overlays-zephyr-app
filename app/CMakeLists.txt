# Copyright (c) 2021-2022 Antmicro
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(video-overlays-app)

FILE(GLOB app_sources src/*.c)

target_include_directories(app PRIVATE include)

target_sources(app PRIVATE ${app_sources})
