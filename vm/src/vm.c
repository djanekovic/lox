#include <stdio.h>
#include <stdarg.h>
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

static void runtime_error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  // we need -1 because we advance past each instr before
  // executing it
  size_t instruction = vm.ip - vm.chunk->code - 1;
  // check what line we were executing
  int line = vm.chunk->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack();
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

static Value peek(int distance) {
  return vm.stack_top[-1 - distance];
}


static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op)                        \
  do {                                                  \
    if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {   \
      runtime_error("Operands must be numbers.");       \
      return INTERPRET_RUNTIME_ERROR;                   \
    }                                                   \
    double b = AS_NUMBER(pop());                        \
    double a = AS_NUMBER(pop());                        \
    push(valueType(a op b));                            \
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
      case OP_ADD:      BINARY_OP(NUMBER_VAL, +); break;
      case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
      case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
      case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
      case OP_NEGATE:
        if (!IS_NUMBER(peek(0))) {
          runtime_error("Operand must be a number.");
          return INTERPRET_RUNTIME_ERROR;
        }

        push(NUMBER_VAL(-AS_NUMBER(pop())));
        break;
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
  Chunk chunk;
  init_chunk(&chunk);

  // compile the source and fill the chunk with bytecode
  if (!compile(source, &chunk)) {
    free_chunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  InterpretResult result = run();

  free_chunk(&chunk);
  return result;
}
