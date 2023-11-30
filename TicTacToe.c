#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FIELD_SIZE 3

void print_field(char f[][FIELD_SIZE]);
void clear_field(char f[][FIELD_SIZE]);
char game(char f[][FIELD_SIZE], char);
int is_game_over(char f[][FIELD_SIZE]);
int findPosition(char, int, char f[][FIELD_SIZE], int* , int*);
int random(int range);
void put_inEmpty(char f[][FIELD_SIZE], int*, int*);
void put_inRandomCorner(char f[][FIELD_SIZE], int*, int*);

int main() {
    char field[FIELD_SIZE][FIELD_SIZE], player, winner;
    clear_field(field);
    printf("Выберите за кого будете играть.\n");
    do {
        printf("Введите символ x или o английской раскладки.\n");
        scanf("%c", &player);
        while(getchar()!='\n');
    }while(player != 'x' && player != 'o');
    player == 'x' ? printf("Вы играете за крестики\n") : printf("Вы играете за нолики\n");
    winner = game(field, player);
    if (winner == player) {
        printf("Вы победили!\n");
    }
    else if (winner == 1) {
        printf("Ничья\n");
    }
    else {
        printf("Вы проиграли!\n");
    }
    system("pause");
    return 0;
}

char game(char f[][FIELD_SIZE], char player_char) {
    int step = 1, check_trap = 0, put_trap = 0, i, j;
    int bot_i, bot_j;
    int angles[4][2] = {{0,0}, {0,2}, {2,0}, {2,2}};
    char ch = 'x';
    char winner;
    int r;
    int rowIndex_towin = 0, columnIndex_towin = 0, is_mainDiag_towin = 0, is_sideDiag_towin = 0;

    print_field(f);
    printf("Для совершения хода введите строку и столбец ячейки в которую хотите сходить, например \"2 3\".\n");
    printf("Первыми ходят крестики.\n");
    do { 
        if (player_char == ch) {
            do {
                printf(">> ");
                scanf("%d %d", &i, &j);
                i--;
                j--;
                while(getchar()!='\n');
            }while(i < 0 || i >= FIELD_SIZE || j < 0 || j >= FIELD_SIZE || f[i][j] != ' ');
            f[i][j] = ch;
            print_field(f);
        }
        else {
            if(step == 1) {
                if(f[1][1] == ' ') {
                    bot_i = 1;
                    bot_j = 1;
                    if(ch == 'x')
                        put_trap = 1;
                }
                else {
                    r = random(4);
                    bot_i = angles[r][0];
                    bot_j = angles[r][1];   
                    check_trap = 1;
                }
                step = 0;
            }
            else if(put_trap == 1 && !((i == 0 || i == 2) && (j == 0 || j == 2))) {
                findPosition(player_char, 2, f, &bot_i, &bot_j);
                put_trap = 2;
            }
            else if(put_trap == 2) {
                if(f[bot_i][1] != ' ') {
                    bot_i = abs(bot_i - 2);
                }
                else {
                    bot_j = abs(bot_j - 2);
                }
                put_trap = 0;
            }
            else if(check_trap && abs(i - bot_i) == 2 && abs(j - bot_j) == 2) {
                bot_i = i;
                bot_j = abs(j - 2);
                check_trap = 0;
            }
            else if(findPosition(ch, 1, f, &bot_i, &bot_j) == 0) {
                if(findPosition(player_char, 1, f, &bot_i, &bot_j) == 0) {
                    put_trap = 0;
                    if(findPosition(ch, 2, f, &bot_i, &bot_j) == 0)
                        put_inEmpty(f, &bot_i, &bot_j);
                }
            }
            

            i = bot_i;
            j = bot_j;

            f[i][j] = ch;
            print_field(f);
            printf("Ход компьютера: %d %d\n", bot_i+1, bot_j+1);
        }

        ch = ch == 'x' ? 'o' : 'x';
        winner = is_game_over(f);
    }while(winner == 0);

    return winner;
}

