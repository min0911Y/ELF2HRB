[BITS 32]
global printstr,exit
section .text
printstr:
  push eax
  push edx
  mov eax,5
  mov edx,[esp + 4 + 8]
  int 0x36
  pop edx
  pop eax
  ret
exit:
  push eax
  mov eax,0x1e
  int 36h
  pop eax
  ret