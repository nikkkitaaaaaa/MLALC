#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int main()
{
    FILE *in = fopen("examples/datasets/PRSA_data_2010.1.1-2014.12.31.csv", "r");
    if (!in) {
        printf("Error: Could not open dataset file.\n");
        printf("Make sure file exists: PRSA_data_2010.1.1-2014.12.31.csv\n");
        return 1;
    }

    FILE *out = fopen("beijing_clean.txt", "w");
    if (!out) {
        printf("Error: Could not create output file.\n");
        fclose(in);
        return 1;
    }

    char line[MAX_LINE];

    fgets(line, sizeof(line), in);

    double temp[50000], dewp[50000], pres[50000], pm25[50000];
    int count = 0;

    while (fgets(line, sizeof(line), in)) {

        char *token;
        int col = 0;

        char *fields[20];
        token = strtok(line, ",");

        while (token != NULL && col < 20) {
            fields[col++] = token;
            token = strtok(NULL, ",");
        }

        if (col < 8) continue;

        char *pm_str   = fields[5];
        char *dewp_str = fields[6];
        char *temp_str = fields[7];
        char *pres_str = fields[8];

        if (strcmp(pm_str, "NA") == 0) continue;
        if (strcmp(dewp_str, "NA") == 0) continue;
        if (strcmp(temp_str, "NA") == 0) continue;
        if (strcmp(pres_str, "NA") == 0) continue;

        pm25[count] = atof(pm_str);
        dewp[count] = atof(dewp_str);
        temp[count] = atof(temp_str);
        pres[count] = atof(pres_str);

        count++;

        if (count >= 5000) break;
    }

    fclose(in);

    int rows = count;
    int cols = 4;

    printf("Valid rows extracted: %d\n", rows);

    fprintf(out, "%d %d\n", rows, cols);

    for (int i = 0; i < rows; i++) {
        fprintf(out, "%.6lf %.6lf %.6lf 1.0\n",
                temp[i], dewp[i], pres[i]);
    }

    for (int i = 0; i < rows; i++) {
        fprintf(out, "%.6lf\n", pm25[i]);
    }

    fclose(out);

    printf("Preprocessed file created: beijing_clean.txt\n");
    printf("Model: PM2.5 = a*TEMP + b*DEWP + c*PRES + d\n");

    return 0;
}
