#include <iostream>
#include <Windows.h>
#include <random>
#include <vector>
#include <regex>
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

//Player cards
class Hand
{

public:

	//distribution of cards to the player
	Hand()
	{
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
				if (type == 1 && j==1)
				{ }
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
		cards.pop_back();
		score -= 10;
		this->addCard();
	}

	//�hanges the value of card A to 1 if it is in hand
	void BustCancel()
	{
		if (bust_act)
			for (auto item : cards)
				if (item.Name() == "A")
				{
					bust_act = false;
					score -= 10;
					break;
				}
	}

	//checking for the ability to split
	bool split_check()
	{
		return cards.size() == 2 && cards[0].Mark() == 10 && cards[1].Mark() == 10;
	}

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

	//get cards in hand
	std::vector<Card>Cards() { return cards; }

protected:

	std::vector<Card>cards;
	int score = 0;
	bool bust_act = true;

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
				break;
			}
			if (stavka > money || stavka < 0) continue;

			money -= stavka;

			Hand dealer;
			Hand player;

			bool split = false;

			std::string choice;
			while (true)
			{
				system("cls");
				all_print(dealer, player, 1);
				if (player.auto_win() || player.auto_win())
					break;
				std::cout << "Your choice: Stand, Hit, Double";
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
				else if (choice == "Double")
				{
					stavka *= 2;
					std::cout << "Stavka: " << stavka << std::endl << std::endl;
					Sleep(2000);
					player.addCard();
					break;
				}
				else if (split && choice == "Split")
					player.Split();
				else
					std::cout << "Wrong choice. Try again" << std::endl;

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
	return 0;
}
