#include <stdint.h>

#define SRAM_START      0x20000000
#define SRAM_SIZE       (40 * 1024)
#define SRAM_END        ((SRAM_START) + (SRAM_SIZE))

#define STACK_START     SRAM_END

void Reset_handler(void);

uint32_t vector_table[] = {
  STACK_START,
  (uint32_t)&Reset_handler;
};

void Reset_handler(void){

}  
