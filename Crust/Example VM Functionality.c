typedef struct {
    uint8_t opcode;
    int operand1;
    int operand2;
} CorrolexInstruction;

void execute_vm(CorrolexInstruction *instructions, size_t count) {
    int registers[8] = {0};
    for (size_t i = 0; i < count; i++) {
        switch (instructions[i].opcode) {
            case 0x01: // LOAD
                registers[instructions[i].operand1] = instructions[i].operand2;
                break;
            case 0x02: // ADD
                registers[instructions[i].operand1] += registers[instructions[i].operand2];
                break;
            case 0x03: // PRINT
                printf("Register[%d] = %d\n", instructions[i].operand1, registers[instructions[i].operand1]);
                break;
        }
    }
}
