/*
 *  sym2db.cpp
 *  Albert
 *
 *  Created by Matthias Melcher on 07.11.09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "sym2db.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

// #include <mxml.h>

FILE *in, *out;

// 0x0036340C ArProbEncodeLu1
// - begin data

// 0x00382324 TReservedContiguousMemory::New(char *) static
// - begin functions

// 0x003AF000 gROMMagicPointerTable
// - data

// (0x003afdc4 SYMcfunction  SYM... symbols)

// 0x0063AD05 MP0000  // end of symbols, data? at odd addresse?

// 0x0067FA44 Rupbitmap // pointers and data

// 0x00681CA0 RSSYM_2A  // two words

// 0x006853DC gEnum80DaysMonths

// 0x0071FC4C ROM$$Size

// 0x01A00000 FIQHandler  // jump table at run time

// 0x01C10858 TVoyagerPlatform::StartKeyboardDriver(unsigned char)  // end of jump table

// 0x04006400 gParamBlockFromImagePhysical  // ??

// 0x0C008400 gParamBlockFromImage  // globals??

// 0x0C107E14 _end  // last symbol, Rom Extension (REX) follows

// 0x007ec7fc end of REX



/*

 Another attempt at the Newton 2100 memory map (ROM 717006):

  0x0000.0000: ROM
      00.0000: CODE reset
      00.0004: CODE undefined instruction
      00.0008: CODE software int
      00.000c: CODE prefetch abort
      00.0010: CODE data abort
      00.0018: CODE IRQ
      00.001c: CODE FIQ
      00.0020: Data
      00.2000: Begin of Patch tables (list of branch instructions) -> 01.285c
      01.3000: Begin of Branch tables (branch instr.) -> 01.5e0c
      01.6000: Begin of ROM page patch table
      01.8400: CODE (Low level calls, intermixed with a few tables!)
      02.1438: CODE C++
      36.340c: Read Only Data, probaably HWR patterns
      38.2324: CODE C++, ends in some low level functions
      3a.f000: Magic Pointer Table (873 entries)
      3a.fda8: Symbols
      67.fa40: Binary Objects
      71.fc4c: ROM code end, beginning of REX (ROM Extension, may contain code)
      7e.c7fc: REX code end, rest is empty
  0x0080.0000: End of ROM
  0x01a0.0000: Jump Table in RAM
  0x01c1.085c: End of Jump Table
  0x0400.6400: Param Block From Image Physical ??
  0x0c00.0000: ??
  0x0c00.8400: system variables start RAM
  0x0c10.7e14: end system variables RAM
  0x0f00.0000: DMA registers
  0x0f18.0000: start of GPIO registers
  0x0f20.0000: other registers (reboot reason, etc.)
  0x2000.0000: Flash RAM mapped to various channels (8 bit, 16 bit)
  0x4000.0000: System RAM 32 bit wide
  0x6000.0000: Current Application (Newton Script)
  0xe000.0000: screen memory
  0xe001.2c00: End of screen memory (320 x 280 x 4 bit)

  0x0036.340C:

SWI's
 00      GetPortSWI
 01      PortSendSWI
 02      PortReceiveSWI
 03      EnterAtomicSWI
 04      ExitAtomicSWI
 05      GenericSWI
 06      GenerateMessageIRQ
 07      PurgeMMUTBLEntry
 08      FlushMMU
 09      FlushCache
 0a      GetCPUVersion
 0b      SemaphoreOpGlue
 0c      SetDomainRegister
 0d      SMemSetBufferSWI
 0e      SMemGetSizeSWI
 0f      SMemCopyToSharedSWI
 10      SMemCopyFromSharedSWI
 11      SMemMsgSetTimerParmsSWI
 12      SMemMsgSetMsgAvailPortSWI
 13      SMemMsgGetSenderTaskIdSWI
 14      SMemMsgSetUserRefConSWI
 15      SMemMsgGetUserRefConSWI
 16      SMemMsgCheckForDoneSWI
 17      SMemMsgMsgDoneSWI
 18      TurnOffCache
 19      TurnOnCache
 1b      MonitorDispatchSWI
 1c      MonitorExitSWI
 1d      MonitorThrowSWI
 1e      EnterFIQAtomicSWI
 1f      ExitFIQAtomicSWI
 20      MonitorFlushSWI
 21      PortResetFilterSWI
 22      DoSchedulerSWI
*/

