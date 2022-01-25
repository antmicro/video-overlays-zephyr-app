/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* GPU */
typedef struct {
	uint8_t CR;			/* offset 0x00*/
	uint8_t FILL_CLR;	/* offset 0x04*/
	uint8_t FILL_CLR_2;	/* offset 0x08*/
	uint8_t FILL_CLR_3;	/* offset 0x0C*/
	uint8_t FILL_CLR_4;	/* offset 0x10*/
} GPU_typedef;

#ifndef CSR_BASE
#define CSR_BASE	(0xf0000000L)
#endif

/* GPU */
#define GPU_BASE	(CSR_BASE + 0x4000L)
#define GPU			((volatile GPU_typedef*) GPU_BASE)

/* GPU operating modes */
#define GPU_CR_IDLE				((uint32_t)0x00000000)
#define GPU_CR_ALPHA_BLENDER	((uint32_t)0x00000001)
#define GPU_CR_FILL_AREA		((uint32_t)0x00000002)

/* Colors */
#define CLR_RED		((uint32_t)0xFF0000FF)
#define CLR_GREEN	((uint32_t)0xFF00FF00)
#define CLR_BLUE	((uint32_t)0xFFFF0000)
