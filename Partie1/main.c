#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../LibYamlDB/database.h"
#include "../LibYamlDB/attribut.h"

void menu(char* databaseName, int baseOpened);
Database* mainNewDatabase(int* baseOpened);
Database* mainOpenDatabase(int* baseOpened);
void mainDatabaseAddNewTable(Database* database);

int main() {
    int baseOpened = 0;
    int choice = -1;
    Database* db = NULL;

    while (choice != 6) {
        if (baseOpened == 0) {
            menu("Null", baseOpened);
        } else {
            menu(db->name, baseOpened);
        }

        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                databaseFree(db);
                db = mainNewDatabase(&baseOpened);
                break;
            case 2:
                databaseFree(db);
                db = mainOpenDatabase(&baseOpened);
                break;
            case 3:
                databaseDelete(db);
                break;
            case 4:
                mainDatabaseAddNewTable(db);
                break;
        }

        printf("\n\n");
    }

    databaseFree(db);

    return 0;
}

Database* mainNewDatabase(int* baseOpened) {
    char dbName[20];

    printf("Nom de la base de données : ");
    scanf("%s", dbName);

    Database* db = databaseNew(dbName);

    if (db == NULL) {
        printf("Can't create this database");
        *baseOpened = 0;
    } else {
        *baseOpened = 1;
    }

    return db;
}

Database* mainOpenDatabase(int* baseOpened) {
    char dbName[50];

    printf("Nom de la base de données : ");
    scanf("%s", dbName);

    Database* db = databaseOpen(dbName);

    if (db == NULL) {
        printf("Can't open this database");
        *baseOpened = 0;
    } else {
        *baseOpened = 1;
    }

    return db;
}

void mainDatabaseAddNewTable(Database* database) {
    printf("Nom de la table : ");
    char tableName[20];
    scanf("%s", tableName);

    printf("Nombre d'attributs : ");
    int lengthAttributes;
    scanf("%d", &lengthAttributes);

    Attribut** attributes = malloc(sizeof(Attribut) * lengthAttributes);

    int i;

    for (i = 0; i < lengthAttributes; i++) {
        char attributeName[20];
        printf("Nom : ");
        scanf("%s", attributeName);

        int type;
        printf("1:char 2:int 3:double 4:char*");
        scanf("%d", &type);

        attributes[i] = malloc(sizeof(Attribut));
        attributes[i]->name = malloc(sizeof(char) * (strlen(attributeName) + 1));
        strcpy(attributes[i]->name, attributeName);
        attributes[i]->type = type;
    }

    printf("pas bug");

    Table* table = tableNew(database->name, tableName, lengthAttributes, attributes);

    printf("%s    %d", table->name, table->lengthAttributes);

    databaseAddNewTable(database, table);
}

void menu(char* databaseName, int baseOpened) {
    printf("======================================\n");
    if (baseOpened) {
        printf("[ %s ]\n", databaseName);
    }
    printf("1- Nouvelle base de données\n");
    printf("2- Ouvrir une base de données\n");

    if (baseOpened) {
        printf("3- Detruire la base de donnees\n");
        printf("4- Ajouter une table\n");
        printf("5- Supprimer une table\n");
    }

    printf("6- Quitter\n");
}