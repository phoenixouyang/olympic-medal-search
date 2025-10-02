#define _CRT_SECURE_NO_WARNINGS

// include libraries
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// include prototypes, structs and macros
#include "project.h"


/* User Input and Validation Functions */

void getYearInput(int* year, struct OlympicGames games[], int capacity)
{
	int valid = 0;

	// prompt and validate user inputted year
	do {
		printf("Enter an Olympic year: ");
		scanf("%d", year);

		clearBuffer();

		valid = validYear(*year, games, capacity);

	} while (!valid);
}

int validYear(int year, struct OlympicGames games[], int capacity)
{
	int i, valid = 1;
	int yearMatches = 0;

	if (year == 2024) {
		printf("Sorry! This program currently does not include data on the 2024 Paris Olympics.\n\n");
		valid = 0;
	}
	else if (year > 2025) {
		printf("Error: %d has not happened yet.\n\n", year);
		valid = 0;
	}
	else if (year < 1896) {
		printf("Error: The first Olympic games did not take place until 1896.\n\n");
		valid = 0;
	}
	else if (year % 2) {
		printf("Error: %d is not a valid Olympic year.\n\n", year);
		valid = 0;
	}
	else if (year == 1916 || year == 1940 || year == 1944) {
		printf("Error: Olympics were not held in %d due to war.\n\n", year);
		valid = 0;
	}
	else {
		// loop through struct array OlympicGames to check if year exists in database
		for (i = 0; i < capacity || games[i].year != 0; i++) {
			if (year == games[i].year) {
				yearMatches = 1;
			}
		}
		if (yearMatches == 0) {
			printf("Error: Olympics were not held in %d.\n\n", year);
			valid = 0;
		}
	}
	
	return valid;
}

void getSeasonInput(char season[], struct OlympicGames games[], int capacity)
{
	int valid = 0;

	// prompt and validate user inputted season
	do {
		printf("Enter an Olympic season ( Winter | Summer | Equestrian | Intercalated ): ");
		scanf("%s", season);

		clearBuffer();

		valid = validSeason(season, games, capacity);

	} while (!valid);
}

int validSeason(char season[], struct OlympicGames games[], int capacity)
{
	int valid = 0;

	// format season with capitalized first word
	season[0] = toupper(season[0]);

	if (strcmp(season, "Winter") == 0 || strcmp(season, "Summer") == 0 || strcmp(season, "Equestrian") == 0 || strcmp(season, "Intercalated") == 0) {
		valid = 1;
	}
	else {
		printf("Error: \"%s\" is not a valid Olympic season\n\n", season);
	}
	return valid;
}

int validYearSeason(int year, char season[], struct OlympicGames games[], int capacity)
{
	int i, valid = 0;

	// loop through struct array OlympicGames to check if year and season combination exists in database
	for (i = 0; i < capacity || games[i].year != 0; i++) {
		if (year == games[i].year) {
			if (strcmp(season, games[i].season) == 0) {
				valid = 1;
			}
		}
	}
	return valid;
}

void clearBuffer(void)
{
	while (getchar() != '\n');
}


/* File Reading Functions */

void readOlympicGames(const char* filename, struct OlympicGames games[], int capacity)
{
	int i = 0;

	FILE* fp = fopen(filename, "r");

	if (fp != NULL) {
		fscanf(fp, "%*[^\n]\n"); // discard first header row of file

		while (i < capacity && fscanf(fp, "%d %40[^,],%*[^\n]\n", 
			&games[i].year, games[i].season) == 2) {
			
			// Discard word "Olympics" in edition column if it exists - only store season
			sscanf(games[i].season, "%40s", games[i].season);

			i++;
		}
		fclose(fp);
		fp = NULL;
	}
	else {
		printf("Sorry, file %s could not be read.", filename);
	}
}

