#include <iostream>
#include <random>
#include <vector>
#include <string>

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution <> gen32(0, 51);
std::vector<std::string> pack;

void pack_init(std::vector<std::string>& cards) {
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
}

class Card 
{

public:

	Card()
	{
		int id;
		while (true)
		{
			id = gen32(rng);
			if (id < pack.size())
				break;
		}
		
		name = pack[id];
		pack.erase(pack.begin()+id);
		if (name == "J" || name == "Q" || name == "K")
			mark = 10;
		else if (name == "A")
			mark = 11;
		else
			mark = stoi(name);
	}

	std::string Name() { return name; }
	int Mark() { return mark; }

protected:

	std::string name;
	int mark;

};

class Hand
{

public:

	Hand()
	{
		for (int i = 0; i < 2; i++)
		{
			Card temp;
			score += temp.Mark();
			cards.push_back(temp);
		}
	}

	void print()
	{
		for (auto item : cards)
			std::cout << item.Name() << " ";
		std::cout << std::endl << "score: " << score << std::endl;
	}

	void addCard()
	{
		Card temp;
		score += temp.Mark();
		cards.push_back(temp);
	}

	int Score() { return score; }

protected:

	std::vector<Card>cards;
	int score = 0;

};

int main()
{
	pack_init(pack);

	int money = 1000;

	while (money>0) {
		int stavka;
		std::cout << "Make a deal:\n";
		std::cin >> stavka;
		if (stavka == 0)
		{
			std::cout << "Thanks for the game\n";
			break;
		}
		if (stavka > money)
			continue;
		Hand player;
		player.print();
		std::string choice;
		while (true)
		{
			std::cout << "Your choice: \nStand\nHit\nDouble\n";
			std::cin >> choice;
			std::cin.ignore();
			if (choice == "Stand")
				break;
			else if (choice == "Hit")
			{
				player.addCard();
				player.print();
				if (player.Score() >= 21)
					break;
			}
			else if (choice == "Double")
			{
				stavka *= 2;
				player.addCard();
				player.print();
				if (player.Score() >= 21)
					break;
			}
		}
	}
	if (money <= 0)
		std::cout << "Casino always wins\n";

	return 0;
}