/* multiboot.h - the header for Multiboot */
/* Copyright (C) 1999, 2001  Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#pragma pack(push,4)
struct MultibootHeader
{
    UInt32 	magic;			// required
    UInt32 	flags;			// required
    UInt32 	checksum;		// required
    UInt32 	header_addr;	// if flags[16] is set
    UInt32 	load_addr;		// if flags[16] is set
    UInt32 	load_end_addr;	// if flags[16] is set
    UInt32 	bss_end_addr;	// if flags[16] is set
    UInt32 	entry_addr;		// if flags[16] is set
    UInt32 	mode_type;		// if flags[2] is set
    UInt32 	width;			// if flags[2] is set
    UInt32 	height;			// if flags[2] is set
    UInt32 	depth;			// if flags[2] is set
};

/* The Multiboot information. */
struct MultibootInfo
{
    UInt32 flags;

    UInt32 mem_lower;
    UInt32 mem_upper;

    UInt32 boot_device;

    UInt32 cmdline;

    UInt32 mods_count;
    UInt32 mods_addr;

    UInt32 syms[3];

    UInt32 mmap_length;
    UInt32 mmap_addr;

    UInt32 drives_length;
    UInt32 drives_addr;

    UInt32 config_table;

    UInt32 boot_loader_name;

    UInt32 apm_table;

    UInt32 vbe_control_info;
    UInt32 vbe_mode_info;
    UInt32 vbe_mode;
    UInt32 vbe_interface_seg;
    UInt32 vbe_interface_off;
    UInt32 vbe_interface_len;

};

/* The module structure. */
struct Module
{
    UInt32 mod_start;
    UInt32 mod_end;
    UInt32 string;
    UInt32 reserved;
} ;

/* The memory map. Be careful that the offset 0 is base_addr_low
but no size. */
struct MemoryMap
{
    UInt32 size;
    UInt32 base_addr_low;
    UInt32 base_addr_high;
    UInt32 length_low;
    UInt32 length_high;
    UInt32 type;
};

#pragma pack(pop)