struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10];
};
struct MemberShare {
    char name[20];
    float balance;
};
float sp_ex(const char *groupName, const char *category, const char *payer,const char *participants, float amount, const char *date) {
    FILE *p = fopen("split.txt", "a+");  
    if (!p) {
            printf("Error creating split file.\n");
            return 0;
            }
    fprintf(p,"\n=== Expense Split for Group: %s ===\n", groupName);
    fprintf(p,"Category: %s\nDate: %s\n", category, date);
    fprintf(p,"Payer: %s\nTotal Amount: %.2f\n\n", payer, amount);
    char parts_copy[200];
    strcpy(parts_copy, participants);
    char *token = strtok(parts_copy, " ,");
    struct MemberShare members[20];
    int count = 0;
    while (token != NULL && count < 20) {
        strcpy(members[count].name, token);
        members[count].balance = 0.0;
        count++;
        token = strtok(NULL, " ,");
    }
    if (count == 0) {
        printf("No participants entered.\n");
        return 0;
    }
    float share = amount / count;
    fprintf(p,"Each member's share: %.2f\n\n", share);
    return share;
    for (int i = 0; i < count; i++) {
        if (strcmp(members[i].name, payer) == 0)
            members[i].balance += amount - share;
        else
            members[i].balance -= share;
    }
    printf("Initial Balances:\n");
    for (int i = 0; i < count; i++)
        printf("%s: %.2f\n", members[i].name, members[i].balance);
    printf("\n--- Direct Settlements ---\n");
    for (int i = 0; i < count; i++) {
        if (members[i].balance < 0) {
            for (int j = 0; j < count; j++) {
                if (members[j].balance > 0) { 
                    float pay = (members[j].balance < -members[i].balance)
                                    ? members[j].balance
                                    : -members[i].balance;
                    if (pay > 0.001) {
                        fprintf(p,"%s pays %.2f to %s\n", members[i].name, pay, members[j].name);
                        members[i].balance += pay;
                        members[j].balance -= pay;
                    }
                }
            }
        }
    }
    printf("---Transitive Settlements---\n");
    for (int i = 0; i < count; i++) {
        if (members[i].balance < 0) {
            for (int j = 0; j < count; j++) {
                if (members[j].balance > 0) {
                    for (int k = 0; k < count; k++) {
                        if (members[k].balance > 0 && k != j) {
                            float pay = (members[k].balance < -members[i].balance)
                                            ? members[k].balance
                                            : -members[i].balance;
                            if (pay > 0.001) {
                                printf("%s indirectly settles %.2f via %s\n",
                                       members[i].name, pay, members[k].name);
                                members[i].balance += pay;
                                members[k].balance -= pay;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("\n--- Final Balances ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %.2f\n", members[i].name, members[i].balance);
    }
    printf("\nExpense successfully split\n");
    struct Expense e;
    strcpy(e.groupName, groupName);
    strcpy(e.category, category);
    strcpy(e.payer, payer);
    strcpy(e.participants, participants);
    e.amount = amount;
    strcpy(e.date, date);
    FILE *fp = fopen("expenses.txt", "a+"); 
    if (!fp) {
        printf("Error opening expenses file\n");
        return 0;
    }
    fprintf(fp, "%s %s %s %s %.2f %s\n",
            e.groupName, e.category, e.payer, e.participants, e.amount, e.date);
    fclose(fp);
}

