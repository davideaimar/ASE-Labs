.data

v1:   .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
v2:   .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
v3:   .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
v4:   .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
      .double 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10
v5: .space 320
v6: .space 320
v7: .space 320
.code 

; ORIGINAL PROGRAM program_2
; 1680 RAW stalls
; 80 structural stalls
; 39 branch taken stalls

main:
    daddi r1, r0, 40                
    daddi r2, r0, -8                

loop:
    daddi r2, r2, 8 ;                
    l.d f1, v1(r2) ; f1 = v1[i]     
    l.d f2, v2(r2) ; f2 = v2[i]     
    l.d f3, v3(r2) ; f3 = v3[i]     
    l.d f4, v4(r2) ; f4 = v4[i]     

    mul.d f3, f2, f3               
    add.d f5, f3, f1               
    mul.d f6, f5, f4               
    div.d f7, f6, f2                

    s.d f5, v5(r2)                  
    s.d f6, v6(r2)                  
    s.d f7, v7(r2)                  

    daddi r1, r1, -1                
    bnez r1, loop                   

    HALT