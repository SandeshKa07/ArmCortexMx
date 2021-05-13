CC=arm-none-eabi-gcc

MACHINE=cortex-m4

CFLAGS= -c -mcpu=${MACHINE} -mthumb -std=gnu11

#$^ -> dependency
#$@ -> denotes target

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^ 
	