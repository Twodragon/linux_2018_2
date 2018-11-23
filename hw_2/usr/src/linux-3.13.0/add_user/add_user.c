#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>

struct user_data{
    char name[12];
    char surname[23];
    char age[3];
    char phone_number[20];
    char mail_address[50];
};


asmlinkage long sys_add_user(struct user_data* input_data){
    char command[150];
    sprintf(command, "add %s %s %s %s %s",
            input_data->name,
            input_data->surname,
            input_data->age,
            input_data->phone_number,
            input_data->mail_address);

    struct file *file;
    loff_t pos = 0;
    int fd;

    mm_segment_t old_fs = get_fs();
    set_fs(KERNEL_DS);

    fd = sys_open("/dev/phone_book", O_WRONLY|O_CREAT, 0644);
    if (fd >= 0) {
      sys_write(fd, command, 150);
      file = fget(fd);
      if (file) {
        vfs_write(file, command, 150, &pos);
        fput(file);
      }
      sys_close(fd);
    }
    set_fs(old_fs);

    return 0;
}