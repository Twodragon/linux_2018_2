#include <linux/kernel.h>
#include <linux/slab.h>

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
    FILE* f = fopen("/dev/phone_book", "w");
    fputs(command, f);
    fclose(f);
    return 0;
}