

# include <stdio.h>
# include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>
#include "cJSON.h"

struct user_information {
    char user_name[40];
    char token[35];
};

struct user_information user_info[100];


char *path_user = "C:\\Users\\paitakht\\Desktop\\serverContent\\Resources\\Users\\";

char *path_tweet = "C:\\Users\\paitakht\\Desktop\\serverContent\\Resources\\Tweets\\";

char *path_tweet_id_counter = "C:\\Users\\paitakht\\Desktop\\serverContent\\Resources\\Tweet_ID_Counter\\Tweet_ID_Counter.txt";

char *connecting();

long long unsigned int initialize(int port);

void start(int server_socket);

void Make_Null();

void Add_Client_Information(char user[40], char tokenize[35]);

int Check_Token(char tokenize[35]);

int Check_User_LoggedIn(char username[40]);

const char *Token_Generator();

void Make_User_File(char info[10000], char username[40]);

void Make_Tweet_File(char info[10000], int id);

const char *Action_Menu(char info[10000]);

int Wrong_Structure_1(char info[10000]);

const char *Parsing_Action(char info[10000]);

const char *Parsing_Second_part(char info[10000]);

const char *Parsing_Third_part(char info[10000]);

const char *SignUp(char info[10000]);

int Repetitive_Username(char username[40]);

int Available_Tweet_ID(char id[50], char username[40]);

int Check_Correct_Password(char password[50], char username[40]);

const char *LogIn(char info[10000]);

const char *Send_Tweet(char info[10000], int check);

int Tweet_ID_Counter();

void Add_TweetID_To_UserFile(int tweet_id, char username[40]);

void Add_TweetID_To_unseenTweets_Of_Followers(int tweet_id, char username[40]);

const char *Search(char info[10000], char searching_user[100], int check);

const char *Follow_Status(char *searching_user, char username[40]);

const char *Follow(char info[10000], int check);

void Add_User_To_Follow_tweets_To_UnseenTweets_Of_Follower(char user_to_follow[100], char username[40]);

const char *Unfollow(char info[10000], int check);

void Delete_User_To_Unfollow_tweets_From_UnseenTweets_Of_Follower(char user_to_unfollow[100], char username[40]);

const char *Refresh(char info[10000], int check);

const char *Like(char info[10000], int check);

const char *Comment(char info[10000], int chec);

const char *Reading_Tweet_File(char id[50]);

const char *Reading_User_File(char user_to_open[50]);

void Writing_Tweet_File(char id[50], char content[10000], char username[40]);

void Writing_User_File(char user_to_open[50], char content[10000], char username[40]);

const char *Message(char type[1000], char message[10000]);

char const *Bio(char info[10000], int check);

const char *LogOut(char info[10000], int check);

const char *Change_Password(char info[10000], int check);

const char *Delete_Tweet(char info[10000], int check);

int Available_Tweet_ID_In_User_File(int id, char username[40]);

void Deleting_Tweet_Id_From_User_File(int id, char username[40]);

int main() {

    Make_Null();
    connecting();

    return 0;
}

char *connecting() {
    int port = 1564;
    int server_socket = (int) initialize(port);
    if (server_socket != INVALID_SOCKET) {
        start(server_socket);
    }
}

long long unsigned int initialize(int port) {
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (wsa_res != 0) {
        printf("Error");
        return INVALID_SOCKET;
    }
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        return INVALID_SOCKET;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);
    int bind_res = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (bind_res == SOCKET_ERROR) {
        printf("bind faild");
        return INVALID_SOCKET;
    }
    listen(server_socket, SOMAXCONN);
    return server_socket;
}

void start(int server_socket) {
    int len = sizeof(struct sockaddr_in);

    int client_socket;
    struct sockaddr_in client_addr;

    char buffer[10000];
    while ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &len)) != INVALID_SOCKET) {
        printf("client connected\n");

        memset(buffer, 0, 10000);
        recv(client_socket, buffer, 9999, 0);

        printf("Client:%s\n", buffer);
        const char *response = Action_Menu(buffer);
        printf("Server:%s\n\n", response);

        send(client_socket, response, 9999, 0);
        closesocket(client_socket);
    }

}

const char *Token_Generator() {
    char token[35];
    srand((unsigned int) (time(NULL)));
    int index = 0;
    char char1[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_0123456789";

    for (index = 0; index < 32; index++) {
        token[index] = char1[rand() % (sizeof char1 - 1)];
    }
    token[32] = '\0';
    char *token_malloc = malloc(sizeof(char) * 35);
    strcpy(token_malloc, token);
    return token_malloc;
}

void Make_User_File(char info[10000], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "w");
    fputs(info, user);
    fclose(user);
}

void Make_Tweet_File(char info[10000], int id) {
    FILE *tweet;
    char id_string[20];
    itoa(id, id_string, 10);
    char path[500];
    strcpy(path, path_tweet);
    strcat(path, id_string);
    strcat(path, ".tweet.json");
    tweet = fopen(path, "w");
    fputs(info, tweet);
    fclose(tweet);
}