unsigned char ROM[0x00800000];
unsigned int ROM_flags[0x00200000];

/**
 * Return the 4-byte word in rom
 */
unsigned int rom_w(unsigned int addr)
{
  if (addr>=0x00800000)
    return 0;
  else
    return (ROM[addr]<<24)|(ROM[addr+1]<<16)|(ROM[addr+2]<<8)|ROM[addr+3];
}

char rom_flags_is_set(unsigned int addr, unsigned int f)
{
  if (addr>=0x00800000) return 0;
  return ((ROM_flags[addr/4]&f) == f);
}

double rom_fixed(unsigned int addr)
{
  int si = (signed int)rom_w(addr);
  double d = si;
  d = d/65536.0;
  return d;
}

/**
 * Return a 8-byte real (double prcision floating point) in rom
 */
double rom_real(unsigned int addr)
{
  union { unsigned char c[8]; double v; };
  if (addr>=0x00800000)
    return 0.0;
  else {
    int i;
    for (i=0; i<8; i++)
      c[7-i] = ROM[addr+i];
    return v;
  }
}

void zeroFlags()
{
  memset(ROM_flags, 0, 0x00200000*sizeof(int));
}


void AsmFlush(FILE *f, const char *buf)
{
  char dbuf[4096];
  // manipulate the buffer so that all comments are either starting at column 0
  // or at column 48 if possible
  const char *s = buf;
  char *d = dbuf;
  int col=0, t=0, x=0;
  // walk to the comment character
  for (;;) {
    char c = *s;
    switch (c) {
      case 0: *d=0; x=1; break;
      case '@': x=1; break;
      case '\t': *d++=c; col = (col+8) & 0xfff8; s++; break;
      case ' ': *d++=c; col++; s++; break;
      default: *d++=c; col++; t=1; s++; break;
    }
    if (x) break;
  }
  // handle a comment if there is one
  if (*s=='@') {
    if (t) {
      // we have text, set the comment at col 48
      while (col<48) {
        *d++ = '\t';
        col = (col+8) & 0xfff8;
      }
      strcpy(d, s); // append the comment to the line
    } else {
      // no text yet: move the comment to the start
      strcpy(dbuf, s); // simply move the text to the start of the line
    }
  }
  fputs(dbuf, f);
}

void AsmPrintf(FILE *f, const char *pat, ...)
{
  static char buf[4096];
  static char *dst = buf;
  // concatenate segments until we can flush an entire line
  va_list va;
  va_start(va, pat);
  vsnprintf(dst, 4096, pat, va);
  va_end(va);
  dst = buf + strlen(buf);
  if (strchr(buf, '\n')) {
    AsmFlush(f, buf);
    dst = buf; buf[0] = 0;
  }
}

const char *get_plain_symbol_at(unsigned int addr)
{
  AlData *data = gMemoryMap.find(addr);
  if (data)
    return data->label();
  else
    return 0;
}

