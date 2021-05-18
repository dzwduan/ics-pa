//将立即数写入高20位，清除低12位
static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

//把符号位扩展的 20 位（左移 12 位）立即数加到 pc 上，结果写入 x[rd]。
//decode已经将imm译码出来了
static inline def_EHelper(auipc) {
  *s0 = cpu.pc + id_src1->imm;
  rtl_li(s,ddest,*s0);
  print_asm_template2(auipc);
}

//将立即数写入reg,不区分高20和低12
//rd = rs1 + imm
static inline def_EHelper(addi) {
  rtl_addi(s,ddest,dsrc1,id_src2->imm);
  print_asm_template2(addi);
}

