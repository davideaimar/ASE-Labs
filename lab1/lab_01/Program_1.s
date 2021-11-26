.data

a:   .byte  0, -2,  0, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
b:   .byte -1, -4,  0, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
c:   .space 30
threshold_high: .byte 0
threshold_low: .byte 0
max: .byte 0
min: .byte 0

.code 

main:
    daddui r1, r0, 30 ; r1 counter register    
    dadd r2, r0, r0 ; r2 index register  (to increment by 1 in order to load byte per byte)
    dadd r3, r0, r0 ; r3 threshold_high counter
    dadd r4, r0, r0 ; r4 threshold_low counter
    lb r21, a(r2) ; r31 = a[j]
    lb r20, b(r2) ; r30 = b[j]
    dadd r5, r31, r30 ; r5 = min
    dadd r6, r31, r30 ; r6 = max

loop:
    dadd r31, r21, r20 ; r31 = c[j] = a[j] + b[j]
    daddi r1, r1, -1
    slti r30, r31, 0 ; set r30=1 if c[j] < 0
    sb r31, c(r2) ; save in memory c[j]
    slti r29, r30, 1 ; if r30 == 0 (c[j]>0) set r29 to 1
    slt r28, r31, r5 ; r28=1 if c[j] < min --> need to save min
    slt r27, r6, r31 ; r27=1 if max < c[j] --> need to save max
    daddi r2, r2, 1
    dadd r3, r3, r29
    dadd r4, r4, r30

    lb r21, a(r2) ; r31 = a[j]

    beqz r28, notmin
    daddi r5, r31, 0
notmin:
    beqz r27, notmax
    daddi r6, r31, 0
notmax:
    
    lb r20, b(r2) ; r30 = b[j]

    bnez r1, loop

    sb r3, threshold_high(r0) ; save in memory 
    sb r4, threshold_low(r0) ; save in memory 
    sb r5, min(r0) ; save in memory the min
    sb r6, max(r0) ; save in memory the max
    
    HALT; end
