#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

//Struct containing the necessary components for creation of election and will also be reference for valid ID users
struct currentValidID{
    int year;
    char branch[6];
    int totalVoters;
};

//Struct containing the necessary information for a candidate 
typedef struct candidate{
    int cid;
    char cname[20];
    int votes;
}CANDIDATE;

//Global variables
struct currentValidID currentValidID; //Create instance and stores current Valid user ID parameters
CANDIDATE candidateArray[20]; //Create instances and stores the information necessary for all candidates
int numberOfCandidates; //Total number of candidates running in an election
char studentVotes[500]; //To store information of votes given by each student

//Sample User ID:2018hello00064 year:2018 branch code:hello rollno:00064

//Function prototypes
void display();
int choiceforUser();
void adminPanel();
int authenticateAdmin();
void initiateNewElection();
void saveElectionInfoInFile();
void createCandidateFiles();
void loadElectionInfoFromFile();
void deleteIllegalVote(char userID[15]);
void banID();
int getWinner();
void studentPanel();
void saveVote(char userID[15],char voteInput);
int isValid(char userID[15]);
int isBanned(char userID[15]);
int isVoted(char userID[15]);
int extractYear(char userID[15]);
int extractRollNo(char userID[15]);
int checkBranchCode(char userID[15]);
int readVoteCount();
void updateVoteCount(int count);
int yearchecker(int year);
int branchchecker(char branch[]);
int totalvoterschecker(int num);
int candidatechecker(int num);

int main(){

    char choice;
    int i;
    char ExitMessage[100] = "\n\tProgram is now exiting... Have a nice day!\n";
    
    while(1){ 
        display();
        choice = choiceforUser();

        switch(choice){
            case '1':
                studentPanel(); //Move towards the student panel
                break;
            case '2':
                adminPanel(); //Move towards the admin panel
                break;
            case '0': 
                
                for(i=0; i<strlen(ExitMessage); i++) //Print exitmessage
                {
                    printf("%c",ExitMessage[i]);
                    Sleep(40);
                }
                return 0;
                break;
            default:
                printf("\nInvalid option. Press any key to continue:");
                getch();
                system("cls");
                break;
        }
    }
    return 0;
}

//This function displays introductory message about the program
void display(){
	printf("\n=================================================================================================================\n");
	printf("THIS PROGRAM IS A MINI-VOTING SYSTEM\n");
    printf("UNANTICIPATED USER ACTIONS OR INPUTS MAY LEAD TO BUGS OR THE TERMINATION OF THE PROGRAM.\n");
	printf("=================================================================================================================\n");
}

//This function displays the menu for the user to choose between student and admin panel
int choiceforUser(){
    char choice;
    
    printf("\n\tUSER CHOICES (Select among the options available)\n");
    printf("\t\t[1] Student Panel\n");
    printf("\t\t[2] Admin Panel\n");
    printf("\t\t[0] Exit\n");
    printf("\tChoice: ");
    scanf(" %c", &choice);

    return choice;
}

//This function handles all related-functions with regards to administrative panel
void adminPanel(){
    while(1){
        system("cls");
        if(authenticateAdmin()!=1){
            printf("\nWrong Username or Password. Press any key to continue:");
            getch();
            system("cls");           
            break;
        }

        printf("\n\nLOGGED IN SUCCESSFULLY. Press any key to continue:");
		getch();
       

        while(1)
        {
            system("cls");
            char inputID[15]; int i;
            char input; char banInp;
            int WinnerCid, totalVotedNow=0;
            printf("\n\tUSER CHOICES (Select among the options available)");
            printf("\n\t\t[1] New Election\n\t\t[2] Continue Previous Election\n\t\t[3] Delete Illegal Vote\n\t\t[4] Ban User IDs\n\t\t[5] Result\n\t\t[6] Logout\n\tChoice: ");
			scanf(" %c",&input);

            switch(input)
            {
                case '1':
                	system("cls");
                    initiateNewElection();
                    saveElectionInfoInFile();
                    createCandidateFiles();
                    printf("\nPress any key to continue: ");
                    getch();
                    break;
                case '2':
                    loadElectionInfoFromFile();
                    break;
                case '3':
                    printf("\nEnter user ID to delete its vote: ");
                    scanf("%s",inputID);
                    deleteIllegalVote(inputID);
                    break;
                case '4':
                	banID();
                    getch();
                    break;
                case '5':
                    WinnerCid = getWinner();
                    if(WinnerCid != -1){
                        printf("\nWinner is %s with %d votes\n",candidateArray[WinnerCid-1].cname,candidateArray[WinnerCid-1].votes);
                    }
                    else{
                        printf("\nIts A TIE");
                    }
                    printf("\nFull Result\n");
                    for(i = 0;i<numberOfCandidates;i++){
                        totalVotedNow+=candidateArray[i].votes;
                        printf("%d. %s -> %d votes\n",candidateArray[i].cid,candidateArray[i].cname,candidateArray[i].votes);
                    }
                    printf("\nVoting Percentage: %d %%\n\n",(totalVotedNow*100)/currentValidID.totalVoters);
                    printf("Press any key to continue");
                    getch();
                    break;
                case '6':
                	system("cls");
                    return; 
                default:
                    printf("Invalid Option. Press any key to continue:");
					getch();
            }
			
        }
    }
}

