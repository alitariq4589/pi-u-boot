// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2011-2012 Freescale Semiconductor, Inc.
 */

#include <common.h>
#include <asm/mmu.h>

struct fsl_e_tlb_entry tlb_table[] = {
	/* TLB 0 - for temp stack in cache */
	SET_TLB_ENTRY(0, CONFIG_SYS_INIT_RAM_ADDR,
		      CONFIG_SYS_INIT_RAM_ADDR_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 0, BOOKE_PAGESZ_4K, 0),
	SET_TLB_ENTRY(0, CONFIG_SYS_INIT_RAM_ADDR + 4 * 1024,
		      CONFIG_SYS_INIT_RAM_ADDR_PHYS + 4 * 1024,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 0, BOOKE_PAGESZ_4K, 0),
	SET_TLB_ENTRY(0, CONFIG_SYS_INIT_RAM_ADDR + 8 * 1024,
		      CONFIG_SYS_INIT_RAM_ADDR_PHYS + 8 * 1024,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 0, BOOKE_PAGESZ_4K, 0),
	SET_TLB_ENTRY(0, CONFIG_SYS_INIT_RAM_ADDR + 12 * 1024,
		      CONFIG_SYS_INIT_RAM_ADDR_PHYS + 12 * 1024,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 0, BOOKE_PAGESZ_4K, 0),

	/* TLB 1 */
	/* *I*** - Covers boot page */
#if defined(CONFIG_SYS_RAMBOOT) && defined(CONFIG_SYS_INIT_L3_ADDR)
	/*
	 * *I*G - L3SRAM. When L3 is used as 1M SRAM, the address of the
	 * SRAM is at 0xfff00000, it covered the 0xfffff000.
	 */
	SET_TLB_ENTRY(1, CONFIG_SYS_INIT_L3_ADDR, CONFIG_SYS_INIT_L3_ADDR,
			MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
			0, 0, BOOKE_PAGESZ_1M, 1),
#elif defined(CONFIG_SRIO_PCIE_BOOT_SLAVE)
	/*
	 * SRIO_PCIE_BOOT-SLAVE. When slave boot, the address of the
	 * space is at 0xfff00000, it covered the 0xfffff000.
	 */
	SET_TLB_ENTRY(1, CONFIG_SYS_SRIO_PCIE_BOOT_SLAVE_ADDR,
		      CONFIG_SYS_SRIO_PCIE_BOOT_SLAVE_ADDR_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_W|MAS2_G,
		      0, 0, BOOKE_PAGESZ_1M, 1),
#else
	SET_TLB_ENTRY(1, 0xfffff000, 0xfffff000,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 0, BOOKE_PAGESZ_4K, 1),
#endif

	/* *I*G* - CCSRBAR */
	SET_TLB_ENTRY(1, CONFIG_SYS_CCSRBAR, CONFIG_SYS_CCSRBAR_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 1, BOOKE_PAGESZ_16M, 1),

	/* *I*G* - Flash, localbus */
	/* This will be changed to *I*G* after relocation to RAM. */
	SET_TLB_ENTRY(1, CONFIG_SYS_FLASH_BASE, CONFIG_SYS_FLASH_BASE_PHYS,
		      MAS3_SX|MAS3_SR, MAS2_W|MAS2_G,
		      0, 2, BOOKE_PAGESZ_256M, 1),

#ifndef CONFIG_SPL_BUILD
	/* *I*G* - PCI */
	SET_TLB_ENTRY(1, CONFIG_SYS_PCIE1_MEM_VIRT, CONFIG_SYS_PCIE1_MEM_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 3, BOOKE_PAGESZ_256M, 1),

	SET_TLB_ENTRY(1, CONFIG_SYS_PCIE1_MEM_VIRT + 0x10000000,
		      CONFIG_SYS_PCIE1_MEM_PHYS + 0x10000000,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 4, BOOKE_PAGESZ_256M, 1),

	/* *I*G* - PCI I/O */
	SET_TLB_ENTRY(1, CONFIG_SYS_PCIE1_IO_VIRT, CONFIG_SYS_PCIE1_IO_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 5, BOOKE_PAGESZ_64K, 1),