int readOlympicResults(const char* filename, struct OlympicResult results[], int capacity, int year, char season[])
{
	int i = 0;

	FILE* fp = fopen(filename, "r");

	if (fp != NULL) {
		fscanf(fp, "%*[^\n]\n"); // discard first header row of file

		while (i < capacity && fscanf(fp, "%d %40[^,],%*[^,],%3s,%*[^,],\"%*[^\"]\",%d,%*[^,],%*[^,],%*[^,],%40[^,],%*[^,],%*[^\n]\n", 
			&results[i].year, results[i].season, results[i].countryNoc, &results[i].resultId, results[i].medal) == 5) {
			sscanf(results[i].season, "%40s", results[i].season);

			// overwrite last record if it does not match the correct year / season inputted by user, and no medal was won
			if (results[i].year == year && strcmp(results[i].season, season) == 0 && strcmp(results[i].medal, "-") != 0) {
				i++;
			}
		}
		fclose(fp);
		fp = NULL;
	}
	else {
		printf("Sorry, file %s could not be read.", filename);
	}
	return i;
}

int readOlympicCountries(const char* filename, struct OlympicCountry countries[], int capacity)
{
	int i = 0;

	FILE* fp = fopen(filename, "r");

	if (fp != NULL) {
		fscanf(fp, "%*[^\n]\n"); // discard first header row of file

		while (i < capacity && fscanf(fp, "%3s,%40[^\n]\n", countries[i].countryNoc, countries[i].country) == 2) {
			// Do not include duplicated line for Russian Olympic Committee
			if (strcmp(countries[i].country, "ROC") != 0) {
				i++;
			}
		}
		fclose(fp);
		fp = NULL;
	}
	else {
		printf("Sorry, file %s could not be read.", filename);
	}

	return i;
}

/* Sorting Functions */

void sortResultsbyCountry(struct OlympicResult results[], int arrSize)
{
	int iterIdx, tstElemIdx, curMinIdx;
	struct OlympicResult tempVal; // <--- used in swapping int elements

	for (iterIdx = 0; iterIdx < arrSize; iterIdx++)
	{
		curMinIdx = iterIdx;

		for (tstElemIdx = iterIdx + 1; tstElemIdx < arrSize; tstElemIdx++)
		{
			// check if the test element is < our current marked lowest value
			if (strcmp(results[tstElemIdx].countryNoc, results[curMinIdx].countryNoc) < 0)
			{
				// mark the test element index as the next lowest
				curMinIdx = tstElemIdx;
			}
		}
		// Swap needs to be done!
		if (curMinIdx != iterIdx)
		{
			tempVal = results[iterIdx];
			results[iterIdx] = results[curMinIdx];
			results[curMinIdx] = tempVal;
		}
	}
}

void sortCountriesbyMedals(struct OlympicCountry countries[], int arrSize)
{
	int iterIdx, tstElemIdx, curMaxIdx;
	struct OlympicCountry tempVal; // <--- used in swapping struct elements

	for (iterIdx = 0; iterIdx < arrSize; iterIdx++)
	{
		curMaxIdx = iterIdx;

		for (tstElemIdx = iterIdx + 1; tstElemIdx < arrSize; tstElemIdx++)
		{
			// check if the test country's total medals are > our current countries marked highest medal
			if (countries[tstElemIdx].totalMedals > countries[curMaxIdx].totalMedals) {
				// mark the test element index as the next highest
				curMaxIdx = tstElemIdx;
			}
			// check gold medals if total medals are tied
			else if (countries[tstElemIdx].totalMedals == countries[curMaxIdx].totalMedals) {
				if (countries[tstElemIdx].goldCount > countries[curMaxIdx].goldCount) {
					curMaxIdx = tstElemIdx;
				}
				// check silver medals if gold medals are tied
				else if (countries[tstElemIdx].goldCount == countries[curMaxIdx].goldCount) {
					if (countries[tstElemIdx].silverCount > countries[curMaxIdx].silverCount) {
						curMaxIdx = tstElemIdx;
					}
					// check bronze medals if total medals are tied
					else if (countries[tstElemIdx].silverCount == countries[curMaxIdx].silverCount) {
						if (countries[tstElemIdx].bronzeCount > countries[curMaxIdx].bronzeCount) {
							curMaxIdx = tstElemIdx;
						}
					}
				}
			}
		}
		// Swap needs to be done!
		if (curMaxIdx != iterIdx) {
			tempVal = countries[iterIdx];
			countries[iterIdx] = countries[curMaxIdx];
			countries[curMaxIdx] = tempVal;
		}
	}
	displayMedals(countries, arrSize);
}


