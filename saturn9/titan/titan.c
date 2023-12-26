#include <titan.h>
#include <stdio.h>
#include <stdint.h>
#include <filesystem.h>
#include <titan_reader.h>
#include <mem.h>
#include <opcodes.h>
#include <datatypes.h>
#include <executable.h>
#include <vm.h>

Executable *executable;

Value *TITAN_process_value() {
    uint8_t data_type_id = TITAN_read_short();
    DataType *data_type=memalloc(sizeof(DataType));
    Value *value=memalloc(sizeof(Value));
    switch (data_type_id) {
        case 0: {
            data_type = &TYPE_INT32;
            value->type=data_type;
            value->u32_value=TITAN_read_uint32(4);
            value->str_value=NULL;
            break;
        }
        case 1: {
            data_type = &TYPE_INT8;
            value->type=data_type;
            value->u32_value=TITAN_read_uint8(1);
            value->str_value=NULL;
            break;
        }
        case 2: {
            data_type = &TYPE_POINTER;
            value->type=data_type;
            value->u32_value=TITAN_read_uint32(8);
            value->str_value=NULL;
            break;
        }
        case 3: {
            data_type = &TYPE_REGISTER;
            value->type=data_type;
            value->u32_value=TITAN_read_short();
            value->str_value=NULL;
            break;
        }
        case 4: {
            data_type = &TYPE_IDENTIFIER;
            unsigned str_size = TITAN_read_uint8(4);
            char *identifier = TITAN_read_string(str_size);
            value->type=data_type;
            value->u32_value=0;
            value->str_value=identifier;
            break;
        }
        default: {
            data_type = &TYPE_INT32;
            value->type=data_type;
            value->u32_value=TITAN_read_uint32(4);
            value->str_value=NULL;
            break;
        }
    }
    return value;
}

void TITAN_read_dcb() {
    unsigned str_size=0;
    for (unsigned _i=0; _i<executable->dcb_size; _i++) {
        str_size = TITAN_read_short();
        char *name = TITAN_read_string(str_size);
        Value *value = TITAN_process_value();
        executable->dcb[_i] = (Constant){name, value, executable->address};
        executable->address++;
    }
}

void TITAN_read_data() {
    unsigned str_size=0;
    for (unsigned _i=0; _i<executable->data_size; _i++) {
        str_size = TITAN_read_short();
        char *name = TITAN_read_string(str_size);
        Value *value = TITAN_process_value();

        char *str_value = memalloc(value->type->size);
        str_value = value->str_value;
        value->str_value = str_value;

        executable->data[_i] = (Variable){name, value, executable->address};
        executable->address++;
    }
}

Operation *TITAN_read_instruction() {
    Operation *operation = (Operation*)memalloc(sizeof(Operation));
    operation->opcode = TITAN_read_short();
    operation->target = memalloc(sizeof(Value));
    operation->source = memalloc(sizeof(Value));

    if (operation->opcode == INS_CALL) {
        DataType *data_type;
        data_type = &TYPE_IDENTIFIER;
        unsigned str_size = TITAN_read_short();
        char *identifier = TITAN_read_string(str_size);
        operation->target->type=data_type;
        operation->target->u32_value=0;
        operation->target->str_value=identifier;
    }
    else if (operation->opcode == INS_PUSH) {
        operation->target = TITAN_process_value();
    }
    else if (operation->opcode == INS_RET) {}
    else {
        // This fucking code is held together with duct tape but turning everything
        // to little-endian fixed a lot of things
        operation->target = TITAN_process_value();
        operation->source = TITAN_process_value();
    }
    return operation;
}

void TITAN_read_text() {
    uint16_t str_size=0;
    for (unsigned _i=0; _i<executable->text_size; _i++) {
        unsigned signature = TITAN_read_uint32(4);
        if (signature != FUNCTION_SIGNATURE) {
            continue;
        }

        str_size = TITAN_read_short();
        char *name = TITAN_read_string(str_size);
        uint16_t nparams = TITAN_read_short();
        uint16_t ninstrs = TITAN_read_short();
        char **parameters=memalloc(sizeof(char*)*nparams);

        for(unsigned _k=0; _k<nparams; _k++) {
            uint16_t psize = TITAN_read_short();
            parameters[_k] = TITAN_read_string(psize);
        }

        // Debugging output
        Operation **fn_body = memalloc(sizeof(Operation)*ninstrs);

        for (unsigned _j=0; _j<ninstrs; _j++) {
            fn_body[_j] = TITAN_read_instruction();
        }

        executable->text[_i] = (Function){name, parameters, fn_body, ninstrs, nparams, executable->address};
        executable->address++;
    }
}

unsigned TITAN_exe_interpreter(File *source) {
    TITAN_reset_pos();
    TITAN_set_file(source);

    if (source->base_size.size < 4) { return 1; }
    unsigned signature = TITAN_read_uint32(4);
    if (signature != TITAN_SIGNATURE) { return 2; }
    unsigned dcb_size = TITAN_read_uint32(4);
    unsigned data_size = TITAN_read_uint32(4);
    unsigned text_size = TITAN_read_uint32(4);

    executable = memalloc(sizeof(Executable));
    executable->dcb = memalloc(sizeof(Constant) * dcb_size);
    executable->data = memalloc(sizeof(Variable) * data_size);
    executable->text = memalloc(sizeof(Function) * text_size);
    executable->dcb_size = dcb_size;
    executable->data_size = data_size;
    executable->text_size = text_size;
    executable->address = 0x0000;

    TITAN_read_dcb();
    TITAN_read_data();
    TITAN_read_text();

    for (unsigned _i=0; _i<dcb_size; _i++) {
        printf_serial("DCB: %s [%x] = %x\n", executable->dcb[_i].name, executable->dcb[_i].address, executable->dcb[_i].value->u32_value);
    }

    for (unsigned _i=0; _i<data_size; _i++) {
        printf_serial("DATA: %s [%x] = %x\n", executable->data[_i].name, executable->data[_i].address, executable->data[_i].value->u32_value);
    }

    return VM_run_executable(executable);
}

void test_titan() {
    TITAN_exe_interpreter(VFS_get_file("/sys/bin/testbin"));
}