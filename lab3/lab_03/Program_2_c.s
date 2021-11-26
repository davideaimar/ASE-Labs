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

; PROGRAM program_2_c
; 621 RAW stalls
; 0 WAR stalls
; 70 structural stalls
; 0 branch taken stalls

main:             
    daddi r1, r0, 9 
    ;-------------------------------- 
      daddi r2, r0, 0   
      daddi r3, r0, 0 

      l.d f1, v2(r2) ; f1 = v2[i]       
      l.d f2, v3(r2) ; f2 = v3[i]
      l.d f3, v4(r2) ; f3 = v4[i]
      mul.d f7, f1, f2 ; f7 = v2*v3      
      l.d f0, v1(r2) ; f4 = v1[i]  
            daddi r4, r0, 8  
            daddi r5, r0, 8
            l.d f9, v2(r4) ; f9 = v2[i]     
            l.d f10, v3(r4) ; f10 = v3[i]
            l.d f16, v1(r4) ; f12 = v1[i]
            mul.d f15, f9, f10 ; f15 = v2*v3
      add.d f4, f0, f7 ; f4 = v1 + v2*v3 
            l.d f11, v4(r4) 
            add.d f12, f16, f15  
      mul.d f5, f4, f3 ; f5 = (v1+v2*v3)*v5        
      div.d f6, f5, f1 ;    
    ;-------------------------------- 
                   
            
            mul.d f13, f12, f11 
                  daddi r6, r0, 16  
                  daddi r7, r0, 16

                  l.d f17, v2(r6) ; f17 = v2[i]       
                  l.d f18, v3(r6) ; f18 = v3[i]
                  l.d f19, v4(r6) ; f19 = v4[i]      
                  l.d f16, v1(r6) ; f20 = v1[i]
            div.d f14, f13, f9     
    ;-------------------------------- 
        
      
                  mul.d f23, f17, f18 ; f23 = v2*v3
                        daddi r8, r0, 24 
                        daddi r9, r0, 24
                  add.d f20, f16, f23 ; f20 = v1 + v2*v3  
                  mul.d f21, f20, f19 ; f21 = (v1+v2*v3)*v5  
                        l.d f25, v2(r8) ; f25 = v2[i]       
                        l.d f26, v3(r8) ; f26 = v3[i]
                        l.d f27, v4(r8) ; f27 = v4[i] 
                        mul.d f31, f25, f26 ; f31 = v2*v3
                        l.d f24, v1(r8) ; f28 = v1[i]       
                  div.d f22, f21, f17 ; 
    ;--------------------------------  
                        
                        add.d f28, f24, f31 ; f28 = v1 + v2*v3  
                        mul.d f29, f28, f27 ; f29 = (v1+v2*v3)*v5        
                        div.d f30, f29, f25 ;         
    ;--------------------------------           

loop:
    ;-----------------------
    daddi r1, r1, -1 
    ;-----------------------
      daddi r2, r2, 32 
      l.d f1, v2(r2) ; f1 = v2[i]       
      l.d f2, v3(r2) ; f2 = v3[i]
      l.d f3, v4(r2) ; f3 = v4[i]  
      mul.d f7, f1, f2 ; f7 = v2*v3    
      l.d f0, v1(r2) ; f4 = v1[i]  
      s.d f4, v5(r3)   
      add.d f4, f0, f7 ; f4 = v1 + v2*v3         
      s.d f5, v6(r3)  
      mul.d f5, f4, f3 ; f5 = (v1+v2*v3)*v5               
      s.d f6, v7(r3) 
            daddi r4, r4, 32  
            l.d f9, v2(r4)        
            l.d f10, v3(r4) 
            l.d f11, v4(r4)
      div.d f6, f5, f1 ; 
      daddi r3, r3, 32
    ;--------------------------------
         
            mul.d f15, f9, f10  
            l.d f16, v1(r4) 
            s.d f12, v5(r5)   
            add.d f12, f16, f15        
            s.d f13, v6(r5)  
            mul.d f13, f12, f11               
            s.d f14, v7(r5) 
                  daddi r6, r6, 32 
                  l.d f17, v2(r6) ; f17 = v2[i]       
                  l.d f18, v3(r6) ; f18 = v3[i]
                  l.d f19, v4(r6) ; f19 = v4[i]
            div.d f14, f13, f9 
            daddi r5, r5, 32 
    ;--------------------------------
                     
                  mul.d f23, f17, f18 ; f23 = v2*v3   
                  l.d f16, v1(r6) ; f20 = v1[i]  
                  s.d f20, v5(r7)   
                  add.d f20, f16, f23 ; f20 = v1 + v2*v3         
                  s.d f21, v6(r7)  
                  mul.d f21, f20, f19 ; f21 = (v1+v2*v3)*v5               
                  s.d f22, v7(r7) 
                        daddi r8, r8, 32 
                        l.d f25, v2(r8) ; f25 = v2[i]       
                        l.d f26, v3(r8) ; f26 = v3[i]
                        l.d f27, v4(r8) ; f27 = v4[i] 
                  div.d f22, f21, f17 ; 
                  daddi r7, r7, 32
    ;--------------------------------
                          
                        mul.d f31, f25, f26 ; f31 = v2*v3   
                        l.d f24, v1(r8) ; f28 = v1[i]  
                        s.d f28, v5(r9)   
                        add.d f28, f24, f31 ; f28 = v1 + v2*v3         
                        s.d f29, v6(r9)  
                        mul.d f29, f28, f27 ; f29 = (v1+v2*v3)*v5               
                        s.d f30, v7(r9) 
                        div.d f30, f29, f25 ; 
    ;--------------------------------

    bnez r1, loop 
                        daddi r9, r9, 32

      s.d f4, v5(r2)           
      s.d f5, v6(r2)                 
      s.d f6, v7(r2)

            s.d f12, v5(r4)           
            s.d f13, v6(r4)                 
            s.d f14, v7(r4)

                  s.d f20, v5(r6)           
                  s.d f21, v6(r6)                 
                  s.d f22, v7(r6)  

                        s.d f28, v5(r8)           
                        s.d f29, v6(r8)                 
                        s.d f30, v7(r8)                

    HALT