int findPosition(char char_to_find, int cells_left, char f[][FIELD_SIZE], int* bot_i, int* bot_j) {
    int i, j, count, sideCount, ind_i, ind_j;
    
    for(i = 0; i < FIELD_SIZE; i++) {
        count = 0;
        sideCount = 0;
        ind_i = -1;
        ind_j = -1;
        for(j = 0; j < FIELD_SIZE; j++) {
            if(f[i][j] == char_to_find) {
                count++;
            }
            else if(f[i][j] == ' ' && ind_i + ind_j < 0) {
                ind_i = i;
                ind_j = j;
            }
            else {
                if(f[i][j] != ' ') sideCount++;
            }
        }
        if(count == FIELD_SIZE - cells_left && sideCount == 0) {
            *bot_i = ind_i;
            *bot_j = ind_j;
            return 1;
        }
    }
    
    for(j = 0; j < FIELD_SIZE; j++) {
        count = 0;
        sideCount = 0;
        ind_i = -1;
        ind_j = -1;
        for(i = 0; i < FIELD_SIZE; i++) {
            if(f[i][j] == char_to_find) {
                count++;
            }
            else if(f[i][j] == ' ' && ind_i + ind_j < 0) {
                ind_i = i;
                ind_j = j;
            }
            else {
                if(f[i][j] != ' ') sideCount++;
            }
        }
        if(count == FIELD_SIZE - cells_left && sideCount == 0) {
            *bot_i = ind_i;
            *bot_j = ind_j;
            return 1;
        }
    }
    

    count = 0; 
    sideCount = 0;
    ind_i = -1;
    for(i = 0; i < FIELD_SIZE; i++) {
        if(f[i][i] == char_to_find) {
                count++;
        }
        else if(f[i][i] == ' ' && ind_i < 0) {
            ind_i = i;
        }
        else {
            if(f[i][i] != ' ') sideCount++;
        }
    }
    if(count == FIELD_SIZE - cells_left && sideCount == 0) {
        *bot_i = ind_i;
        *bot_j = ind_i;
        return 1;
    }

    count = 0; 
    sideCount = 0;
    ind_i = -1;
    ind_j = -1;
    for(i = 0; i < FIELD_SIZE; i++) {
        if(f[i][FIELD_SIZE - 1 - i] == char_to_find) {
                count++;
        }
        else if(f[i][FIELD_SIZE - 1 - i] == ' ' && ind_i + ind_j < 0) {
            ind_i = i;
            ind_j = FIELD_SIZE - 1 - i;
        }
        else {
            if(f[i][FIELD_SIZE - 1 - i] != ' ') sideCount++;
        }
    }
    if(count == FIELD_SIZE - cells_left && sideCount == 0) {
        *bot_i = ind_i;
        *bot_j = ind_j;
        return 1;
    }
    return 0;
}

void put_inEmpty(char f[][FIELD_SIZE], int* bot_i, int* bot_j) {
    for(int i = 0; i < FIELD_SIZE; i++) {
        for(int j = 0; j < FIELD_SIZE; j++) {
            if(f[i][j] == ' ') {
                *bot_i = i;
                *bot_j = j;
                return;
            }
        }
    }
}

int is_game_over(char f[][FIELD_SIZE]) {
    int i, j, count;
    for(i = 0; i < FIELD_SIZE; i++) {
        count = 1;
        j = 0;
        while(j + 1 < FIELD_SIZE && f[i][j] != ' ' && f[i][j] == f[i][++j])
            count++;
        if(count == FIELD_SIZE) 
            return f[i][j];
    }

    for(j = 0; j < FIELD_SIZE; j++) {
        count = 1;
        i = 0;
        while(i + 1 < FIELD_SIZE && f[i][j] != ' '  && f[i][j] == f[++i][j])
            count++;
        if(count == FIELD_SIZE) 
            return f[i][j];
    }

    i = 0;
    count = 1;
    while(i + 1 < FIELD_SIZE && f[i][i] != ' ' && f[i][i] == f[++i][i])
        count++;
    if(count == FIELD_SIZE) return f[i][i];

    i = 0;
    count = 1;
    while(i + 1 < FIELD_SIZE && f[i][FIELD_SIZE - 1 - i] != ' ' && f[i][FIELD_SIZE - 1 - i] == f[++i][FIELD_SIZE - 1 - i])
        count++;
    if(count == FIELD_SIZE) return f[i][FIELD_SIZE - 1 - i];

    count = 0;
    for(i = 0; i < FIELD_SIZE; i++) {
        for(j = 0; j < FIELD_SIZE; j++) {
            if(f[i][j] != ' ')
                count++;
            else
                break;
        }
    }
    if(count == FIELD_SIZE * FIELD_SIZE)
        return 1;
    
    return 0;
}

void print_field(char f[][FIELD_SIZE]) {
    system("cls");
    printf(" ");
    for(int i = 0; i < FIELD_SIZE; i++) printf(" %d", i+1);
    printf("\n");

    for(int i = 0; i < FIELD_SIZE; i++) {
        printf("%d", i+1);
        for(int j = 0; j < FIELD_SIZE; j++)
            printf(" %c", f[i][j]);
        printf("\n");
    }
}

void clear_field(char f[][FIELD_SIZE]) {
    for (int i = 0; i < FIELD_SIZE; i++)
        for(int j = 0; j < FIELD_SIZE; j++)
            f[i][j] = ' ';
}

int random(int range) {
    time_t t = time(NULL);
    srand(t);
    return rand() % range;
}
