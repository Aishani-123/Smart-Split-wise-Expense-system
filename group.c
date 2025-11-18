struct Member {
    char name[20];
    char contact[20];
    char upi[20];
};
struct Group {
    char groupName[20];
    struct Member members[20];
    int memberCount;
};
void create_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "a+");
    FILE *d = fopen("groups_d.txt", "a+");
    if (fp == NULL) {
        printf("Error opening group file.\n");
        return;
    }
    int found = 0;
    while (fscanf(fp, "%s %d", g.groupName, &g.memberCount) == 2) {
        if (strcmp(g.groupName, groupName) == 0) {
            found = 1;
            break;
        }
        for (int i = 0; i < g.memberCount; i++)
            fscanf(fp, "%s %s %s", g.members[i].name, g.members[i].contact, g.members[i].upi);
    }
    if (found) {
        fclose(fp);
        printf("Group already exists.\n");
        return;
    }
    strcpy(g.groupName, groupName);
    g.memberCount = 0;
    fprintf(fp, "%s %d\n", g.groupName, g.memberCount);
    fclose(fp);
    if (d) {
        fprintf(d, "%s %d\n", g.groupName, g.memberCount);
        fclose(d);
    }
    printf("Group created successfully.\n");
}
void list_groups() {
    struct Group g;
    FILE *fp = fopen("groups.txt", "r");
    if (fp == NULL) {
        printf("No groups found.\n");
        return;
    }
    printf("\n--- Groups ---\n");
    while (fscanf(fp, "%s %d", g.groupName, &g.memberCount) == 2) {
        if (strcmp(g.groupName, "DELETED") != 0)
            printf("Group: %s | Members: %d\n", g.groupName, g.memberCount);
        for (int i = 0; i < g.memberCount; i++)
            fscanf(fp, "%s %s %s", g.members[i].name, g.members[i].contact, g.members[i].upi);
    }
    fclose(fp);
}
int add_member(const char *groupName, const char *memberName, const char *contact, const char *upi) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return 0;
    }
    int found = 0;
    while (fscanf(fp, "%s %d", g.groupName, &g.memberCount) == 2) {
        for (int i = 0; i < g.memberCount; i++)
            fscanf(fp, "%s %s %s", g.members[i].name, g.members[i].contact, g.members[i].upi);
        if (strcmp(g.groupName, groupName) == 0) {
            if (g.memberCount >= 20) {
                printf("Group is full!\n");
                fclose(fp);
                fclose(temp);
                remove("temp.txt");
                return found;
            }
            strcpy(g.members[g.memberCount].name, memberName);
            strcpy(g.members[g.memberCount].contact, contact);
            strcpy(g.members[g.memberCount].upi, upi);
            g.memberCount++;
            found = 1;
        }
        fprintf(temp, "%s %d\n", g.groupName, g.memberCount);
        for (int i = 0; i < g.memberCount; i++)
            fprintf(temp, "%s %s %s\n", g.members[i].name, g.members[i].contact, g.members[i].upi);
    }
    fclose(fp);
    fclose(temp);
    remove("groups.txt");
    rename("temp.txt", "groups.txt");
    if (found)
    {
        printf("Member added successfully.\n");
    return 1;}
    else
    { 
        printf("Group not found.\n"); return 0;}
}
int delete_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    int found = 0;
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return 0;
    }
    while (fscanf(fp, "%s %d", g.groupName, &g.memberCount) == 2) {
        for (int i = 0; i < g.memberCount; i++)
            fscanf(fp, "%s %s %s", g.members[i].name, g.members[i].contact, g.members[i].upi);
        if (strcmp(g.groupName, groupName) != 0) {
            fprintf(temp, "%s %d\n", g.groupName, g.memberCount);
            for (int i = 0; i < g.memberCount; i++)
                fprintf(temp, "%s %s %s\n", g.members[i].name, g.members[i].contact, g.members[i].upi);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);
    remove("groups.txt");
    rename("temp.txt", "groups.txt");
    if (found)
        printf("Group deleted successfully.\n");
    else
        printf("Group not found.\n");
    return found;
}
