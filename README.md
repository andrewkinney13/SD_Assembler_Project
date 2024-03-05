Assembler project for my Software Design class in Fall, 2022. Translates an assembly language text file into machine language, runs that machine code in an emulator. An example of an assebly code file is as follows:

;this is a test
        org    100
hi     read    x;comment immediately after statement
       load    x
hay   store    y ; This is the another comment.
      write    x
         bp    hi
       halt
;test
x       dc     5
y       ds     99
b       dc     555
a       dc     100
        end
