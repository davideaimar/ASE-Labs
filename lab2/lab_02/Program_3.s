.data

i:  .double -28.20, -105.66, 58.11, 62.22, 34.46, 31.86, 60.48, 67.97, 41.20, 18.66
    .double 57.16, 25.00, 24.56, 78.92, 28.62, 4.01, 50.55, 105.50, 112.12, 18.38
    .double 37.54, 11.26, 18.76, 15.03, 60.97, 18.05, 95.58, 70.65, 22.01, 47.75
w:  .double 23.91, 10.35, 95.41, 33.13, 97.45, 51.64, 16.96, 43.10, 80.98, 79.88
    .double 92.14, 20.56, 29.74, 77.50, 110.69, 21.56, 76.58, 31.71, 61.74, 25.03
    .double 60.01, 32.75, 87.73, 27.37, 59.11, 5.87, 39.90, 25.19, 21.31, 85.12
b:  .double 10
y:  .space 8

.code 

main:
    daddi r31, r0, 0x7ff0 ; mask for NaN check
    daddi r1, r0, 30
    daddi r2, r0, 0
    dsll r31, r31, 31
    l.d f1, b(r0) ; f1 = b constant
    ; f2 = cumulative register
    

loop:
    l.d f3, i(r2) ; f3 = i[j]
    
    l.d f4, w(r2) ; f4 = w[j]
    daddi r2, r2, 8
    mul.d f3, f3, f4
    daddi r1, r1, -1
    add.d f2, f2, f3
    
    bnez r1, loop

    add.d f2, f2, f1
    dsll r31, r31, 17
    mfc1 r3, f2
    and r3, r31, r3
    bne r31, r3, isnum
isnan:
    mtc1 r0, f2
isnum:
    s.d f2, y(r0)

    HALT