//This function that check whether the user inputs the right admin credentials
int authenticateAdmin(){
    char username[16], password[11];
    
    printf("\nPlease input Admin Credentials to log-in Admin Panel: ");
    printf("\n\nEnter username(Max of 15 characters): ");
    scanf("%s",username);
    if((strcmp(username,"Admin"))!=0) //Checks if the inputted username matches the right username
        return 0;
    else
    {
        printf("Enter Password(Max of 10 characters): ");
        int i=0;
        for(i=0;i<10;i++)
        {
            password[i]=getch();    
            printf("%c",'*');
        }
        password[i]='\0';
        if((strcmp(password,"votewisely"))!=0){ //Checks if the inputted password matches the right password
            printf("\n");
            return 0;
        }
    }
    return 1;
}

//This function handles the creation of election 
void initiateNewElection(){
    printf("\nNew Election Initiation:\n");
    
    do{
        printf("\nElections for which Year: ");
        scanf("%d",&currentValidID.year);

        if(!yearchecker(currentValidID.year)){
            printf("\nPlease re-enter a valid four-digit year.\n");
        }
    }while(!yearchecker(currentValidID.year));
       

    do{
        printf("\nEnter branch code (should be exactly 5 characters): ");
        scanf("%s",currentValidID.branch);

        if(!branchchecker(currentValidID.branch)){
            printf("\nPlease re-enter a valid 5 character branch code\n");
        }
    }while(!branchchecker(currentValidID.branch));

    do{
        printf("\nEnter maximum number of voters (Maximum of 500 voters): ");
        scanf("%d",&currentValidID.totalVoters);

        if(!totalvoterschecker(currentValidID.totalVoters)){
            printf("\nPlease re-enter a valid amount of voters\n");
        }
    }while(!totalvoterschecker(currentValidID.totalVoters));
    

    do{
        printf("Enter the no. of candidates (Maximum of 20 candidates): ");
        scanf("%d",&numberOfCandidates);

        if(!candidatechecker(numberOfCandidates)){
            printf("\nPlease re-enter a valid amount of candidates\n");
        }  
    }while(!candidatechecker(numberOfCandidates));
        

    
    
    int i;
   	for (i = 0; i < currentValidID.totalVoters; i++)
    {
        studentVotes[i] = '0';
    }

    for (i = 0;i < numberOfCandidates; i++) // Number of candidates is already pre-determined with maximum of 20 candidates
    {
        candidateArray[i].cid=i+1;
        printf("\nEnter name of candidate %d: ",i+1);
        scanf(" %s",candidateArray[i].cname);
        candidateArray[i].votes=0;
    }

    FILE *votepointer = fopen("vote_count.txt", "w");

    fclose(votepointer);
    return;
}


//This function creates a text file to save election information
void saveElectionInfoInFile(){
    printf("\nSaving Election Info in File...\n");
    FILE *fpointer = fopen("ElectionInfo.txt", "w");
    if(fpointer==NULL)
    {
        printf("\nError in file creation\n");
        fclose(fpointer);
        return;
    }
    fprintf(
        fpointer,"YEAR: %d\nBRANCH: %s\nTOTAL VOTERS: %d\nNUMBER OF CANDIDATES: %d",
        currentValidID.year,
        currentValidID.branch,
        currentValidID.totalVoters,
        numberOfCandidates
    );
    fclose(fpointer);
    printf("\nSaved Successfully :>");
}

