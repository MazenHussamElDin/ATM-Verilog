


#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#define IDLE 0
#define CardCheck 1
#define PASS_ENT_3 2
#define PASS_ENT_2 3
#define PASS_ENT_1 4
#define LOCK 5
#define ATM_MENU 6
#define Deposit 7
#define Withdraw 8
#define Balance 9
int counterCase= 0; //for debugging

// Declare global variables to represent your Verilog signals
int card[15]; // 16x4 array
int password[15]; // 16x4 array
bool passTrue;
int foundloc;	
bool cardTrue;
int balance[15];
int balancedisplay;
int current_state;
int next_state;
bool start_timer;
bool start_timer1;
bool time_up = false; // Define time_up as a boolean variable and initialize it
bool time_up1 = false; // Define time_up1 as a boolean variable and initialize it



void timer(bool clk, int rst, bool start_timer, bool *time_up) {
    static int counter = 0;

    if (rst) {
        counter = 0;
        *time_up = false;
    } else {
        if (start_timer) {
            if (counter < 1) {
                counter++;
            } else {
                counter = counter;
            }
        } else {
            counter = 0;
        }

        *time_up = (counter == 1);
    }
}

void timer1(bool clk, int rst, bool start_timer, bool *time_up) {
    static int counter = 0;

    if (rst) {
        counter = 0;
        *time_up = false;
    } else {
        if (start_timer) {
            if (counter < 10) {
                counter++;
            } else {
                counter = counter;
            }
        } else {
            counter = 0;
        }

        *time_up = (counter == 10);
    }
}



void readFiles() {
    
    FILE* filecard = fopen("cardForC.txt", "r");
    if (filecard != NULL) {
        // Read binary values from the file into the 'card' array
        for (int i = 0; i < 15; i++) {
            char binaryString[5]; // Assuming a 32-bit binary number
            if (fscanf(filecard, "%s", binaryString) == 1) {
                // Convert binary string to integer
                card[i] = strtol(binaryString, NULL, 2);
            } else {
                printf("Error reading cardForC.txt file\n");
                break;
            }
        }
        fclose(filecard);
    } else {
        printf("Error opening cardForC.txt file\n");
    }


    FILE* filepass = fopen("passForC.txt", "r");
    if (filepass != NULL) {
        // Read binary values from the file into the 'card' array
        for (int i = 0; i < 15; i++) {
            char binaryString[5]; // Assuming a 32-bit binary number
            if (fscanf(filepass, "%s", binaryString) == 1) {
                // Convert binary string to integer
                password[i] = strtol(binaryString, NULL, 2);
            } else {
                printf("Error reading cardForC.txt file\n");
                break;
            }
        }
        fclose(filepass);
    } else {
        printf("Error opening cardForC.txt file\n");
    }


    FILE* filebal = fopen("balanceForC.txt", "r");
    if (filebal != NULL) {
        // Read binary values from the file into the 'card' array
        for (int i = 0; i < 15; i++) {
            char binaryString[5]; // Assuming a 32-bit binary number
            if (fscanf(filebal, "%s", binaryString) == 1) {
                // Convert binary string to integer
                balance[i] = strtol(binaryString, NULL, 2);
            } else {
                printf("Error reading BalanceForC.txt file\n");
                break;
            }
        }
        fclose(filebal);
    } else {
        printf("Error opening balanceForC.txt file\n");
    }

    // Similar logic for other arrays (password, balance)...
}

void writeBalanceToFile() {
    FILE* filebalance = fopen("balanceForC.txt", "w");
    if (filebalance != NULL) {
        // Write values from 'balance' array to the 'balanceForC.txt' file as binary numbers
        for (int i = 0; i < 15; i++) {
            // Convert integer value to 4-bit binary string
            char binaryString[5]; // For a 4-bit binary number (plus null terminator)
            for (int j = 3; j >= 0; j--) {
                binaryString[j] = ((balance[i] >> (3 - j)) & 1) + '0';
            }
            binaryString[4] = '\0'; // Null-terminate the string

            // Write binary string to the file
            fprintf(filebalance, "%s\n", binaryString);
        }
        fclose(filebalance);
    } else {
        printf("Error opening balanceForC.txt file\n");
    }
}



