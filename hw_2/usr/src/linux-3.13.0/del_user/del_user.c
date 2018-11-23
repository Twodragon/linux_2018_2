#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

asmlinkage long sys_del_user(const char* surname, unsigned int len){
    char *command = (char*)kmalloc(len + 3, GFP_KERNEL);
    sprintf(command, "rm %s", surname);

    struct file *file;
    loff_t pos = 0;
    int fd;

    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    fd = sys_open("/dev/phone_book", O_WRONLY|O_CREAT, 0644);
    if (fd >= 0) {
      sys_write(fd, command, strlen(command));
      file = fget(fd);
      if (file) {
        vfs_write(file, command, strlen(command), &pos);
        fput(file);
      }
      sys_close(fd);
    }
    set_fs(old_fs);

    kfree(command);
    return 0;
}