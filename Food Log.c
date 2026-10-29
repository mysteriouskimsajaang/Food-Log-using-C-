#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the FoodLog structure
typedef struct {
    char name[51];                // Food Name (3-50 alphanumeric characters)
    char type;                    // Food Type (a=appetizer, m=main course, d=dessert)
    int timesEaten;               // Times Eaten (positive number)
    char dateFirstTried[11];      // Date First Tried (mm/dd/yyyy format)
    char location[31];            // Location First Tried (non-empty string of size 30)
    char description[301];        // Description (non-empty string of up to 300 characters)
} FoodLog;

// Define the Recipe structure
typedef struct {
    char name[51];              // Recipe Name (3-50 alphanumeric characters)
    char description[161];      // Recipe Description (non-empty, max 160 chars)
    int timeToPrepare;          // Time to prepare in minutes (positive number)
    int timeToCook;             // Time to cook in minutes (positive number)
    int numIngredients;         // Number of ingredients (positive number)
    char ingredients[20][81];   // List of ingredients (max 20, each max 80 chars)
    int numInstructions;        // Number of instructions (positive number)
    char instructions[20][201]; // Instructions (max 20, each max 200 chars)
} Recipe;

// Function prototypes
// Standard Utility Functions
void clearScreen(void);
void pauseProgram(void);
void removeNewline(char *str);
int getInput(char *prompt, char *input, int maxLength);

// User Validation Functions
void validateUsername(char *username);
void validatePassword(char *password);
void validateFullName(char *fullName);
void validateEmail(char *email);
void validateMobileNumber(char *mobileNumber);

// Food Log Validation Functions
int validateFoodName(char *foodName);
int validateFoodType(char type);
int validateTimesEaten(int timesEaten);
int validateDateFirstTried(char *date);
int validateLocation(char *location);
int validateDescription(char *description);

// Recipe Validation Functions
int validateRecipeName(char *recipeName);

// File Operations and Data Management
void saveFoodLogs(char *username, FoodLog foodLogs[], int foodLogCount);
void saveRecipes(char *username, Recipe recipes[], int recipeCount);
void loadFoodLogs(char *username, FoodLog foodLogs[], int *foodLogCount);
void loadRecipes(char *username, Recipe recipes[], int *recipeCount);
void exportData(char *username, char *password, char *fullName, char *email, char *mobileNumber, FoodLog foodLogs[], int foodLogCount, Recipe recipes[], int recipeCount);
int loadSingleFoodLog(FILE *file, FoodLog *foodLog);
int loadSingleRecipe(FILE *file, Recipe *recipe);
int isFoodLogDuplicate(FoodLog foodLogs[], int foodLogCount, char *name);
int isRecipeDuplicate(Recipe recipes[], int recipeCount, char *name);

// Import/Export and Modify Functions
void importData(char *username, FoodLog foodLogs[], int *foodLogCount, Recipe recipes[], int *recipeCount);
void FoodLogFunc(FoodLog foodLogs[], int *foodLogCount);
void modifyFoodLog(FoodLog foodLogs[], int *foodLogCount);
void modifyRecipe(Recipe recipes[], int *recipeCount);

// Display Functions
void displayUser(char *username, char *fullName, char *email, char *mobileNumber);
void displayAllFoodLogs(FoodLog foodLogs[], int foodLogCount);
void displayAllRecipes(Recipe recipes[], int recipeCount);
void searchFoodLog(FoodLog foodLogs[], int foodLogCount);
void searchRecipe(Recipe recipes[], int recipeCount);

// Main Menu Function
void mainMenu(char *username, char *password, char *fullName, char *email, char *mobileNumber, FoodLog foodLogs[], int *foodLogCount, Recipe recipes[], int *recipeCount);

// Bonus Feature 1: Top 5 Most Eaten Food Logs
void displayTopFoodLogs(FoodLog foodLogs[], int foodLogCount);

// Bonus Feature 2: Top 3 Recipes with Shortest Time
void displayTopRecipes(Recipe recipes[], int recipeCount);

// Utility Functions

/***********************************************
* Function : clearScreen()
* Description : Clears the console screen based on the operating system
* Preconditions : None
* Parameters : None
* Return Value : None
***********************************************/
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/***********************************************
* Function : pauseProgram()
* Description : Pauses the program and waits for the user to press Enter to continue
* Preconditions : None
* Parameters : None
* Return Value : None
***********************************************/
void pauseProgram() {
    printf("\nPress Enter to continue...");
    getchar();
}

/***********************************************
* Function : removeNewline()
* Description : Removes the trailing newline character from a string if present
* Preconditions : str must be a valid null-terminated string
* Parameters : str - The string to modify by removing the newline
* Return Value : None
***********************************************/
void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0U && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/***********************************************
* Function : getInput()
* Description : Reads a line of input from the user with a prompt and stores it in the provided buffer
* Preconditions : prompt and input must be valid strings
* Parameters : prompt - Prompt to display
*              input - Buffer to store input
*              maxLength - Max length of the buffer
* Return Value : 1 if input is read, 0 otherwise
***********************************************/
int getInput(char *prompt, char *input, int maxLength) {
    printf("%s", prompt);
    if (fgets(input, maxLength, stdin)) {
        removeNewline(input);
        return 1;
    }
    return 0;
}

/***********************************************
* Function : getValidInteger()
* Description : Reads a valid integer within a specified range
* Preconditions : min and max must be valid integers
* Parameters : min - Minimum allowed value
*              max - Maximum allowed value
* Return Value : The valid integer entered by the user
***********************************************/
int getValidInteger(int min, int max) {
    int num;
    while (1) {
        if (scanf("%d", &num) == 1 && num >= min && num <= max) {
            while (getchar() != '\n'); // Clear buffer
            return num;
        }
        printf("Invalid input! Please enter a number between %d and %d: ", min, max);
        while (getchar() != '\n'); // Clear buffer
    }
}

// User Validation Functions

/***********************************************
* Function : validateUsername()
* Description : Validates a username to ensure it is 8-50 alphanumeric characters
* Preconditions : username buffer must be initialized
* Parameters : username - Buffer to store validated username
* Return Value : None
***********************************************/
void validateUsername(char *username) {
    int valid = 0;
    while (!valid) {
        getInput("Enter username (8-50 alphanumeric characters): ", username, 51);
        int len = strlen(username);
        valid = (len >= 8 && len <= 50);
        for (int i = 0; i < len && valid; i++) {
            if (!((username[i] >= 'A' && username[i] <= 'Z') ||
                  (username[i] >= 'a' && username[i] <= 'z') ||
                  (username[i] >= '0' && username[i] <= '9'))) {
                valid = 0;
            }
        }
        if (!valid) printf("Invalid username. Only 8-50 alphanumeric characters allowed.\n");
    }
}

/***********************************************
* Function : validatePassword()
* Description : Validates a password to ensure it is 8-20 characters and includes uppercase, lowercase, number, and special character
* Preconditions : password buffer must be initialized
* Parameters : password - Buffer to store validated password
* Return Value : None
***********************************************/
void validatePassword(char *password) {
    int valid = 0;
    while (!valid) {
        getInput("Enter password (8-20 chars, must include uppercase, lowercase, number, special char): ", password, 21);
        int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
        int len = strlen(password);
        if (len >= 8 && len <= 20) {
            for (int i = 0; i < len; i++) {
                if (password[i] >= 'A' && password[i] <= 'Z') hasUpper = 1;
                if (password[i] >= 'a' && password[i] <= 'z') hasLower = 1;
                if (password[i] >= '0' && password[i] <= '9') hasDigit = 1;
                if (strchr("!@#$%&*.", password[i])) hasSpecial = 1;
            }
        }
        valid = hasUpper && hasLower && hasDigit && hasSpecial;
        if (!valid) printf("Invalid password. Must include uppercase, lowercase, number, and special character.\n");
    }
}

/***********************************************
* Function : validateFullName()
* Description : Validates a full name to ensure it is 5-80 alphabetic characters (spaces allowed)
* Preconditions : fullName buffer must be initialized
* Parameters : fullName - Buffer to store validated full name
* Return Value : None
***********************************************/
void validateFullName(char *fullName) {
    int valid = 0;
    while (!valid) {
        getInput("Enter full name (5-80 alphabetic characters): ", fullName, 81);
        int len = strlen(fullName);
        valid = (len >= 5 && len <= 80);
        for (int i = 0; i < len && valid; i++) {
            if (!((fullName[i] >= 'A' && fullName[i] <= 'Z') ||
                  (fullName[i] >= 'a' && fullName[i] <= 'z') ||
                  fullName[i] == ' ')) {
                valid = 0;
            }
        }
        if (!valid) printf("Invalid full name. Only 5-80 alphabetic characters allowed.\n");
    }
}

