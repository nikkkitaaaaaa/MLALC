#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREVIEW_ROWS 10


/* --------------------------------------------------
   ascii logo
--------------------------------------------------*/
void print_logo()
{
    printf("\n");
    printf("============================================================\n");
    printf(" __  __ _            _        _____ \n");
    printf("|  \\/  | |          | |      / ____|\n");
    printf("| \\  / | | __ _  ___| | __  | |     \n");
    printf("| |\\/| | |/ _` |/ __| |/ /  | |     \n");
    printf("| |  | | | (_| | (__|   <   | |____ \n");
    printf("|_|  |_|_|\\__,_|\\___|_|\\_\\   \\_____|\n");
    printf("\n");
    printf("   MLALC : Modular Linear Algebra Library in C\n");
    printf("============================================================\n");
}


/* --------------------------------------------------
   show contents of a file
--------------------------------------------------*/
void show_file(const char *filename)
{
    FILE *f = fopen(filename, "r");
    char ch;

    if (!f)
    {
        printf("error: could not open %s\n", filename);
        return;
    }

    while ((ch = fgetc(f)) != EOF)
        putchar(ch);

    fclose(f);
}


/* --------------------------------------------------
   preview dataset
--------------------------------------------------*/
void preview_dataset(const char *filename, int rows)
{
    FILE *f = fopen(filename, "r");

    if (!f)
    {
        printf("error: dataset not found\n");
        return;
    }

    char line[1024];
    int count = 0;

    printf("\n============================================================\n");
    printf(" DATASET PREVIEW : BEIJING AIR QUALITY (first %d rows)\n", rows);
    printf("============================================================\n\n");

    while (fgets(line, sizeof(line), f) && count < rows)
    {
        count++;

        if (count == 1)
            continue;

        printf("%s", line);
    }

    fclose(f);

    printf("\n------------------------------------------------------------\n");
}


/* --------------------------------------------------
   dataset info
--------------------------------------------------*/
void dataset_info(const char *filename)
{
    FILE *f = fopen(filename, "r");

    if (!f)
    {
        printf("error: dataset not found\n");
        return;
    }

    int rows = 0;
    int cols = 0;
    char line[2048];

    while (fgets(line, sizeof(line), f))
    {
        if(rows > 5000)
            break;

        rows++;

        if (rows == 2)
        {
            char *token = strtok(line, " ");

            while (token)
            {
                cols++;
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(f);

    printf("\n============================================================\n");
    printf(" DATASET INFORMATION\n");
    printf("============================================================\n");

    printf(" dataset file      : beijing_clean.txt\n");
    printf(" number of rows    : %d\n", rows - 1);
    printf(" number of columns : %d\n", cols);

    printf("------------------------------------------------------------\n");
}


/* --------------------------------------------------
   matrix utility tests
--------------------------------------------------*/
void test_utils()
{
    printf("\n============================================================\n");
    printf(" MODULE : MATRIX UTILITIES TEST\n");
    printf("============================================================\n");

    system("./test_utils > c_output.txt");
    system("python3 tests/test_utils.py > py_output.txt");

    printf("\n---------------- C IMPLEMENTATION OUTPUT -------------------\n");
    show_file("c_output.txt");

    printf("\n---------------- PYTHON IMPLEMENTATION OUTPUT --------------\n");
    show_file("py_output.txt");

    printf("\n------------------------------------------------------------\n");
}


/* --------------------------------------------------
   matrix operations tests
--------------------------------------------------*/
void test_ops()
{
    printf("\n============================================================\n");
    printf(" MODULE : MATRIX OPERATIONS TEST\n");
    printf("============================================================\n");

    system("./test_ops > c_output.txt");
    system("python3 tests/test_ops.py > py_output.txt");

    printf("\n---------------- C IMPLEMENTATION OUTPUT -------------------\n");
    show_file("c_output.txt");

    printf("\n---------------- PYTHON IMPLEMENTATION OUTPUT --------------\n");
    show_file("py_output.txt");

    printf("\n------------------------------------------------------------\n");
}


/* --------------------------------------------------
   LU / QR test
--------------------------------------------------*/
void test_luqr()
{
    printf("\n============================================================\n");
    printf(" MODULE : LU / QR DECOMPOSITION TEST\n");
    printf("============================================================\n");

    system("./test_luqr > c_output.txt");
    system("python3 tests/test_luqr.py > py_output.txt");

    printf("\n---------------- C IMPLEMENTATION OUTPUT -------------------\n");
    show_file("c_output.txt");

    printf("\n---------------- PYTHON IMPLEMENTATION OUTPUT --------------\n");
    show_file("py_output.txt");

    printf("\n------------------------------------------------------------\n");
}


/* --------------------------------------------------
   solver comparison
--------------------------------------------------*/
void solver()
{
    printf("\n============================================================\n");
    printf(" MODULE : REAL DATASET SOLVER\n");
    printf("============================================================\n");

    printf(" solving linear system using dataset : beijing_clean.txt\n");
    printf(" formulation : Ax ≈ b (least squares problem)\n");

    system("./solver beijing_clean.txt > c_output.txt");
    system("python3 examples/python_test.py > py_output.txt");

    printf("\n---------------- C SOLVER OUTPUT ---------------------------\n");
    show_file("c_output.txt");

    printf("\n---------------- PYTHON SOLVER OUTPUT ----------------------\n");
    show_file("py_output.txt");

    printf("\n------------------------------------------------------------\n");
}


/* --------------------------------------------------
   main menu
--------------------------------------------------*/
int main()
{
    int choice;

    system("clear");

    print_logo();

    dataset_info("beijing_clean.txt");

    preview_dataset("beijing_clean.txt", PREVIEW_ROWS);

    while (1)
    {
        printf("\n============================================================\n");
        printf("                    MAIN MENU\n");
        printf("============================================================\n");

        printf(" 1. run matrix utilities test\n");
        printf(" 2. run matrix operations test\n");
        printf(" 3. run lu / qr decomposition test\n");
        printf(" 4. run dataset solver comparison\n");
        printf(" 0. exit\n");

        printf("\n enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                test_utils();
                break;

            case 2:
                test_ops();
                break;

            case 3:
                test_luqr();
                break;

            case 4:
                solver();
                break;

            case 0:
                printf("\nexiting program\n");
                return 0;

            default:
                printf("\ninvalid choice\n");
        }
    }

    return 0;
}