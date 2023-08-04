#ifndef EXE_LOADER_H

#define EXE_LOADER_H

#include "../stdlib/stdint.h"
#include "fs.h"
#include "sys.h"

typedef struct
{
	uint16_t ss, ds, es, fs, gs;
	uint16_t ax, dx, bx, sp, bp, si, di;
	uint8_t ah, ch, dh, bh;
	uint16_t eflags;
} cpu_state;

cpu_state far* cpu_state_stack;

cpu_state pop_cpu_state();
void push_cpu_state(cpu_state state);

void load_exe_from_file(disk_t far* disk, char far* name);

#endif