const char *Action_Menu(char info[10000]) {
    char *response = malloc(sizeof(char) * 10000);
    char action[100];
    strcpy(action, Parsing_Action(info));
    int i;
    char tokenize[35];
    strcpy(tokenize, Parsing_Second_part(info));
    if (strcmp(action, "signup") == 0) {
        strcpy(response, SignUp(info));

    } else if (strcmp(action, "login") == 0) {
        strcpy(response, LogIn(info));

    } else if ((i = Check_Token(tokenize)) != -1) {

        if (strcmp(action, "send tweet") == 0) {
            strcpy(response, Send_Tweet(info, i));

        } else if (strcmp(action, "search") == 0) {
            char searching_user[100];
            strcpy(searching_user, Parsing_Third_part(info));
            strcpy(response, Search(info, searching_user, i));

        } else if (strcmp(action, "follow") == 0) {
            strcpy(response, Follow(info, i));

        } else if (strcmp(action, "unfollow") == 0) {
            strcpy(response, Unfollow(info, i));

        } else if (strcmp(action, "refresh") == 0) {
            strcpy(response, Refresh(info, i));

        } else if (strcmp(action, "like") == 0) {
            strcpy(response, Like(info, i));

        } else if (strcmp(action, "comment") == 0) {
            strcpy(response, Comment(info, i));

        } else if (strcmp(action, "set bio") == 0) {
            strcpy(response, Bio(info, i));

        } else if (strcmp(action, "profile") == 0) {
            char username[40];
            strcpy(username, user_info[i].user_name);
            strcpy(response, Search(info, username, i));

        } else if (strcmp(action, "logout") == 0) {
            strcpy(response, LogOut(info, i));

        } else if (strcmp(action, "change password") == 0) {
            strcpy(response, Change_Password(info, i));

        } else if (strcmp(action, "delete") == 0) {
            strcpy(response, Delete_Tweet(info, i));

        }else{
            strcpy(response, Message("Error", "Wrong structure"));
        }
    } else {
        strcpy(response, Message("Error", "Invalid token"));
    }

    return response;
}

const char *Parsing_Action(char info[10000]) {
    char content[1000];
    int i = 0;
    while (((info[i] != ',') && (info[i] != '\n')) || ((info[i + 1] != ' ') && (info[i] != '\n'))) {
        ++i;
    }
    while (info[i] != ' ') {
        --i;
    }
    --i;
    int j;
    for (j = 0; j <= i; ++j) {
        content[j] = info[j];
    }
    content[j] = '\0';
    char *feedback = malloc(sizeof(char) * 100);
    strcpy(feedback, content);
    return feedback;
}

const char *Parsing_Second_part(char info[10000]) {
    char content[1000];
    int i = 0;
    while (((info[i] != ',') && (info[i] != '\n')) || ((info[i + 1] != ' ') && (info[i] != '\n'))) {
        ++i;
    }
    while (info[i] != ' ') {
        --i;
    }
    ++i;
    int j = 0;
    while (((info[i] != ',') && (info[i] != '\n')) || ((info[i + 1] != ' ') && (info[i] != '\n'))) {
        content[j] = info[i];
        ++j;
        ++i;
    }
    content[j] = '\0';
    char *feedback = malloc(sizeof(char) * 1000);
    strcpy(feedback, content);
    return feedback;
}

const char *Parsing_Third_part(char info[10000]) {
    char content[10000];
    int i = 0;
    while (((info[i] != ',') && (info[i] != '\n')) || ((info[i + 1] != ' ') && (info[i] != '\n'))) {
        ++i;
    }
    ++i;
    ++i;
    int j = 0;
    while (info[i] != '\n') {
        content[j] = info[i];
        ++j;
        ++i;
    }
    content[j] = '\0';
    char *feedback = malloc(sizeof(char) * 10000);
    strcpy(feedback, content);
    return feedback;
}

const char *SignUp(char info[10000]) {
    char username[40];
    strcpy(username, Parsing_Second_part(info));
    char password[50];
    strcpy(password, Parsing_Third_part(info));

    if (Repetitive_Username(username) == 0) {

        int len = 0;
        for (int k = 0; k < strlen(password); ++k) {
            ++len;
        }
        if (len <= 16) {
            cJSON *file_content = cJSON_CreateObject();

            cJSON *username_cjson = NULL;
            username_cjson = cJSON_CreateString(username);
            cJSON_AddItemToObject(file_content, "username", username_cjson);

            cJSON *password_cjson = NULL;
            password_cjson = cJSON_CreateString(password);
            cJSON_AddItemToObject(file_content, "password", password_cjson);

            cJSON *bio_cjson = NULL;
            bio_cjson = cJSON_CreateString("");
            cJSON_AddItemToObject(file_content, "bio", bio_cjson);

            cJSON *followers_cjson = NULL;
            followers_cjson = cJSON_CreateArray();
            cJSON_AddItemToObject(file_content, "followers", followers_cjson);

            cJSON *following_cjson = NULL;
            following_cjson = cJSON_CreateArray();
            cJSON_AddItemToObject(file_content, "followings", following_cjson);

            cJSON *tweet_IDs_cjson = NULL;
            tweet_IDs_cjson = cJSON_CreateArray();
            cJSON_AddItemToObject(file_content, "personalTweets", tweet_IDs_cjson);

            cJSON *unseen_tweet_IDs_cjson = NULL;
            unseen_tweet_IDs_cjson = cJSON_CreateArray();
            cJSON_AddItemToObject(file_content, "unseen tweets", unseen_tweet_IDs_cjson);

            char *result = cJSON_Print(file_content);
            Make_User_File(result, username);
            cJSON *response = cJSON_CreateObject();

            cJSON *type = NULL;
            type = cJSON_CreateString("Successful");
            cJSON_AddItemToObject(response, "type", type);

            cJSON *message = NULL;
            message = cJSON_CreateString("");
            cJSON_AddItemToObject(response, "message", message);

            char *string_response = cJSON_Print(response);
            return string_response;
        } else {
            char *response = malloc(sizeof(char) * 10000);
            strcpy(response,
                   Message("Error", "Input password is too long. Please make it smaller than 17 characters."));
            return response;
        }
    } else {
        cJSON *response = cJSON_CreateObject();

        cJSON *type = NULL;
        type = cJSON_CreateString("Error");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("This username is already taken.");
        cJSON_AddItemToObject(response, "message", message);

        char *string_response = cJSON_Print(response);
        return string_response;
    }

}

int Repetitive_Username(char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    int i = 0;
    char nothing[10000];
    while (fgets(nothing, 9999, user) != NULL) {
        ++i;
    }

    fclose(user);
    if (i == 0) {
        return 0;
    } else {
        return 1;
    }

}