int fillType(const char **srcp, char *dst, const char *sym)
{
  static int depth = 0;

  int len, rpt, idx;
  char pre[1024], post[1024];
  pre[0] = 0; post[0] = 0;
  const char *src = *srcp;
  char c = *src++;
  *srcp = src;
  if (c==0)
    return 0;
  depth++;
  for (;;) {
    switch (c) {
      case 0:
        depth--; return 0; // unexpected!
      case 'P':
        if (*src=='C') { strcat(post, "const "); src++; }
        strcat(post, "*"); break;
      case 'R':
        if (*src=='C') { strcat(post, "const "); src++; }
        strcat(post, "&"); break;
      case 'C': strcat(post, "const "); break;
      case 'U': strcat(pre, "unsigned "); break;
      case 'S': strcat(pre, "signed "); break;
      case 'V': strcat(pre, "volatile "); break;
      case 'F': { // PFiii_v  ->  void (*)(int, int, int)
        if (depth>3) {
          return 0;
        }
        char buf[4096];
        char args[4096];
        args[0] = 0;
        for (;;) {
          if (fillType(&src, buf, sym)==0)
            break;
          if (args[0])
            strcat(args, ", ");
          strcat(args, buf);
        }
        if (src[-1]=='_') {
          fillType(&src, buf, sym);
        }
        sprintf(dst, "%s (%s)(%s)", buf, post, args);
        *srcp = src;
        depth--; return 1; }
      case 'M': { // Pointer to member, followed by two arbitrary types
        char buf[4096];
        fillType(&src, buf, sym);
        strcat(post, buf);
        strcat(post, "::");
        break; }
      case 'Q': {
        char buf[4096];
        int i;
        rpt = (*src++)-'0';
        for (i=rpt; i>0; i--) {
          fillType(&src, buf, sym);
          strcat(pre, buf);
          if (i>1)
            strcat(pre, "::");
        }
        goto done; }
      case 'A':
        len = atoi(src);
        while (isdigit(*src)) src++;
        sprintf(post, "[%d]", len);
        if (*src=='_') c = *src++;
        goto done;
      case 'T':
        idx = (*src++)-'0';
        sprintf(pre, "#1%d", idx);
        goto done;
      case 'N':
        rpt = (*src++)-'0';
        idx = (*src++)-'0';
        sprintf(pre, "#%d%d", rpt, idx); // FIXME: this does not work if called in a recursion!
        goto done;
      case 'l': strcat(pre, "long"); goto done;
      case 'd': strcat(pre, "double"); goto done;
      case 's': strcat(pre, "short"); goto done;
      case 'i': strcat(pre, "int"); goto done;
      case 'c': strcat(pre, "char"); goto done;
      case 'v': strcat(pre, "void"); goto done;
      case 'e': strcat(pre, "..."); goto done;
        //case '_': strcat(post, ")"); goto done; // end of function pointer arguments
      case '0'...'9':
        len = atoi(src-1);
        while (isdigit(c)) c = *src++;
        strncat(pre, src-1, len);
        src = src + len -1;
        goto done; // verbatim copy
      case '_':
        depth--; return 0;
      default:
        printf("\nUnknown type: '%c' (%s)\n", c, sym);
        depth--; return 0;
    }
    c = *src++;
    *srcp = src;
  }
done:
  strcpy(dst, pre);
  if (post[0]) strcat(dst, " ");
  strcat(dst, post);
  *srcp = src;
  depth--;
  return 1;
}

