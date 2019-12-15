#include "cal_fun.h"


void see(list*, stack*);

int main(int argc, char* argv[]){
    char* buf;
    FILE *ifp, *ofp;

    // argument check
    if (argc != 2){
        printf("\n%s%s%s\n\n%s\n%s\n\n",
        "Usage: ", argv[0], " infile  outfile",
        "The uppercase letters in infile will be counted.",
        "The results will be written in outfile.");
        exit(1);
    }

    // file read
    ifp = fopen(argv[1], "r");
    // Init
    list* link = (list*)malloc(sizeof(list));
    stack* op = (stack*)malloc(sizeof(stack)); 
    char x, y;
    int one = 1;
    init(link);
    s_init(op);

    if (fscanf(ifp, "%c", &x) == EOF){
        printf("No input in file.\n");
        exit(1);
    }
    if (fscanf(ifp, "%c", &y) == EOF){
        one = 0;
        data_push(link, x);
    }
    // Save
    printf("Calculating...\n");
    int chk[4] = {0};
    chk[0] = chk[1] = chk[2] = chk[3] = 0;
    // chk[0] = '.', chk[1] = '(' 수 확인, chk[2] = 방금 전이 '(' 이였는지, chk[3] = 연산자 수 확인
    int play = 1;
    while(1 && one){
        if (x == '(' || x == ')'){ // '()'
            if (x == '('){ //'('
                if (play) {
                    chk[1]++;
                    chk[2] = 1;
                    s_push(op, x);
                } else{
                    printf("Wrong input found from back.\nExiting...\n");
                    exit(1);
                }
            } else{
                if (chk[1]){
                    chk[1]--;
                    chk[2] = 0;
                    while(s_top(op) != '('){
                        if (s_top(op) == '+'){
                            see(link, op);
                            add(link);
                            s_pop(op);
                        } 
                        link->cnt--;
                        see(link, op);
                    }
                    s_pop(op);
                } else{
                    printf("Wrong input found: )\nExiting...\n");
                    exit(1);
                }
            }

        } else if (x == '+' || x == '-' || x == '*'){
            if (play == 0){
                printf("Wrong input found from back.\nExiting...\n");
                exit(1);
            } else if (x == '-' && chk[2]){
                data_push(link, x);
            } else if(chk[3]){
                printf("Wrong input found.\nExiting...\n");
                exit(1);
            } else if (x == '-'){
                s_push(op, '+');
                node_add(link);
                data_push(link, x);
                chk[3] = 1;
                chk[0] = 0;
            } else {
                s_push(op, x);
                node_add(link);
                chk[3] = 1;
                chk[0] = 0;
            }

        } else if ('0' <= x && x <= '9'){
            if (chk[0]) link->back->b_size++;
            else link->back->f_size++;
            data_push(link, x);
            chk[3] = 0;
            chk[2] = 0;

            
        } else if (x == '.'){
            if (chk[0]){
                printf("Wrong input found: %c\nExiting...\n", x);
                exit(1);
            } else{
                if (play && '0' <= y && y <= '9'){
                    data_push(link, x);
                    chk[0] = 1;
                }
            }

        } else{
            if (x != ' '){
                printf("Wrong input found. Something gone wrong.\nExiting...\n");
                exit(1);
            }
        }
        
        x = y;
        if (play){
            if (fscanf(ifp, "%c", &y) == EOF) play = 0;
        } else{
            break;
        }
    }

    see(link, op);
    // Last Calculate
    while(s_empty(op)){
        if (s_top(op) == '+'){
            add(link);
            s_pop(op);
            see(link, op);
        }
        link->cnt--;
    }
    
    //ERROR?
    if (link->cnt != 1){
        printf("Something gone wrong\nExiting...\n");
        exit(1);
    }

    // Check
    //see(link, op);
    printf("Finished Calculating.\n");
    printf("The result is : ");
    node* curr = link->back;
    while(curr != NULL){
        num* num_curr = curr->head;
        while(num_curr != NULL){
            printf("%c", num_curr->data);
            num_curr = num_curr->next;
        }
        curr = curr->pre_data;
    }
    printf("\n");

    ofp = fopen("output", "w");
    curr = link->back;
    while(curr != NULL){
        num* num_curr = curr->head;
        while(num_curr != NULL){
            fputc(num_curr->data, ofp);
            num_curr = num_curr->next;
        }
        curr = curr->pre_data;
    }
    //Finish    
    free(op);
    free(link);
    fclose(ifp);
    fclose(ofp);
    return 0;
}

void see(list* link, stack* op){
    printf("Showing what is inside in link\n");
    node* curr = link->back;
    while(curr != NULL){
        num* num_curr = curr->head;
        printf("NODE: ");
        while(num_curr != NULL){
            printf("%c", num_curr->data);
            num_curr = num_curr->next;
        }
        printf(" Front Size: %d Back Size: %d\n", curr->f_size ,curr->b_size);
        curr = curr->pre_data;
    }
    printf("\nShowing what is inside in stack\n");
    s_node* sc = op->head;
    while(sc != NULL){
        printf("DATA: %c\n", sc->op_data);
        sc = sc->next_node;
    }
    printf("\n");
}