void atmFunction(bool clk, int rst, int BTN3, int BTN2, int BTN1, int Language, int value, int pin, int pin1, int pin2, int cardnumber, int current_state, int next_state, int foundloc) {
	
    
    timer(clk, rst, start_timer, &time_up);
                   
	
    if (rst) {
        current_state = IDLE;
    } else {
        switch (current_state) {
        case IDLE:
            
            foundloc =15;
            balancedisplay=0;
            passTrue=false;
            cardTrue=false;
            time_up1 =0;
            time_up=0;
            start_timer1=0;
            start_timer1=0;
            if (BTN1 == 1 && BTN2 == 0 && BTN3 == 0) {
                if (Language == 0 || Language == 1) {
                    next_state = CardCheck;
                } else {
                    next_state = IDLE;
                }
            } else {
                next_state = IDLE;
            }
            break;

        case CardCheck:
            start_timer1 =1;
            time_up=0;
            timer1(clk, rst, start_timer1, &time_up1);
            if (time_up1 == 1) {
                
                next_state = IDLE; // In case time ended
            } else {
                for (int i = 0; i < 16; i++) {
                    
                    if (cardnumber == card[i]) {
                        cardTrue = true;
                        foundloc = i;
                        break;
                    }
                }
                if (cardTrue == true) {
                    next_state = PASS_ENT_3;
                    break;
                } else {
                    next_state = IDLE;
                    break;
                }
            }
            break;
        case PASS_ENT_3:
            start_timer1 =1;
            time_up=0;
            timer1(clk, rst, start_timer1, &time_up1);
            if(time_up1==1){
                next_state = IDLE; // In case time ended
            }else{
            if (pin == password[foundloc]) {
                passTrue = true;
            }
            if (passTrue) {
                next_state = ATM_MENU;
            } else {
                next_state = PASS_ENT_2;
            }
            
            break;
            }
        case PASS_ENT_2:
            start_timer1 =1;
            time_up=0;
            timer1(clk, rst, start_timer1, &time_up1);
            if(time_up1==1){
                next_state = IDLE; // In case time ended
            }else{
            if (pin1 == password[foundloc]) {
                passTrue = true;
            }
            if (passTrue) {
                next_state = ATM_MENU;
            } else {
                next_state = PASS_ENT_1;
            }
            
            break;
            }

        case PASS_ENT_1:
            start_timer1 =1;
            time_up=0;
            timer1(clk, rst, start_timer1, &time_up1);
            if(time_up1==1){
                next_state = IDLE; // In case time ended
            }else{
            if (pin2 == password[foundloc]) {
                passTrue = true;
            }
            if (passTrue) {
                next_state = ATM_MENU;
            } else {
                next_state = LOCK;
            }
            break;
            }

        case LOCK:
            start_timer=1;
            if (time_up == true) {
                next_state = IDLE; // In case lock time ended
            } else {
                next_state = LOCK; // In case still locked
            }
            break;

        case ATM_MENU:
            if (time_up1 == 1) {
                next_state = IDLE; // In case time ended
            } else {
                if (BTN1 == 0 && BTN2 == 0 && BTN3 == 0) {
                    next_state = IDLE;
                } else if (BTN1 == 0 && BTN2 == 0 && BTN3 == 1) {
                    next_state = Deposit;
                } else if (BTN1 == 0 && BTN2 == 1 && BTN3 == 0) {
                    next_state = Withdraw;
                } else if (BTN1 == 0 && BTN2 == 1 && BTN3 == 1) {
                    next_state = Balance;
                } else {
                    next_state = ATM_MENU;
                }
            }
            break;

        case Deposit:
            
                printf("\n");
                 printf("Deposit case\n");
                 printf("Counter : %d\n",counterCase);
                 
                 printf("Card Number Location: %d\n", foundloc);
                 printf("Current balance: %d\n", balance[foundloc]);
                 int check = balance[foundloc] + value;
                 if(check> 15){
                    next_state = ATM_MENU;
                 }else{
                    balance[foundloc] = balance[foundloc] + value;
                 printf("New balance after deposit: %d\n", balance[foundloc]);
                 }
                 
            
                 next_state = ATM_MENU;
                 break;
     
        case Withdraw:
            
                printf("\n");
                printf("Withdraw case\n");
                
                 printf("Card Number Location: %d\n", foundloc);
                printf("Current balance: %d\n", balance[foundloc]);
                printf("VALUE: %d\n", value);
            if (balance[foundloc] >= value) {
                balance[foundloc] = balance[foundloc] - value;
                printf("New balance after withdrawal: %d\n", balance[foundloc]);
                balancedisplay = balance[foundloc];
                printf("Withdrawal Successful! Balance Change: -%d\n", value);
                printf("Updated Balance: %d\n", balance[foundloc]);
                //writeBalanceToFile();
                //readFiles();
                next_state = ATM_MENU;
                break;
            } else {

                printf(" Value is more than balanceeee\n");
                next_state = ATM_MENU;
                break;
            }
            break;
            

        case Balance:
           
            balancedisplay = balance[foundloc];
            next_state = ATM_MENU;
            break;
            
    }
    }

}