char *decodeCppSymbol(const char *sym) {
  static char buf[4096];
  buf[0] = 0;
  char isConst = 0;
  if (strstr(sym, "__")) {
    char name[4096], klass[4096];
    const char *src = sym;
    char *dst = name;
    char c = *src++;
    *dst++ = c;
    for (;;) {
      c = *src++;
      if (c==0) break;
      if (c=='_' && src[0]=='_' && src[1]!='_') {
        *dst = 0;
        break;
      }
      *dst++ = c;
    }
    if (c=='_') {
      src++;
      c = *src;
      if (isdigit(c)) {
        int len = atoi(src);
        while (isdigit(c)) c = *src++;
        memcpy(klass, src-1, len);
        klass[len] = 0;
        src+=len-1;
      } else {
        klass[0] = 0;
      }
      if (name[0]=='_' && name[1]=='_') {
        if (strcmp(name, "__ct")==0) strcpy(name, klass);
        else if (strcmp(name, "__dt")==0) sprintf(name, "~%s", klass);
        else if (strcmp(name, "__ne")==0) strcpy(name, "operator!=");
        else if (strcmp(name, "__as")==0) strcpy(name, "operator=");
        else if (strcmp(name, "__eq")==0) strcpy(name, "operator==");
        else if (strcmp(name, "__gt")==0) strcpy(name, "operator>");
        else if (strcmp(name, "__lt")==0) strcpy(name, "operator<");
        else if (strcmp(name, "__rs")==0) strcpy(name, "operator>>");
        else if (strcmp(name, "__ls")==0) strcpy(name, "operator<<");
        else if (strcmp(name, "__vc")==0) strcpy(name, "operator[]");
        else if (strcmp(name, "__nw")==0) strcpy(name, "operator new");
        else if (strcmp(name, "__dl")==0) strcpy(name, "operator delete");
        else if (strcmp(name, "__nw_v")==0) strcpy(name, "operator new[]");
        else if (strcmp(name, "__dl_v")==0) strcpy(name, "operator delete[]");
        else if (strcmp(name, "__opl")==0) strcpy(name, "operator (long)");
        else if (strcmp(name, "__opUs")==0) strcpy(name, "operator (unsigned short)");
        else if (strcmp(name, "__opPc")==0) strcpy(name, "operator (char*)");
        else if (strcmp(name, "__opd")==0) strcpy(name, "operator (double)");
        else if (strcmp(name, "__push_ddtor")==0) { /* unknown */ }
        else if (strcmp(name, "__GetErrNo")==0) strcpy(name, "_GetErrNo");
        else if (strcmp(name, "__pvfn")==0) { /* non-existing virtaul function (=0) */ }
        // pvfn pointer to a function returning a void
        else {
          fprintf(stderr, "decodeCppSymbol: unknown '%s'\n", name);
          // unknown shortcut, maybe a bad name, maybe intentional, maybe a symbol starting in a single "_"
        }
      }
      c = *src++;
      if (c=='S') { strcat(buf, "static "); c = *src++; }
      if (c=='C') { isConst = 1; c = *src++; }
      if (klass[0]) { strcat(buf, klass); strcat(buf, "::"); }
      strcat(buf, name);
      if (c=='F') {
        strcat(buf, "(");
        char prev[30][128];
        int i, rpt = 0, idx = 0;
        for (i=1;;i++) {
          char abuf[1024]; abuf[0] = 0;
          if (rpt==0) {
            if (fillType(&src, abuf, sym)==0) break;
            if (abuf[0]=='#') {
              rpt = abuf[1]-'0';
              idx = abuf[2]-'0';
            }
          }
          if (rpt) {
            strcpy(abuf, prev[idx]);
            rpt--;
          }
          strcpy(prev[i], abuf);
          if (i>1) strcat(buf, ", ");
          strcat(buf, abuf);
        }
        strcat(buf, ")");
      }
      if (isConst) strcat(buf, " const");
    }
  } else {
    return 0;
  }
  return buf;
}

const char *get_symbol_at(unsigned int addr)
{
#if 0
  // FIXME: this should return the CPP decoded function call
  AlData *data = gMemoryMap.find(addr);
  if (data) {
    AlCPPMethod *m = dynamic_cast<AlCPPMethod*>(data);
    if (m) {
      return m->prototype();
    } else {
      return 0L;
    }
  } else {
    return 0;
  }
#else
  const char *sym = get_plain_symbol_at(addr);
  if (!sym) {
    return 0;
  }
  const char *cps = decodeCppSymbol(sym);
  if (cps) {
    return cps;
  } else {
    return 0;
  }
#endif
}

