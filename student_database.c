//C program to create student database using MongoDB


#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>

void insert_student(mongoc_collection_t *collection) {
    char name[100], department[100];
    int age;

    printf("Enter Student Name: ");
    scanf("%99s", name);

    printf("Enter Student Age: ");
    scanf("%d", &age);

    printf("Enter Department: ");
    scanf("%99s", department);

    bson_t *doc = BCON_NEW("name", BCON_UTF8(name), "age", BCON_INT32(age), "department", BCON_UTF8(department));
    bson_error_t error;

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        printf("Insert failed: %s\n", error.message);
    } else {
        printf("Student inserted successfully!\n");
    }

    bson_destroy(doc);
}

void retrieve_students(mongoc_collection_t *collection) {
    mongoc_cursor_t *cursor;
    const bson_t *doc;
    bson_t *query = bson_new();
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    printf("\nStudent Records:\n");
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
}

int main() {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_init();

    client = mongoc_client_new("mongodb://localhost:27017/");
    if (!client) {
        printf("Failed to connect to MongoDB.\n");
        return EXIT_FAILURE;
    }

    collection = mongoc_client_get_collection(client, "university", "students");

    int choice;
    while (1) {
        printf("\n1. Insert Student\n2. Retrieve Students\n3. Exit\nChoose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                insert_student(collection);
                break;
            case 2:
                retrieve_students(collection);
                break;
            case 3:
                printf("Exiting program.\n");
                mongoc_collection_destroy(collection);
                mongoc_client_destroy(client);
                mongoc_cleanup();
                return EXIT_SUCCESS;
            default:
                printf("Invalid choice, please try again.\n");
        }
    }
}

