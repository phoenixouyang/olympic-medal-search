#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "project.h"

int main(void) 
{
	int yearInput, validOlympics = 0, resultCount, countryCount;
	char seasonInput[MAX_STRING + 1];

	// initialize empty struct arrays
	struct OlympicGames allGames[MAX_RECORDS] = { { 0 } };
	struct OlympicCountry allCountries[MAX_RECORDS] = { { 0 } };
	struct OlympicResult allResults[MAX_EVENT] = { { 0 } };

	// populate required structs from files
	countryCount = readOlympicCountries(COUNTRY_FILE, allCountries, MAX_RECORDS);
	readOlympicGames(GAMES_FILE, allGames, MAX_RECORDS);


	printf("*** Olympic Medal Search Program ***\n");
	printf("This program searches an Olympic game (year and season), and displays the top 10 performing countries by medal. Ties are broken with total golds, then silvers, then bronze medals.\n\n");

	// get user input for olympic year and season, only continue if valid year/season combination
	do {
		getYearInput(&yearInput, allGames, MAX_RECORDS);
		getSeasonInput(seasonInput, allGames, MAX_RECORDS);

		// validate if user entered season/year is a valid Olympic data combination
		validOlympics = validYearSeason(yearInput, seasonInput, allGames, MAX_RECORDS);

		if (!validOlympics) {
			printf("Sorry, %d %s Olympics is not a valid year/season combination. Please enter a valid Olympic game.\n\n", yearInput, seasonInput);
		}

	} while (!validOlympics);

	putchar('\n');

	// populate OlympicResult struct array with events from user inputted year/season
	resultCount = readOlympicResults(EVENT_RESULTS_FILE, allResults, MAX_EVENT, yearInput, seasonInput);

	// sort event results by country
	sortResultsbyCountry(allResults, resultCount);

	// count gold, silver and bronze medals of each country
	countMedals(allCountries, allResults, countryCount, resultCount);

	// sum total medals for all countries
	totalMedals(allCountries, countryCount);

	// sort each country by total medals, breaking ties with gold -> silver -> bronze
	sortCountriesbyMedals(allCountries, countryCount);

	// display histogram of top 10 countries by total medals
	displayMedalHistogram(allCountries, yearInput, seasonInput);

	return 0;
}