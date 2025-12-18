# F-WAL
This is gonna be the ultimate OS that destroys Windows and Linux completely. So, get ready for the ride now.
## How to run it?
Insert your pendrive, then run the command:<br>
```fdisk -l```<br><br>
Check the partition your pendrive is loaded to, e.g. ```/dev/sdX```<br>
In my case, it's ```/dev/sdb```<br><br>
⚠️⚠️⚠️ <strong>Note:</strong> Double check it. No. Triple check it. Clean your glasses and then come back. If you mess it up you may destroy the wrong partition. In short you may blow up your damn PC ⚠️⚠️⚠️<br>
If you are willing to ⚠️wipe out everything⚠️ from your pendrive, only then modify the ```Makefile```.<br>
Change the line that says ```USB      = /dev/sdb``` to your pendrive partition. If your partition is ```/dev/sdc```, then change the line to ```USB      = /dev/sdc```.<br><br>
After that, all you have to do is run the following command once:<br>
```make partition```<br>
It will create a FAT32 partition to your pendrive. Then everytime you want to run the OS, just use the command:<br>
```make clean && make && qemu-system-i386 -drive format=raw,file=/dev/sdX```


