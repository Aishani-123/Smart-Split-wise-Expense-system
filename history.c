struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10];
};
void ex_hist() {
    struct Expense e;
    FILE *h = fopen("hist.txt", "r");   
    if (!h) {
        printf("No history file found.\n");
        return;
    }
    printf("\n========== Expense History ==========\n\n");
    int count = 0;
    while (fscanf(h, "%s %s %s %s %f %[^\n]",
                  e.groupName, e.category, e.payer,
                  e.participants, &e.amount, e.date) == 6) {
        printf("Group Name  : %s\n", e.groupName);
        printf("Category    : %s\n", e.category);
        printf("Payer       : %s\n", e.payer);
        printf("Participants: %s\n", e.participants);
        printf("Amount      : %.2f\n", e.amount);
        printf("Date        : %s\n", e.date);
        count++;
    }
    if (count == 0)
        printf("No records found in history.\n");
    fclose(h);
}