int Available_Tweet_ID(char id[50], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_tweet);
    strcat(path, id);
    strcat(path, ".tweet.json");
    user = fopen(path, "r");
    int i = 0;
    char nothing[10000];
    while (fgets(nothing, 9999, user) != NULL) {
        ++i;
    }

    fclose(user);
    if (i == 0) {
        return 0;
    } else {
        return 1;
    }

}

int Check_Correct_Password(char password[50], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    char info[10000];
    info[0] = '\0';
    char info2[10000];
    info2[0] = '\0';
    while (fgets(info, 9999, user) != NULL) {
        strcat(info2, info);
    }
    cJSON *json = cJSON_Parse(info2);
    cJSON *password_in_file = cJSON_GetObjectItemCaseSensitive(json, "password");
    if (strcmp(password, password_in_file->valuestring) == 0) {
        return 0;
    } else {
        return 1;
    }
}

const char *LogIn(char info[10000]) {
    char username[40];
    strcpy(username, Parsing_Second_part(info));
    char password[50];
    strcpy(password, Parsing_Third_part(info));

    char token[35];
    strcpy(token, Token_Generator());

    cJSON *response = cJSON_CreateObject();

    cJSON *type = NULL;

    cJSON *message = NULL;

    if (Repetitive_Username(username) == 0) {
        type = cJSON_CreateString("Error");
        message = cJSON_CreateString("This username is not valid.");

    } else {
        if (Check_Correct_Password(password, username) == 0) {
            if (Check_User_LoggedIn(username) == -1) {
                type = cJSON_CreateString("Token");
                Token_Generator();
                message = cJSON_CreateString(token);
                Add_Client_Information(username, token);
            } else {
                type = cJSON_CreateString("Error");
                message = cJSON_CreateString("User is already logged in.");
            }
        } else {
            type = cJSON_CreateString("Error");
            message = cJSON_CreateString("Incorrect password.");
        }
    }

    cJSON_AddItemToObject(response, "type", type);
    cJSON_AddItemToObject(response, "message", message);
    char *string_response = cJSON_Print(response);
    return string_response;
}

void Add_Client_Information(char user[40], char tokenize[35]) {
    int i = 0;
    while ((user_info[i].token[0] != '\0') || (user_info[i].user_name[0] != '\0')) {
        ++i;
    }
    strcpy(user_info[i].token, tokenize);
    strcpy(user_info[i].user_name, user);
}

void Make_Null() {
    for (int i = 0; i < 100; ++i) {
        user_info[i].token[0] = '\0';
        user_info[i].user_name[0] = '\0';
    }
}

int Check_Token(char tokenize[35]) {
    int i = 0;
    while (strcmp(user_info[i].token, tokenize) != 0) {
        ++i;
        if (i >= 100) {
            break;
        }
    }
    if (i >= 100) {
        return -1;
    } else {
        return i;
    }
}

int Wrong_Structure_1(char info[10000]){
    int i = 0;
    int count = 0;
    while (info[i] != '\0'){
        if(info[i] == ','){
            ++count;
        }
        ++i;
    }
    if(count == 0){
        return 0;
    }else{
        return -1;
    }
}

int Check_User_LoggedIn(char username[40]) {
    int i = 0;
    while (strcmp(user_info[i].user_name, username) != 0) {
        ++i;
        if (i >= 100) {
            break;
        }
    }
    if (i >= 100) {
        return -1;
    } else {
        return i;
    }
}

const char *Send_Tweet(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char tweet_content[10000];
    strcpy(tweet_content, Parsing_Third_part(info));

    cJSON *file_content = cJSON_CreateObject();

    cJSON *id_cjson = NULL;
    int tweet_id = Tweet_ID_Counter();
    id_cjson = cJSON_CreateNumber(tweet_id);
    cJSON_AddItemToObject(file_content, "id", id_cjson);

    cJSON *tweet_author_cjson = NULL;
    tweet_author_cjson = cJSON_CreateString(username);
    cJSON_AddItemToObject(file_content, "author", tweet_author_cjson);

    cJSON *tweet_content_cjson = NULL;
    tweet_content_cjson = cJSON_CreateString(tweet_content);
    cJSON_AddItemToObject(file_content, "content", tweet_content_cjson);

    cJSON *tweet_comments_cjson = NULL;
    tweet_comments_cjson = cJSON_CreateObject();
    cJSON_AddItemToObject(file_content, "comments", tweet_comments_cjson);

    cJSON *tweet_like_cjson = NULL;
    tweet_like_cjson = cJSON_CreateArray();
    cJSON_AddItemToObject(file_content, "likes", tweet_like_cjson);

    cJSON *comment_counter_json = NULL;
    comment_counter_json = cJSON_CreateNumber(0);
    cJSON_AddItemToObject(file_content, "comment counter", comment_counter_json);

    char *result = cJSON_Print(file_content);
    Make_Tweet_File(result, tweet_id);
    Add_TweetID_To_UserFile(tweet_id, username);
    Add_TweetID_To_unseenTweets_Of_Followers(tweet_id, username);

    cJSON *response = cJSON_CreateObject();

    cJSON *type = NULL;
    type = cJSON_CreateString("Successful");
    cJSON_AddItemToObject(response, "type", type);

    cJSON *message = NULL;
    message = cJSON_CreateString("Tweet is sent successfully.");
    cJSON_AddItemToObject(response, "message", message);

    char *string_response = cJSON_Print(response);
    return string_response;
}

int Tweet_ID_Counter() {
    int i;
    FILE *tweet_id_counter;
    char path[500];
    strcpy(path, path_tweet_id_counter);
    tweet_id_counter = fopen(path, "r");
    fscanf(tweet_id_counter, "%d", &i);
    --i;
    fclose(tweet_id_counter);
    tweet_id_counter = fopen(path, "w");
    fprintf(tweet_id_counter, "%d", i);
    fclose(tweet_id_counter);
    return i;
}

