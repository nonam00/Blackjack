#include <iostream>
#include <Windows.h>
#include <random>
#include <vector>
#include <string>

//random
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution <> gen32(0, 51);

//vector of card pack
std::vector<std::string> pack;

//filling of vector
void pack_init(std::vector<std::string>& cards)
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
}

//Player cards
class Hand
{

public:

	//entity of the card
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
			pack.erase(pack.begin() + id);

			if (name == "J" || name == "Q" || name == "K")
				mark = 10;
			else if (name == "A")
				mark = 11;
			else
				mark = stoi(name);
		}

		std::string Name() { return name; }

		void Mark(int new_mark) { mark = new_mark; }
		int Mark() { return mark; }


	private:

		std::string name;
		int mark;

	};

	//distribution of cards to the player
	Hand()
	{
		score = 0;
		for (int i = 0; i < 2; i++)
			this->addCard();
	}

	//prints player cards
	void print(int type)
	{	
		if(type==0)
			std::cout << "Your cards: " << std::endl << std::endl;
		else
			std::cout << "Dealer's cards: " << std::endl;
		for (int i = 0; i < cards.size(); i++)
			std::cout << "##############\t";
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < cards.size(); j++)
			{
				if (type == 1 && j == 1) {}
				else
				{
					if (i == 1)
					{
						std::cout << "#  " << cards[j].Name();
						if(cards[j].Name()=="10")
							std::cout << "        #\t";
						else
							std::cout << "         #\t";
						continue;
					}
					else if (i == 5)
					{
						std::cout << "#         " << cards[j].Name();
						if (cards[j].Name() == "10")
							std::cout << " #\t";
						else
							std::cout << "  #\t";
						continue;
					}
				}
				if(i == 7)
					std::cout << "##############\t";
				else
					std::cout << "#            #\t";
			}
			std::cout << std::endl;
		}
	}

	//adds a card to the person
	void addCard()
	{
		Card temp;
		score += temp.Mark();
		cards.push_back(temp);
	}

	//do a split
	void Split()
	{
		score -= cards[cards.size()-1].Mark();
		cards.pop_back();
		this->addCard();
	}

	//ñhanges the value of card A to 1 if it is in hand
	void BustCancel()
	{
		for (auto item : cards)
			if (item.Name() == "A" && item.Mark()==11)
			{
				item.Mark(1);
				score -= 10;
				break;
			}
	}

	//checking for the ability to split
	bool split_check() { return cards.size() == 2 && cards[0].Mark() == cards[1].Mark(); }

	//checks about score
	bool auto_win()
	{
		if ((cards[0].Name() == "A" && cards[1].Name() == "A") || score==21)
			return true;
		else
			return false;
	}

	//bust check
	bool Bust() { return score > 21; }

	//set score
	void Score(int new_score) { score = new_score; }

	//get score
	int Score() { return score; }

private:

	std::vector<Card>cards;
	int score;

};

//print game results
void final(char operation, double stavka, double money)
{
	std::cout << operation << stavka << std::endl;
}

//print game table
void all_print(Hand dealer, Hand player, int game)
{
	dealer.print(game);
	std::cout << std::endl << std::endl << std::endl;

	player.print(0);
	std::cout << std::endl << std::endl;
}

int main()
{
	double money;
	std::cout << "Your budget: ";
	std::cin >> money; std::cout << std::endl << std::endl;
	if (money > 0)
	{
		while (money > 0)
		{
			pack_init(pack);
			std::cout << "Money: " << money << std::endl;

			double stavka;
			std::cout << "Make a deal:\n";
			std::cin >> stavka;

			if (stavka == 0) //game ends if price will be equal to 0
			{
				std::cout << "Thanks for the game" << std::endl;
				std::cout << "Money: " << money << std::endl;
				break;
			}
			if (stavka > money || stavka < 0)
			{
				system("cls");
				continue;
			}

			money -= stavka;

			Hand dealer;
			Hand player;

			bool split = false;

			std::string choice;
			while (true)
			{
				system("cls");
				all_print(dealer, player, 1);
				if (player.auto_win() || dealer.auto_win())
					break;
				std::cout << "Your choice: Stand, Hit";
				if (stavka * 2 <= money+stavka)
					std::cout << ", Double ";
				if (player.split_check())
				{
					std::cout << ", Split";
					split = true;
				}
				std::cout << std::endl;
				std::cin >> choice;
				std::cin.ignore();
				std::cout << std::endl;

				if (choice == "Stand")
					break;

				else if (choice == "Hit")
				{
					player.addCard();
					if (player.auto_win())
						break;
					else if (player.Bust())
					{
						player.BustCancel();
						if (player.Score() > 21)
							break;
					}
				}
				else if (choice == "Double" && stavka*2<=money+stavka)
				{
					stavka *= 2;
					std::cout << "Stavka: " << stavka << std::endl << std::endl;
					Sleep(2000);
					player.addCard();
					if (player.Bust())
					{
						player.BustCancel();
						if (player.Score() > 21)
							break;
					}
					break;
				}
				else if (split && choice == "Split")
					player.Split();
			}

			system("cls");

			all_print(dealer, player, 2);

			if (player.auto_win())
			{
				if (dealer.auto_win())
				{
					std::cout << "Draw" << std::endl;
					money += stavka;
				}
				else
				{
					std::cout << "21!" << std::endl;
					money += 2.5 * stavka;
					std::cout << "+" << stavka * 1.5 << std::endl;
				}
			}

			else if (dealer.auto_win())
			{
				std::cout << "Dealer wins" << std::endl;
				final('-', stavka, money);
			}

			else if (player.Score() < dealer.Score())
			{
				std::cout << "Dealer wins" << std::endl;
				final('-', stavka, money);
			}

			else if (dealer.Score() == player.Score())
			{
				std::cout << "Draw" << std::endl;
				money += stavka;
			}

			else if (player.Bust())
			{
				std::cout << "Bust" << std::endl;
				final('-', stavka, money);
			}

			else if (player.Score() > dealer.Score())
			{
				while ((dealer.Score() <= player.Score()) && !dealer.Bust())
				{
					Sleep(2000);
					system("cls");

					std::cout << "Dealer hits" << std::endl << std::endl;

					dealer.addCard();

					if (dealer.Bust())
						dealer.BustCancel();

					all_print(dealer, player, 2);
				}

				if (dealer.Score() == player.Score())
				{
					std::cout << "Draw" << std::endl;
					money += stavka;
				}
				else if (dealer.Bust() || player.Score()>dealer.Score())
				{
					std::cout << "Win" << std::endl;
					final('+', stavka, money);
					money += stavka * 2;
				}
				else
				{
					std::cout << "Dealer wins" << std::endl;
					final('-', stavka, money);
				}
			}
			system("pause");
			system("cls");
			pack.clear();
		}
		if (money <= 0) std::cout << std::endl << "Casino always wins" << std::endl;
	}
	else
	{
		std::cout << "You don't have any money" << std::endl;
	}
	return 0;
}
