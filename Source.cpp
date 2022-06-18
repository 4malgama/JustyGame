#include <iostream>
#include <Windows.h>
#define ENABLE_DEBUG_MODE false
#define DARK_BLUE 1
#define GREEN 2
#define BLUE 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GREY 8
#define CONTRAST_GREEN 10
#define BLUE_GREEN 11
#define LIGHT_RED 12
#define CONTRAST_PURPLE 13
#define LIGHT_YELLOW 14
#define Color(x) SetConsoleTextAttribute(Console, x)

////////////////////////// C O N F I G //////////////////////////

namespace Config
{
	int Base[4] = { 50, 64, 78, 95 };		// 4 modifications, values health (Don't create new values)
	int BaseCost[4] = { 0, 70, 210, 700 };		// 4 cost for modifications (Don't create new values)
	int Turret[4] = { 11, 15, 24, 35 };		// 4 modifications, values damage (Don't create new values)
	int TurretCost[4] = { 0, 30, 90, 300 };		// 4 cost for modifications (Don't create new values)
	int money = 5;		// Start balance (default 5)
	int enemy_hit_chance = 30;		// Chance get damage from enemy (default 25)
	int player_hit_chance = 70;		// Chance send damage to enemy (default 100)

	// Below you can change color-configurations

	WORD PlayerColor = CONTRAST_GREEN;			// Player info color
	WORD EnemyColor = LIGHT_RED;				// Enemy info color
	WORD WinLoseColor = YELLOW;					// Win or Lose title color
	WORD RoundTitleColor = RED;					// Start or End round color
	WORD PlainTextColor = WHITE;				// Default color text
	WORD CreatorsHeaderColor = LIGHT_RED;	// Default color header in authors menu
}

////////////////////////// D O N ' T - T O U C H - H E R E //////////////////////////

static HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
static int Profit;

////////////////////////// V A R I A B L E S //////////////////////////

struct __Char
{
	char& get()
	{
		return s;
	}
	char& toLower()
	{
		if ((int)s > 96 && (int)s < 123)
			return s;
		else if ((int)s > 64 && (int)s < 91)
			s = (char)((int)s + 32);
		else
			throw std::exception("Not letter.");
		return s;
	}
	char& toUpper()
	{
		if ((int)s > 97 && (int)s < 123)
			s = (char)((int)s - 32);
		else if ((int)s > 64 && (int)s < 91)
			return s;
		else
			throw std::exception("Not letter.");
		return s;
	}
private:
	char s;
};

////////////////////////// G A M E //////////////////////////

enum class GameState
{
	MENU,
	PROCESSING,
	GARAGE,
	AUTHORS
};

static GameState gameState;

class Player
{
	static int Health;
	static int Damage;
	int HitChance;

public:
	Player(int Hit_Chance = Config::player_hit_chance)
	{
		HitChance = Hit_Chance;
	}

	void set(int hp, int dmg)
	{
		Health = hp;
		Damage = dmg;
	}

	inline int get_Health()
	{
		return Health;
	}

	inline int get_Damage()
	{
		return Damage;
	}

	inline int get_HitChance()
	{
		return HitChance;
	}
};

class Enemy
{
	int Health;
	int Damage;
	int HitChance;

public:
	Enemy(int Hit_Chance = Config::enemy_hit_chance) : Health{ 0 }, Damage{ 0 }
	{
		HitChance = Hit_Chance;
	}

	void Generate()
	{
		Health = Config::Base[rand() % 4];
		Damage = Config::Turret[rand() % 4];
	}

	void takeDamage(int dmg)
	{
		Health -= dmg;
	}

	inline int get_Health()
	{
		return Health;
	}

	inline int get_Damage()
	{
		return Damage;
	}

	inline int get_HitChance()
	{
		return HitChance;
	}
};