void Add_TweetID_To_UserFile(int tweet_id, char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    char info[10000];
    info[0] = '\0';
    char info2[10000];
    info2[0] = '\0';
    while (fgets(info, 9999, user) != NULL) {
        strcat(info2, info);
    }
    cJSON *tweet_id_json = cJSON_CreateNumber(tweet_id);
    cJSON *json = cJSON_Parse(info2);
    cJSON *allTweets = cJSON_GetObjectItemCaseSensitive(json, "personalTweets");
    cJSON_AddItemToArray(allTweets, tweet_id_json);
    char *string = cJSON_Print(json);
    user = fopen(path, "w");
    fputs(string, user);
    fclose(user);
}

void Add_TweetID_To_unseenTweets_Of_Followers(int tweet_id, char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    char info[10000];
    info[0] = '\0';
    char info2[10000];
    info2[0] = '\0';
    while (fgets(info, 9999, user) != NULL) {
        strcat(info2, info);
    }
    cJSON *json = cJSON_Parse(info2);
    cJSON *all_followers_json = NULL;
    all_followers_json = cJSON_GetObjectItemCaseSensitive(json, "followers");
    int index = 0;
    while (cJSON_GetArrayItem(all_followers_json, index) != NULL) {
        cJSON *follower_json = cJSON_GetArrayItem(all_followers_json, index);
        char *follower_username = cJSON_Print(follower_json);
        char follower_usermame_array[50];
        int i = 0;
        for (i = 0; i < strlen(follower_username) - 2; ++i) {
            follower_usermame_array[i] = follower_username[i + 1];
        }
        follower_usermame_array[i] = '\0';
        FILE *follower_user_file;
        strcpy(path, path_user);
        strcat(path, follower_usermame_array);
        strcat(path, ".user.json");
        follower_user_file = fopen(path, "r");
        char info3[10000];
        info3[0] = '\0';
        char info4[10000];
        info4[0] = '\0';
        while (fgets(info3, 9999, follower_user_file) != NULL) {
            strcat(info4, info3);
        }
        cJSON *follower_username_json = cJSON_Parse(info4);
        cJSON *unseen_tweets_follower_json = NULL;
        unseen_tweets_follower_json = cJSON_GetObjectItemCaseSensitive(follower_username_json, "unseen tweets");
        cJSON *tweet_id_json = cJSON_CreateNumber(tweet_id);
        cJSON_AddItemToArray(unseen_tweets_follower_json, tweet_id_json);
        char *result = cJSON_Print(follower_username_json);

        strcpy(path, path_user);
        strcat(path, follower_usermame_array);
        strcat(path, ".user.json");
        user = fopen(path, "w");
        fputs(result, user);
        fclose(user);

        ++index;
    }
}

const char *Search(char info[10000], char searching_user[100], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, searching_user);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    if (strcmp(info3, "") == 0) {
        cJSON *response = cJSON_CreateObject();

        cJSON *type = NULL;
        type = cJSON_CreateString("Error");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("This username is not valid.");
        cJSON_AddItemToObject(response, "message", message);

        char *string_response = cJSON_Print(response);
        return string_response;
    } else {
        cJSON *response = cJSON_CreateObject();
        cJSON *json_user = cJSON_Parse(info3);

        cJSON *type = NULL;
        type = cJSON_CreateString("Profile");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateObject();
        cJSON_AddItemToObject(response, "message", message);

        cJSON *searching_username_json1 = NULL;
        cJSON *searching_username_json2 = NULL;
        searching_username_json2 = cJSON_GetObjectItemCaseSensitive(json_user, "username");
        searching_username_json1 = cJSON_CreateString(searching_username_json2->valuestring);
        cJSON_AddItemToObject(message, "username", searching_username_json1);

        cJSON *searching_bio_json1 = NULL;
        cJSON *searching_bio_json2 = NULL;
        searching_bio_json2 = cJSON_GetObjectItemCaseSensitive(json_user, "bio");
        searching_bio_json1 = cJSON_CreateString(searching_bio_json2->valuestring);
        cJSON_AddItemToObject(message, "bio", searching_bio_json1);

        cJSON *counting_followers = NULL;
        counting_followers = cJSON_GetObjectItemCaseSensitive(json_user, "followers");
        int index = 0;
        while (cJSON_GetArrayItem(counting_followers, index) != NULL) {
            ++index;
        }
        cJSON *number_of_followers = cJSON_CreateNumber(index);
        cJSON_AddItemToObject(message, "numberOfFollowers", number_of_followers);

        cJSON *counting_followings = NULL;
        counting_followings = cJSON_GetObjectItemCaseSensitive(json_user, "followings");
        index = 0;
        while (cJSON_GetArrayItem(counting_followings, index) != NULL) {
            ++index;
        }
        cJSON *number_of_followings = cJSON_CreateNumber(index);
        cJSON_AddItemToObject(message, "numberOfFollowings", number_of_followings);

        cJSON *following_status_json = cJSON_CreateString(Follow_Status(searching_user, username));
        cJSON_AddItemToObject(message, "followStatus", following_status_json);

        cJSON *allTweets = NULL;
        allTweets = cJSON_CreateArray();
        cJSON *all_tweets_json = cJSON_GetObjectItemCaseSensitive(json_user, "personalTweets");
        index = 0;
        while (cJSON_GetArrayItem(all_tweets_json, index) != NULL) {
            FILE *tweet;
            cJSON *id_string_json = cJSON_GetArrayItem(all_tweets_json, index);
            char *id_string = cJSON_Print(id_string_json);
            char path2[500];
            strcpy(path2, path_tweet);
            strcat(path2, id_string);
            strcat(path2, ".tweet.json");
            tweet = fopen(path2, "r");

            char info4[10000];
            char info5[10000];
            info4[0] = '\0';
            info5[0] = '\0';
            while (fgets(info4, 9999, tweet) != NULL) {
                strcat(info5, info4);
            }
            cJSON *tweet_json = NULL;
            tweet_json = cJSON_Parse(info5);
            cJSON_AddItemToArray(allTweets, tweet_json);
            ++index;
        }
        cJSON_AddItemToObject(message, "allTweets", allTweets);

        char *string_response = cJSON_Print(response);
        return string_response;

    }
}

