/*
struct user_data;

asmlinkage long sys_add_user(struct user_data* input_data);
asmlinkage long sys_del_user(const char* surname, unsigned int len);
asmlinkage long sys_get_user(const char* surname, unsigned int len, struct user_data* output_data);