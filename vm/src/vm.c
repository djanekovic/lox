#include <stdio.h>
#include <assert.h>

#include "clox/compiler.h"
#include "clox/vm.h"

#define DEBUG_TRACE_EXECUTION
#ifdef DEBUG_TRACE_EXECUTION
#include "clox/debug.h"
#endif // DEBUG_TRACE_EXECUTION

VM vm;

static void reset_stack() {
  vm.stack_top = vm.stack;
}

void init_vm() {
  reset_stack();
}

void free_vm() {
}

void push(Value value) {
  *vm.stack_top = value;
  vm.stack_top++;
}

Value pop() {
  assert(vm.stack_top >= vm.stack);
  vm.stack_top--;
  return *vm.stack_top;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)   \
  do {                  \
    Value b = pop();    \
    Value a = pop();    \
    push(a op b);       \
  } while(0)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    fprintf(stdout, "    ");
    for (Value *slot = vm.stack; slot < vm.stack_top; slot++) {
      fprintf(stdout, "[ ");
      print_value(*slot);
      fprintf(stdout, " ]");
    }
    fprintf(stdout, "\n");
    disassemble_instruction(vm.chunk, (size_t)(vm.ip - vm.chunk->code));
#endif // DEBUG_TRACE_EXECUTION
    uint8_t instr;
    switch(instr = READ_BYTE()) {
      case OP_CONSTANT: push(READ_CONSTANT()); break;
      case OP_ADD:      BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;
      case OP_NEGATE:   push(-pop()); break;
      case OP_RETURN:
        print_value(pop());
        fprintf(stdout, "\n");
        return INTERPRET_OK;
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(const char *source) {
  compile(source);
  return INTERPRET_OK;
}
