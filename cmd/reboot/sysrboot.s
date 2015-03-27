/ reboot == C library

/        reboot(f)

.globl  _reboot, cerror

.reboot = 55.

_reboot:
        mov     r5,-(sp)
        mov     sp,r5
        mov     4(r5),0f
        sys     0; 9f
        .data
9:
        sys     .reboot; 0:..
        .text
        bec     1f
        jmp     cerror
1:
        mov     (sp)+,r5
        rts     pc