//This function creates file for each candidates
void createCandidateFiles(){
    printf("\nCreating candidate files...\n");
    FILE *fp;
	char filename[20];
	int i;
    for(i = 1;i <= numberOfCandidates; i++){
        sprintf(filename,"candidate%d.txt",i);
		fp=fopen(filename,"w");
        fprintf(
            fp,"0\n%s",
            candidateArray[i-1].cname
        );
		fclose(fp);
    }
    printf("\nCreated Files successfully\n");
}

//This functions load the information that was already created into the program 
void loadElectionInfoFromFile()
{
    FILE *f1, *f2, *f3;
    int successFlag = 1; // Flag to track loading success

    // Open ElectionInfo.txt for reading
    f1 = fopen("ElectionInfo.txt", "r"); //
    if (f1 == NULL){
        printf("\tError: Unable to open the election information file for reading.\n");
        successFlag = 0; // Set the flag to indicate failure
        fclose(f1);
    }
    else{

        fscanf(f1, "YEAR: %d", &currentValidID.year);
        fseek(f1, 2, SEEK_CUR);
        fscanf(f1, "BRANCH: %s", currentValidID.branch);
        fseek(f1, 2, SEEK_CUR);
        fscanf(f1, "TOTAL VOTERS: %d", &currentValidID.totalVoters);
        fseek(f1, 2, SEEK_CUR);
        fscanf(f1, "NUMBER OF CANDIDATES: %d", &numberOfCandidates);

        fclose(f1);

        // Load candidates info and student votes
        int i;
        for (i = 0; i < currentValidID.totalVoters; i++){

            studentVotes[i] = '0';
        }
        for (i = 1; i <= numberOfCandidates; i++){

            int location;
            char filename[20];
            sprintf(filename, "candidate%d.txt", i);
            // Open candidate file for reading
            f2 = fopen(filename, "r+");

            if (f2 == NULL){
                printf("\tError: Unable to open %s for reading.\n", filename);
                fclose(f2);        // Close the ElectionInfo.txt file before returning
                successFlag = 0;    // Set the flag to indicate failure
                break;             // Exit the loop to prevent further attempts
            }
            else{
    
                candidateArray[i - 1].cid = i;
                fscanf(f2,"%d",&candidateArray[i-1].votes);
                fscanf(f2,"%s",candidateArray[i-1].cname);
               
                while (fscanf(f2, "%d", &location) == 1)
                {
                    studentVotes[location - 1] = i + 48;
                }
            }

            fclose(f2);
        }
        
        // Load banned votes
        int location;
        f3 = fopen("banned.txt", "r+");
        if (f3 == NULL){
            fclose(f3);        // Close the banned.txt file before returning
            
        }
        else{
            while (fscanf(f3, "%d", &location) == 1){

                studentVotes[location - 1] = '$';
            }
        }

        fclose(f3);
        
    }
    // Print success message only if all files were loaded successfully
    if (successFlag){
        printf("\n\tData was loaded successfully. Press any key to continue:");
    }
    getch();
}

//This function deletes illegal votes
void deleteIllegalVote(char userID[15]){

    FILE *fp,*fcp;
    char filename[20];
    char line[20];

    if(isValid(userID) != 1){
        printf("\nInvalid User ID. Press any key to exit");
        getch();
        return;
    }

    int location = extractRollNo(userID);
    sprintf(filename,"candidate%d.txt",candidateArray[studentVotes[location-1]-49].cid);
    candidateArray[studentVotes[location-1]-49].votes--;
    studentVotes[location-1]='0';

    if ((fp = fopen(filename,"r")) == NULL){    

        printf("\nFile cannot be opened...1");
        return;
    }
    printf("\nDeleting in process...\n ");    

    if ((fcp = fopen("tmp.txt","w")) == NULL){    

        printf("\nFile cannot be opened...2");
        return;
    }
    
    while (!feof(fp)){

        fscanf(fp,"%s",line);
        fprintf(fcp,"%s\n",line);
    }

    fclose(fp);
    fclose(fcp);

    if ((fp = fopen(filename,"w")) == NULL){    

        printf("\nFile cannot be opened...3");
        return;
    }

    int numFromFile;
    char cnameFromFile[20];
    fcp = fopen("tmp.txt","r");  
    fscanf(fcp,"%d",&numFromFile);
    fprintf(fp,"%d",numFromFile-1);
    fscanf(fcp,"%s",cnameFromFile);
    fprintf(fp,"\n%s",cnameFromFile);

    while(!feof(fcp)){
        fscanf(fcp,"%d",&numFromFile);
        if(numFromFile!=location)
            fprintf(fp,"\n%d",numFromFile);
    }
    fclose(fp);
    fclose(fcp);
    remove("tmp.txt");

    int countremover = readVoteCount();
    updateVoteCount(--countremover);

    printf("\nVote deleted successfully\nPress any key to continue:");
    getch();
}

