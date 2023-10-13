cd ..
ls
make -j 4
clear
ls
make modules -j 4
sudo make modules_install -j 4
sudo make install
sudovim /etc/default/grub
sudo vi /etc/default/grub
sudo update-grub
uname -r
exit
df -h
clear
ls
df -s
df -h
lvextend -l +100%FREE /dev/mapper/ubuntu--vg-ubuntu--lv
sudo lvextend -l +100%FREE /dev/mapper/ubuntu--vg-ubuntu--lv
df -h
sudo resize2fs /dev/mapper/ubuntu--vg-ubuntu--lv
df -h
clear
ls
uname -r
sudo apt build-dep linux
sudo apt-get update
sudo apt build-dep linux
sudo vi /etc/apt/sources.list
sudo apt build-dep linux
apt-get update
sudo apt-get update
sudo apt build-dep linux
clear
ls
wget https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.4.214.tar.gz
ls
clear
ls
tar -xvf linux-5.4.214.tar.gz 
ls
clear
df -h
ls
cd linux-5.4.214/
ls
clear
ls
make menuconfig
sudo make menuconfig
apt install libncurses-dev
sudo apt install libncurses-dev
make menuconfig
ls
cd kernel/
ls
clear
ls
vi my_syscall.c
ls
cat my_syscall.c 
vi Makefile 
vi ../include/uapi/asm-generic/unistd.h
vi ../include/linux/syscalls.h
vi ../include/uapi/asm-generic/unistd.h
tmux
tmux ls
tmux attach -t 0
exit
