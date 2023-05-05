#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cctype>

//Prize is hidden behind a "door".  
class Door
{
    public:
        Door(bool win)
        {
            winner = win;
        }
        bool get_winner()
        {
            return winner;
        }

    
    private:
        bool winner;
};

//simulates the monty hall problem x iterations
int montyhall_sim(int sim, int door_count, int final_unrevealed_count)
{
    int playerwincount = 0;
    std::vector<Door> unrevealedDoors;
    char option;
    //give option for player to switch or not switch
    std::cout << "After all losing doors except " << final_unrevealed_count << " are revealed, switch the initial chosen door with remaining door? (y/n)" << std::endl;
    std::cin >> option;
    printf("\n");
    if (tolower(option) != 'y' && tolower(option) != 'n')
    {
        std::cerr << "Error: Please reply using a valid response (y for Yes, n for No)\n" << std::endl;
        return montyhall_sim(sim, door_count, final_unrevealed_count);
    }

    //perform iteratively
    for (int j = 0; j < sim; j++)
    {
        //For a set of doors, pick one random winner and one random player's choice
        int winner_position = rand() % door_count;
        int player_position = rand() % door_count;

        //Assemble all of the doors 
        for (int i = 0; i < door_count; i++)
        {
            unrevealedDoors.push_back(Door(false));
        }

        //Assign door at winner_position the prize
        unrevealedDoors[winner_position] = Door(true);

        //remove player's door from unrevealed Door
        Door player_door = unrevealedDoors[player_position];
        unrevealedDoors.erase(unrevealedDoors.begin() + player_position);

        //"reveal" loser doors until final_unrevealed_count remains
        while (unrevealedDoors.size() > final_unrevealed_count)
        {
            //reveal in ascending order
            if (!unrevealedDoors[0].get_winner())
            {
                unrevealedDoors.erase(unrevealedDoors.begin());
            }
            else if (!unrevealedDoors[unrevealedDoors.size() - 1].get_winner())
            {
                unrevealedDoors.pop_back();
            }
        }

        //switch player door with remaining unrevealed door if option was selected.  currently constructed for only 1 remaining unrevealed door
        if (tolower(option) == 'y')
        {
            unrevealedDoors.push_back(player_door);
            player_door = unrevealedDoors[0];
            unrevealedDoors.erase(unrevealedDoors.begin());
        }

        //increase tally of player wins
        if (player_door.get_winner())
        {
            playerwincount++;
        }
        unrevealedDoors.clear();
    }
    return playerwincount;
}

int main()
{
    int door_count;
    const int simulations = 10000;
    //Amount of unrevealed doors (not chosen by player) prior to player switch
    const int final_unrevealed_count = 1;
    srand(time(nullptr));
    
    printf("Monty Hall Problem Simulator\n");
    printf("----------------------------\n");

    printf("Enter the number of doors:");

    //Display error message if door count provided is not enough to demonstrate the Monte Hall problem
    std::cin >> door_count;
    if (door_count < 3)
    {
        std::cerr << "Error: Monty Hall Problem only applies with 3 or more doors" << std::endl;
        return -1;
    }

    int playerwincount = montyhall_sim(simulations, door_count, final_unrevealed_count);
    std::cout << "Player won " << playerwincount << " times out of " << simulations << " attempts." << std::endl;

    return 0;
}