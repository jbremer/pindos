#include <stdio.h>
#include <windows.h>

unsigned __int64 sum(unsigned __int64 x)
{
    // sum(1..x) = x * (x + 1) / 2
    return x * (x + 1) / 2;
}

void pindos(unsigned int size)
{
    // overwrite the first few bytes with some simple initialization code, so
    // we can call this address
    unsigned char first[] = {
        // call $+0
        0xe8, 0x00, 0x00, 0x00, 0x00,
        // pop eax
        0x58,
        // add eax, dword [esp+4] ; size as parameter
        0x03, 0x44, 0x24, 0x04,
        // add eax, size
        0x83, 0xc0, 0x00,
        // jmp eax ; the address right behind the last nop
        0xff, 0xe0,
        // eventually we'll get here, so here's a return instruction
        0xc3,
    };

    // overwrite the last few bytes with a jmp back into the nopsled
    unsigned char last[] = {
        // dec eax
        0x48,
        // jmp eax
        0xff, 0xe0,
    };

    unsigned char *addr = (unsigned char *) VirtualAlloc(NULL,
        size + sizeof(first) + sizeof(last),
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // patch "size" in "first"
    first[12] = sizeof(first) - 5;

    // initialize ALL the bytes
    memcpy(addr, first, sizeof(first));
    memset(addr + sizeof(first), 0x90, size);
    memcpy(addr + sizeof(first) + size, last, sizeof(last));

    void (*fn)(unsigned int size) = (void(*)(unsigned int)) addr;

    printf(
        "nop-instructions: %d\n"
        "executing nops:   %lld\n",
        size, sum(size));
    fflush(stdout);

    unsigned int ticks = GetTickCount();

    fn(size);

    printf("that took %f milliseconds\n", (GetTickCount() - ticks) / 1000.f);
}

void pindos2(unsigned int size, unsigned int step)
{
    // check whether this size and step match
    if((size % step) != 0) {
        printf("size and step don't match, size mod step should return 0\n");
        return;
    }

    // overwrite the first few bytes with some simple initialization code, so
    // we can call this address
    unsigned char first[] = {
        // call $+0
        0xe8, 0x00, 0x00, 0x00, 0x00,
        // pop eax
        0x58,
        // add eax, dword [esp+4] ; size as parameter
        0x03, 0x44, 0x24, 0x04,
        // add eax, size
        0x83, 0xc0, 0x00,
        // jmp eax ; the address right behind the last nop
        0xff, 0xe0,
        // eventually we'll get here, so here's a return instruction
        0xc3,
    };

    // overwrite the last few bytes with a jmp back into the nopsled
    unsigned char last[] = {
        // add eax, step
        0x2d, 0x00, 0x00, 0x00, 0x00,
        // jmp eax
        0xff, 0xe0,
    };

    *(unsigned int *) &last[1] = step;

    // add step - 1 to make sure we jump to the correct address
    unsigned char *addr = (unsigned char *) VirtualAlloc(NULL,
        size + step - 1 + sizeof(first) + sizeof(last),
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // patch "size" in "first"
    first[12] = sizeof(first) - 5;

    // initialize ALL the bytes
    memcpy(addr, first, sizeof(first));
    memset(addr + sizeof(first), 0x90, size + step - 1);
    memcpy(addr + sizeof(first) + size + step - 1, last, sizeof(last));

    void (*fn)(unsigned int size) = (void(*)(unsigned int)) addr;

    printf(
        "nop-instructions: %d\n"
        "executing nops:   %lld\n",
        size, step * sum(size / step));
    fflush(stdout);

    unsigned int ticks = GetTickCount();

    fn(size + step - 1);

    printf("that took %f milliseconds\n", (GetTickCount() - ticks) / 1000.f);
}

int main()
{
    pindos(0x10000);
    pindos2(0x1000000, 0x10000);
}
