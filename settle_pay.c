struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10]; };
void settle(const char *groupName, const char *payer, const char *receiver) {
    struct Expense e;
    FILE *fp = fopen("expenses.txt", "rb");
    if (!fp) {
        fp = fopen("expenses.txt", "ab+");
        if (!fp) {
            printf("Error creating expense file.\n");
            return;
        }
    }
    FILE *temp = fopen("temp.txt", "wb");
    int found = 0;
    if (!fp || !temp) {
        printf("Error opening expense files.\n");
        return;
    }
    FILE *h = fopen("hist.txt", "ab");
    if (!h) {
        printf("Error opening history file.\n");
        return;
    }
    while (fread(&e, sizeof(struct Expense), 1, fp)) {
        if (strcmp(e.groupName, groupName) == 0 &&
            strstr(e.participants, receiver) != NULL &&
            strcmp(e.payer, payer) == 0) {
            fwrite(&e,sizeof(struct Expense),1,h);
            if (strstr(e.date, "(Settled)") == NULL) {
                strcat(e.date, " (Settled)");
                found = 1;
            }
        }
        fwrite(&e, sizeof(struct Expense), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove("expenses.txt");
    rename("temp.txt", "expenses.txt");
    if (found)
        printf("Settlement successful.\n");
    else
        printf(" No unpaid expense found.\n");
}