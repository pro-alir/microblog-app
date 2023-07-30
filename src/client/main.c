#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int initialize(int);

char *send_data(char *);

char *connecting();

void RegesterMenu();

void LogIn();

void LogIn2();

void SignUp();

void MainMenu();

void TimeLine();

void SendTweet();

void Refresh();

void SeeComments(char info[10000]);

void CountComments(char str2[], char idd[]);

void like();

void comment();

void Search();

void TweetProfile();

void PersonalArea();

void bio();

void changePassword();

void LogOut();

void unfollow(char *username);

void follow(char *username);

void AcceptableFormat(char *a, const char *b, const char *c);

void AcceptableFormat2(char *a, const char *b);

void AcceptableFormat3(char *a, const char *b, const char *c, const char *d);

struct sockaddr_in server;
int m_index = 0;
char Token_server[1000];

int main() {
   printf("Welcome to my Twitter\n");
    RegesterMenu();


    return 0;
}

int initialize(int port) {


    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsaStartUp != 0) {
        printf("Error");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;

}

char *send_data(char *data) {

    char *buffer = malloc(10000);
    memset(buffer, 0, 10000);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        //printf("1");
        return buffer;
    }

    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if (can_connect != 0) {
        //printf("2");
        return buffer;
    }

    send(client_socket, data, strlen(data), 0);
    recv(client_socket, buffer, 9999, 0);
    closesocket(client_socket);
    return buffer;


}

char *connecting(char *a) {
    int port = 1564;
    int can_initialize = initialize(port);

    while (can_initialize == 0) {
        char messege[10000];
        strcpy(messege, a);
        int len = strlen(messege);
        messege[len] = '\n';
        messege[len + 1] = 0;
        char *feedback = send_data(messege);
        return feedback;
    }
}

void RegesterMenu() {
    printf("1.Log in\n");
    printf("2.Sign up\n");
    char a[100];
    //fgets(a, 2, stdin);
    scanf("%s", a);
    //printf("%s\n", a);
    if (strcmp(a, "1") == 0) {
        LogIn();
    } else if (strcmp(a, "2") == 0) {
        SignUp();
    } else {
        printf("Oops, please try again\n");
        RegesterMenu();
    }
}

void LogIn() {
    printf("Log in menu\n");
    char action[1000] = "login";
    printf("Please enter your username:\n");
    char username[100];
    scanf("%s", username);
    printf("Please enter your password:\n");
    char password[100];
    scanf("%s", password);
    AcceptableFormat(action, username, password);
    char message[1000];
    strcpy(message, connecting(action));
    cJSON *json = cJSON_Parse(message);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *server_answer = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Token") == 0) {
        strcpy(Token_server, server_answer->valuestring);
        MainMenu();
    } else {

        printf("%s\n", server_answer->valuestring);
        RegesterMenu();

    }
}

void SignUp() {
    char action[1000] = "signup";
    printf("Please enter a username:\n");
    char username[100];
    scanf("%s", username);
    printf("Please enter a strong password:\n");
    char password[100];
    scanf("%s", password);
    AcceptableFormat(action, username, password);
    char message[1000];
    strcpy(message, connecting(action));
    cJSON *json = cJSON_Parse(message);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("You\'ve been signed up successfully :)\n");
        LogIn();
    } else {
        printf("%s\n", error->valuestring);
        RegesterMenu();
    }

}

void AcceptableFormat(char *a, const char *b, const char *c) {
    char space[] = " ";
    char comma[] = ",";
    char newline[] = "\n";
    strcat(a, space);
    strcat(a, b);
    strcat(a, comma);
    strcat(a, space);
    strcat(a, c);
}

void AcceptableFormat2(char *a, const char *b) {
    char space[] = " ";
    char newline[] = "\n";
    strcat(a, space);
    strcat(a, b);
}

void AcceptableFormat3(char *a, const char *b, const char *c, const char *d) {
    char space[] = " ";
    char comma[] = ",";
    char newline[] = "\n";
    strcat(a, space);
    strcat(a, b);
    strcat(a, comma);
    strcat(a, space);
    strcat(a, c);
    strcat(a, comma);
    strcat(a, space);
    strcat(a, d);

}

