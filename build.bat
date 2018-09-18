set asm_name=hello-world.asm
cp %asm_name% libraries\
cd libraries\
..\rgbds\rgbasm.exe -o hello.obj %asm_name%
..\rgbds\rgblink.exe -m hello.map -n hello.sym -o hello.gb hello.obj
..\rgbds\rgbfix.exe -v -p 0 ..\hello.gb
rm %asm_name%
rm hello.obj
mv hello.* ..\