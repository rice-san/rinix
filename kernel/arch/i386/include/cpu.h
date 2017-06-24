#ifndef _I386_CPU_H
#define _I386_CPU_H

typedef struct cpu_registers{
  unsigned int gs, fs, es, ds;
  unsigned int edi, esi, ebp, ebx, edx, ecx, eax;
  unsigned int eip, cs, eflags, esp, ss;
}cpu_regs_t;

typedef struct cpu_task_model{

}cpu_tsk_model_t;

int store_registers(cpu_regs_t*); // Store the Registers of the current program


#endif // _I386_CPU_H