//This functions can ban students using their specific roll number
void banID(){

    printf("\nCreating Banned.txt...\n");
    FILE *fp=fopen("Banned.txt", "w");

    if(fp==NULL){

        printf("Error: Banned.txt not created.\n");
        fclose(fp);
        return;
    }
    printf("Just enter last roll number to ban\nPress 0 to exit... ");
    int input;
    while(1){

        printf("\nEnter Number: ");
        scanf("%d",&input);
        if(input==0)
            break;
        studentVotes[input-1]='$';
        fprintf(fp,"%d\n",input);
    }
    fclose(fp);
    printf("\nCreated Successfully. Press any key to continue:");
}

//This function calculates the winner of the election
int getWinner(){
	int i;
    int maxV = -1;
    int winnerCid;
    for(i = 0;i < numberOfCandidates; i++){
        if(candidateArray[i].votes > maxV) {
            winnerCid = candidateArray[i].cid;
            maxV = candidateArray[i].votes;      
        }
        else if(candidateArray[i].votes == maxV) {
            return -1;
        }
    }
    return winnerCid;
}

//This function is a student panel for voting 
void studentPanel()
{
    system("cls");
    char key;
    char userID[15];
    char voteInput;
    int i, totalVoters;
	static int ctr;
	int year = currentValidID.year;
    totalVoters = currentValidID.totalVoters;

	ctr = readVoteCount();
    do
    {
    	printf("\n\n\t\t      ***STUDENT OFFICIAL BALLOT***\n");
    	printf("\t\t    ***SPECIAL GENERAL ELECTION***\n");
    	printf("\n\t REMINDERS:");
	    printf("\n\t 1. The length of the userID should be 14.");
	    printf("\n\t 2. The branch code and year should be valid.");
	    printf("\n\t 3. Make sure the format of the User ID is accurate.");
	    printf("\n\t 4. Input the YEAR, BRANCH CODE, and the ROLL NUMBER respectively.");
        printf("\n\t Enter 0 to exit the student panel");
        printf("\n\t  Enter user ID:");
        scanf("%s", userID);

		//The following are the conditions based on the user input
        if (strcmp(userID, "0") == 0)
        {
            return;
        }
        else if (!isValid(userID))
        {
            printf("\n\tInvalid User ID. Please make sure your User ID follows the correct format and is 14 characters long. \n\tPress Enter to continue. If you wish to exit, press 0.");
            key = getch();
			if (key == '0')
			{
			    system("cls");
			    return;
			}
            system("cls");
            continue;
        }
        else if (isBanned(userID))
        {
            printf("\n\tThe provided User ID has been temporarily suspended.\n\tPlease reach out to the administrator to inquire about the reason for the suspension.\n\tPress Enter to proceed. If you wish to exit, press 0.");
            key = getch();
			if (key == '0')
			{
			    system("cls");
			    return;
			}
            system("cls");
            continue;
        }
        else if (isVoted(userID))
        {
            printf("\n\tThe User ID you have entered has already voted. \n\tContact Admin for further query. \n\tPress Enter to continue. If you wish to exit, press 0.");
            key = getch();
			if (key == '0')
			{
			    system("cls");
			    return;
			}
            system("cls");
            continue;
        }
		
		//Displays candidates 
        printf("\n\n\tCANDIDATES OF ELECTION:");
		for (i = 0; i < numberOfCandidates; i++)
		{
		    printf("\n\t[%d] %s", i + 1, candidateArray[i].cname);
		}
		printf("\n\n\tPlease enter the number corresponding to your choice for casting your vote.");
		printf("\n\tVOTE:");
		voteInput = getch();
		printf("*");
		
		//A condition if vote input is invalid
		if (voteInput-48 < 1 || voteInput-48 > numberOfCandidates)
		{
		    printf("\n\tVote is INVALID.\n\tPress any key to try again.");
		    getch(); // Store the result in a variable
		    system("cls");
	        continue;
		}
		saveVote(userID, voteInput);
		printf("\n\t-----------------------------------------------------------------------------------------------------------\n");
		printf("\tCongratulations! Your vote has been cast successfully. ");
		printf("\n\tOnce done, press enter to cast another vote for the next voter.");
		printf("\n\tIf you wish to exit, press 0.");
        ctr++;
		updateVoteCount(ctr);

		key = getch(); // Store the result in a variable
		if (key == '0')
		{
		    system("cls");
		    return;
		}
		system("cls");
    } while (ctr < totalVoters);

	// Check if the loop ended because ctr exceeded totalVoters
	if (ctr == totalVoters)
	{
	char EndMessage[100]     = " \n\tAll votes have been cast. Exiting...\n";
	    for(i=0; i<strlen(EndMessage); i++)
	    {
	        printf("%c",EndMessage[i]);
	        Sleep(40);
	    }
    Sleep(3000);
	system("cls");	
	}
}

