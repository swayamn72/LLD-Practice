#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

class Ability{
    protected:
        std::string name;
        int cooldownSeconds;
        bool isReady;
        int currentTimer;
    public:
        Ability(std::string n, int cd) : name(n), cooldownSeconds(cd), isReady(true), currentTimer(0){}
        virtual ~Ability() = default;

        virtual void execute() = 0;

        std::string getName(){ return name; }

        void updateTimer(int secondsPassed){
            if(!isReady){
                currentTimer -= secondsPassed;
                if(currentTimer<=0){
                    currentTimer = 0;
                    isReady = true;
                    std::cout << "[System] " << name << " is ready" << "\n";
                }
            }
        }
};

class InstantAbility : public Ability{
    public: InstantAbility(std::string n, int cd) : Ability(n,cd){}
    void execute() override{
        if(isReady){
            std::cout << "[Cast] " << name << " used instantly" << "\n";
            isReady = false;
            currentTimer = cooldownSeconds;
        }else{
            std::cout << "Wait " << name << " is on cooldown for" << currentTimer << " more seconds" "\n";
        }
    }
};

class DeployableAbility : public Ability{
    private:
        bool isDeployed;
    public: DeployableAbility(std::string n, int cd) : Ability(n,cd), isDeployed(false){}
    void execute() override{
        if(!isDeployed && isReady){
            std::cout << "Deploy " << name << " placed on the map" << "\n";
            isDeployed = true;
        }else if(isDeployed){
            std::cout << "Reactivate " << name << " triggered" << "\n";
            isDeployed = false;
            isReady = false;
        }else{
            std::cout << "Wait " << name << " is on cooldown for " << currentTimer << " more seconds" << "\n";
        }
    }
};

class Player{
    private: 
        std::string username;
        std::unordered_map<std::string, std::unique_ptr<Ability>> loadout;

    public:
        Player(std::string name) : username(name){};
        // Player(std::string name){   // uses 2 steps hence initialization is preferred
        //     username = name;
        // };
        void equipAbility(const std::string& slot, std::unique_ptr<Ability> ability){
            loadout[slot] = std::move(ability);
            std::cout << username << " equipped an ability to slot " << slot << " .\n";
        }
        void pressKey(const std::string& slot){
            if(loadout.count(slot)){
                loadout[slot]->execute();
            }else{
                std::cout << "Slot " << slot << " is empty";
            }
        }
        void passTime(int seconds) {
            std::cout << "\n--- " << seconds << " seconds pass ---\n";
            for (auto& pair : loadout) {
                pair.second->updateTimer(seconds); 
            }
        }
};

int main(){

    Player player1("Swayam");
    
    // Equip a 15-second cooldown Flashbang
    player1.equipAbility("Q", std::make_unique<InstantAbility>("Flashbang", 15));
    
    // Equip a 30-second cooldown Proximity Mine
    player1.equipAbility("E", std::make_unique<DeployableAbility>("Proximity Mine", 30));

    std::cout << "\n--- Gameplay Start ---\n";
    player1.pressKey("Q"); // Casts flashbang
    player1.pressKey("E"); // Deploys mine
    
    std::cout << "\n--- Trying again immediately ---\n";
    player1.pressKey("Q"); 
    player1.pressKey("E"); 
    player1.passTime(10); 
    
    player1.pressKey("Q"); 
    player1.pressKey("E");

    player1.passTime(5); 

    player1.pressKey("Q");

    return 0;
}