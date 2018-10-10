#include <linux/kernel.h>
#include <linux/slab.h>

struct user_data{
    char name[12];
    char surname[23];
    char age[3];
    char phone_number[20];
    char mail_address[50];
};


asmlinkage long sys_get_user(const char* surname, unsigned int len, struct user_data* output_data){
    char *command = (char*)kmalloc(len + 4);
    printf(command, "get %s", surname);
    FILE* f = fopen("/dev/phone_book", "w");
    fputs(command, f);
    fclose(f);

    f = fopen("/dev/phone_book", "r");
    fscanf(f, "GOT!\nname: %s\nsurname: %s\nage: %s\nphone_number: %s\nmail_address: %s\n",
           output_data->name,
           output_data->surname,
           output_data->age,
           output_data->phone_number,
           output_data->mail_address);
    fclose(f);
    kfree(command);
    return 0;
}