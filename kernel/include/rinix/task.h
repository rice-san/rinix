#ifndef _RINIX_TASK_H
#define _RINIX_TASK_H

#include <arch/cpu.h>

typedef struct task{
  int state;
  cpu_tsk_model_t task_data;
  cpu_page_dir_t page_dir;
  task_t* next;
}task_t;

#endif // _RINIX_TASK_H