void MainMenu() {
    printf("Mainmenu:\n");
    printf("1.Timeline\n");
    printf("2.Search\n");
    printf("3.Tweet profile\n");
    printf("4.Personal area\n");
    printf("5.Log out\n");
    char choice[100];
    scanf("%*c%[^\n]s", choice);
    if (strcmp(choice, "1") == 0) {
        TimeLine();
    } else if (strcmp(choice, "2") == 0) {
        Search();
    } else if (strcmp(choice, "3") == 0) {
        TweetProfile();
    } else if (strcmp(choice, "4") == 0) {
        PersonalArea();
    } else if (strcmp(choice, "5") == 0) {
        LogOut();
    } else {
        printf("Oops! Please try again\n");
        MainMenu();
    }


}

void TimeLine() {
    printf("1.Send Tweet\n");
    printf("2.Refresh by time\n");
    printf("3.Back\n");
    char choice[100];
    scanf("%*c%[^\n]s", choice);
    if (strcmp(choice, "1") == 0) {
        SendTweet();
    } else if (strcmp(choice, "2") == 0) {
        Refresh();
    } else if (strcmp(choice, "3") == 0) {
        MainMenu();
    } else {
        printf("Oops! Please try again\n");
        TimeLine();
    }

}

void SendTweet() {
    printf("Please write you Tweet\n");
    char tweet[1000];
    char action[3000] = "send tweet";
    scanf("%*c%[^\n]s", tweet);
    AcceptableFormat(action, Token_server, tweet);

    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
        TimeLine();
    } else {
        printf("%s\n", message->valuestring);
        TimeLine();
    }
}

void Refresh() {
    char action[20000] = "refresh";
    AcceptableFormat2(action, Token_server);

    strcpy(action, connecting(action));

    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    cJSON *tweet = NULL;
    cJSON_ArrayForEach(tweet, message) {
        cJSON *author = cJSON_GetObjectItemCaseSensitive(tweet, "author");
        printf("author: %s    ", author->valuestring);
        cJSON *id = cJSON_GetObjectItemCaseSensitive(tweet, "id");
        printf("ID: %d\n", id->valueint);
        cJSON *content = cJSON_GetObjectItemCaseSensitive(tweet, "content");
        printf("%s\n", content->valuestring);
        cJSON *comments = cJSON_GetObjectItemCaseSensitive(tweet, "comment counter");
        printf("Comments: %d    ", comments->valueint);

        cJSON *likes = cJSON_GetObjectItemCaseSensitive(tweet, "likes");
        int index = 0;
        while (cJSON_GetArrayItem(likes, index) != NULL) {
            ++index;
        }
        printf("likes: %d\n\n", index);


    }
    printf("1.Like or comment\n");
    printf("2.back\n");
    char aa[100];
    scanf("%s", aa);
    while (strcmp(aa, "2") != 0) {
        if (strcmp(aa, "1") == 0) {
            printf("1.Like\n");
            printf("2.Comment\n");
            printf("3.See comments\n");
            printf("4.back\n");
            char bb[100];
            scanf("%s", bb);
            if (strcmp(bb, "1") == 0) {
                like();
            } else if (strcmp(bb, "2") == 0) {
                comment();
            } else if (strcmp(bb, "3") == 0) {
                SeeComments(action);
            } else if (strcmp(bb, "4") == 0) {

            } else {}
        } else {}
        printf("1.Like or comment\n");
        printf("2.back\n");
        scanf("%s", aa);

    }
    if (strcmp(aa, "2") == 0) {
        TimeLine();
    }


}

void SeeComments(char info[10000]) {
    int id;
    printf("Please enter the ID:\n");
    scanf("%d", &id);

    cJSON *info_json = NULL;
    info_json = cJSON_Parse(info);

    cJSON *message_json = NULL;
    message_json = cJSON_GetObjectItemCaseSensitive(info_json, "message");

    cJSON *tweet_json = NULL;

    int flag = 0;
    int index = 0;
    while (cJSON_GetArrayItem(message_json, index) != NULL) {
        tweet_json = cJSON_GetArrayItem(message_json, index);
        //printf("tweet:%s\n", tweet_json->valuestring);


        cJSON *id_json = NULL;
        id_json = cJSON_GetObjectItem(tweet_json, "id");
        if (id_json->valueint == id) {
            flag = 1;

            cJSON *comment_json = NULL;
            comment_json = cJSON_GetObjectItemCaseSensitive(tweet_json, "comments");
            printf("Comment:");
            char comment[10000];
            strcpy(comment, cJSON_Print(comment_json));
            for (int i = 0; i < strlen(comment); ++i) {
                if ((comment[i] == '\"') || (comment[i] == '{') || (comment[i] == '}') || (comment[i] == '\t') ||
                    (comment[i] == ',')) {
                    comment[i] = ' ';
                }
            }
            printf("%s\n", comment);

        }
        ++index;
    }
    if (flag == 0) {
        printf("Tweet with this id is not available.\n");
    }

}

