    .data

a:   .byte  -10, -9,  -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
b:   .byte  -10, -9,  -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
c:   .space 30
max: .byte 0
min: .byte 0
th: .byte 0
tl: .byte 0

    .code 

main:
    daddui r1, r0, 30
    daddui r2, r0, 0
    daddui r3, r0, 0
    daddui r4, r0, 0

    lb r21, a(r2)
    lb r20, b(r2)

    dadd r5, r31, r30 
    dadd r6, r31, r30 

loop:
    dadd r31, r21, r20
    
    slti r30, r31, 0
    sb r31, c(r2)
    slti r29, r30, 1
    slt r28, r31, r5
    slt r27, r6, r31 
    
    dadd r3, r3, r29
    dadd r4, r4, r30

    beqz r28, notmin
    daddi r5, r31, 0

notmin:
    beqz r27, notmax
    daddi r6, r31, 0

notmax:
    daddi r2, r2, 1
    daddi r1, r1, -1

    lb r20, b(r2) 
    lb r21, a(r2)

    bnez r1, loop

    sb r5, min(r0) 
    sb r6, max(r0)
    sb r3, th(r0) 
    sb r4, tl(r0)
    
    
    HALT