/***********************************************
* Function : validateEmail()
* Description : Validates an email address to ensure it is max 30 characters and contains '@' and '.'
* Preconditions : email buffer must be initialized
* Parameters : email - Buffer to store validated email address
* Return Value : None
***********************************************/
void validateEmail(char *email) {
    int valid = 0;
    while (!valid) {
        getInput("Enter email address (max 30 chars): ", email, 31);
        int len = strlen(email);
        int hasAt = 0, hasDot = 0, atPos = -1, dotPos = -1;
        if (len <= 30) {
            for (int i = 0; i < len; i++) {
                if (email[i] == '@') {
                    hasAt = 1;
                    atPos = i;
                }
                if (email[i] == '.') {
                    hasDot = 1;
                    dotPos = i;
                }
            }
        }
        valid = hasAt && hasDot && atPos < dotPos;
        if (!valid) printf("Invalid email. Must contain '@' and '.' and be max 30 characters long.\n");
    }
}

/***********************************************
* Function : validateMobileNumber()
* Description : Validates a mobile number to ensure it is 11 digits and starts with 0
* Preconditions : mobileNumber buffer must be initialized
* Parameters : mobileNumber - Buffer to store validated number
* Return Value : None
***********************************************/
void validateMobileNumber(char *mobileNumber) {
    int valid = 0;
    while (!valid) {
        getInput("Enter mobile number (11 digits, starts with 0): ", mobileNumber, 12);
        int len = strlen(mobileNumber);
        valid = (len == 11 && mobileNumber[0] == '0');
        for (int i = 0; i < len && valid; i++) {
            if (mobileNumber[i] < '0' || mobileNumber[i] > '9') {
                valid = 0;
            }
        }
        if (!valid) printf("Invalid mobile number. Must be 11 digits and start with 0.\n");
    }
}

/***********************************************
* Function : usernameExists()
* Description : Checks if a username already exists in the users.txt file
* Preconditions : None
* Parameters : username - The username to check for existence
* Return Value : 1 if the username exists, 0 otherwise
***********************************************/
int usernameExists(char *username) {
    FILE *users = fopen("users.txt", "r");
    if (users == NULL) return 0;
    char line[51];
    int found = 0;
    while (fgets(line, sizeof(line), users) && !found) {
        removeNewline(line);
        if (strcmp(line, username) == 0) {
            found = 1;
        }
    }
    fclose(users);
    return found;
}

/***********************************************
* Function : registerUser()
* Description : Registers a new user by collecting and validating their details and saving them to files
* Preconditions : Buffers must be initialized
* Parameters : username, password, fullName, email, mobileNumber
* Return Value : None
***********************************************/
void registerUser(char *username, char *password, char *fullName, char *email, char *mobileNumber) {
    printf("Let's set you up.\n");
    int usernameTaken = 1;
    while (usernameTaken) {
        validateUsername(username);
        if (usernameExists(username)) {
            printf("Username is taken. Try another.\n");
        } else {
            usernameTaken = 0;
        }
    }
    validatePassword(password);
    validateFullName(fullName);
    validateEmail(email);
    validateMobileNumber(mobileNumber);
    char filename[56];
    snprintf(filename, sizeof(filename), "%s.txt", username);
    FILE *userFile = fopen(filename, "w");
    if (userFile == NULL) {
        printf("Error: Could not create user file.\n");
        return;
    }
    fprintf(userFile, "%s\n%s\n%s\n%s\n", password, fullName, email, mobileNumber);
    fclose(userFile);
    FILE *users = fopen("users.txt", "a");
    if (users == NULL) {
        printf("Error: Could not update users list.\n");
        return;
    }
    fprintf(users, "%s\n", username);
    fclose(users);
    printf("You’re in! Registered as %s.\n", username);
}

/***********************************************
* Function : loginUser()
* Description : Logs in a user by validating their username and password, with up to 3 attempts
* Preconditions : Buffers must be initialized
* Parameters : username, password, fullName, email, mobileNumber
* Return Value : 1 if login is successful, 0 otherwise
***********************************************/
int loginUser(char *username, char *password, char *fullName, char *email, char *mobileNumber) {
    printf("Hi again! Login.\n");
    getInput("Enter username: ", username, 51);
    char filename[56];
    snprintf(filename, sizeof(filename), "%s.txt", username);
    FILE *userFile = fopen(filename, "r");
    if (userFile == NULL) {
        printf("Username not found.\n");
        return 0;
    }
    char storedPassword[21];
    fgets(storedPassword, sizeof(storedPassword), userFile);
    removeNewline(storedPassword);
    fgets(fullName, 81, userFile);
    removeNewline(fullName);
    fgets(email, 31, userFile);
    removeNewline(email);
    fgets(mobileNumber, 12, userFile);
    removeNewline(mobileNumber);
    fclose(userFile);
    int attempts = 3;
    int loggedIn = 0;
    while (attempts > 0 && !loggedIn) {
        getInput("Enter password: ", password, 21);
        if (strcmp(password, storedPassword) == 0) {
            printf("Welcome back, %s!\n", username);
            loggedIn = 1;
        } else {
            attempts--;
            if (attempts > 0) {
                printf("Incorrect password. %d attempt(s) remaining.\n", attempts);
            } else {
                printf("Incorrect password. No attempts remaining. Login failed.\n");
            }
        }
    }
    return loggedIn;
}

// VALIDATION FUNC
/***********************************************
* Function : validateFoodName()
* Description : Validates that the food name is between 3-50 alphanumeric characters.
* Preconditions : foodName must be a valid string
* Parameters : foodName (string to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateFoodName(char *foodName) {
    int len = strlen(foodName);
    if (len < 3 || len > 50) return 0; // Ensure length is between 3 and 50
    for (int i = 0; i < len; i++) {
        if (!((foodName[i] >= 'A' && foodName[i] <= 'Z') ||
              (foodName[i] >= 'a' && foodName[i] <= 'z') ||
              (foodName[i] >= '0' && foodName[i] <= '9') ||
              foodName[i] == ' ')) { // Allow spaces
            return 0;
        }
    }
    return 1;
}

/***********************************************
* Function : validateFoodType()
* Description : Validates that the food type is 'a', 'm', or 'd'.
* Preconditions : type must be a single character
* Parameters : type (the character to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateFoodType(char type) {
    return (type == 'a' || type == 'm' || type == 'd' || type == 'A' || type == 'M' || type == 'D');
}

/***********************************************
* Function : validateTimesEaten()
* Description : Validates that timesEaten is a positive number.
* Preconditions : timesEaten must be a positive integer
* Parameters : timesEaten (integer to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateTimesEaten(int timesEaten) {
    return timesEaten > 0;
}

/***********************************************
* Function : isLeapYear()
* Description : Checks if a given year is a leap year.
*               A leap year is divisible by 4, but not by 100, except when it is divisible by 400.
* Preconditions : year must be a positive integer
* Parameters : year (integer representing the year to check)
* Return Value : 1 if the year is a leap year, 0 otherwise
***********************************************/
int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1; // It's a leap year
    }
    return 0; // Not a leap year
}

