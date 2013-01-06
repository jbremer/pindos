#include <stdio.h>
#include "pin.H"

void tracertn(TRACE trace, void *v)
{
    printf("parsing 0x%08x\n", TRACE_Address(trace));
}

void traceins(INS ins, void *v)
{
    printf("parsing 0x%08x\n", INS_Address(ins));
}

int main(int argc, char *argv[])
{
    if(PIN_Init(argc, argv)) {
        return 0;
    }

    //TRACE_AddInstrumentFunction(&tracertn, NULL);
    //INS_AddInstrumentFunction(&traceins, NULL);
    PIN_StartProgram();
    return 0;
}
