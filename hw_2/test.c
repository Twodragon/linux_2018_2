#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <linux/kernel.h>
#include <sys/syscall.h>

struct user_data{
    char name[12];
    char surname[23];
    char age[3];
    char phone_number[20];
    char mail_address[50];
};

int main()
{
    int ok = 0;
    struct user_data* usr = (struct user_data*) malloc(sizeof(struct user_data));

    sprintf(usr->name, "name");
    sprintf(usr->surname, "surname");
    sprintf(usr->age, "18");
    sprintf(usr->phone_number, "phone_number");
    sprintf(usr->mail_address, "mail_address");

    long int amma = syscall(318, usr); // add

    struct user_data* second_usr = (struct user_data*) malloc(sizeof(struct user_data));

    amma = syscall(320, "name", 4, second_usr); // get

    if ((strcmp(second_usr->name, "name") != 0) || \
	        (strcmp(second_usr->surname, "surname") != 0) || \
	        (strcmp(second_usr->age, "18") != 0) || \
	        (strcmp(second_usr->phone_number, "phone_number") != 0) || \
	        (strcmp(second_usr->mail_address, "mail_address") != 0)) {
        ok = -1;
    }

    amma = syscall(319, "name", 4); // delete

    struct user_data* third_usr = (struct user_data*) malloc(sizeof(struct user_data));

    amma = syscall(320, "name", 4, third_usr); // get deleted

    if (strcmp(third_usr->name, "none") != 0)
		ok  = -1;

    free(usr);
    free(second_usr);
    free(third_usr);

    return ok;
}