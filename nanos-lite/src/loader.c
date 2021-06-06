#include <proc.h>
#include <elf.h>


#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#if defined(__ISA_AM_NATIVE__)
# define EXPECT_TYPE EM_X86_64
#elif defined(__ISA_RISCV64__)
# define EXPECT_TYPE EM_RISCV  // see /usr/include/elf.h to get the right type 
#else
# error Unsupported ISA
#endif

#define RAMDISK_OFFSET 0

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  
  Elf_Phdr * ph  = NULL;

  char buf[1024];
  // ram -> buf , offset从0开始

  ramdisk_read((void *)buf,RAMDISK_OFFSET,1024);
  Elf_Ehdr * elf=NULL;

  const uint32_t ELF_MAGIC = 0x464c457f;
  assert(*(uint32_t *)buf==ELF_MAGIC);
  elf = (void*)buf;

  int i = 0;
  for(i=0;i<elf->e_phnum;i++) {
    //phentsize是每一个表项的大小  all entries are the same size
    //找到program header table的位置读取第i个表项
    ph = (Elf_Phdr *)(buf + elf->e_phoff + i*elf->e_phentsize);
    //refer to xv6-public
    if(ph->p_type != PT_LOAD)                   continue;
    if(ph->p_memsz < ph->p_filesz)              return -1;
    if(ph->p_vaddr + ph->p_memsz < ph->p_vaddr) return -1;

    ramdisk_read((void *)ph->p_vaddr,ph->p_offset,ph->p_filesz);
    //填充.bss全0
    memset((void *)(ph->p_vaddr+ph->p_filesz), 0, ph->p_memsz-ph->p_filesz);
  }

  uintptr_t entry = elf->e_entry;

  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  printf("entry 0x%x\n",entry);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

