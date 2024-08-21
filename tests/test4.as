.extern a1  
a2: .entry a1
a1:  add r2,r3

main: lea a1,r2
a2:     mov #4,r3
mov #-4,r9

clr a1
rts
jsr *r5
jsr #4
rts t
clr t
t:  inc r2
inc t
inc #4
stop 4
