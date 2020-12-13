### Random Number Linux Kernel 
**Requirements:**
- Write a linux kernel, which used to generate random number.
- Create a character device that allows process in userspace to open and read random number from linux kernel in kernelspace.

**Quick guide:**

Build:
```
cd randevice
make
```
*Note: Before running below command, please switch to root mode*

Insert module:
```
insmod randevice.ko
```
Remove module:
```
rmmod randevice
```
Get random number for randevice module:
```
./user_space
```
*Note: If command prompt has announced that "False to open randevice", please run chmod command below:*
```
chmod a+r /dev/randevice
