#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (3, ld, 8)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (3, st, 8)
    default: exec_inv(s);
  }
}

static inline def_EHelper(r2r){
  switch (s->isa.instr.s.funct3) {

    default: exec_inv(s);
  }
}

static inline def_EHelper(r2i){
  switch (s->isa.instr.s.funct3) {
    EXW (0, addi, 8)
    
    default: exec_inv(s);
  }
}

static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  //opcode第0 1 位都必须为1
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)
    IDEX (0b01101, U, lui)
    //TODO: add decode of R and B
    // IDEX (0b01100, R, r2r)
    // IDEX (0b11000, B, branch)
    IDEX (0b00101, U, auipc)
    IDEX (0b11011, J, jal)
    IDEX (0b00100, I, r2i)
    EX   (0b11010, nemu_trap)
    IDEX (0b11001, I, jalr)
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_d(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);
  
  reset_zero();

  return s.seq_pc;
}