void CountComments(char str2[], char idd[]) {
    char new[9000] = "\"id\":";
    char comments[9000]; /////////
    strcpy(comments, str2);
    strcat(new, idd);
    char *newcomment = strstr(comments, new);
    if (newcomment == NULL) {
        printf("wrong id\n");
    } else {
        char *commentbefore = strtok_r(newcomment, "}", &newcomment);
        char *badnames = strstr(commentbefore, "comments");
        strtok_r(badnames, "{", &badnames);
        char *token1;
        int countt = 0;
        while ((token1 = strtok_r(badnames, ",", &badnames))) {
            char *token2;
            int count = 0;
            while ((token2 = strtok_r(token1, ":", &token1))) {
                ++token2;
                token2[strlen(token2) - 1] = '\0';

                countt++;
            }

        }
        printf(" Comments: %d\n\n", countt / 2);
    }
}

void like() {
    char id[100];
    printf("Please write the ID you want to like\n");
    scanf("%s", id);
    char action[200] = "like";
    AcceptableFormat(action, Token_server, id);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
    } else {
        printf("%s\n", message->valuestring);
    }
}

void comment() {
    char id[100];
    printf("Please write the ID you want to comment\n");
    scanf("%s", id);
    char action[1000] = "comment";
    char commentContent[900];
    printf("Please write your comment\n");
    scanf("%*c%[^\n]s", commentContent);
    AcceptableFormat3(action, Token_server, id, commentContent);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
    } else {
        printf("%s\n", message->valuestring);
    }
}

void Search() {
    printf("Please enter the username\n");
    char SearchingName[100];
    scanf("%*c%[^\n]s", SearchingName);
    char action[4000] = "search";
    AcceptableFormat(action, Token_server, SearchingName);
    //printf("%s\n", action);
    strcpy(action, connecting(action));
    //printf("%s\n", action);
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (strcmp(type->valuestring, "Profile") == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(message, "username");
        printf("Username: %s\n", username->valuestring);
        cJSON *bio = cJSON_GetObjectItemCaseSensitive(message, "bio");
        printf("bio: %s\n", bio->valuestring);
        cJSON *numberOfFollowers = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowers");
        printf("followers: %d\t", numberOfFollowers->valueint);
        cJSON *numberOfFollowings = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowings");
        printf("following: %d\n", numberOfFollowings->valueint);
        cJSON *followStatus = cJSON_GetObjectItemCaseSensitive(message, "followStatus");
        printf("%s\n", followStatus->valuestring);
        cJSON *allTweets = cJSON_GetObjectItemCaseSensitive(message, "allTweets");
        cJSON *onetweet = NULL;
        cJSON_ArrayForEach(onetweet, allTweets) {

            cJSON *content = cJSON_GetObjectItemCaseSensitive(onetweet, "content");
            printf("\n%s\n", content->valuestring);
            cJSON *id = cJSON_GetObjectItemCaseSensitive(onetweet, "id");
            printf("ID: %d\n", id->valueint);
            cJSON *comments = cJSON_GetObjectItemCaseSensitive(onetweet, "comment counter");
            printf("comment: %d  ", comments->valueint);
            cJSON *likes = cJSON_GetObjectItemCaseSensitive(onetweet, "likes");

            int index = 0;
            while (cJSON_GetArrayItem(likes, index) != NULL) {
                ++index;
            }
            printf("likes: %d\n", index);

        }
        printf("1.back\n");
        printf("To follow or unfollow, please write \"follow\" or \"unfollow\"\n");
        char action2[100];
        scanf("%s", action2);
        while (strcmp(action2, "1") != 0) {
            if ((strcmp(action2, "unfollow") == 0) || (strcmp(action2, "Unfollow") == 0) ||
                (strcmp(action2, "UNFOLLOW") == 0)) {
                unfollow(SearchingName);
                printf("1.back\n");
            } else if ((strcmp(action2, "follow") == 0) || (strcmp(action2, "follow") == 0) ||
                       (strcmp(action2, "FOLLOW") == 0)) {
                follow(SearchingName);
                printf("1.back\n");
            }
            scanf("%s", action2);
        }
        if (strcmp(action2, "1") == 0) {
            MainMenu();
        }
    } else if (strcmp(type->valuestring, "Error") == 0) {
        printf("%s\n", message->valuestring);
        printf("1.Try again\n");
        printf("2.Go to mainmenu\n");
        char c[100];
        scanf("%s", c);
        if (strcmp(c, "1") == 0) {
            Search();
        } else {
            MainMenu();
        }

    }
}