//This function verifies if the userID is valid
int isValid(char userID[15]) 
{
    if(strlen(userID)!=14)
        return 0;
    
    int inputedYear=extractYear(userID);
    int inputedRollNo = extractRollNo(userID); //NOTE: The UserID is invalid when the remaining 5 digits are more than the number of voters. 
    
    if(inputedYear!=currentValidID.year || checkBranchCode(userID)!=1 || inputedRollNo>currentValidID.totalVoters)
        return 0;

    return 1;
}

//This function checks if the userID has already voted
int isVoted(char userID[15])
{
    int location = extractRollNo(userID);

    if(studentVotes[location-1]=='0')
        return 0; // The student has not voted for any candidate
    else
        return 1; // The student has already voted
    
}

//This function checks if the userID is banned
int isBanned(char userID[15]){
    int location=extractRollNo(userID);
    if(studentVotes[location-1]=='$')
        return 1; // The student is banned
    else
        return 0; // The student is not banned
}

//This function updates the number of votes, and the roll out number of the voter inside the file
void saveVote(char userID[15], char voteInput)
{
    char filename[20];
    sprintf(filename, "candidate%d.txt", voteInput - 48);
    FILE *fp = fopen(filename, "r+");

    if (fp == NULL)
    {
        printf("Error: Unable to open %s for reading.\n", filename);
        return;
    }

    int location = extractRollNo(userID);
    studentVotes[location - 1] = voteInput;

    // Update the in-memory votes count
    candidateArray[voteInput - 49].votes++;
    
    // Write the updated votes count
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", candidateArray[voteInput - 49].votes);
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "\n%d", location);

    fclose(fp);
}

int extractYear(char userID[15]){
	
	int i;
    int year=0;
    char tmp;
    for(i = 0;i < 4; i++){
        tmp=userID[i];
		year=(year*10)+(tmp-48);
    }
    return year;
}

int extractRollNo(char userID[15]){
	
	int i;
    int rollno=0;
    char tmp;
    for(i = 9; i < 14; i++){
        tmp=userID[i];
		rollno=(rollno*10)+(tmp-48);
    }
    return rollno;  
}

//Will check whether the global branch code and inputed branch code is matching or not
int checkBranchCode(char userID[15]){
	
	int i;
    char branchCode[6];
    for(i = 4; i < 9; i++){
        branchCode[i-4]=userID[i];
    }
    branchCode[5]='\0';
    if(strcmp(branchCode,currentValidID.branch)==0)
        return 1;
    else
        return 0;
}

//This function reads the number of votes
int readVoteCount() {
    FILE *file = fopen("vote_count.txt", "r");
    int count = 0;
    if (file != NULL) {
        fscanf(file, "%d", &count);
        fclose(file);
    }
    return count;
}

//This function updates the number of votes
void updateVoteCount(int count) {
    FILE *file = fopen("vote_count.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", count);
        fclose(file);
    }
}

//This function checks if the user inputs valid year
int yearchecker(int year)
{
    if (year >= 1000 && year <= 9999)
    {
        return 1; // Valid year
    }
    else
    {
        return 0; // Invalid year
    }
}

//This function checks if the user inputs valid branch code 
int branchchecker(char branch[]){
    if (strlen(branch) == 5)
    {
        return 1; // Valid branch code
    }
    else
    {
        return 0; // Invalid branch code
    }
}

//This function checks if the user inputs valid amount of voters
int totalvoterschecker(int num){
    if (num >= 1 && num <= 500)
    {
        return 1; // Valid number of total voters
    }
    else
    {
        return 0; // Invalid number of total voters
    }
}
//This function checks if the user inputs valid amount of candidates

int candidatechecker(int num){
    if (num >= 1 && num <= 20)
    {
        return 1; // Valid number of total candidates
    }
    else
    {
        return 0; // Invalid number of total candidates
    }
}




