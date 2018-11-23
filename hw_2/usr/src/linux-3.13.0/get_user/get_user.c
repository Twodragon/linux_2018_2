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


asmlinkage long sys_get_user(const char* surname, unsigned int len, struct user_data* output_data){
    char *command = (char*)kmalloc(len + 4, GFP_KERNEL);
    sprintf(command, "get %s", surname);

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


    char buf[1];
    char answer[300];
    int answer_i = 0;

    old_fs = get_fs();
    set_fs(KERNEL_DS);

    fd = sys_open("/dev/phone_book", O_RDONLY, 0);
    if (fd >= 0) {
      while (sys_read(fd, buf, 1) == 1)
	answer[answer_i] = buf[0];
      sys_close(fd);
    }
    set_fs(old_fs);

    sscanf(answer, "GOT!\nname: %s\nsurname: %s\nage: %s\nphone_number: %s\nmail_address: %s\n",
           output_data->name,
           output_data->surname,
           output_data->age,
           output_data->phone_number,
           output_data->mail_address);
    fclose(f);
    kfree(command);
    return 0;
}