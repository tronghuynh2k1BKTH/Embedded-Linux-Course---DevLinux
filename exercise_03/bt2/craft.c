// #include <stdio.h>
// #include <unistd.h>

// int main() {
//     printf("Running ls -l using execlp...\n");
//     execlp("ls", "ls", "-l", NULL);
//     printf("This will not be printed if execlp() succeeds.\n");
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>

int main() {
    char *env_value = getenv("MY_ENV_VAR"); // Lấy giá trị biến môi trường

    if (env_value == NULL) {
        printf("MY_ENV_VAR is not set. Setting a default value...\n");
        setenv("MY_ENV_VAR", "Default Value", 1); // Đặt giá trị mặc định
        env_value = getenv("MY_ENV_VAR"); // Lấy lại giá trị sau khi đặt
    }

    printf("MY_ENV_VAR = %s\n", env_value); // In ra giá trị của biến môi trường

    return 0;
}