const char *Follow_Status(char *searching_user, char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info1[10000];
    char info2[10000];
    info1[0] = '\0';
    info2[0] = '\0';
    while (fgets(info1, 9999, user) != NULL) {
        strcat(info2, info1);
    }
    cJSON *json_user = cJSON_Parse(info2);
    cJSON *counting_followings = NULL;
    counting_followings = cJSON_GetObjectItemCaseSensitive(json_user, "followings");
    int index = 0;
    int flag = 0;
    while (cJSON_GetArrayItem(counting_followings, index) != NULL) {
        cJSON *following_json = cJSON_GetArrayItem(counting_followings, index);
        if (strcmp(following_json->valuestring, searching_user) == 0) {
            flag = 1;
        }
        ++index;
    }
    char result[20];
    if (flag == 0) {
        strcpy(result, "NotFollowed");
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, result);
        return response;
    } else {
        strcpy(result, "Followed");
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, result);
        return response;
    }
}

const char *Follow(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char user_to_follow[100];
    strcpy(user_to_follow, Parsing_Third_part(info));
    cJSON *response = cJSON_CreateObject();
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    cJSON *json_user = cJSON_Parse(info3);

    cJSON *followings_json = NULL;
    followings_json = cJSON_GetObjectItemCaseSensitive(json_user, "followings");
    int flag = 0;
    int index = 0;
    while (cJSON_GetArrayItem(followings_json, index) != NULL) {
        if (strcmp(cJSON_GetArrayItem(followings_json, index)->valuestring, user_to_follow) == 0) {
            flag = 1;
        }
        ++index;
    }
    if (flag == 0) {
        cJSON *user_to_follow_json = NULL;
        user_to_follow_json = cJSON_CreateString(user_to_follow);

        cJSON_AddItemToArray(followings_json, user_to_follow_json);

        char *result = cJSON_Print(json_user);
        strcpy(path, path_user);
        strcat(path, username);
        strcat(path, ".user.json");
        user = fopen(path, "w");
        fputs(result, user);
        fclose(user);

        strcpy(path, path_user);
        strcat(path, user_to_follow);
        strcat(path, ".user.json");
        user = fopen(path, "r");
        info2[0] = '\0';
        info3[0] = '\0';
        while (fgets(info2, 9999, user) != NULL) {
            strcat(info3, info2);
        }
        fclose(user);
        cJSON *json_user_to_follow = cJSON_Parse(info3);

        cJSON *followers_json = NULL;
        followers_json = cJSON_GetObjectItemCaseSensitive(json_user_to_follow, "followers");

        cJSON *username_json = NULL;
        username_json = cJSON_CreateString(username);

        cJSON_AddItemToArray(followers_json, username_json);

        char *result2 = cJSON_Print(json_user_to_follow);
        strcpy(path, path_user);
        strcat(path, user_to_follow);
        strcat(path, ".user.json");
        user = fopen(path, "w");
        fputs(result2, user);
        fclose(user);

        cJSON *type = NULL;
        type = cJSON_CreateString("Successful");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("User is followed successfully.");
        cJSON_AddItemToObject(response, "message", message);

        Add_User_To_Follow_tweets_To_UnseenTweets_Of_Follower(user_to_follow, username);
    } else {
        cJSON *type = NULL;
        type = cJSON_CreateString("Error");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("User is already followed by you.");
        cJSON_AddItemToObject(response, "message", message);
    }


    char *string_response = cJSON_Print(response);
    return string_response;
}

void Add_User_To_Follow_tweets_To_UnseenTweets_Of_Follower(char user_to_follow[100], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, user_to_follow);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    fclose(user);

    cJSON *user_to_follow_json = cJSON_Parse(info3);
    cJSON *tweets_json = NULL;
    tweets_json = cJSON_GetObjectItemCaseSensitive(user_to_follow_json, "personalTweets");

    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    fclose(user);

    cJSON *username_json = cJSON_Parse(info3);
    cJSON *unseen_tweets_json = NULL;
    unseen_tweets_json = cJSON_GetObjectItemCaseSensitive(username_json, "unseen tweets");
    int index = 0;
    while (cJSON_GetArrayItem(tweets_json, index) != NULL) {
        cJSON *tweet_json = cJSON_GetArrayItem(tweets_json, index);
        cJSON *sher = NULL;
        sher = cJSON_CreateNumber(tweet_json->valueint);
        cJSON_AddItemToArray(unseen_tweets_json, sher);
        ++index;
    }

    char *result = cJSON_Print(username_json);

    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "w");
    fputs(result, user);
    fclose(user);

}

