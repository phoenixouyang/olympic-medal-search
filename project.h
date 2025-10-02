#define MAX_EVENT 3000
#define MAX_STRING 40
#define MAX_RECORDS 250

#define GAMES_FILE "Olympics_Games.csv"
#define EVENT_RESULTS_FILE "Olympic_Athlete_Event_Results.csv"
#define COUNTRY_FILE "Olympics_Country.csv"


struct OlympicResult {
	int year;
	char season[MAX_STRING + 1];
	char countryNoc[MAX_STRING + 1];
	int resultId;
	char medal[MAX_STRING + 1];
};

struct OlympicCountry {
	int goldCount;
	int silverCount;
	int bronzeCount;
	int totalMedals;
	char countryNoc[MAX_STRING + 1];
	char country[MAX_STRING + 1];
};

struct OlympicGames {
	int year;
	char season[MAX_STRING + 1];
};



/* User Input and Validation Functions */

/* Function prompts user for year input */
void getYearInput(int* year, struct OlympicGames games[], int capacity);

/* Function prompts user for an Olympic Season to search */
void getSeasonInput(char season[], struct OlympicGames games[], int capacity);

/* Function validates whether the passed year is a valid Olympic year by checking the struct OlympicGames 
to see if there is data for that year, and displaying errors to help user enter a valid year. 
Returns 1 for a valid year, and 0 for an unvalid year */
int validYear(int year, struct OlympicGames games[], int capacity);

/* Function validates whether the passed season is valid. (Either Winter, Summer, Equestrian or Intercalated.
Returns 1 for a valid year, and 0 for an unvalid year */
int validSeason(char season[], struct OlympicGames games[], int capacity);

/* Function validates whether the passed year and season combination matches an Olympic Game that has occured.
It searches an OlympicGames struct, and returns 1 for a valid combination, and 0 for an unvalid combination */
int validYearSeason(int year, char season[], struct OlympicGames games[], int capacity);

/* Function clears input buffer */
void clearBuffer(void);



/* File Reading Functions */

/* Function reads a csv file listing of Olympic games, and saves it into an OlympicGames struct */
void readOlympicGames(const char* filename, struct OlympicGames games[], int capacity);

/* Function reads a csv file of Olympic results, and saves it into an OlympicResult struct. It returns the number of lines it saved to the struct */
int readOlympicResults(const char* filename, struct OlympicResult results[], int capacity, int year, char season[]);

/* Function reads a csv file of Olympic Countries, and saves it into a OlympicCountry struct. It returns the number of lines it saved to the struct */
int readOlympicCountries(const char* filename, struct OlympicCountry countries[], int capacity);



/* Sorting Functions */

/* Function receives a struct array of Olympic Results, and sorts it by Country */
void sortResultsbyCountry(struct OlympicResult results[], int arrSize);

/* Function receives a struct array of OlympicCountry, and sorts it by total medals. It breaks ties by checking gold medals,
then silver medals, then bronze medals */
void sortCountriesbyMedals(struct OlympicCountry countries[], int arrSize);



/* Medal Counting Functions */

/* Function receives a struct array of Olympic Results. It loops through the results. For each result, it loops through the
OlympicCountry struct array, and adds a medal count to the matching country. It also checks for repeated lines based on
resultId, Country and medal, and skips any repeated results due to team sports */
void countMedals(struct OlympicCountry countries[], struct OlympicResult results[], int countrySize, int resultSize);

/* Function calculates total medals by adding gold, silver and bronze medals */
void totalMedals(struct OlympicCountry countries[], int capacity);



/* Display Functions */

/* Function receives a struct array of OlympicCountries and displays the medal count of each country */
void displayMedals(struct OlympicCountry countries[], int capacity);

/* This function receives a struct array of Olympic Country that has already been sorted, and prints the top 10 results in the array in a histogram */
void displayMedalHistogram(struct OlympicCountry countries[], int year, char season[]);