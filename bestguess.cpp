#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>

int middle(int a, int b)
{
	return (a + b) / 2;
}

int main(int argc, char *argv[])
{
	int min, max;

	if (argc >= 3) {
		min = std::atoi(argv[1]);
		max = std::atoi(argv[2]);
	} else {
		std::cout << "Lower bound: ";
		std::cin >> min;
		std::cout << "Upper bound: ";
		std::cin >> max;
	}

	if (min == max) {
		std::cout << "I don't know; " << min << " is as good a guess as any." << std::endl;
		return 0;
	} else if (min > max) {
		std::swap(min, max);
	}

	std::cout << "Enter other guesses ('stop' when done):" << std::endl;

	std::vector<int> otherGuesses;
	bool stop = false;

	while (!stop) {
		std::string input;
		std::cin >> input;
		if (input == "stop") {
			stop = true;
		} else {
			int guess = std::atoi(input.c_str());
			if (min <= guess && guess <= max)
				otherGuesses.push_back(guess);
			else
				std::cout << guess << " is out of range; ignoring" << std::endl;
		}
	}

	otherGuesses.push_back(min);
	otherGuesses.push_back(max);
	std::sort(otherGuesses.begin(), otherGuesses.end());

	std::size_t count = otherGuesses.size();
	std::size_t guessBeforeBest = 0;
	unsigned int bestDifference = 0;
	for (std::size_t i = 0; i < count - 1; i++) {
		unsigned int diff = otherGuesses[i+1] - otherGuesses[i];
		if (diff > bestDifference) {
			guessBeforeBest = i;
			bestDifference = diff;
		}
	}

	int bestGuess = middle(otherGuesses[guessBeforeBest], otherGuesses[guessBeforeBest + 1]);
	
	std::cout << "The best number to guess is " << bestGuess << "." << std::endl;

	return 0;
}