/* Medal Counting Functions */

void countMedals(struct OlympicCountry countries[], struct OlympicResult results[], int countrySize, int resultSize)
{
	int i, j, k, skip;

	for (i = 0; i < resultSize; i++) {
		// check if the same result/medal/country has been counted already, if so, skip the line
		skip = 0;
		for (k = 0; k < i; k++) {
			if (strcmp(results[i].countryNoc, results[k].countryNoc) == 0 &&
				results[i].resultId == results[k].resultId &&
				strcmp(results[i].medal, results[k].medal) == 0) {
				skip = 1;
			}
		}

		if (skip == 0) { // if this is a new line, add medal to corresponding country's count
			for (j = 0; j < countrySize; j++) {
				if (strcmp(results[i].countryNoc, countries[j].countryNoc) == 0) {
					if (strcmp(results[i].medal, "Bronze") == 0) {
						countries[j].bronzeCount++;
					}
					else if (strcmp(results[i].medal, "Silver") == 0) {
						countries[j].silverCount++;
					}
					else if (strcmp(results[i].medal, "Gold") == 0) {
						countries[j].goldCount++;
					}
				}
			}
		}
	}
}

void totalMedals(struct OlympicCountry countries[], int capacity)
{
	int i;

	for (i = 0; i < capacity; i++) {
		countries[i].totalMedals = countries[i].goldCount + countries[i].silverCount + countries[i].bronzeCount;
	}
}


/* Display Functions */

void displayMedals(struct OlympicCountry countries[], int capacity)
{
	int i;

	printf("Country				Gold  Silver	Bronze	Total\n"
		"-----------------------------  ------ ------- ------- -------\n");

	for (i = 0; i < 10; i++) {
		printf("%-30.30s %d	%d	%d	%d\n", countries[i].country, countries[i].goldCount, countries[i].silverCount, countries[i].bronzeCount, countries[i].totalMedals);
	}
}

void displayMedalHistogram(struct OlympicCountry countries[], int year, char season[])
{
	int i, j;

	putchar('\n');
	printf("%40s %d %s Olympics: Top 10 Countries by Medal\n\n", "", year, season);

	// print each country by row
	for (i = 0; i < 10; i++) {
		printf("%40s |", countries[i].country);
		
		// Display one 'G' character for each gold medal
		for (j = 0; j < countries[i].goldCount; j++) {
			putchar('G');
		}

		// Display one 'S' character for each silver medal
		for (j = 0; j < countries[i].silverCount; j++) {
			putchar('S');
		}

		// Display one 'B' character for each bronze medal
		for (j = 0; j < countries[i].bronzeCount; j++) {
			putchar('B');
		}
		putchar('\n');
	}

	// print legend and count at bottom
	printf("%41s", "");
	for (i = 0; i < countries[0].totalMedals; i++) {
		putchar('-');
	}
	putchar('\n');
	printf("%41s", "");

	for (i = 0; i < countries[0].totalMedals; i++) {
		if (i % 10 == 0) {
			printf("%d", i/10);
		}
		else {
			putchar(' ');
		}
	}
	putchar('\n');
	printf("%40s %s", "", "Total Medals (10's) - G: Gold | S: Silver | B: Bronze\n\n");
}