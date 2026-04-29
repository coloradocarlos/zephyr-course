/*
 * Copyright (c) 2024 Antmicro <www.antmicro.com>
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/printk-hooks.h>

#if defined(CONFIG_RAM_CONSOLE) && !defined(CONFIG_RAM_CONSOLE_BUFFER_SECTION)

extern char ram_console_buf[];
extern int z_ram_console_write_pos;

/*
 * board_early_init_hook() runs before PRE_KERNEL_1, so ram_console_init() has
 * not installed the printk hook yet (printk would otherwise be discarded by
 * the weak arch_printk_char_out). Mirror ram_console_out into the same buffer
 * and share z_ram_console_write_pos (see ram_console.c in Zephyr).
 */
static int board_early_ram_console_out(int character)
{
	ram_console_buf[z_ram_console_write_pos] = (char)character;
	z_ram_console_write_pos = (z_ram_console_write_pos + 1) %
				  (CONFIG_RAM_CONSOLE_BUFFER_SIZE - 1);
	return character;
}

#endif /* CONFIG_RAM_CONSOLE && !CONFIG_RAM_CONSOLE_BUFFER_SECTION */

void board_early_init_hook(void)
{
#if defined(CONFIG_RAM_CONSOLE) && !defined(CONFIG_RAM_CONSOLE_BUFFER_SECTION)
	__printk_hook_install(board_early_ram_console_out);
#endif
	printk("Board initialized\n");
}