static void Garage(char& next)
{
	gameState = GameState::GARAGE;
	system("cls");
	Player ply;
	int need_for_T{-1};
	int need_for_B{-1};
	int current_T{};
	int current_B{};
	for (;;)
	{
		system("cls");
		Color(GREEN);
		std::cout << "[ " << Config::money << " $ ]" << std::endl;
		Color(Config::PlainTextColor);
		for (int i{ 0 }; i < 4; i++)
		{
			if (ply.get_Damage() == Config::Turret[i])
			{
				std::cout << "Turret is [ M" << i << " ]\t";
				current_T = i;
				break;
			}
		}
		if (current_T != 3)
			need_for_T = Config::TurretCost[current_T + 1];
		else
			need_for_T = -1;
		std::cout << "[ Upgrade: " << need_for_T << "$ ]" << std::endl;
		for (int i{ 0 }; i < 4; i++)
		{
			if (ply.get_Health() == Config::Base[i])
			{
				std::cout << "Hull is [ M" << i << " ]\t\t";
				current_B = i;
				break;
			}
		}
		if (current_B != 3)
			need_for_B = Config::BaseCost[current_B + 1];
		else
			need_for_B = -1;
		std::cout << "[ Upgrade: " << need_for_B << "$ ]" << std::endl;
		std::cout << std::endl;
		std::cout << "Select:\n1 - Turret\n2 - Hull\n0 - Exit" << std::endl;
		int _r;
		std::cin >> _r;
		if (_r == 1)
		{
			if (current_T == 3)
			{
				std::cout << "You have a MAX Turret!" << std::endl;
				system("pause");
				continue;
			}
			if (Config::money < Config::TurretCost[current_T + 1])
			{
				std::cout << "Not enough money!" << std::endl;
				system("pause");
				continue;
			}
			ply.set(ply.get_Health(), Config::Turret[current_T + 1]);
			Config::money -= Config::TurretCost[current_T + 1];
			continue;
		}
		else if (_r == 2)
		{
			if (current_B == 3)
			{
				std::cout << "You have a MAX Hull!" << std::endl;
				system("pause");
				continue;
			}
			if (Config::money < Config::BaseCost[current_B + 1])
			{
				std::cout << "Not enough money!" << std::endl;
				system("pause");
				continue;
			}
			ply.set(Config::Base[current_B + 1], ply.get_Damage());
			Config::money -= Config::BaseCost[current_B + 1];
			continue;
		}
		else
		{
			std::cout << "Exiting..." << std::endl;
			break;
		}
	}
	next = 'm';
}

static void Processing(char& next)
{
	gameState = GameState::PROCESSING;
	system("cls");
	Player ply;
	Enemy eny;
	int tHP = ply.get_Health();
	int tDMG = ply.get_Damage();
	eny.Generate();
	Color(Config::EnemyColor);
	std::cout << "Enemy:\n" << std::endl;
	std::cout << eny.get_Health() << " - Health.\n" << eny.get_Damage() << " - Damage" << std::endl << std::endl;
	Color(Config::PlayerColor);
	std::cout << "You:\n" << std::endl;
	std::cout << tHP << " - Health.\n" << tDMG << " - Damage" << std::endl << std::endl;
	Color(Config::PlainTextColor);
	std::cout << "Press [Enter]" << std::endl;
	system("pause");
	system("cls");
	int round{ 0 };
	while (tHP > 0 && eny.get_Health() > 0)
	{
		round++;
		Color(Config::RoundTitleColor);
		std::cout << "\n=== S T A R T = " << round << " = R O U N D ===\n" << std::endl;
		Color(Config::PlainTextColor);
		std::cout << "YOU -> ENEMY\t";
		if (rand() % 100 <= ply.get_HitChance())
		{
			eny.takeDamage(tDMG);
			Color(GREEN);
			std::cout << "[ SUCCES ]\n" << std::endl;
			Color(Config::PlainTextColor);
		}
		else
		{
			Color(LIGHT_RED);
			std::cout << "[ YOU MISS ]\n" << std::endl;
			Color(Config::PlainTextColor);
		}
		system("pause");
		std::cout << "YOU <- ENEMY\t";
		if (rand() % 100 <= eny.get_HitChance())
		{
			tHP -= eny.get_Damage();
			Color(GREEN);
			std::cout << "[ SUCCES ]\n" << std::endl;
			Color(Config::PlainTextColor);
		}
		else
		{
			Color(LIGHT_RED);
			std::cout << "[ ENEMY MISS ]\n" << std::endl;
			Color(Config::PlainTextColor);
		}
		Color(Config::RoundTitleColor);
		std::cout << "\n=== E N D = " << round << " = R O U N D ===\n" << std::endl;
		Color(Config::EnemyColor);
		std::cout << "Enemy:\n" << std::endl;
		std::cout << eny.get_Health() << " - Health.\n" << eny.get_Damage() << " - Damage" << std::endl << std::endl;
		Color(Config::PlayerColor);
		std::cout << "You:\n" << std::endl;
		std::cout << tHP << " - Health.\n" << tDMG << " - Damage" << std::endl << std::endl;
		system("pause");
		system("cls");
	}
	Color(RED);
	std::cout << "[!] E N D [!]" << std::endl << std::endl;
	if (tHP > 0)
	{
		Color(YELLOW);
		std::cout << "Y O U - W I N" << std::endl;
		Profit = rand() % 20;
		Config::money += Profit;
	}
	else if (eny.get_Health() > 0)
	{
		Color(YELLOW);
		std::cout << "Y O U - L O S E" << std::endl;
		Profit = -(rand() % 20);
		Config::money += Profit;
	}
	else
	{
		Color(YELLOW);
		std::cout << "D R A W" << std::endl;
		Profit = 0;
	}
	
	system("pause");
	next = 'm';
}

