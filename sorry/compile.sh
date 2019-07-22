name=sorry

cc65_dir=../cc65/bin

CC65_HOME=../

$cc65_dir/cc65 -Oirs $name.c --add-source
$cc65_dir/ca65 crt0.s
$cc65_dir/ca65 $name.s -g

$cc65_dir/ld65 -C nrom_32k_vert.cfg -o $name.nes crt0.o $name.o nes.lib -Ln labels.txt

rm *.o

mv labels.txt BUILD/
mv $name.s BUILD/
mv $name.nes BUILD/

# pause

open BUILD/$name.nes
