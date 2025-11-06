struct Member {
    char name[20];
    char contact[20];
    char upi[20]; };
struct Group {
    char groupName[20];
    struct Member members[20];
    int memberCount; };
void create_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "ab+");
    FILE *d = fopen("groups_d.txt", "ab+");
    if (fp == NULL) {
        printf("Error opening group file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) == 0) {
            fclose(fp);
            printf("Group already exists.\n");
            return;
        }
    }
    strcpy(g.groupName, groupName);
    g.memberCount = 0;
    fwrite(&g, sizeof(struct Group), 1, fp);
    if(d){fprintf(d,"Group: %s | Members: %d\n",g.groupName,g.memberCount);
    fclose(d);}
    fclose(fp);
    printf("Group created successfully.\n");
}
void list_groups() {
    struct Group g;
    FILE *fp = fopen("groups.txt", "rb+");
    if (fp == NULL) {
        printf("No groups found.\n");
        return;
    }
    printf("\n--- Groups ---\n");
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, "DELETED") != 0) {
            printf("Group: %s | Members: %d\n", g.groupName, g.memberCount);
        }
    }
    fclose(fp);
}
void add_member(const char *groupName, const char *memberName, const char *contact, const char *upi) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "rb+");
    if (fp == NULL) {
        printf("Error opening group file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) == 0) {
            if (g.memberCount >= 20) {
                printf("Group is full!\n");
                fclose(fp);
                return;
            }
            strcpy(g.members[g.memberCount].name, memberName);
            strcpy(g.members[g.memberCount].contact, contact);
            strcpy(g.members[g.memberCount].upi, upi);
            g.memberCount++;
            fseek(fp, -(long)sizeof(struct Group), SEEK_CUR);
            fwrite(&g, sizeof(struct Group), 1, fp);
            fclose(fp);
            printf("Member added successfully.\n");
            return;
        }
    }
    fclose(fp);
    printf("Group not found.\n");
}
void delete_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.txt", "rb+");
    FILE *temp = fopen("temp.txt", "wb+"); 
    int found = 0;
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) != 0) {
            fwrite(&g, sizeof(struct Group), 1, temp);
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
}