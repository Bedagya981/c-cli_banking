#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
struct bankData{
	long int accNumber;
	char accHolderName[100];
	double accBalance;
	int pin;
};
void writeData();
void readData();
int getAccNo();
void bankadmin();
void userCheck();
void addBalance();
void transferBalance();
void changePin();
void typeWriter(const char *text);
void history();
int main(){
	char ch;
	typeWriter("Welcome to Everest Bank");
	do{
	printf("Select the service you want to use:\n 1- Create a new account(w)\n 2- Access user details(r)\n 3- Deposit money in account(a)\n 4- Transfer money(t)\n 5- Change PIN(c)\n 6- Access user history(h)\nInput: "); // ask the user for input
	ch = getchar();
	while(getchar() != '\n');
	}while  (ch != 'w' && ch != 'r' && ch != 'a' && ch != 't' && ch != 'c' && ch != 'h');
	switch (ch){ //calls function acc to users needs
		case 'w':
			writeData();
			break;
		case 'r':
			readData();
			break;
		case 'a':
			addBalance();
			break;
		case 't':
			transferBalance();
			break;
		case 'c':
			changePin();
			break;
		case 'h':
			history();
			break;
	}
	return 0;
}
void typeWriter(const char *text) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    // get console info
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int len = strlen(text);
    int spaces = (columns - len) / 2;

    for(int i = 0; i < len; i++) {
        if(i == 0)
            for(int j = 0; j < spaces; j++) printf(" ");
        printf("%c", text[i]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\n");
}
void writeData(){ // used to write data
	FILE *fp;
	FILE *h;
	h = fopen(".\\history.txt", "a");
	fp = fopen(".\\bankdata.txt", "a");
	int n, i;
	printf("Enter the number of accounts you want to create: ");
	scanf("%d", &n);
	struct bankData data[n];
	long lastAcc = getAccNo();
	for( i = 0; i < n; i++ ){
		getchar();
		printf("Enter the name of account holder %d: ",i+1);
		scanf(" %[^\n]", data[i].accHolderName);
		printf("Enter the amount of balance you want to deposit initially: ");
		scanf("%lf", &data[i].accBalance);
		printf("Enter the PIN for your account(4 digits): ");
		scanf("%d", &data[i].pin);
		data[i].accNumber = ++lastAcc;
		fprintf(h, "%08ld\t%s\t%lf\t%d\n", data[i].accNumber, data[i].accHolderName, data[i].accBalance, data[i].pin);
		fprintf(fp, "%08ld\t%s\t%lf\t%d\n", data[i].accNumber, data[i].accHolderName, data[i].accBalance, data[i].pin);
		printf("The account number of %s is %08ld\nThe data has been succesfully uploaded", data[i].accHolderName, data[i].accNumber);
	}
	fclose(fp);
}
int getAccNo(){ // used to get the account number of the last user
	FILE *fp;
	fp = fopen(".\\bankdata.txt", "r");
	if(!fp){
		return 0;
	}
	long int last = 0;
	struct bankData temp;
	while(fscanf(fp, "%ld %s %lf %d", &temp.accNumber, temp.accHolderName, &temp.accBalance, &temp.pin) == 4){
		last = temp.accNumber;
	}
	fclose(fp);
	return last;
}
void readData(){ // used to read data of the user
	char ch;
	do{
	printf("If you are admin press(a), if you want to read your bank details press(u): "); //checks if admin or user
	ch = getchar();
	while(getchar() != '\n');
	}while  (ch != 'a' && ch != 'u');
	if (ch == 'a'){
		bankadmin();
	} else if(ch == 'u'){
		userCheck();
	}
}
void bankadmin(){
	char apass[6];
	char userno;
	long int accNo;
	int found = 0;
	printf("Enter the password for admin privellages: ");
	scanf(" %s", &apass);
		if(strcmp(apass, "bAdmin") == 0){
			FILE *fp;
			fp = fopen(".\\bankdata.txt", "r");
			struct bankData temp;
			do{
			printf("For the detail of single user press(s), for all users of the bank press(a): ");
			scanf(" %c", &userno);
		}while(userno != 'a' && userno != 's');
		if(userno == 'a'){
			while(fscanf(fp, "%ld %[^\t] %lf %d", &temp.accNumber, temp.accHolderName, &temp.accBalance, &temp.pin) == 4){
				printf("\n%08ld\t%s\t%lf", temp.accNumber, temp.accHolderName, temp.accBalance); // prints details of every user of bank for the admin
			}
		}
		else{
			printf("Enter the account number of the user: ");
			scanf("%ld", &accNo);
						printf("The details of user is:\n");

			while(fscanf(fp, "%ld %[^\t] %lf %d", &temp.accNumber, temp.accHolderName, &temp.accBalance, &temp.pin) == 4){
				if(accNo == temp.accNumber){
					printf("\n%08ld\t%s\t%lf", temp.accNumber, temp.accHolderName, temp.accBalance);
					found = 1;
				}
			}
			if(!found){
				printf("Account number not found");
			}
		}
			fclose(fp);
		}else{
			printf("Wrong password");
		}
}
void userCheck(){
	int pin;
	long int accNo;
	int found = 0;
	printf("Enter your account number: ");
	scanf("%ld", &accNo);
	struct bankData temp;
	FILE *fp;
	fp = fopen(".\\bankdata.txt", "r");
	while(fscanf(fp, "%ld %[^\t] %lf %d", &temp.accNumber, temp.accHolderName, &temp.accBalance, &temp.pin) == 4){
		if(accNo == temp.accNumber){ // checks the account number of user
			printf("Enter the pin to access your details: ");
			scanf("%d", &pin);
			if(pin == temp.pin){ // checks the pin of the user
				printf("\n%08ld\t%s\t%lf", temp.accNumber, temp.accHolderName, temp.accBalance);
			} else{
				printf("Wrong Pin");
			}
			found = 1;
		}
	}
	if(!found){
		printf("Account number not found");
	}
	fclose(fp);
}
void addBalance(){
	int pin;
	int found = 0;
	long int accNo;
	double addB;
	printf("Enter your account number: ");
	scanf("%ld", &accNo);
	FILE *fp;
	FILE *temp;
	FILE *h;
	h = fopen(".\\history.txt", "a");
	fp = fopen(".\\bankdata.txt", "r");
	temp = fopen(".\\tempdata.txt", "w");
	struct bankData tempstruct;
	while(fscanf(fp, "%ld %[^\t] %lf %d", &tempstruct.accNumber, tempstruct.accHolderName, &tempstruct.accBalance, &tempstruct.pin) == 4){
		if(accNo == tempstruct.accNumber){ // checks the account number of user
			printf("How much balance do you want to add? ");
			scanf("%lf", &addB);
			printf("Enter the pin to add the balance: ");
			scanf("%d", &pin);
			if(pin == tempstruct.pin){ // checks the pin of the user
				tempstruct.accBalance += addB;
				fprintf(temp, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
			} else{
				printf("Wrong Pin");
			}
			found = 1;
		}
			fprintf(h, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
	}
	fclose(fp);
	fclose(temp);
	fclose(h);
	if(found){
		remove(".\\bankdata.txt");
		rename(".\\tempdata.txt", ".\\bankdata.txt");
		printf("Balance succesfully added please exit the program and check your balance in read mode");

	} else{
		remove(".\\tempdata.txt");
		printf("Account number not found");
	}
}
void transferBalance(){
	int pin;
	int found = 0, correct = 0;
	long int accNo;
	double traB;
	printf("Enter your account number: ");
	scanf("%ld", &accNo);
	FILE *fp;
	FILE *temp;
	FILE *h;
	h = fopen(".\\history.txt", "a");
	fp = fopen(".\\bankdata.txt", "r");
	temp = fopen(".\\tempdata.txt", "w");
	struct bankData tempstruct;
	while(fscanf(fp, "%ld %[^\t] %lf %d", &tempstruct.accNumber, tempstruct.accHolderName, &tempstruct.accBalance, &tempstruct.pin) == 4){
		if(accNo == tempstruct.accNumber){ // checks the account number of user
			printf("How much balance do you want to transfer? ");
			scanf("%lf", &traB);
			if(tempstruct.accBalance >= traB){
			printf("Enter the pin to transfer balance: ");
			scanf("%d", &pin);
			if(pin == tempstruct.pin){ // checks the pin of the user
				tempstruct.accBalance -= traB;
				fprintf(h, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
				correct = 1;
			} else{
				printf("Wrong Pin");
			}
			} else{
				printf("\nThe balance you want to transfer is higher than your account balance, transaction failed");
			}
			found = 1;
		}
			fprintf(temp, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
	}
	fclose(fp);
	fclose(temp);
	fclose(h);
	if(correct){
		remove(".\\bankdata.txt");
		rename(".\\tempdata.txt", ".\\bankdata.txt");
		printf("Balance succesfully transferred please exit the program and check your balance in read mode");

	} else{
		remove(".\\tempdata.txt");
	}
	if(!found){
		printf("Account number not found");
		remove(".\\tempdata.txt");

	}
}
void changePin(){
	int Cpin, Npin;
	int found = 0;
	int correct = 0;
	long int accNo;
	printf("Enter your account number: ");
	scanf("%ld", &accNo);
	FILE *fp;
	FILE *temp;
	FILE *h;
	h = fopen(".\\history.txt", "a");
	fp = fopen(".\\bankdata.txt", "r");
	temp = fopen(".\\tempdata.txt", "w");
	struct bankData tempstruct;
	while(fscanf(fp, "%ld %[^\t] %lf %d", &tempstruct.accNumber, tempstruct.accHolderName, &tempstruct.accBalance, &tempstruct.pin) == 4){
		if(accNo == tempstruct.accNumber){ // checks the account number of user
			printf("Enter Your current pin: ");
			scanf("%d", &Cpin);
			if(Cpin == tempstruct.pin){ // checks the pin of the user
				printf("Enter new pin(4 digit): ");
				scanf("%d", &Npin);
				tempstruct.pin = Npin;
				fprintf(h, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
				correct = 1;
			} else{
				printf("Wrong Pin");
			}
			found = 1;
		}
			fprintf(temp, "%08ld\t%s\t%lf\t%d\n", tempstruct.accNumber, tempstruct.accHolderName, tempstruct.accBalance, tempstruct.pin);
	}
	fclose(fp);
	fclose(temp);
	fclose(h);
	if(correct){
		remove(".\\bankdata.txt");
		rename(".\\tempdata.txt", ".\\bankdata.txt");
		printf("PIN succesfully changed please exit the program and check your PIN in read mode");
	} else{
		remove(".\\tempdata.txt");
	}
	if(!found){
		remove(".\\tempdata.txt");
		printf("Account number not found");
	}
}
void history() {
    FILE *h = fopen(".\\history.txt", "r");
    FILE *fp = fopen(".\\bankdata.txt", "r");

    if(!h || !fp) {
        printf("Error opening files.\n");
        return;
    }

    long int accNo;
    int pin;
    int found = 0;

    printf("Enter your account number: ");
    scanf("%ld", &accNo);

    struct bankData bankvalidation;
    while(fscanf(fp, "%ld %[^\t] %lf %d", &bankvalidation.accNumber, bankvalidation.accHolderName,
                 &bankvalidation.accBalance, &bankvalidation.pin) == 4) {
        if(bankvalidation.accNumber == accNo) {
            found = 1;
            printf("Enter your PIN: ");
            scanf("%d", &pin);
            if(pin != bankvalidation.pin) {
                printf("Wrong PIN\n");
                fclose(fp);
                fclose(h);
                return;
            }
            break;
        }
    }

    if(!found) {
        printf("Account number not found\n");
        fclose(fp);
        fclose(h);
        return;
    }
    struct bankData tempstruct;
    while(fscanf(h, "%ld %[^\t] %lf %d", &tempstruct.accNumber, tempstruct.accHolderName,
                 &tempstruct.accBalance, &tempstruct.pin) == 4) {
        if(tempstruct.accNumber == accNo) {
            printf("%08ld %s %lf %d\n", tempstruct.accNumber, tempstruct.accHolderName,
                   tempstruct.accBalance, tempstruct.pin);
        }
    }

    fclose(fp);
    fclose(h);
}