#include <iostream>
#include <Windows.h>
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

void final(char operation, double stavka, double money)
{
	std::cout << operation << stavka << std::endl;
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

	void Name(std::string new_name) { name = new_name; }
	std::string Name() { return name; }
	
	void Mark(int new_mark) { mark = new_mark; }
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
		std::cout << "Your cards: " << std::endl;
		for (auto item : cards)
			std::cout << item.Name() << " ";
		std::cout << std::endl << "score: " << score << std::endl << std::endl;
	}

	void enemy_print(bool game)
	{
		std::cout << "Dealer's cards: " << std::endl;
		if (game)
		{
			for (auto item : cards)
				std::cout << item.Name() << " ";
			std::cout << std::endl << "score: " << score << std::endl;
		}
		else
		{
			std::cout << cards[0].Name() << " #" << std::endl;
			std::cout << "score: " << cards[0].Mark() << std::endl;
		}
		std::cout << std::endl;
	}

	void addCard()
	{
		Card temp;
		score += temp.Mark();
		cards.push_back(temp);
	}

	int Score() { return score; }

	std::vector<Card>Cards() { return cards; }

protected:

	std::vector<Card>cards;
	int score = 0;

};

int main()
{
	pack_init(pack);

	double money = 1000;
	

	while (money>0) {

		std::cout << "Money: " << money << std::endl;

		double stavka;
		std::cout << "Make a deal:\n";
		std::cin >> stavka;

		if (stavka == 0)
		{
			std::cout << "Thanks for the game\n";
			break;
		}
		if (stavka > money)
			continue;

		money -= stavka;

		Hand dealer;
		dealer.enemy_print(0);

		std::cout << std::endl << std::endl;

		Hand player;
		player.print();
		
		std::string choice;
		while (true)
		{
			if (player.Cards()[0].Name() == "A" || player.Cards()[1].Name() == "A")
				break;
			std::cout << "Your choice: Stand, Hit, Double\n";
			std::cin >> choice;
			std::cin.ignore();
			std::cout << std::endl;

			if (choice == "Stand")
				break;

			else if (choice == "Hit")
			{
				player.addCard();
				player.print();
				if (player.Score() >= 21)
				{
					for (auto item : player.Cards())
					{
						if (item.Name() == "A")
						{
							item.Mark(1);
						}
					}
					if (player.Score() >= 21)
					{
						break;
					}
				}
					
			}
			else if (choice == "Double")
			{
				
				stavka *= 2;
				std::cout << "Stavka" << stavka << std::endl << std::endl;
				player.addCard();
				player.print();
				break;
			}
		}
		dealer.enemy_print(1);
		std::cout << std::endl;

		if (player.Score() < dealer.Score())
		{
			std::cout << "Dealer wins" << std::endl;
			final('-', stavka, money);
		}
		
		else if (player.Score() > 21)
		{
			std::cout << "Bust" << std::endl;
			final('-', stavka, money);
		}

		else if (player.Score() > dealer.Score())
		{
			std::cout << "Win" << std::endl;
			final('+', stavka, money);
			money += stavka*2;
		}
		
		else if (player.Cards()[0].Name() == "A" || player.Cards()[1].Name() == "A")
		{
			std::cout << "21!" << std::endl;
			money += 2.5 * stavka;
		}

		else if (player.Score() == 21)
		{
			if (dealer.Score() == 21)
			{
				std::cout << "Draw" << std::endl;
			}
			else
			{
				std::cout << "21!" << std::endl;
				money += 2.5 * stavka;
			}
		}
		std::cout << "Money: " << money << std::endl << std::endl;
		system("pause");
		system("cls");
	}
	if (money <= 0)
		std::cout << "Casino always wins\n";

	return 0;
}