struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10];
};
void add_expense(const char *groupName, const char *category, const char *payer,
                 const char *participants, float amount, const char *date) {
    struct Expense e;
    FILE *fp = fopen("expenses.txt", "a+");
    if (fp == NULL) {
        printf("Error opening expense file.\n");
        return;
    }
    strcpy(e.groupName, groupName);
    strcpy(e.category, category);
    strcpy(e.payer, payer);
    strcpy(e.participants, participants);
    e.amount = amount;
    strcpy(e.date, date);
    fprintf(fp, "%s %s %s %s %.2f %s\n",
            e.groupName, e.category, e.payer, e.participants, e.amount, e.date);

    fclose(fp);
}
void list_expenses() {
    struct Expense e;
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) {
        printf("No expenses found.\n");
        return;
    }
    int count = 0;
    while (fscanf(fp, "%s %s %s %s %f %s",
                  e.groupName, e.category, e.payer, e.participants, &e.amount, e.date) == 6) {
        printf("%d. Group: %s | Category: %s | Payer: %s | Participants: %s | Amount: %.2f | Date: %s\n",
               ++count, e.groupName, e.category, e.payer, e.participants, e.amount, e.date);
    }
    fclose(fp);
}
