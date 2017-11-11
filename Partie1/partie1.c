#include <stdlib.h>
#include <string.h>

#include "partie1.h"

void launch() {
    int baseOpened = 0;
    int choice = -1;
    Database* db = NULL;

    //system("clear");

    while (choice != 6) {
        if (baseOpened == 0) {
            menu("Null", baseOpened);
        } else {
            menu(db->name, baseOpened);
        }

        printf("Choix : ");
        scanf("%d", &choice);

        //system("clear");

        switch (choice) {
            case 1:
                databaseFree(db);
                db = mainNewDatabase(&baseOpened);
                if (db != NULL) {
                    show(db);
                }
                break;
            case 2:
                databaseFree(db);
                db = mainOpenDatabase(&baseOpened);
                if (db != NULL) {
                    show(db);
                }
                break;
            case 3:
                mainDeleteDatabase(db, &baseOpened);
                break;
            case 4:
                mainDatabaseAddNewTable(db);
                show(db);
                break;
            case 5:
                mainDatabaseDeleteTable(db);
                show(db);
                break;
        }
    }

    if (baseOpened) {
        databaseFree(db);
    }
}

void show(Database* d) {
    printf("%s\n", d->name);
    printf("%d / %d tables\n\n", d->lengthTables, d->capacityTables);

    int i;
    int j;

    for (i = 0; i < d->lengthTables; i++) {
        printf("\n%s\n", d->tables[i]->name);

        for (j = 0; j < d->tables[i]->lengthAttributes; j++) {
            char* type = attributTypeGet(d->tables[i]->attributes[j]->type);
            printf("    - %s   %s\n", d->tables[i]->attributes[j]->name, type);
            free(type);
        }
    }
}

Database* mainNewDatabase(int* baseOpened) {
    char *dbName = malloc(sizeof(char) * 20);

    printf("Nom de la base de données : ");
    scanf("%s", dbName);
    //system("clear");

    if (strchr(dbName, '.') != NULL) {
        *baseOpened = 0;
        printf("Database name cannot contains dots");
        free(dbName);
        return NULL;
    }

    Database* db = databaseNew(dbName);

    if (db == NULL) {
        printf("Can't create this database");
        *baseOpened = 0;
        free(dbName);
    } else {
        *baseOpened = 1;
    }

    return db;
}

Database* mainOpenDatabase(int* baseOpened) {
    char *dbName = malloc(sizeof(char) * 20);

    printf("Liste des bases de données :\n");
    system("basename -a -s .yaml *.yaml");
    printf("Nom de la base de données à ouvrir: ");
    scanf("%s", dbName);
    //system("clear");

    Database* db = databaseOpen(dbName);

    if (db == NULL) {
        printf("Can't open this database");
        *baseOpened = 0;
        free(dbName);
    } else {
        *baseOpened = 1;
    }

    return db;
}

void mainDeleteDatabase(Database* database, int* baseOpened) {
    if (!*baseOpened) {
        char *databaseName = malloc(sizeof(char) * 20);

        printf("Liste des bases de données :\n");
        system("basename -a -s .yaml *.yaml");
        printf("Nom de la base de données à supprimer: ");
        scanf("%s", databaseName);

        Database* toDelete = databaseOpen(databaseName);
        databaseDelete(toDelete);
    } else {
        *baseOpened = 0;
        databaseDelete(database);
    }
}

// todo check if table have no dots
void mainDatabaseAddNewTable(Database* database) {
    printf("Nom de la table : ");
    char *tableName = malloc(sizeof(char) * 20);
    scanf("%s", tableName);

    if (strchr(tableName, '.') != NULL) {
        printf("Table name cannot contains dots");
        free(tableName);
        return;
    }

    if (tableExists(database, tableName)) {
        printf("Table already exists\n");
        return;
    }

    printf("Nombre d'attributs : ");
    int lengthAttributes;
    scanf("%d", &lengthAttributes);

    Attribut** attributes = malloc(sizeof(Attribut) * lengthAttributes);

    int i;

    for (i = 0; i < lengthAttributes; i++) {
        char *attributeName = malloc(sizeof(char) * 20);
        printf("Nom : ");
        scanf("%s", attributeName);

        int type;
        printf("1- char\n2- int\n3- double\n4- string\n5- int autoincrement\n");
        scanf("%d", &type);

        attributes[i] = attributNew(attributeName, type);
    }

    Table* table = tableNew(database->name, tableName, lengthAttributes, attributes);

    databaseAddNewTable(database, table);
}

void showAllTables(Database* database) {
    int i;

    for (i = 0; i < database->lengthTables; i++) {
        printf("%s\n", database->tables[i]->name);
    }
}

void mainDatabaseDeleteTable(Database* database) {
    printf("Tables à supprimer :\n");
    showAllTables(database);

    char* tableName = malloc(sizeof(char) * 20);

    scanf("%s", tableName);

    databaseDeleteTable(database, tableName);

    free(tableName);
}

void menu(char* databaseName, int baseOpened) {
    printf("======================================\n");
    if (baseOpened) {
        printf("[ %s ]\n", databaseName);
    }
    printf("1- Nouvelle base de données\n");
    printf("2- Ouvrir une base de données\n");
    printf("3- Detruire la base de donnees\n");

    if (baseOpened) {
        printf("4- Ajouter une table\n");
        printf("5- Supprimer une table\n");
    }

    printf("6- Quitter\n");
}