class Klass {
  static Klass **pKlass;
  static int pn, pN;
  char *pName;
public:
  Klass(const char *name) {
    pName = strdup(name);
  }
  ~Klass() {
    free(pName);
  }
  static void add(const char *name) {
    int i;
    for (i=0; i<pn; i++) {
      if (strcmp(pKlass[i]->pName, name)==0)
        return;
    }
    if (pn==pN) {
      pN += 100;
      pKlass = (Klass**)realloc(pKlass, sizeof(Klass**)*pN);
    }
    pKlass[pn++] = new Klass(name);
  }
  static void write(FILE *out) {
    int i;
    for (i=0; i<pn; i++) {
      fprintf(out, "begin class\n");
      fprintf(out, "  sym %s\n", pKlass[i]->pName);
      fprintf(out, "  base FIXME\n");
      fprintf(out, "  size FIXME\n");
      fprintf(out, "  datac FIXME\n");
      fprintf(out, "end\n\n");
    }
  }
};

Klass **Klass::pKlass = 0;
int Klass::pn = 0, Klass::pN = 0;


/**
 * Convert a string into its individual arguments.
 * There is no leading bracket!
 */
void print_attributes(const char *args)
{
  const char *ket = strrchr(args, ')');
  if (ket) {
    if (strstr(args, "static"))
      fprintf(out, "  static\n");
    if (strstr(args, "const"))
      fprintf(out, "  const\n");
  }
}

/**
 * Convert a string into its individual arguments.
 * There is no leading bracket!
 */
void print_args(const char *line)
{
  int bc = 1;
  int argc = 0, i;
  char *args = strdup(line);
  char *argv[20], *s = args;
  // count the arguments and set seperating NUL's
  for (;;) {
    if (*s==')') break;
    if (*s==0) {
      printf("Unexpected end of argument list in '(%s'\n", line);
      break;
    }
    if (isspace(*s)) s++;
    argv[argc] = s;
    argc++;
    for (;;) {
      if (*s==0) {
        break;
      } else if (*s=='(') {
        bc++;
      } else if (*s==')') {
        bc--;
        if (bc==0) {
          *s = 0;
          s++;
          break;
        }
      } else if (*s==',') {
        if (bc==1) {
          *s = 0;
          s++;
          break;
        }
      }
      s++;
    }
  }
  // ignore single 'void' argument
  if (argc==1 && strcmp(argv[0], "void")==0)
    argc = 0;
  // now print all the args
  fprintf(out, "  argc %d\n", argc);
  for (i=0; i<argc; i++) {
    fprintf(out, "  arg %d %s\n", i, argv[i]);
  }
  free(args);
}


/**
 * Convert an address/symbol pair
 */
