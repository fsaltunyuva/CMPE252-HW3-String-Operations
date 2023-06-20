#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>

char *substr(const char *src, int m, int n) {
    int len = n - m;

    char *dest = (char *) malloc(sizeof(char) * (len + 1));

    for (int i = m; i < n && (*(src + i) != '\0'); i++) {
        *dest = *(src + i);
        dest++;
    }

    *dest = '\0';

    return dest - len;
}

typedef struct {
    char name[128];
    char surname[128];
    double duration;
} Student;

int pstrcmp(const void *a, const void *b) {

    Student *student1 = (Student *)a;
    Student *student2 = (Student *)b;

    char name_surname1[128];
    char name_surname2[128];

    /*
    char* temp_name_surname1 = "";
    char* temp_name_surname2 = "";

    temp_name_surname1 = NULL;
    temp_name_surname2 = NULL;

    strcat(temp_name_surname1, student1->surname);
    strcat(temp_name_surname1, " ");
    strcat(temp_name_surname1, student1->name);

    strcat(temp_name_surname2, student2->surname);
    strcat(temp_name_surname2, " ");
    strcat(temp_name_surname2, student2->name);
     */

    sprintf(name_surname1, "%s %s", student1->surname, student1->name);
    sprintf(name_surname2, "%s %s", student2->surname, student2->name);

    for(int i = 0; i < 128; i++){ //TERZI Sevil <--> Tekin Ilhan (solving the problem in the input1.txt)
        name_surname1[i] = tolower(name_surname1[i]);
        name_surname2[i] = tolower(name_surname2[i]);
    }

    return strcmp(name_surname1, name_surname2);
}

void sort(const char *arr[], int n) {
    qsort(arr, n, sizeof(const char *), pstrcmp);
}

int main() {
    FILE *ptr;
    char file_name[10];
    char sort[3];
    int sort_condition = 0;

    printf("Enter the input file name: ");
    scanf("%s", file_name);

    ptr = fopen(file_name, "r");

    if (NULL == ptr) {
        printf("Could not access the file. \n");
        return 1;
    }

    printf("\n");

    printf("Sort the results? (yes/no): ");
    scanf("%s", sort);

    printf("\n");

    if (strcasecmp(sort, "yes") == 0) {
        sort_condition = 1;
    } else if (strcasecmp(sort, "no") == 0) {
        sort_condition = 0;
    }

    char line[128];
    Student student_arr[500];
    Student current_student;

    int iteration_count = 0;
    int current_index_of_student = 0;

    while (fgets(line, sizeof(line), ptr) != NULL) { //Traverse all lines in the text file except the first line
        iteration_count++;
        if (iteration_count == 1) continue; //Skip the first line of the text file

        int change = 0; 

        char *array[5]; //An array that stores the split strings by ","
        char *p = strtok(line, ",");

        int i = 0;
        while (p != NULL) { //Splitting the strings w.r.t ","
            array[i++] = p;
            p = strtok(NULL, ",");
        }

        char* duration_string = array[2];
        double duration = atof(duration_string);
        //sprintf(duration_string, "%.2f", duration_string);

        int num_of_spaces = 0; //Stores the number of spaces in the Name Surname

        for (int x = 0; x < strlen(array[3]); x++) { //Counts the numbers of spaces in the Name Surname (3rd index of the split values)
            if (isspace(array[3][x])) {
                num_of_spaces++;
            }
        }

        int num_of_spaces2 = 0;

        if (num_of_spaces == 1) { //If the user has one name (Name Surname)
            for (int x = 0; x < strlen(array[3]); x++) {
                if (isspace(array[3][x])) {
                    num_of_spaces2++;
                }
                if (num_of_spaces2 == 1) { //If the loop reached the character that stores the space, name is [0, num_of_spaces2] surname is [x+1, length(Name Surname)]
                    strcpy(current_student.surname, substr(array[3], x + 1, strlen(array[3])));
                    strcpy(current_student.name, substr(array[3], 0, x));
                    break;
                }
            }
        }

        int num_of_spaces3 = 0;

        if (num_of_spaces == 2) { //If the user has two names (Name Name2 Surname)
            for (int x = 0; x < strlen(array[3]); x++) {
                if (isspace(array[3][x])) {
                    num_of_spaces3++;
                }
                if (num_of_spaces3 == 2) {//If the loop reached the second character that stores the space, name is [0, num_of_spaces3] surname is [x+1, length(Name Surname)]
                    strcpy(current_student.surname,substr(array[3], x + 1, strlen(array[3])));
                    strcpy(current_student.name, substr(array[3], 0, x));
                    break;
                }
            }
        }

        int num_of_spaces4 = 0;

        if (num_of_spaces == 3) { //If the user has three names (Name Name2 Name3 Surname)
            for (int x = 0; x < strlen(array[3]); x++) {
                if (isspace(array[3][x])) {
                    num_of_spaces4++;
                }
                if (num_of_spaces4 == 3) {//If the loop reached the second character that stores the space, name is [0, num_of_spaces4] surname is [x+1, length(Name Surname)]
                    strcpy(current_student.surname,substr(array[3], x + 1, strlen(array[3])));
                    strcpy(current_student.name, substr(array[3], 0, x));
                    break;
                }
            }
        }

        for(int y = 0; y < current_index_of_student; y++){ //Does not use in first and second iteration
            if(strcasecmp(student_arr[y].name, current_student.name) == 0 && strcasecmp(student_arr[y].surname, current_student.surname) == 0){
                change = y;
                break;
            }
        }

        if (change == 0){
            strcpy(student_arr[current_index_of_student].name, current_student.name);
            strcpy(student_arr[current_index_of_student].surname, current_student.surname);
            student_arr[current_index_of_student].duration = duration;
            current_index_of_student++;
        }
        else{
            student_arr[change].duration = (student_arr[change].duration) + (duration);
        }

    }

    if (sort_condition == 1) { //If the user wanted to sort the resulting array
        qsort(student_arr, current_index_of_student, sizeof(Student), pstrcmp);
    }

    for (int i = 0; i < current_index_of_student; i++) {
        printf("%s %s %.2f\n", student_arr[i].surname, student_arr[i].name, student_arr[i].duration);
    }

    return 0;
}
