#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> cards;

class Card {
public:
	Card() {
		srand()
		int =
	}
protected:
	std::string name;
	int ball;
};

class Hand {
	std::vector<Card>cards;
};

int main()
{
	for (int i = 2; i <= 10; i++)
		for (int j = 0; j < 4; j++)
			cards.push_back(std::to_string(i));
	for (int i = 0; i < 4; i++)
	{
		cards.push_back("J");
		cards.push_back("Q");
		cards.push_back("K");
		cards.push_back("A");
	}
	int money = 1000;
	while (true) {
		if (money == 0)
			break;
		int stavka;
		std::cin >> stavka;
		if (stavka > money)
			continue;


	}
}