/***********************************************
* Function : validateDateFirstTried()
* Description : Validates that the date is in the correct format (mm/dd/yyyy).
* Preconditions : date must be a string in the format mm/dd/yyyy
* Parameters : date (string to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateDateFirstTried(char *date) {
    int month, day, year;
    printf("\nEnter the month (1-12): ");
    if (scanf("%d", &month) != 1 || month < 1 || month > 12) {
        printf("Invalid month! Month must be between 1 and 12.\n");
        return 0; // Invalid month
    }
    printf("Enter the year (yyyy): ");
    if (scanf("%d", &year) != 1 || year < 1) {
        printf("Invalid year! Please enter a valid year.\n");
        return 0; // Invalid year
    }
    int maxDays = 31; // Default max days for most months
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            maxDays = 31;
            break;
        case 4: case 6: case 9: case 11:
            maxDays = 30;
            break;
        case 2:
            maxDays = (isLeapYear(year)) ? 29 : 28; // Leap year check
            break;
        default:
            printf("Invalid month! Month must be between 1 and 12.\n");
            return 0; // Invalid month
    }
    printf("Enter the day (1-%d): ", maxDays);
    if (scanf("%d", &day) != 1 || day < 1 || day > maxDays) {
        printf("Invalid day! Day must be between 1 and %d.\n", maxDays);
        return 0; // Invalid day
    }
    // Format and store the date
    sprintf(date, "%02d/%02d/%04d", month, day, year);
    return 1; // Date is valid
}

/***********************************************
* Function : validateLocation()
* Description : Validates that the location is non-empty and has a maximum length of 30 characters.
* Preconditions : location must be a valid string
* Parameters : location (string to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateLocation(char *location) {
    int valid = 0;
    while (!valid) {
        getInput("Enter Location First Tried (max 30 characters): ", location, 31);
        if (strlen(location) > 0 && strlen(location) <= 30) {
            valid = 1;  // If input is valid, break the loop
        } else {
            printf("Invalid input. Please ensure the location is between 1 and 30 characters.\n");
        }
    }
    return valid;  // Return 1 for valid location
}

/***********************************************
* Function : validateDescription()
* Description : Validates that the description is non-empty and has a maximum length of 300 characters.
* Preconditions : description must be a valid string
* Parameters : description (string to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateDescription(char *description) {
    return (strlen(description) > 0 && strlen(description) <= 300);
}

/***********************************************
* Function : validateRecipeName()
* Description : Validates that the recipe name is between 3-50 alphanumeric characters (spaces allowed).
* Preconditions : recipeName must be a valid string
* Parameters : recipeName (string to validate)
* Return Value : 1 if valid, 0 otherwise
***********************************************/
int validateRecipeName(char *recipeName) {
    int len = strlen(recipeName);
    if (len < 3 || len > 50) return 0; // Ensure length is between 3 and 50
    for (int i = 0; i < len; i++) {
        if (!((recipeName[i] >= 'A' && recipeName[i] <= 'Z') ||
              (recipeName[i] >= 'a' && recipeName[i] <= 'z') ||
              (recipeName[i] >= '0' && recipeName[i] <= '9') ||
              recipeName[i] == ' ')) { // Allow spaces
            return 0;
        }
    }
    return 1;
}

//DELETION FUNC
/***********************************************
* Function : deleteFoodLog()
* Description : Deletes a food log entry from the array based on the food name
* Preconditions : foodLogs array must be initialized
* Parameters : foodLogs - Array of FoodLog structures
*              foodLogCount - Pointer to the number of food logs
* Return Value : None
***********************************************/
void deleteFoodLog(FoodLog foodLogs[], int *foodLogCount) {
    if (*foodLogCount == 0) {
        printf("No food logs available to delete. Please add food logs first.\n");
        pauseProgram();
        return;
    }
    char foodToDelete[51];
    clearScreen();
    getInput("Enter the food name to delete: ", foodToDelete, 51);
    int foodIndex = -1;
    for (int i = 0; i < *foodLogCount; i++) {
        if (strcmp(foodToDelete, foodLogs[i].name) == 0) {
            foodIndex = i;
            break;
        }
    }
    if (foodIndex != -1) {
        printf("\nFood Log to Delete:\n");
        printf("Food Type: %c\n", foodLogs[foodIndex].type);
        printf("Food Name: %s\n", foodLogs[foodIndex].name);
        printf("Times Eaten: %d\n", foodLogs[foodIndex].timesEaten);
        printf("Location First Tried: %s\n", foodLogs[foodIndex].location);
        printf("Description: %s\n", foodLogs[foodIndex].description);
        char confirmation;
        printf("Are you sure you want to delete this food log? [y/n]: ");
        scanf(" %c", &confirmation);
        while (getchar() != '\n');
        if (confirmation == 'y' || confirmation == 'Y') {
            for (int j = foodIndex; j < *foodLogCount - 1; j++) {
                foodLogs[j] = foodLogs[j + 1];
            }
            (*foodLogCount)--;
            printf("Food log deleted successfully!\n");
        } else {
            printf("Deletion canceled.\n");
        }
    } else {
        printf("Food log not found.\n");
    }
    pauseProgram();
}

/***********************************************
* Function : deleteRecipe()
* Description : Deletes a recipe entry from the array based on the recipe name
* Preconditions : recipes array must be initialized
* Parameters : recipes - Array of Recipe structures
*              recipeCount - Pointer to the number of recipes
* Return Value : None
***********************************************/
void deleteRecipe(Recipe recipes[], int *recipeCount) {
    if (*recipeCount == 0) {
        printf("No recipes available to delete. Please add recipes first.\n");
        pauseProgram();
        return;
    }
    char recipeToDelete[51];
    clearScreen();
    getInput("Enter the recipe name to delete: ", recipeToDelete, 51);
    int recipeIndex = -1;
    for (int i = 0; i < *recipeCount; i++) {
        if (strcmp(recipeToDelete, recipes[i].name) == 0) {
            recipeIndex = i;
            break;
        }
    }
    if (recipeIndex != -1) {
        printf("\nRecipe to Delete: %s\n", recipes[recipeIndex].name);
        printf("Description: %s\n", recipes[recipeIndex].description);
        printf("Time to Prepare: %d minutes\n", recipes[recipeIndex].timeToPrepare);
        printf("Time to Cook: %d minutes\n", recipes[recipeIndex].timeToCook);
        printf("Number of Ingredients: %d\n", recipes[recipeIndex].numIngredients);
        printf("Ingredients:\n");
        for (int j = 0; j < recipes[recipeIndex].numIngredients; j++) {
            printf("  %d. %s\n", j + 1, recipes[recipeIndex].ingredients[j]);
        }
        printf("Number of Instructions: %d\n", recipes[recipeIndex].numInstructions);
        char confirmation;
        printf("Are you sure you want to delete this recipe? [y/n]: ");
        scanf(" %c", &confirmation);
        while (getchar() != '\n');
        if (confirmation == 'y' || confirmation == 'Y') {
            for (int j = recipeIndex; j < *recipeCount - 1; j++) {
                recipes[j] = recipes[j + 1];
            }
            (*recipeCount)--;
            printf("Recipe deleted successfully!\n");
        } else {
            printf("Deletion canceled.\n");
        }
    } else {
        printf("Recipe not found.\n");
    }
    pauseProgram();
}

// Data Management Functions
/***********************************************
* Function : saveFoodLogs()
* Description : Saves all food logs to a user-specific file
* Preconditions : foodLogs array must be populated with valid entries
* Parameters : username (user's name), foodLogs (array of FoodLog), foodLogCount (number of logs)
* Return Value : None
***********************************************/
void saveFoodLogs(char *username, FoodLog foodLogs[], int foodLogCount) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_foodlogs.txt", username);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error saving food logs.\n");
        return;
    }
    for (int i = 0; i < foodLogCount; i++) {
        fprintf(file, "%c\n%s\n%d\n%s\n%s\n%s\n",
                foodLogs[i].type, foodLogs[i].name, foodLogs[i].timesEaten,
                foodLogs[i].dateFirstTried, foodLogs[i].location, foodLogs[i].description);
    }
    fclose(file);
}

/***********************************************
* Function : saveRecipes()
* Description : Saves all recipes to a user-specific file
* Preconditions : recipes array must be populated with valid entries
* Parameters : username (user's name), recipes (array of Recipe), recipeCount (number of recipes)
* Return Value : None
***********************************************/
void saveRecipes(char *username, Recipe recipes[], int recipeCount) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_recipes.txt", username);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error saving recipes.\n");
        return;
    }
    for (int i = 0; i < recipeCount; i++) {
        fprintf(file, "%s\n", recipes[i].name);
        fprintf(file, "%s\n", recipes[i].description);
        fprintf(file, "%d\n", recipes[i].timeToPrepare);
        fprintf(file, "%d\n", recipes[i].timeToCook);
        fprintf(file, "%d\n", recipes[i].numIngredients);
        for (int j = 0; j < recipes[i].numIngredients; j++) {
            fprintf(file, "%s\n", recipes[i].ingredients[j]);
        }
        fprintf(file, "%d\n", recipes[i].numInstructions);
        for (int j = 0; j < recipes[i].numInstructions; j++) {
            fprintf(file, "%s\n", recipes[i].instructions[j]);
        }
    }
    fclose(file);
}

