#define I_MASK (0x0040)
#define GET_INT_STATUS(x) \
{ _asm("STC FLG, $$[FB]", x); \
  x &= I_MASK;}
   
#define ENABLE_INTS \
 {_asm("FSET I");}
#define DISABLE_INTS \
 {_asm("FCLR I");}
 
 #define OS_TASK_SW() _asm("INT #1")
 
void disable_interrupt();
void enable_interrupt();
extern unsigned long t;
unsigned long get_time();
extern void init_tick();