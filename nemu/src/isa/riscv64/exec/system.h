
//rtl_li  *dst =  imm
//rtl_mv  *dst = *src

//返回值与s0 s1一致
static inline rtlreg_t encode_csr(uint32_t csr) {
  rtlreg_t ret = 0;
  switch(csr) {
    //supervisor trap setup
    case 0x100: ret=cpu.sstatus.val;break;
    //case 0x102: ret=&cpu.sedeleg    ;break;  
    //case 0x103: ret=&cpu.sideleg    ;break;
    //case 0x104: ret=&cpu.sstatus.sie        ;break;
    case 0x105: ret=cpu.stvec      ;break;
    //case 0x106: ret=&cpu.scounteren;break;

    //supervisor trap setup
    case 0x140: ret=cpu.sscratch   ;break;
    case 0x141: ret=cpu.sepc       ;break;
    case 0x142: ret=cpu.scause     ;break;
    case 0x143: ret=cpu.stval      ;break;
    // case 0x144: ret=&cpu.sip        ;break;
    // case 0x180: ret=&cpu.satp       ;break;

    default: Log("wrong csr code 0x%x\n",csr);
  }
  return ret;
}


extern void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc);

//ecall sret
static inline def_EHelper(inter) {
  switch(s->isa.instr.csr.csr) {
    case 0x0: 
      raise_intr(s, 9, cpu.pc);
      print_asm("ecall");
      break;
    case 0x102:
      //置 pc 为 CSRs[sepc]，权限模式为 CSRs[sstatus].SPP，
      //CSRs[sstatus].SIE 为 CSRs[sstatus].SPIE，CSRs[sstatus].SPIE 为 1，CSRs[sstatus].spp 为 0。
      //cpu.pc = cpu.sepc;
      cpu.sstatus.sie = cpu.sstatus.spie;
      cpu.sstatus.spie = 1;
      rtl_j(s, cpu.sepc);
      //TODO():这里忽略了spp权限模式位
      print_asm("sret");
      break;
    default: Log("wrong csr code 0x%x\n",s->isa.instr.csr.csr);
  }
}

//t = CSRs[csr]; CSRs[csr] = x[rs1]; x[rd] = t
static inline def_EHelper(csrrw) {
  *s0 = encode_csr(id_src2->imm);
  rtl_mv(s,s0,dsrc1);
  rtl_mv(s,ddest,s0);
  print_asm_template3(csrrw);
}



//t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
static inline def_EHelper(csrrs) {
  *s0 = encode_csr(id_src2->imm);
  rtl_or(s,s1,s0,dsrc1);
  rtl_mv(s,s0,s1);
  rtl_mv(s,ddest,s0);
  print_asm_template3(csrrs);
}

//t = CSRs[csr]; CSRs[csr] = t &~x[rs1]; x[rd] = t
// static inline def_EHelper(csrrc) {
//   s0 = encode_csr(id_src2->imm);
//   rtl_not(s,s2,dsrc1);
//   rtl_or(s,s1,s0,s2);
//   rtl_mv(s,s0,s1);
//   rtl_mv(s,ddest,s0);
//   print_asm_template3(csrrc);
// }

// //x[rd] = CSRs[csr]; CSRs[csr] = zimm
// static inline def_EHelper(csrrwi) {
//   s0 = encode_csr(id_src2->imm);
//   rtl_mv(s,ddest,s0);
//   rtl_li(s,s0,id_src1->imm);
//   print_asm_template3(csrrwi);
// }

// //CSRs[csr] |= zimm
// static inline def_EHelper(csrrci) {
//   s0 = encode_csr(id_src2->imm);
//   rtl_or(s,s0,s0,id_src1->imm);
//   print_asm_template3(csrrci);
// }


// static inline def_EHelper(csrrsi) {
//   TODO();
//   //print_asm_template3(csrrsi);
// }