const char *Unfollow(char info[10000], int check) {
    //printf("unfollow1\n");
    char username[40];
    strcpy(username, user_info[check].user_name);
    char user_to_follow[100];
    strcpy(user_to_follow, Parsing_Third_part(info));
    cJSON *response = cJSON_CreateObject();
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';


    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    cJSON *json_user = cJSON_Parse(info3);


    cJSON *followings_json = NULL;
    followings_json = cJSON_GetObjectItemCaseSensitive(json_user, "followings");
    int flag = 0;
    int index = 0;
    int sign = 0;
    while (cJSON_GetArrayItem(followings_json, index) != NULL) {
        if (strcmp(cJSON_GetArrayItem(followings_json, index)->valuestring, user_to_follow) == 0) {
            flag = 1;
            sign = index;
        }
        ++index;
    }
    if (flag == 1) {
        cJSON *user_to_follow_json = NULL;
        cJSON_DeleteItemFromArray(followings_json, sign);

        char *result = cJSON_Print(json_user);
        strcpy(path, path_user);
        strcat(path, username);
        strcat(path, ".user.json");
        user = fopen(path, "w");
        fputs(result, user);
        fclose(user);

        strcpy(path, path_user);
        strcat(path, user_to_follow);
        strcat(path, ".user.json");
        user = fopen(path, "r");
        info2[0] = '\0';
        info3[0] = '\0';
        while (fgets(info2, 9999, user) != NULL) {
            strcat(info3, info2);
        }
        cJSON *json_user_to_follow = cJSON_Parse(info3);



        cJSON *followers_json = NULL;
        followers_json = cJSON_GetObjectItemCaseSensitive(json_user_to_follow, "followers");

        index = 0;
        sign = 0;
        while (cJSON_GetArrayItem(followers_json, index) != NULL) {
            if (strcmp(cJSON_GetArrayItem(followers_json, index)->valuestring, user_to_follow) == 0) {
                sign = index;
            }
            ++index;
        }
        cJSON *username_json = NULL;
        cJSON_DeleteItemFromArray(followers_json, sign);


        char *result2 = cJSON_Print(json_user_to_follow);
        strcpy(path, path_user);
        strcat(path, user_to_follow);
        strcat(path, ".user.json");
        user = fopen(path, "w");
        fputs(result2, user);
        fclose(user);


        cJSON *type = NULL;
        type = cJSON_CreateString("Successful");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("User is unfollowed successfully.");
        cJSON_AddItemToObject(response, "message", message);


        Delete_User_To_Unfollow_tweets_From_UnseenTweets_Of_Follower(user_to_follow, username);


    } else {
        cJSON *type = NULL;
        type = cJSON_CreateString("Error");
        cJSON_AddItemToObject(response, "type", type);

        cJSON *message = NULL;
        message = cJSON_CreateString("User is already unfollowed by you.");
        cJSON_AddItemToObject(response, "message", message);
    }

    char *string_response = cJSON_Print(response);
    return string_response;
}

void Delete_User_To_Unfollow_tweets_From_UnseenTweets_Of_Follower(char user_to_unfollow[100], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, user_to_unfollow);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {

        strcat(info3, info2);
    }
    fclose(user);

    cJSON *user_to_unfollow_json = cJSON_Parse(info3);
    cJSON *tweets_json = NULL;
    tweets_json = cJSON_GetObjectItemCaseSensitive(user_to_unfollow_json, "personalTweets");

    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    fclose(user);

    cJSON *username_json = cJSON_Parse(info3);
    cJSON *unseen_tweets_json = NULL;
    unseen_tweets_json = cJSON_GetObjectItemCaseSensitive(username_json, "unseen tweets");

    int index = 0;
    int len = -1;
    int test;
    while (cJSON_GetArrayItem(unseen_tweets_json, index) != NULL) {
        test = cJSON_GetArrayItem(unseen_tweets_json, index)->valueint;
        ++len;
        ++index;
    }
    index = 0;
    while (cJSON_GetArrayItem(tweets_json, index) != NULL) {
        for (int i = 0; i < len; ++i) {
            int a = cJSON_GetArrayItem(tweets_json, index)->valueint;
            if (cJSON_GetArrayItem(unseen_tweets_json, i) != NULL) {
                int b = cJSON_GetArrayItem(unseen_tweets_json, i)->valueint;
                if (a == b) {
                    cJSON_DeleteItemFromArray(unseen_tweets_json, i);
                }
            }
        }
        ++index;
    }
    char *result = cJSON_Print(username_json);

    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "w");
    fputs(result, user);
    fclose(user);
}

const char *Refresh(char info[10000], int check) {
    if(Wrong_Structure_1(info) == -1){
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, Message("Error", "Wrong structure"));
        return response;
    }
    char username[40];
    strcpy(username, user_info[check].user_name);

    cJSON *response = cJSON_CreateObject();

    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info2[10000];
    char info3[10000];
    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    fclose(user);
    cJSON *json_user = cJSON_Parse(info3);

    cJSON *type = NULL;
    type = cJSON_CreateString("List");
    cJSON_AddItemToObject(response, "type", type);

    cJSON *message = NULL;
    message = cJSON_CreateArray();
    cJSON *unseen_tweets_json = cJSON_GetObjectItemCaseSensitive(json_user, "unseen tweets");
    int index = 0;
    while (cJSON_GetArrayItem(unseen_tweets_json, index) != NULL) {
        FILE *tweet;
        cJSON *id_string_json = cJSON_GetArrayItem(unseen_tweets_json, index);
        char *id_string = cJSON_Print(id_string_json);
        char path2[500];
        strcpy(path2, path_tweet);
        strcat(path2, id_string);
        strcat(path2, ".tweet.json");
        tweet = fopen(path2, "r");

        char info4[10000];
        char info5[10000];
        info4[0] = '\0';
        info5[0] = '\0';
        while (fgets(info4, 9999, tweet) != NULL) {
            strcat(info5, info4);
        }
        cJSON *tweet_json = NULL;
        tweet_json = cJSON_Parse(info5);
        cJSON_AddItemToArray(message, tweet_json);
        ++index;
    }
    cJSON_AddItemToObject(response, "message", message);

    strcpy(path, path_user);
    strcat(path, username);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    info2[0] = '\0';
    info3[0] = '\0';
    while (fgets(info2, 9999, user) != NULL) {
        strcat(info3, info2);
    }
    cJSON *json_user_delete = cJSON_Parse(info3);
    cJSON *unseen_tweets_delete_json = cJSON_GetObjectItemCaseSensitive(json_user_delete, "unseen tweets");
    index = 0;
    while (cJSON_GetArrayItem(unseen_tweets_delete_json, index) != NULL) {
        cJSON_DeleteItemFromArray(unseen_tweets_delete_json, index);
    }
    fclose(user);

    user = fopen(path, "w");
    char *string_new = cJSON_Print(json_user_delete);
    fputs(string_new, user);
    fclose(user);

    char *string_response = cJSON_Print(response);
    return string_response;
}