	/* Bman/Qman */
#ifdef CONFIG_SYS_BMAN_MEM_PHYS
	SET_TLB_ENTRY(1, CONFIG_SYS_BMAN_MEM_BASE, CONFIG_SYS_BMAN_MEM_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 6, BOOKE_PAGESZ_16M, 1),
	SET_TLB_ENTRY(1, CONFIG_SYS_BMAN_MEM_BASE + 0x01000000,
		      CONFIG_SYS_BMAN_MEM_PHYS + 0x01000000,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 7, BOOKE_PAGESZ_16M, 1),
#endif
#ifdef CONFIG_SYS_QMAN_MEM_PHYS
	SET_TLB_ENTRY(1, CONFIG_SYS_QMAN_MEM_BASE, CONFIG_SYS_QMAN_MEM_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, 0,
		      0, 8, BOOKE_PAGESZ_16M, 1),
	SET_TLB_ENTRY(1, CONFIG_SYS_QMAN_MEM_BASE + 0x01000000,
		      CONFIG_SYS_QMAN_MEM_PHYS + 0x01000000,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 9, BOOKE_PAGESZ_16M, 1),
#endif
#endif
#ifdef CONFIG_SYS_DCSRBAR_PHYS
	SET_TLB_ENTRY(1, CONFIG_SYS_DCSRBAR, CONFIG_SYS_DCSRBAR_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 10, BOOKE_PAGESZ_32M, 1),
#endif
#ifdef CONFIG_SYS_NAND_BASE
	/*
	 * *I*G - NAND
	 */
	SET_TLB_ENTRY(1, CONFIG_SYS_NAND_BASE, CONFIG_SYS_NAND_BASE_PHYS,
			MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
			0, 11, BOOKE_PAGESZ_64K, 1),
#endif
	SET_TLB_ENTRY(1, QIXIS_BASE, QIXIS_BASE_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 12, BOOKE_PAGESZ_4K, 1),

	/*
	 * *I*G - SRIO
	 * entry 14 and 15 has been used hard coded, they will be disabled
	 * in cpu_init_f, so we use entry 16 for SRIO2.
	 */
#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_SYS_SRIO1_MEM_PHYS
	/* *I*G* - SRIO1 */
	SET_TLB_ENTRY(1, CONFIG_SYS_SRIO1_MEM_VIRT, CONFIG_SYS_SRIO1_MEM_PHYS,
		MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 13, BOOKE_PAGESZ_256M, 1),
#endif
#ifdef CONFIG_SYS_SRIO2_MEM_PHYS
	/* *I*G* - SRIO2 */
	SET_TLB_ENTRY(1, CONFIG_SYS_SRIO2_MEM_VIRT, CONFIG_SYS_SRIO2_MEM_PHYS,
		MAS3_SW|MAS3_SR, MAS2_I|MAS2_G,
		      0, 16, BOOKE_PAGESZ_256M, 1),
#endif
#ifdef CONFIG_SRIO_PCIE_BOOT_SLAVE
	/*
	 * SRIO_PCIE_BOOT-SLAVE. 1M space from 0xffe00000 for
	 * fetching ucode and ENV from master
	 */
	SET_TLB_ENTRY(1, CONFIG_SYS_SRIO_PCIE_BOOT_UCODE_ENV_ADDR,
		      CONFIG_SYS_SRIO_PCIE_BOOT_UCODE_ENV_ADDR_PHYS,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_G,
		      0, 17, BOOKE_PAGESZ_1M, 1),
#endif
#endif

#if defined(CONFIG_RAMBOOT_PBL) && !defined(CONFIG_SPL_BUILD)
	SET_TLB_ENTRY(1, CONFIG_SYS_DDR_SDRAM_BASE, CONFIG_SYS_DDR_SDRAM_BASE,
		      MAS3_SX|MAS3_SW|MAS3_SR, MAS2_M,
		      0, 17, BOOKE_PAGESZ_2G, 1)
#endif
};

int num_tlb_entries = ARRAY_SIZE(tlb_table);
