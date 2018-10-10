#include <linux/kernel.h>
#include <linux/slab.h>

asmlinkage long sys_del_user(const char* surname, unsigned int len){
    char *command = (char*)kmalloc(len + 3);
    sprintf(command, "rm %s", surname);

    FILE* f = fopen("/dev/phone_book", "w");
    fputs(command, f);
    fclose(f);
    kfree(command);
    return 0;
}