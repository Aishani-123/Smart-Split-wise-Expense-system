struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10]; };
void add_expense(const char *groupName, const char *category, const char *payer,
                 const char *participants, float amount, const char *date) {
    struct Expense e;
    FILE *fp = fopen("expenses.txt", "ab+");
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
    fwrite(&e, sizeof(struct Expense), 1, fp);
    fclose(fp);
}
void list_expenses() {
    struct Expense e;
    FILE *fp = fopen("expenses.txt", "rb");
    if (fp == NULL) {
        printf("No expenses found.\n");
        return;
    }
    int count = 0;
    while (fread(&e, sizeof(struct Expense), 1, fp)) {
        printf("%d. Group: %s | Category: %s | Payer: %s | Participants: %s | Amount: %.2f | Date: %s\n",
               ++count, e.groupName, e.category, e.payer, e.participants, e.amount, e.date);
    }
    fclose(fp);
}