static void CreatorsMenu(char& next)
{
	gameState = GameState::AUTHORS;
	system("cls");
	Color(Config::CreatorsHeaderColor);
	std::cout << "\n[ C + + | G A M E ]\n" << std::endl;
	Color(Config::PlainTextColor);
	std::cout << "Developer: ";
	Color(RED);
	std::cout << "NeWulla (Mr. NaN)" << std::endl;
	Color(Config::PlainTextColor);
	system("pause");
	next = 'm';
}

static void ExecuteMenu(char& next)
{
	gameState = GameState::MENU;
	Player ply;
	system("cls");
	Color(RED);
	std::cout << "\n[ M E N U ]\n" << std::endl;
	Color(Config::PlainTextColor);
	std::cout << "You\'re have:\n" << Config::money << " - $.\t\t[ Profit: " << Profit << " ]\n" << ply.get_Health()
		<< "- Health.\n" << ply.get_Damage() << " - Damage." << std::endl << std::endl;
	Color(RED);
	std::cout << "[ S E L E C T - O P T I O N ]" << std::endl;
	std::cout << std::endl;
	Color(BLUE);
	std::cout << "g - Start game\ne - Exit\na - Creators\nu - Garage" << std::endl << std::endl;
	Color(Config::PlainTextColor);
	try
	{
		__Char option;
		std::cout << "> ";
		std::cin >> option.get();

		if (option.toLower() == 'g')
			next = 'g';
		else if (option.toLower() == 'e')
			next = 'e';
		else if (option.toLower() == 'a')
			next = 'a';
		else if (option.toLower() == 'u')
			next = 'u';
		else
			return;
	}
	catch (std::exception& e)
	{
		std::cout << "[-] Error: " << e.what() << std::endl;
	}
}

////////////////////////// T E S T S //////////////////////////

#if ENABLE_DEBUG_MODE
constexpr void assert_this(bool state, const char* msg)
{
	if (!state) throw msg;
}

void Player_Health_Is_Config()
{
	Player ply;
	assert_this(ply.get_Health() == Config::Base[0], "Player health is NOT Base[0].");
}

void Player_Damage_Is_Config()
{
	Player ply;
	assert_this(ply.get_Damage() == Config::Turret[0], "Player damage is NOT Turret[0].");
}

void Player_HitChance_Is_Config()
{
	Player ply;
	assert_this(ply.get_HitChance() == Config::player_hit_chance, "Player hit chance is NOT configurated.");
}

void Enemy_Health_Is_Config()
{
	Enemy eny;
	assert_this(eny.get_Health() == 0, "Enemy health is NOT zero.");
}

void Enemy_Damage_Is_Config()
{
	Enemy eny;
	assert_this(eny.get_Damage() == 0, "Enemy damage is NOT zero.");
}

void Enemy_HitChance_Is_Config()
{
	Enemy eny;
	assert_this(eny.get_HitChance() == Config::enemy_hit_chance, "Enemy hit chance is NOT configurated.");
}

void Enemy_generate_is_unique()
{
	Enemy eny;
	struct Args { int a, b; }; Args args;
	args.a = eny.get_Health();
	args.b = eny.get_Damage();
	eny.Generate();
	assert_this(eny.get_Damage() != args.b && eny.get_Health() != args.a, "Enemy health or damage is NOT unique.");
}
#endif

////////////////////////// R U N - T E S T S //////////////////////////

#if ENABLE_DEBUG_MODE
void run_test(const char* nameTest, void (*unit_test)())
{
	try
	{
		unit_test();
		Color(BLUE_GREEN);
		std::cout << "[+] Test " << nameTest << " succesful." << std::endl;
	}
	catch (const char* e)
	{
		Color(RED);
		std::cout << "[-] Test " << nameTest << " failure.\n\tAbout this: " << e << std::endl;
	}
}

void test_module()
{
	run_test("Health player is Base[0]", Player_Health_Is_Config);
	run_test("Damage player is Turret[0]", Player_Damage_Is_Config);
	run_test("Hit chance player is config", Player_HitChance_Is_Config);
	run_test("Health enemy is zero", Enemy_Health_Is_Config);
	run_test("Damage enemy is zero", Enemy_Damage_Is_Config);
	run_test("Hit chance enemy is config", Enemy_HitChance_Is_Config);
	run_test("Enemy unique values", Enemy_generate_is_unique);
}
#endif

////////////////////////// M A I N //////////////////////////

int Player::Health = Config::Base[0];
int Player::Damage = Config::Turret[0];

int main()
{
#if ENABLE_DEBUG_MODE
	test_module();
	Color(WHITE);
#else
	srand(time(0));		// seed generator
	char nextState = 'm';		// start state (default 'm' - menu)
	
	for (;;)
	{
		switch (nextState)
		{
		case 'm':
			ExecuteMenu(nextState);
			break;
		case 'g':
			Processing(nextState);
			break;
		case 'a':
			CreatorsMenu(nextState);
			break;
		case 'e':
			exit(0);
			break;
		case 'u':
			Garage(nextState);
			break;
		default:
			break;
		}
	}
#endif
	return 0;
}