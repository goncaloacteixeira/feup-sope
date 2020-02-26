#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 512
#define MAX_STUDENTS 100

struct Student{
    char name[50];
    int grade;
};



int display_students(struct Student* arr, int num) {
    for (int i = 0; i < num; i++) {
        write(STDOUT_FILENO, arr[i].name, 50);
        write(STDOUT_FILENO, " : ", 3);
        write(STDOUT_FILENO, (char*)(intptr_t)(arr[i].grade), 2);
        write(STDOUT_FILENO, "\n", 1);
    }
    return 0;
}

// Not yet completed
// TODO - print students' info

int main(int argc, char *argv[])
{
    int fd1, nr, nw;
    unsigned char buffer[BUFFER_SIZE];
    if (argc != 2) {
        printf("Usage: %s <source>\n", argv[0]);
        return 1;
    }

    fd1 = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd1 == -1) {
        perror(argv[1]);
        return 2;
    }

    int i = 0;
    nr = 1;
    struct Student student;

    struct Student students[MAX_STUDENTS];
    int j = 0;

    for (;;) {
        int name = 0;
        if (i % 2 == 0) {
            write(STDOUT_FILENO, "Name (^D to exit): ", 19);
            
            nr = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (nr <= 0) {
                break;
            }
            name = nr;
            strncpy(student.name, buffer, nr);
        } else {
            write(STDOUT_FILENO, "Grade: ", 7);
            
            nr = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            if (nr <= 0) {
                break;
            }

            student.grade = atoi(buffer);

            write(fd1, student.name, name);
            write(fd1, (char*)(intptr_t)(student.grade), nr);

            students[j] = student;
            j++;
        }
        i++;
    }

    display_students(students, j);

    close(fd1);
    return 0;
}