const char *Like(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char id_to_like[50];
    strcpy(id_to_like, Parsing_Third_part(info));
    int check1 = Available_Tweet_ID(id_to_like, username);
    if (check1 != 0) {
        cJSON *tweet_json = cJSON_Parse(Reading_Tweet_File(id_to_like));

        cJSON *like_json = NULL;
        like_json = cJSON_GetObjectItemCaseSensitive(tweet_json, "likes");

        int index = 0;
        int sign;
        int flag = 0;
        while (cJSON_GetArrayItem(like_json, index) != NULL) {
            cJSON *how_liked_json = cJSON_GetArrayItem(like_json, index);
            if (strcmp(how_liked_json->valuestring, username) == 0) {
                flag = 1;
                sign = index;
            }
            ++index;
        }
        if (flag == 0) {
            cJSON *how_liked_json = NULL;
            how_liked_json = cJSON_CreateString(username);
            cJSON_AddItemToArray(like_json, how_liked_json);

            char *result = cJSON_Print(tweet_json);
            Writing_Tweet_File(id_to_like, result, username);

            char *response = malloc(sizeof(char) * 10000);
            strcpy(response, Message("Successful", "Tweet successfully liked."));
            return response;

        } else {
            char *response = malloc(sizeof(char) * 10000);
            strcpy(response, Message("Error", "Tweet has been already liked. You can\'t like twice."));
            return response;
        }
    } else {
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, Message("Error", "Tweet with this id is not available."));
        return response;
    }

}

const char *Comment(char info[10000], int chec) {
    char username[40];
    strcpy(username, user_info[chec].user_name);
    char string[10000];
    strcpy(string, Parsing_Third_part(info));
    char id[10];
    char comment_content[1000];
    int i = 0;
    while ((string[i] != ',') || (string[i + 1] != ' ')) {
        id[i] = string[i];
        ++i;
    }
    id[i] = '\0';
    ++i;
    ++i;

    int j = 0;
    while ((string[i] != '\n') && (string[i] != '\0')) {
        comment_content[j] = string[i];
        ++j;
        ++i;
    }
    comment_content[j] = '\0';

    int check = Available_Tweet_ID(id, username);


    if (check != 0) {
        char tweet[10000];
        strcpy(tweet, Reading_Tweet_File(id));
        cJSON *tweet_json = NULL;
        tweet_json = cJSON_Parse(tweet);

        cJSON *comments_json = NULL;
        comments_json = cJSON_GetObjectItemCaseSensitive(tweet_json, "comments");

        cJSON *comment_json = NULL;
        comment_json = cJSON_CreateString(comment_content);
        cJSON_AddItemToObject(comments_json, username, comment_json);

        cJSON *comment_counter_json = NULL;
        comment_counter_json = cJSON_GetObjectItemCaseSensitive(tweet_json, "comment counter");
        int comment_counter = comment_counter_json->valueint;
        ++comment_counter;
        cJSON *new_comment_counter_json = NULL;
        new_comment_counter_json = cJSON_CreateNumber(comment_counter);
        cJSON_DeleteItemFromObjectCaseSensitive(tweet_json, "comment counter");
        cJSON_AddItemToObject(tweet_json, "comment counter", new_comment_counter_json);

        char *result = cJSON_Print(tweet_json);

        Writing_Tweet_File(id, result, username);
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, Message("Successful", "Comment is sent successfully."));
        return response;
    } else {
        char *response = malloc(sizeof(char) * 10000);
        strcpy(response, Message("Error", "Tweet with this id is not available."));
        return response;
    }
}

const char *Reading_Tweet_File(char id[50]) {
    FILE *tweet;
    char path[500];
    strcpy(path, path_tweet);
    strcat(path, id);
    strcat(path, ".tweet.json");
    tweet = fopen(path, "r");

    char info[10000];
    char info2[10000];
    info[0] = '\0';
    info2[0] = '\0';
    while (fgets(info, 9999, tweet) != NULL) {
        strcat(info2, info);
    }
    fclose(tweet);

    char *result = malloc(sizeof(char) * 1000);
    strcpy(result, info2);
    return result;
}

const char *Reading_User_File(char user_to_open[50]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, user_to_open);
    strcat(path, ".user.json");
    user = fopen(path, "r");

    char info[10000];
    char info2[10000];
    info[0] = '\0';
    info2[0] = '\0';
    while (fgets(info, 9999, user) != NULL) {
        strcat(info2, info);
    }
    fclose(user);

    char *result = malloc(sizeof(char) * 1000);
    strcpy(result, info2);
    return result;
}

void Writing_Tweet_File(char id[50], char content[10000], char username[40]) {
    FILE *tweet;
    char path[500];
    strcpy(path, path_tweet);
    strcat(path, id);
    strcat(path, ".tweet.json");
    tweet = fopen(path, "w");
    fputs(content, tweet);
    fclose(tweet);
}

void Writing_User_File(char user_to_open[50], char content[10000], char username[40]) {
    FILE *user;
    char path[500];
    strcpy(path, path_user);
    strcat(path, user_to_open);
    strcat(path, ".user.json");
    user = fopen(path, "w");
    fputs(content, user);
    fclose(user);
}