void convert(const char *s, unsigned int next)
{
  unsigned int addr;
  char sym[512];
  int n = sscanf(s, "0x%08X", &addr);
  if (n!=1) {
    printf("Failed to convert line '%s'\n", s);
  }
  strcpy(sym, s+11);
  int l = strlen(sym);
  if (sym[l-1]=='\n')
    sym[l-1] = 0;
  /*
  00.0000: CODE reset
  00.0004: CODE undefined instruction
  00.0008: CODE software int
  00.000c: CODE prefetch abort
  00.0010: CODE data abort
  00.0018: CODE IRQ
  00.001c: CODE FIQ
  00.0020: Data
  00.2000: Begin of Patch tables (list of branch instructions) -> 01.285c
  01.3000: Begin of Branch tables (branch instr.) -> 01.5e0c
  01.6000: Begin of ROM page patch table
  01.8400: CODE (Low level calls, intermixed with a few tables!)
  02.1438: CODE C++
  36.340c: Read Only Data
  38.2324: CODE C++, ends in some low level functions
  3a.f000: Magic Pointer Table (873 entries)
  3a.fda8: Symbols
  67.fa40: Binary Objects
  71.fc4c: ROM code end, beginning of REX (ROM Extension, may contain code)
  7e.c7fc: REX code end, rest is empty
  */
  // lets interprete the different possible symbols that we can recognize at this point
  char *bra = strchr(sym, '(');
  char *ket = strrchr(sym, ')');
  char *cpp = strstr(sym, "::");
  if (addr>=0x00800000) {
    fprintf(out, "begin RAM\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  } else if (cpp && bra) { // this is a C++ class function
    *cpp = 0;
    *bra = 0;
    char ret = 1;
    fprintf(out, "begin cpp_member_function\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  class %s\n", sym);
    Klass::add(sym);
    fprintf(out, "  sym %s\n", cpp+2);
    if (strcmp(sym, cpp+2)==0) { fprintf(out, "  ctor\n"); ret = 0; }
    if (cpp[2]=='~' && strcmp(sym, cpp+3)==0) { fprintf(out, "  dtor\n"); ret = 0; }
    if (ket) print_attributes(ket+1);
    print_args(bra+1);
    if (ret) fprintf(out, "  returns FIXME\n"); // unknown return type
    fprintf(out, "end\n\n");
  } else if (bra) { // this is a C++ function outside a class
    *bra = 0;
    fprintf(out, "begin cpp_function\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    if (ket) print_attributes(ket+1);
    print_args(bra+1);
    fprintf(out, "  returns FIXME\n"); // unknown return type
    fprintf(out, "end\n\n");
  } else if (rom_w(addr)==0xE1A0C00D) { // find "C" functions: mov r12,sp
    fprintf(out, "begin c_function\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "  argc FIXME\n"); // unknown argument count
    fprintf(out, "  returns FIXME\n"); // unknown return type
    fprintf(out, "end\n\n");
  } else if (strncmp(sym, "SYM", 3)==0) {
    fprintf(out, "begin nsSymbol\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  } else if (sym[0]=='k' && isupper(sym[1])) {
    fprintf(out, "begin const_data\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  } else if (sym[0]=='g' && isupper(sym[1])) {
    fprintf(out, "begin global_data\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  } else if ((addr>=0x0036340C && addr<0x00382324)   // unknown data
          || (addr>=0x003AF000 && addr<=0x0071FC4C)  // Newton Script and more
          || (addr>=0x00000020 && addr<=0x000184d0)) // mostly jump tables in this segment
  {
    fprintf(out, "begin data\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  } else { // default:
    fprintf(out, "begin unknown\n");
    fprintf(out, "  at 0x%08X\n", addr);
    fprintf(out, "  next 0x%08X\n", next);
    fprintf(out, "  sym %s\n", sym);
    fprintf(out, "end\n\n");
  }
}


/**
 * Convert the symbols list into our new database format.
 */
int main(int argc, char **argv)
{
  FILE *rom = fopen("data/717006", "rb");
  if (!rom) {
    puts("Can't read ROM!");
    return -1;
  }
  fread(ROM, 0x00800000, 1, rom);
  fclose(rom);

  in = fopen("data/717006.symbols.txt", "rb");
  if (!in) {
    puts("Can't open Symbol file for reading.");
    return -1;
  }
  out = fopen("symbols.txt", "wb");
  if (!out) {
    puts("Can't open Database file for writing.");
    fclose(in);
    return -1;
  }
  fprintf(out, "\n");
  fprintf(out, "#import classes.txt\n");
  fprintf(out, "\n");

  unsigned int next = 0;
  char buf[1024], buf2[1024];
  fgets(buf, 1024, in);
  while (!feof(in)) {
    strcpy(buf2, buf);
    char *s = fgets(buf, 1024, in);
    if (s)
      sscanf(s, "0x%08X", &next);
    else
      next = 0x0C107E14; // "_end"
    convert(buf2, next);
    if (!s)
      break;
  }

  fclose(in);
  fclose(out);

  out = fopen("classes.txt", "wb");
  if (!out) {
    puts("Can't open class database file for writing.");
    return -1;
  }
  Klass::write(out);
  fclose(out);

  return 0;
}