char *intToBinary(int num) {
    // Convert an integer to a binary string
    static char binary[5]; // Assuming a 16-bit binary number
    for (int i = 3; i >= 0; i--) {
        binary[i] = (num & 1) + '0';
        num >>= 1;
    }
    binary[4] = '\0'; // Null-terminate the string
    return binary;
}

int main() {


        readFiles();


        int clk ; // For instance, alternates clk value
        int rst;      // For instance, simulates a reset initially

        // Simulate randomization of other input signals
        int BTN3;
        int BTN2 ;
        int BTN1 ;
        
        int value ; // Just an example value
        int pin ;
        int pin1;
        int pin2 ;
        int cardnumber ;
        int Language ;
        char cardnumber_str[5], pin_str[5], pin1_str[5], pin2_str[5], value_str[5], current_state_str[5], next_state_str[5], foundloc_Str[5];
        

        FILE *file = fopen("RanV.txt", "r");
        if (file == NULL) {
        perror("Error opening file");
        return 1;
        }

    char line[256];
    
    while (fgets(line, sizeof(line), file)) {
      
    // Parse line to extract values as strings
     sscanf(line, "# %*d: , Reset:%d , BTN1:%d, BTN2:%d, BTN3:%d, Card Number: %4s, Pin: %4s, Pin1: %4s, Pin2: %4s, Value: %4s , Current state: %4s, Next State: %4s, FoundLoc: %4s",
               &rst, &BTN1, &BTN2, &BTN3, cardnumber_str, pin_str, pin1_str, pin2_str, value_str, current_state_str, next_state_str, foundloc_Str);
    


 
    // Assuming card number, pin, pin1, pin2, and value are in binary string format
        cardnumber = strtol(cardnumber_str, NULL, 2);
        pin = strtol(pin_str, NULL, 2);
        pin1 = strtol(pin1_str, NULL, 2);
        pin2 = strtol(pin2_str, NULL, 2);
        value = strtol(value_str, NULL, 2);
        current_state = strtol(current_state_str, NULL, 2);
        next_state = strtol(next_state_str, NULL, 2);
        foundloc = strtol(foundloc_Str, NULL, 2);


        
    // Call the function with parsed values
    atmFunction(0, rst, BTN3, BTN2, BTN1, 1, value, pin, pin1, pin2, cardnumber, current_state, next_state, foundloc);
    counterCase++;
}
    fclose(file);

    // Write updated balances to file
    writeBalanceToFile();

    return 0;
}