const char *Message(char type[1000], char message[10000]) {

    cJSON *response_cjson = cJSON_CreateObject();

    cJSON *type_cjson = NULL;
    type_cjson = cJSON_CreateString(type);
    cJSON_AddItemToObject(response_cjson, "type", type_cjson);

    cJSON *message_cjson = NULL;
    message_cjson = cJSON_CreateString(message);
    cJSON_AddItemToObject(response_cjson, "message", message_cjson);

    char *response = cJSON_Print(response_cjson);
    return response;
}

char const *Bio(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char bio[5000];
    strcpy(bio, Parsing_Third_part(info));
    char user_content[10000];
    strcpy(user_content, Reading_User_File(username));

    cJSON *user_json = NULL;
    user_json = cJSON_Parse(user_content);

    cJSON_DeleteItemFromObject(user_json, "bio");

    cJSON *new_bio_json = NULL;
    new_bio_json = cJSON_CreateString(bio);
    cJSON_AddItemToObject(user_json, "bio", new_bio_json);

    char *result = cJSON_Print(user_json);

    Writing_User_File(username, result, username);

    char *response = malloc(sizeof(char) * 10000);
    strcpy(response, Message("Successful", "Bio is updated successfully."));
    return response;
}

const char *LogOut(char info[10000], int check) {
    user_info[check].user_name[0] = '\0';
    user_info[check].token[0] = '\0';

    char *response = malloc(sizeof(char) * 10000);
    strcpy(response, Message("Successful", ""));
    return response;
}

const char *Change_Password(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char string[10000];
    strcpy(string, Parsing_Third_part(info));
    char current_password[1000];
    char new_password[1000];
    int i = 0;
    while ((string[i] != ',') || (string[i + 1] != ' ')) {
        current_password[i] = string[i];
        ++i;
    }
    current_password[i] = '\0';
    printf(".%s.\n", current_password);
    ++i;
    ++i;

    int j = 0;
    while ((string[i] != '\n') && (string[i] != '\0')) {
        new_password[j] = string[i];
        ++j;
        ++i;
    }
    new_password[j] = '\0';
    //printf(".%s.\n", new_password);

    char *response = malloc(sizeof(char) * 10000);

    int len = 0;
    for (int k = 0; k < strlen(new_password); ++k) {
        ++len;
    }
    if (len <= 16) {

        char user_file[10000];
        strcpy(user_file, Reading_User_File(username));

        cJSON *user_json = NULL;
        user_json = cJSON_Parse(user_file);

        cJSON *current_password_json = NULL;
        current_password_json = cJSON_GetObjectItemCaseSensitive(user_json, "password");

        if (strcmp(current_password, current_password_json->valuestring) == 0) {
            cJSON_DeleteItemFromObjectCaseSensitive(user_json, "password");

            cJSON *new_password_json = NULL;
            new_password_json = cJSON_CreateString(new_password);
            cJSON_AddItemToObject(user_json, "password", new_password_json);
            char *result = cJSON_Print(user_json);

            Writing_User_File(username, result, username);

            strcpy(response, Message("Successful", "Password successfully changed."));
            return response;

        } else {
            strcpy(response, Message("Error", "Entered current password is wrong."));
            return response;

        }
    } else {
        strcpy(response, Message("Error", "Input password is too long. Please make it smaller than 17 characters."));
        return response;
    }

}

// long length error for sign up

const char *Delete_Tweet(char info[10000], int check) {
    char username[40];
    strcpy(username, user_info[check].user_name);

    char tweet_id_to_delete[50];
    strcpy(tweet_id_to_delete, Parsing_Third_part(info));
    int tweet_id_to_delete_int = atoi(tweet_id_to_delete);

    char *response = malloc(sizeof(char) * 10000);

    int check1 = Available_Tweet_ID(tweet_id_to_delete, username);
    if (check1 == 0) {
        strcpy(response, Message("Error", "Tweet with this id is not available."));
        return response;
    } else {
        if (Available_Tweet_ID_In_User_File(tweet_id_to_delete_int, username) == 0) {
            strcpy(response, Message("Error", "This tweet is not for you."));
            return response;
        } else {
            // deleting id from user file
            Deleting_Tweet_Id_From_User_File(tweet_id_to_delete_int, username);

            cJSON *followers_json = NULL;

        }

    }
}

int Available_Tweet_ID_In_User_File(int id, char username[40]) {
    char user_content[10000];
    strcpy(user_content, Reading_User_File(username));
    cJSON *user_content_json = NULL;
    user_content_json = cJSON_Parse(user_content);

    cJSON *tweet_ids_json = NULL;
    tweet_ids_json = cJSON_GetObjectItemCaseSensitive(user_content_json, "personalTweets");

    int index = 0;
    int flag = 0;
    while (cJSON_GetArrayItem(tweet_ids_json, index)) {
        cJSON *tweet_id_json = NULL;
        tweet_id_json = cJSON_GetArrayItem(tweet_ids_json, index);
        if (tweet_id_json->valueint == id) {
            flag = 1;
        }
        ++index;
    }
    if (flag == 0) {
        return 0;
    } else {
        return 1;
    }
}

void Deleting_Tweet_Id_From_User_File(int id, char username[40]) {
    char user_content[10000];
    strcpy(user_content, Reading_User_File(username));
    cJSON *user_content_json = NULL;
    user_content_json = cJSON_Parse(user_content);

    cJSON *tweet_ids_json = NULL;
    tweet_ids_json = cJSON_GetObjectItemCaseSensitive(user_content_json, "personalTweets");

    int index = 0;
    while (cJSON_GetArrayItem(tweet_ids_json, index)) {
        cJSON *tweet_id_json = NULL;
        tweet_id_json = cJSON_GetArrayItem(tweet_ids_json, index);
        if (tweet_id_json->valueint == id) {
            cJSON_DeleteItemFromArray(tweet_ids_json, index);
        }
        ++index;
    }
}