/***********************************************
* Function : loadFoodLogs()
* Description : Loads food logs for a user from a file into the foodLogs array
* Preconditions : Username must be provided
* Parameters : username (user's name), foodLogs (array of FoodLog), foodLogCount (pointer)
* Return Value : None
***********************************************/
void loadFoodLogs(char *username, FoodLog foodLogs[], int *foodLogCount) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_foodlogs.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *foodLogCount = 0;
        return;
    }
    *foodLogCount = 0;
    while (*foodLogCount < 50 && !feof(file)) {
        if (fscanf(file, " %c\n", &foodLogs[*foodLogCount].type) == 1) {
            fgets(foodLogs[*foodLogCount].name, 51, file);
            removeNewline(foodLogs[*foodLogCount].name);
            fscanf(file, "%d\n", &foodLogs[*foodLogCount].timesEaten);
            fgets(foodLogs[*foodLogCount].dateFirstTried, 11, file);
            removeNewline(foodLogs[*foodLogCount].dateFirstTried);
            fgets(foodLogs[*foodLogCount].location, 31, file);
            removeNewline(foodLogs[*foodLogCount].location);
            fgets(foodLogs[*foodLogCount].description, 301, file);
            removeNewline(foodLogs[*foodLogCount].description);
            (*foodLogCount)++;
        }
    }
    fclose(file);
}

/***********************************************
* Function : loadRecipes()
* Description : Loads recipes for a user from a file into the recipes array
* Preconditions : Username must be provided
* Parameters : username (user's name), recipes (array of Recipe), recipeCount (pointer)
* Return Value : None
***********************************************/
void loadRecipes(char *username, Recipe recipes[], int *recipeCount) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_recipes.txt", username);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *recipeCount = 0;
        return;
    }
    *recipeCount = 0;
    while (*recipeCount < 20 && !feof(file)) {
        if (fgets(recipes[*recipeCount].name, 51, file) != NULL) {
            removeNewline(recipes[*recipeCount].name);
            fgets(recipes[*recipeCount].description, 161, file);
            removeNewline(recipes[*recipeCount].description);
            fscanf(file, "%d\n", &recipes[*recipeCount].timeToPrepare);
            fscanf(file, "%d\n", &recipes[*recipeCount].timeToCook);
            fscanf(file, "%d\n", &recipes[*recipeCount].numIngredients);
            for (int i = 0; i < recipes[*recipeCount].numIngredients; i++) {
                fgets(recipes[*recipeCount].ingredients[i], 81, file);
                removeNewline(recipes[*recipeCount].ingredients[i]);
            }
            fscanf(file, "%d\n", &recipes[*recipeCount].numInstructions);
            for (int i = 0; i < recipes[*recipeCount].numInstructions; i++) {
                fgets(recipes[*recipeCount].instructions[i], 201, file);
                removeNewline(recipes[*recipeCount].instructions[i]);
            }
            (*recipeCount)++;
        }
    }
    fclose(file);
}

/***********************************************
* Function : exportData()
* Description : Exports user data, food logs, and recipes to files
* Preconditions : User must be logged in and data must be available
* Parameters : username, password, fullName, email, mobileNumber, foodLogs, foodLogCount, recipes, recipeCount
* Return Value : None
***********************************************/
void exportData(char *username, char *password, char *fullName, char *email, char *mobileNumber, FoodLog foodLogs[], int foodLogCount, Recipe recipes[], int recipeCount) {
    char userFilename[56];
    snprintf(userFilename, sizeof(userFilename), "%s.dat", username);
    FILE *userFile = fopen(userFilename, "w");
    if (userFile == NULL) {
        printf("Error exporting user data.\n");
        return;
    }
    fprintf(userFile, "%s\n%s\n%s\n%s\n%s\n", username, password, fullName, email, mobileNumber);
    fclose(userFile);
    saveFoodLogs(username, foodLogs, foodLogCount);
    saveRecipes(username, recipes, recipeCount);
    printf("Data exported successfully!\n");
    pauseProgram();
}

/***********************************************
* Function : loadSingleFoodLog()
* Description : Loads a single food log entry from a file and validates its fields
* Preconditions : File must be opened and contain properly formatted data
* Parameters : file (pointer to FILE), foodLog (pointer to FoodLog structure)
* Return Value : 1 if successful and valid, 0 otherwise
***********************************************/
int loadSingleFoodLog(FILE *file, FoodLog *foodLog) {
    char line[301];
    // Read type
    int ch = fgetc(file);
    if (ch == EOF) return 0; // End of file
    foodLog->type = (char)ch;
    if (fgetc(file) != '\n') return 0; // Consume newline
    if (foodLog->type != 'A' && foodLog->type != 'M' && foodLog->type != 'D') {
        printf("Invalid food type: %c\n", foodLog->type);
        return 0; // Invalid type
    }
    // Read name
    if (fgets(foodLog->name, sizeof(foodLog->name), file) == NULL) {
        printf("Error reading food name.\n");
        return 0;
    }
    removeNewline(foodLog->name);
    if (strlen(foodLog->name) < 3 || strlen(foodLog->name) > 50) {
        printf("Invalid food name: %s\n", foodLog->name);
        return 0; // Invalid name
    }
    // Read timesEaten
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading times eaten.\n");
        return 0;
    }
    foodLog->timesEaten = atoi(line);
    if (foodLog->timesEaten <= 0) {
        printf("Invalid times eaten: %d\n", foodLog->timesEaten);
        return 0; // Invalid timesEaten
    }
    // Read dateFirstTried
    if (fgets(foodLog->dateFirstTried, 11, file) == NULL) {
        printf("Error reading date first tried.\n");
        return 0;
    }
    removeNewline(foodLog->dateFirstTried);
    // Read location
    if (fgets(foodLog->location, sizeof(foodLog->location), file) == NULL) {
        printf("Error reading location.\n");
        return 0;
    }
    removeNewline(foodLog->location);
    if (strlen(foodLog->location) == 0 || strlen(foodLog->location) > 30) {
        printf("Invalid location: %s\n", foodLog->location);
        return 0; // Invalid location
    }
    // Read description
    if (fgets(foodLog->description, sizeof(foodLog->description), file) == NULL) {
        printf("Error reading description.\n");
        return 0;
    }
    removeNewline(foodLog->description);
    if (strlen(foodLog->description) == 0 || strlen(foodLog->description) > 300) {
        printf("Invalid description: %s\n", foodLog->description);
        return 0; // Invalid description
    }
    return 1; // Successfully loaded
}

/***********************************************
* Function : loadSingleRecipe()
* Description : Loads a single recipe entry from a file and validates its fields
* Preconditions : File must be opened and contain properly formatted data
* Parameters : file (pointer to FILE), recipe (pointer to Recipe structure)
* Return Value : 1 if successful and valid, 0 otherwise
***********************************************/
int loadSingleRecipe(FILE *file, Recipe *recipe) {
    char line[161];
    // Read name
    if (fgets(recipe->name, sizeof(recipe->name), file) == NULL) {
        printf("Error reading recipe name.\n");
        return 0;
    }
    removeNewline(recipe->name);
    if (strlen(recipe->name) < 3 || strlen(recipe->name) > 50) {
        printf("Invalid recipe name: %s\n", recipe->name);
        return 0; // Invalid name
    }
    // Read description
    if (fgets(recipe->description, sizeof(recipe->description), file) == NULL) {
        printf("Error reading recipe description.\n");
        return 0;
    }
    removeNewline(recipe->description);
    if (strlen(recipe->description) == 0 || strlen(recipe->description) > 160) {
        printf("Invalid recipe description: %s\n", recipe->description);
        return 0; // Invalid description
    }
    // Read timeToPrepare
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading time to prepare.\n");
        return 0;
    }
    recipe->timeToPrepare = atoi(line);
    if (recipe->timeToPrepare <= 0) {
        printf("Invalid time to prepare: %d\n", recipe->timeToPrepare);
        return 0; // Invalid timeToPrepare
    }
    // Read timeToCook
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading time to cook.\n");
        return 0;
    }
    recipe->timeToCook = atoi(line);
    if (recipe->timeToCook <= 0) {
        printf("Invalid time to cook: %d\n", recipe->timeToCook);
        return 0; // Invalid timeToCook
    }
    // Read numIngredients
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading number of ingredients.\n");
        return 0;
    }
    recipe->numIngredients = atoi(line);
    if (recipe->numIngredients <= 0 || recipe->numIngredients > 20) {
        printf("Invalid number of ingredients: %d\n", recipe->numIngredients);
        return 0; // Invalid numIngredients
    }
    // Read ingredients
    for (int i = 0; i < recipe->numIngredients; i++) {
        if (fgets(recipe->ingredients[i], sizeof(recipe->ingredients[i]), file) == NULL) {
            printf("Error reading ingredient %d.\n", i + 1);
            return 0;
        }
        removeNewline(recipe->ingredients[i]);
        if (strlen(recipe->ingredients[i]) == 0 || strlen(recipe->ingredients[i]) > 80) {
            printf("Invalid ingredient %d: %s\n", i + 1, recipe->ingredients[i]);
            return 0; // Invalid ingredient
        }
    }
    // Read numInstructions
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading number of instructions.\n");
        return 0;
    }
    recipe->numInstructions = atoi(line);
    if (recipe->numInstructions <= 0) {
        printf("Invalid number of instructions: %d\n", recipe->numInstructions);
        return 0; // Invalid numInstructions
    }
    // Read instructions
    for (int i = 0; i < recipe->numInstructions; i++) {
        if (fgets(recipe->instructions[i], sizeof(recipe->instructions[i]), file) == NULL) {
            printf("Error reading instruction %d.\n", i + 1);
            return 0;
        }
        removeNewline(recipe->instructions[i]);
        if (strlen(recipe->instructions[i]) == 0 || strlen(recipe->instructions[i]) > 200) {
            printf("Invalid instruction %d: %s\n", i + 1, recipe->instructions[i]);
            return 0; // Invalid instruction
        }
    }
    return 1; // Successfully loaded
}