void TweetProfile() {
    char action[2000] = "profile";
    AcceptableFormat2(action, Token_server);
    strcpy(action, connecting(action));
    //printf("%s\n", action);
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (strcmp(type->valuestring, "Profile") == 0) {
        cJSON *username = cJSON_GetObjectItemCaseSensitive(message, "username");
        printf("Username: %s\n", username->valuestring);
        cJSON *bio = cJSON_GetObjectItemCaseSensitive(message, "bio");
        printf("bio: %s\n", bio->valuestring);
        cJSON *numberOfFollowers = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowers");
        printf("followers: %d\t", numberOfFollowers->valueint);
        cJSON *numberOfFollowings = cJSON_GetObjectItemCaseSensitive(message, "numberOfFollowings");
        printf("following: %d\n", numberOfFollowings->valueint);
        cJSON *followStatus = cJSON_GetObjectItemCaseSensitive(message, "followStatus");
        cJSON *allTweets = cJSON_GetObjectItemCaseSensitive(message, "allTweets");
        cJSON *onetweet = NULL;
        cJSON_ArrayForEach(onetweet, allTweets) {

            cJSON *content = cJSON_GetObjectItemCaseSensitive(onetweet, "content");
            printf("\n%s\n", content->valuestring);
            cJSON *id = cJSON_GetObjectItemCaseSensitive(onetweet, "id");
            printf("ID: %d\n", id->valueint);
            cJSON *comments = cJSON_GetObjectItemCaseSensitive(onetweet, "comment counter");
            printf("comment: %d  ", comments->valueint);

            cJSON *likes = cJSON_GetObjectItemCaseSensitive(onetweet, "likes");
            int index = 0;
            while (cJSON_GetArrayItem(likes, index) != NULL) {
                ++index;
            }
            printf("likes: %d\n", index);
        }
    }
    printf("1.back\n");
    char aa[100];
    scanf("%*c%[^\n]s", aa);
    while (strcmp(aa, "1") != 0) {
        printf("Wrong order, please try again\n");
        scanf("%s", aa);
    }
    MainMenu();

}

void PersonalArea() {
    printf("Personal area\n");
    printf("1.Set a bio\n");
    printf("2.Change password\n");
    printf("3.back\n");
    char a[100];
    scanf("%*c%[^\n]s", a);
    if (strcmp(a, "1") == 0) {
        bio();
    } else if (strcmp(a, "2") == 0) {
        changePassword();
    } else if (strcmp(a, "3") == 0) {
        MainMenu();
    } else {
        printf("Please try again\n");
        PersonalArea();
    }


}

void bio() {
    char action[2000] = "set bio";
    char bioContent[1000];
    printf("Please write your bio\n");
    scanf("%*c%[^\n]s", bioContent);
    AcceptableFormat(action, Token_server, bioContent);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
        PersonalArea();
    } else {
        printf("%s\n", message->valuestring);
        PersonalArea();
    }

}

void changePassword() {
    char action[600] = "change password";
    char currentPassword[50];
    char newPassword[50];
    printf("Please enter your current password\n");
    scanf("%s", currentPassword);
    printf("Please enter your new password\n");
    scanf("%s", newPassword);
    AcceptableFormat3(action, Token_server, currentPassword, newPassword);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "message");

    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", error->valuestring);
        PersonalArea();
    } else {
        printf("%s\n", error->valuestring);
        PersonalArea();
    }

}

void LogOut() {
    char action[400] = "logout";
    AcceptableFormat2(action, Token_server);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *error = cJSON_GetObjectItemCaseSensitive(json, "message");
    printf("%s\n", error->valuestring);
    if (strcmp(type->valuestring, "Successful") == 0) {
        RegesterMenu();
    } else {
        printf("%s\n", error->valuestring);
        MainMenu();
    }

}

void unfollow(char *username) {
    char action[400] = "unfollow";
    AcceptableFormat(action, Token_server, username);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
    } else {
        printf("%s\n", message->valuestring);
    }


}

void follow(char *username) {
    char action[400] = "follow";
    AcceptableFormat(action, Token_server, username);
    strcpy(action, connecting(action));
    cJSON *json = cJSON_Parse(action);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
    cJSON *message = cJSON_GetObjectItemCaseSensitive(json, "message");
    if (strcmp(type->valuestring, "Successful") == 0) {
        printf("%s\n", message->valuestring);
    } else {
        printf("%s\n", message->valuestring);
    }
}