/***********************************************
* Function : isFoodLogDuplicate()
* Description : Checks if a food log with the given name already exists
* Preconditions : foodLogs array must be initialized
* Parameters : foodLogs (array of FoodLog), foodLogCount (number of logs), name (name to check)
* Return Value : 1 if duplicate exists, 0 otherwise
***********************************************/
int isFoodLogDuplicate(FoodLog foodLogs[], int foodLogCount, char *name) {
    for (int i = 0; i < foodLogCount; i++) {
        if (strcmp(foodLogs[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

/***********************************************
* Function : isRecipeDuplicate()
* Description : Checks if a recipe with the given name already exists
* Preconditions : recipes array must be initialized
* Parameters : recipes (array of Recipe), recipeCount (number of recipes), name (name to check)
* Return Value : 1 if duplicate exists, 0 otherwise
***********************************************/
int isRecipeDuplicate(Recipe recipes[], int recipeCount, char *name) {
    for (int i = 0; i < recipeCount; i++) {
        if (strcmp(recipes[i].name, name) == 0) {
            return 1;
        }
    }
    return 0;
}

/***********************************************
* Function : importData()
* Description : Imports food logs or recipes from a file with validation and duplicate checks
* Preconditions : User must specify a valid file with correct format
* Parameters : username (user's name), foodLogs (array of FoodLog), foodLogCount (pointer), recipes (array of Recipe), recipeCount (pointer)
* Return Value : None
***********************************************/
void importData(char *username, FoodLog foodLogs[], int *foodLogCount, Recipe recipes[], int *recipeCount) {
    clearScreen();
    int importChoice;
    printf("What would you like to import?\n");
    printf("1. Import Food Logs\n");
    printf("2. Import Recipes\n");
    printf("Enter your choice: ");
    if (scanf("%d", &importChoice) != 1) {
        printf("Invalid input. Please enter 1 or 2.\n");
        while (getchar() != '\n');
        pauseProgram();
        return;
    }
    while (getchar() != '\n');
    char filename[100];
    int entriesAdded = 0, entriesSkipped = 0;
    if (importChoice == 1) {
        if (*foodLogCount >= 50) {
            printf("Food log limit reached (max 50 entries). Cannot import more.\n");
            pauseProgram();
            return;
        }
        getInput("Enter the filename to import food logs from (e.g., foodlogs.txt): ", filename, 100);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Could not open file %s.\n", filename);
            pauseProgram();
            return;
        }
        FoodLog tempLog;
        while (*foodLogCount < 50) {
            int validEntry = loadSingleFoodLog(file, &tempLog);
            if (!validEntry) {
                if (feof(file)) break;
                entriesSkipped++;
            } else {
                if (isFoodLogDuplicate(foodLogs, *foodLogCount, tempLog.name)) {
                    printf("Duplicate food log found: %s. Skipping...\n", tempLog.name);
                    entriesSkipped++;
                } else {
                    foodLogs[*foodLogCount] = tempLog;
                    (*foodLogCount)++;
                    entriesAdded++;
                }
            }
        }
        fclose(file);
        printf("Food logs import completed: %d entries added, %d entries skipped.\n", entriesAdded, entriesSkipped);
    } else if (importChoice == 2) {
        if (*recipeCount >= 20) {
            printf("Recipe limit reached (max 20 entries). Cannot import more.\n");
            pauseProgram();
            return;
        }
        getInput("Enter the filename to import recipes from (e.g., recipes.txt): ", filename, 100);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error: Could not open file %s.\n", filename);
            pauseProgram();
            return;
        }
        Recipe tempRecipe;
        while (*recipeCount < 20) {
            int validEntry = loadSingleRecipe(file, &tempRecipe);
            if (!validEntry) {
                if (feof(file)) break;
                entriesSkipped++;
            } else {
                if (isRecipeDuplicate(recipes, *recipeCount, tempRecipe.name)) {
                    printf("Duplicate recipe found: %s. Skipping...\n", tempRecipe.name);
                    entriesSkipped++;
                } else {
                    recipes[*recipeCount] = tempRecipe;
                    (*recipeCount)++;
                    entriesAdded++;
                }
            }
        }
        fclose(file);
        printf("Recipes import completed: %d entries added, %d entries skipped.\n", entriesAdded, entriesSkipped);
    } else {
        printf("Invalid option.\n");
    }
    pauseProgram();
}

/***********************************************
* Function : FoodLogFunc()
* Description : Adds a new food log entry after validating user input
* Preconditions : foodLogCount must be less than maximum (50)
* Parameters : foodLogs (array of FoodLog), foodLogCount (pointer)
* Return Value : None
***********************************************/
void FoodLogFunc(FoodLog foodLogs[], int *foodLogCount) {
    if (*foodLogCount >= 50) {
        printf("Food log limit reached (max 50 entries).\n");
        pauseProgram();
        return;
    }
    clearScreen();
    char foodName[51];
    int validFoodName = 0;
    while (!validFoodName) {
        getInput("Enter Food Name (3-50 alphanumeric characters, spaces allowed): ", foodName, 51);
        validFoodName = validateFoodName(foodName);
        if (!validFoodName) {
            printf("Invalid Food Name. Must be 3-50 alphanumeric characters.\n");
        }
    }
    char foodType[2];
    int validFoodType = 0;
    while (!validFoodType) {
        getInput("Enter Food Type (a=appetizer, m=main course, d=dessert): ", foodType, 2);
        if (strlen(foodType) == 1 && validateFoodType(foodType[0])) {
            validFoodType = 1;
        } else {
            printf("Invalid Food Type. Must be 'a', 'm', or 'd'.\n");
        }
    }
    printf("Enter Times Eaten (positive number): ");
    int timesEaten = getValidInteger(1, 1000); // Assuming reasonable max
    char dateFirstTried[11];
    int validDate = 0;
    while (!validDate) {
        printf("Enter Date First Tried:\n");
        validDate = validateDateFirstTried(dateFirstTried);
        if (!validDate) {
            printf("Invalid date format. Please try again.\n");
        }
    }
    char location[31];
    int validLocation = 0;
    while (!validLocation) {
        getInput("Enter Location First Tried (max 30 characters): ", location, 31);
        validLocation = validateLocation(location);
        if (!validLocation) {
            printf("Location must be non-empty and max 30 characters.\n");
        }
    }
    char description[301];
    int validDescription = 0;
    while (!validDescription) {
        getInput("Enter Description (max 300 characters): ", description, 301);
        validDescription = validateDescription(description);
        if (!validDescription) {
            printf("Description must be non-empty and max 300 characters.\n");
        }
    }
    // Save the new food log
    FoodLog *log = &foodLogs[*foodLogCount];
    strncpy(log->name, foodName, 51);
    log->type = foodType[0];
    log->timesEaten = timesEaten;
    strncpy(log->dateFirstTried, dateFirstTried, 11);
    strncpy(log->location, location, 31);
    strncpy(log->description, description, 301);
    (*foodLogCount)++;
    printf("\nFood Log Saved Successfully!\n");
    printf("Food Name: %s\n", log->name);
    printf("Food Type: %c\n", log->type);
    printf("Times Eaten: %d\n", log->timesEaten);
    printf("Date First Tried: %s\n", log->dateFirstTried);
    printf("Location: %s\n", log->location);
    printf("Description: %s\n", log->description);
    pauseProgram();
}

/***********************************************
* Function : modifyFoodLog()
* Description : Modifies the name of an existing food log
* Preconditions : foodLogs array must contain at least one entry
* Parameters : foodLogs (array of FoodLog), foodLogCount (pointer)
* Return Value : None
***********************************************/
void modifyFoodLog(FoodLog foodLogs[], int *foodLogCount) {
    if (*foodLogCount == 0) {
        printf("No food logs available to modify. Please add food logs first.\n");
        pauseProgram();
        return;
    }
    char foodToModify[51];
    clearScreen();
    getInput("Enter the food name you want to modify: ", foodToModify, 51);
    int foodIndex = -1;
    for (int i = 0; i < *foodLogCount; i++) {
        if (strcmp(foodToModify, foodLogs[i].name) == 0) {
            foodIndex = i;
            break;
        }
    }
    if (foodIndex == -1) {
        printf("Food log not found.\n");
        pauseProgram();
        return;
    }
    int modifying = 1; // Flag to control loop
    while (modifying) {
        clearScreen();
        printf("\nModifying Food Log: %s\n", foodLogs[foodIndex].name);
        printf("1. Modify Food Name\n");
        printf("2. Modify Food Type\n");
        printf("3. Modify Times Eaten\n");
        printf("4. Modify Date First Tried\n");
        printf("5. Modify Location\n");
        printf("6. Modify Description\n");
        printf("7. Save and Exit\n");
        printf("Choose an option (1-7): ");
        int choice;
        scanf("%d", &choice);
        while (getchar() != '\n');
        if (choice == 1) {
            char newFoodName[51];
            getInput("Enter new food name: ", newFoodName, 51);
            strcpy(foodLogs[foodIndex].name, newFoodName);
            printf("Food name updated!\n");
            pauseProgram();
        } else if (choice == 2) {
            char newFoodType;
            printf("Enter new Food Type (a=appetizer, m=main course, d=dessert): ");
            scanf(" %c", &newFoodType);
            while (getchar() != '\n');
            if (validateFoodType(newFoodType)) {
                foodLogs[foodIndex].type = newFoodType;
                printf("Food type updated!\n");
            } else {
                printf("Invalid food type. No changes made.\n");
            }
            pauseProgram();
        } else if (choice == 3) {
            int newTimesEaten;
            printf("Enter new Times Eaten (positive number): ");
            newTimesEaten = getValidInteger(1, 1000);
            foodLogs[foodIndex].timesEaten = newTimesEaten;
            printf("Times Eaten updated!\n");
            pauseProgram();
        } else if (choice == 4) {
            int validDate = 0;
            while (!validDate) {
                printf("Enter new Date First Tried:\n");
                validDate = validateDateFirstTried(foodLogs[foodIndex].dateFirstTried);
                if (!validDate) {
                    printf("Invalid date. Please try again.\n");
                }
            }
            printf("Date First Tried updated!\n");
            pauseProgram();
        } else if (choice == 5) {
            char newLocation[31];
            getInput("Enter new Location: ", newLocation, 31);
            if (validateLocation(newLocation)) {
                strcpy(foodLogs[foodIndex].location, newLocation);
                printf("Location updated!\n");
            } else {
                printf("Invalid location. No changes made.\n");
            }
            pauseProgram();
        } else if (choice == 6) {
            char newDescription[301];
            getInput("Enter new Description: ", newDescription, 301);
            if (validateDescription(newDescription)) {
                strcpy(foodLogs[foodIndex].description, newDescription);
                printf("Description updated!\n");
            } else {
                printf("Invalid description. No changes made.\n");
            }
            pauseProgram();
        } else if (choice == 7) {
            modifying = 0; // Exit loop when user chooses to save and exit
        } else {
            printf("Invalid option. Please choose 1-7.\n");
            pauseProgram();
        }
    }
    printf("Food log successfully updated!\n");
    pauseProgram();
}

/***********************************************
* Function : modifyRecipe()
* Description : Modifies fields of an existing recipe entry
* Preconditions : recipes array must contain at least one entry
* Parameters : recipes (array of Recipe), recipeCount (pointer)
* Return Value : None
***********************************************/
void modifyRecipe(Recipe recipes[], int *recipeCount) {
    if (*recipeCount == 0) {
        printf("No recipes available to modify. Please add recipes first.\n");
        pauseProgram();
        return;
    }
    char recipeToModify[51];
    clearScreen();
    getInput("Enter the recipe name you want to modify: ", recipeToModify, 51);
    int recipeIndex = -1;
    for (int i = 0; i < *recipeCount; i++) {
        if (strcmp(recipeToModify, recipes[i].name) == 0) {
            recipeIndex = i;
            break;
        }
    }
    if (recipeIndex == -1) {
        printf("Recipe not found.\n");
        pauseProgram();
        return;
    }
    // Display current recipe
    printf("\nCurrent Recipe: %s\n", recipes[recipeIndex].name);
    printf("Description: %s\n", recipes[recipeIndex].description);
    printf("Time to Prepare: %d minutes\n", recipes[recipeIndex].timeToPrepare);
    printf("Time to Cook: %d minutes\n", recipes[recipeIndex].timeToCook);
    printf("Number of Ingredients: %d\n", recipes[recipeIndex].numIngredients);
    // Get new values (press Enter to keep current)
    char newName[51];
    getInput("Enter new recipe name (or press Enter to keep current): ", newName, 51);
    if (strlen(newName) == 0) strcpy(newName, recipes[recipeIndex].name);
    char newDescription[161];
    getInput("Enter new description (or press Enter to keep current): ", newDescription, 161);
    if (strlen(newDescription) == 0) strcpy(newDescription, recipes[recipeIndex].description);
    printf("Enter new time to prepare (minutes, or 0 to keep current): ");
    int newTimeToPrepare = getValidInteger(0, 1000);
    if (newTimeToPrepare == 0) newTimeToPrepare = recipes[recipeIndex].timeToPrepare;
    printf("Enter new time to cook (minutes, or 0 to keep current): ");
    int newTimeToCook = getValidInteger(0, 1000);
    if (newTimeToCook == 0) newTimeToCook = recipes[recipeIndex].timeToCook;
    // Confirm changes
    char confirmation;
    printf("Save changes? (y/n): ");
    scanf(" %c", &confirmation);
    while (getchar() != '\n');
    if (confirmation == 'y' || confirmation == 'Y') {
        strcpy(recipes[recipeIndex].name, newName);
        strcpy(recipes[recipeIndex].description, newDescription);
        recipes[recipeIndex].timeToPrepare = newTimeToPrepare;
        recipes[recipeIndex].timeToCook = newTimeToCook;
        printf("Recipe updated successfully!\n");
    } else {
        printf("No changes were made.\n");
    }
    pauseProgram();
}

/***********************************************
* Function : displayUser()
* Description : Displays the user's profile information
* Preconditions : User must be logged in
* Parameters : username, fullName, email, mobileNumber
* Return Value : None
***********************************************/
void displayUser(char *username, char *fullName, char *email, char *mobileNumber) {
    clearScreen();
    printf("User Profile:\n");
    printf("Username: %s\n", username);
    printf("Full Name: %s\n", fullName);
    printf("Email: %s\n", email);
    printf("Mobile Number: %s\n", mobileNumber);
    pauseProgram();
}

/***********************************************
* Function : displayAllFoodLogs()
* Description : Displays all food logs stored in the array
* Preconditions : foodLogs array must be loaded with valid entries
* Parameters : foodLogs (array of FoodLog), foodLogCount (number of food logs)
* Return Value : None
***********************************************/
void displayAllFoodLogs(FoodLog foodLogs[], int foodLogCount) {
    clearScreen();
    if (foodLogCount == 0) {
        printf("No food logs to display.\n");
    } else {
        printf("All Food Logs:\n");
        for (int i = 0; i < foodLogCount; i++) {
            printf("\nFood Log %d:\n", i + 1);
            char *foodTypeDescription;
            if (foodLogs[i].type == 'a' || foodLogs[i].type == 'A') {
                foodTypeDescription = "Appetizer";
            } else if (foodLogs[i].type == 'm' || foodLogs[i].type == 'M') {
                foodTypeDescription = "Main Course";
            } else if (foodLogs[i].type == 'd' || foodLogs[i].type == 'D') {
                foodTypeDescription = "Dessert";
            } else {
                foodTypeDescription = "Unknown";
            }
            printf("Food Type: %s\n", foodTypeDescription);
            printf("Food Name: %s\n", foodLogs[i].name);
            printf("Times Eaten: %d\n", foodLogs[i].timesEaten);
            printf("Date First Tried: %s\n", foodLogs[i].dateFirstTried);
            printf("Location First Tried: %s\n", foodLogs[i].location);
            printf("Description: %s\n", foodLogs[i].description);
        }
    }
    pauseProgram();
}

/***********************************************
* Function : displayAllRecipes()
* Description : Displays all recipes stored in the array
* Preconditions : Recipes must be loaded into the array
* Parameters : recipes (array of Recipe), recipeCount (number of recipes)
* Return Value : None
***********************************************/
void displayAllRecipes(Recipe recipes[], int recipeCount) {
    clearScreen();
    if (recipeCount == 0) {
        printf("No recipes to display.\n");
    } else {
        printf("All Recipes:\n");
        for (int i = 0; i < recipeCount; i++) {
            printf("\nRecipe %d: %s\n", i + 1, recipes[i].name);
            printf("Description: %s\n", recipes[i].description);
            printf("Time to Prepare: %d minutes\n", recipes[i].timeToPrepare);
            printf("Time to Cook: %d minutes\n", recipes[i].timeToCook);
            printf("Number of Ingredients: %d\n", recipes[i].numIngredients);
            printf("Ingredients:\n");
            for (int j = 0; j < recipes[i].numIngredients; j++) {
                printf("  %d. %s\n", j + 1, recipes[i].ingredients[j]);
            }
            printf("Number of Instructions: %d\n", recipes[i].numInstructions);
            printf("Instructions:\n");
            for (int j = 0; j < recipes[i].numInstructions; j++) {
                printf("  Step %d: %s\n", j + 1, recipes[i].instructions[j]);
            }
        }
    }
    pauseProgram();
}

/***********************************************
* Function : searchFoodLog()
* Description : Searches for food logs by name and displays matching entries
* Preconditions : Food logs must be loaded into the array
* Parameters : foodLogs (array of FoodLog), foodLogCount (number of food logs)
* Return Value : None
***********************************************/
void searchFoodLog(FoodLog foodLogs[], int foodLogCount) {
    if (foodLogCount == 0) {
        printf("No food logs available to search. Please add food logs first.\n");
        pauseProgram();
        return;
    }
    char searchTerm[51];
    clearScreen();
    getInput("Enter food name to search: ", searchTerm, 51);
    int found = 0;
    for (int i = 0; i < foodLogCount; i++) {
        if (strstr(foodLogs[i].name, searchTerm) != NULL) {
            printf("\nFood Log Found:\n");
            printf("Food Type: %c\n", foodLogs[i].type);
            printf("Food Name: %s\n", foodLogs[i].name);
            printf("Times Eaten: %d\n", foodLogs[i].timesEaten);
            printf("Location First Tried: %s\n", foodLogs[i].location);
            printf("Description: %s\n", foodLogs[i].description);
            found = 1;
        }
    }
    if (!found) {
        printf("No food logs found matching '%s'.\n", searchTerm);
    }
    pauseProgram();
}

/***********************************************
* Function : searchRecipe()
* Description : Searches for recipes by name and displays matching entries
* Preconditions : Recipes must be loaded into the array
* Parameters : recipes (array of Recipe), recipeCount (number of recipes)
* Return Value : None
***********************************************/
void searchRecipe(Recipe recipes[], int recipeCount) {
    if (recipeCount == 0) {
        printf("No recipes available to search. Please add recipes first.\n");
        pauseProgram();
        return;
    }
    char searchTerm[51];
    clearScreen();
    getInput("Enter recipe name to search: ", searchTerm, 51);
    int found = 0;
    for (int i = 0; i < recipeCount; i++) {
        if (strstr(recipes[i].name, searchTerm) != NULL) {
            printf("\nRecipe Found: %s\n", recipes[i].name);
            printf("Description: %s\n", recipes[i].description);
            printf("Time to Prepare: %d minutes\n", recipes[i].timeToPrepare);
            printf("Time to Cook: %d minutes\n", recipes[i].timeToCook);
            printf("Number of Ingredients: %d\n", recipes[i].numIngredients);
            printf("Ingredients:\n");
            for (int j = 0; j < recipes[i].numIngredients; j++) {
                printf("  %d. %s\n", j + 1, recipes[i].ingredients[j]);
            }
            printf("Number of Instructions: %d\n", recipes[i].numInstructions);
            printf("Instructions:\n");
            for (int j = 0; j < recipes[i].numInstructions; j++) {
                printf("  Step %d: %s\n", j + 1, recipes[i].instructions[j]);
            }
            found = 1;
        }
    }
    if (!found) {
        printf("No recipes found matching '%s'.\n", searchTerm);
    }
    pauseProgram();
}

/***********************************************
* Function : displayTopFoodLogs()
* Description : Displays the top 5 most eaten food logs
* Preconditions : Food logs must be loaded into the array
* Parameters : foodLogs (array of FoodLog), foodLogCount (number of food logs)
* Return Value : None
***********************************************/
void displayTopFoodLogs(FoodLog foodLogs[], int foodLogCount) {
    if (foodLogCount == 0) {
        printf("No food logs available to display.\n");
        pauseProgram();
        return;
    }
    // Sort food logs by timesEaten in descending order
    for (int i = 0; i < foodLogCount - 1; i++) {
        for (int j = i + 1; j < foodLogCount; j++) {
            if (foodLogs[i].timesEaten < foodLogs[j].timesEaten) {
                FoodLog temp = foodLogs[i];
                foodLogs[i] = foodLogs[j];
                foodLogs[j] = temp;
            }
        }
    }
    printf("Top 5 Most Eaten Food Logs:\n");
    for (int i = 0; i < foodLogCount && i < 5; i++) {
        printf("\nFood Log %d:\n", i + 1);
        printf("Food Type: %c\n", foodLogs[i].type);
        printf("Food Name: %s\n", foodLogs[i].name);
        printf("Times Eaten: %d\n", foodLogs[i].timesEaten);
        printf("Location First Tried: %s\n", foodLogs[i].location);
        printf("Description: %s\n", foodLogs[i].description);
    }
    pauseProgram();
}

/***********************************************
* Function : displayTopRecipes()
* Description : Displays the top 3 recipes with the shortest total time
* Preconditions : Recipes must be loaded into the array
* Parameters : recipes (array of Recipe), recipeCount (number of recipes)
* Return Value : None
***********************************************/
void displayTopRecipes(Recipe recipes[], int recipeCount) {
    if (recipeCount == 0) {
        printf("No recipes available to display.\n");
        pauseProgram();
        return;
    }
    // Sort recipes by total time in ascending order
    for (int i = 0; i < recipeCount - 1; i++) {
        for (int j = i + 1; j < recipeCount; j++) {
            int totalTimeI = recipes[i].timeToPrepare + recipes[i].timeToCook;
            int totalTimeJ = recipes[j].timeToPrepare + recipes[j].timeToCook;
            if (totalTimeI > totalTimeJ) {
                Recipe temp = recipes[i];
                recipes[i] = recipes[j];
                recipes[j] = temp;
            }
        }
    }
    printf("Top 3 Recipes with Shortest Time:\n");
    for (int i = 0; i < recipeCount && i < 3; i++) {
        printf("\nRecipe %d: %s\n", i + 1, recipes[i].name);
        printf("Description: %s\n", recipes[i].description);
        printf("Time to Prepare: %d minutes\n", recipes[i].timeToPrepare);
        printf("Time to Cook: %d minutes\n", recipes[i].timeToCook);
        printf("Total Time: %d minutes\n", recipes[i].timeToPrepare + recipes[i].timeToCook);
    }
    pauseProgram();
}

/***********************************************
* Function : addRecipe()
* Description : Adds a new recipe after validating user input
* Preconditions : recipeCount must be less than maximum (20), and at least one food log must exist
* Parameters : recipes (array of Recipe), recipeCount (pointer), foodLogCount (number of food logs)
* Return Value : None
***********************************************/
void addRecipe(Recipe recipes[], int *recipeCount, int foodLogCount) {
    if (foodLogCount == 0) {
        printf("You must add a food log before you can add a recipe!\n");
        return;
    }
    if (*recipeCount >= 20) {
        printf("Maximum number of recipes reached (max 20).\n");
        return;
    }
    Recipe *newRecipe = &recipes[*recipeCount];
    int validName = 0;
    while (!validName) {
        getInput("Enter recipe name (3-50 alphanumeric characters, spaces allowed): ", newRecipe->name, 51);
        validName = validateRecipeName(newRecipe->name);
        if (!validName) {
            printf("Invalid recipe name. Must be 3-50 alphanumeric characters (spaces allowed).\n");
        }
    }
    int validDesc = 0;
    while (!validDesc) {
        getInput("Enter description (max 160 chars): ", newRecipe->description, 161);
        if (strlen(newRecipe->description) > 0 && strlen(newRecipe->description) <= 160) {
            validDesc = 1;
        } else {
            printf("Description must be non-empty and max 160 characters.\n");
        }
    }
    printf("Enter time to prepare (minutes): ");
    newRecipe->timeToPrepare = getValidInteger(1, 1000);
    printf("Enter time to cook (minutes): ");
    newRecipe->timeToCook = getValidInteger(1, 1000);
    printf("Enter number of ingredients (1-20): ");
    newRecipe->numIngredients = getValidInteger(1, 20);
    for (int j = 0; j < newRecipe->numIngredients; j++) {
        char prompt[100];
        snprintf(prompt, sizeof(prompt), "Enter ingredient %d (max 80 chars): ", j + 1);
        getInput(prompt, newRecipe->ingredients[j], 81);
    }
    printf("Enter number of instructions: ");
    newRecipe->numInstructions = getValidInteger(1, 20);
    for (int j = 0; j < newRecipe->numInstructions; j++) {
        char prompt[100];
        snprintf(prompt, sizeof(prompt), "Enter step %d (max 200 chars): ", j + 1);
        getInput(prompt, newRecipe->instructions[j], 201);
    }
    (*recipeCount)++;
    printf("Recipe added successfully!\n");
}

/***********************************************
* Function : mainMenu()
* Description : Displays the main menu and handles user interactions
* Preconditions : User must be registered and logged in
* Parameters : username, password, fullName, email, mobileNumber, foodLogs, foodLogCount, recipes, recipeCount
* Return Value : None
***********************************************/
void mainMenu(char *username, char *password, char *fullName, char *email, char *mobileNumber, FoodLog foodLogs[], int *foodLogCount, Recipe recipes[], int *recipeCount) {
    int choice = 0;
    while (choice != 12) {
        clearScreen();
        printf("What's up, %s?\n", username);
        printf("1. Add Food Log\n");
        printf("2. Add Recipe\n");
        printf("3. Modify Food Log or Recipe\n");
        printf("4. Delete Food Log or Recipe\n");
        printf("5. Display User Profile\n");
        printf("6. Display All Food Logs and Recipes\n");
        printf("7. Search Food Log or Recipe\n");
        printf("8. Export Data\n");
        printf("9. Import Data\n");
        printf("10. Display Top 5 Most Eaten Food Logs\n");
        printf("11. Display Top 3 Recipes with Shortest Time\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 12.\n");
            while (getchar() != '\n');
            pauseProgram();
            continue;
        }
        while (getchar() != '\n');
        if (choice == 1) {
            FoodLogFunc(foodLogs, foodLogCount);
        } else if (choice == 2) {
            clearScreen();
            addRecipe(recipes, recipeCount, *foodLogCount);
            pauseProgram();
        } else if (choice == 3) {
            clearScreen();
            int modifyChoice;
            printf("What would you like to modify?\n");
            printf("1. Modify Food Log\n");
            printf("2. Modify Recipe\n");
            printf("Enter your choice: ");
            if (scanf("%d", &modifyChoice) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n');
                pauseProgram();
            } else {
                while (getchar() != '\n');
                if (modifyChoice == 1) {
                    modifyFoodLog(foodLogs, foodLogCount);
                } else if (modifyChoice == 2) {
                    modifyRecipe(recipes, recipeCount);
                } else {
                    printf("Invalid option.\n");
                    pauseProgram();
                }
            }
        } else if (choice == 4) {
            clearScreen();
            int deleteChoice;
            printf("What would you like to delete?\n");
            printf("1. Delete Food Log\n");
            printf("2. Delete Recipe\n");
            printf("Enter your choice: ");
            if (scanf("%d", &deleteChoice) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n');
                pauseProgram();
            } else {
                while (getchar() != '\n');
                if (deleteChoice == 1) {
                    deleteFoodLog(foodLogs, foodLogCount);
                } else if (deleteChoice == 2) {
                    deleteRecipe(recipes, recipeCount);
                } else {
                    printf("Invalid option.\n");
                    pauseProgram();
                }
            }
        } else if (choice == 5) {
            displayUser(username, fullName, email, mobileNumber);
        } else if (choice == 6) {
            displayAllFoodLogs(foodLogs, *foodLogCount);
            displayAllRecipes(recipes, *recipeCount);
        } else if (choice == 7) {
            clearScreen();
            int searchChoice;
            printf("What would you like to search?\n");
            printf("1. Search Food Log\n");
            printf("2. Search Recipe\n");
            printf("Enter your choice: ");
            if (scanf("%d", &searchChoice) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n');
                pauseProgram();
            } else {
                while (getchar() != '\n');
                if (searchChoice == 1) {
                    searchFoodLog(foodLogs, *foodLogCount);
                } else if (searchChoice == 2) {
                    searchRecipe(recipes, *recipeCount);
                } else {
                    printf("Invalid option.\n");
                    pauseProgram();
                }
            }
        } else if (choice == 8) {
            exportData(username, password, fullName, email, mobileNumber, foodLogs, *foodLogCount, recipes, *recipeCount);
        } else if (choice == 9) {
            importData(username, foodLogs, foodLogCount, recipes, recipeCount);
        } else if (choice == 10) {
            displayTopFoodLogs(foodLogs, *foodLogCount);
        } else if (choice == 11) {
            displayTopRecipes(recipes, *recipeCount);
        } else if (choice == 12) {
            saveFoodLogs(username, foodLogs, *foodLogCount);
            saveRecipes(username, recipes, *recipeCount);
            printf("Data saved. Exiting...\n");
        } else {
            printf("Invalid choice. Please try again.\n");
            pauseProgram();
        }
    }
}

/***********************************************
* Function : main()
* Description : Program entry point. Handles initial login/registration flow
* Preconditions : None
* Parameters : None
* Return Value : 0 on success, 1 on failure
***********************************************/
int main() {
    char username[51], password[21], fullName[81], email[31], mobileNumber[12];
    FoodLog foodLogs[50];   // Updated to 50 food logs limit
    int foodLogCount = 0;
    Recipe recipes[20];     // Updated to 20 recipes limit
    int recipeCount = 0;
    clearScreen();
    int loginChoice;
    printf("Welcome to the Food Log and Recipe Manager!\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("Enter your choice: ");
    if (scanf("%d", &loginChoice) != 1) {
        printf("Invalid input. Exiting...\n");
        while (getchar() != '\n');
        return 1;
    }
    while (getchar() != '\n');
    clearScreen();
    if (loginChoice == 1) {
        registerUser(username, password, fullName, email, mobileNumber);
    } else if (loginChoice == 2) {
        if (!loginUser(username, password, fullName, email, mobileNumber)) {
            printf("Login failed. Exiting...\n");
            pauseProgram();
            return 1;
        }
    } else {
        printf("Invalid choice. Exiting...\n");
        pauseProgram();
        return 1;
    }
    // Load Food Logs and Recipes after login
    loadFoodLogs(username, foodLogs, &foodLogCount);
    loadRecipes(username, recipes, &recipeCount);
    mainMenu(username, password, fullName, email, mobileNumber, foodLogs, &foodLogCount, recipes, &recipeCount);
    return 0;
}