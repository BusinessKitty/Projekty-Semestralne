#pragma warning(push, 0)
#include <SFML/Graphics.hpp>
#pragma warning(pop)
#include <memory>
#include <iostream>
#include "..\Headers\Text.h"
#include "..\Headers\Drawable.h"
#include "..\Headers\Slot.h"
#include "..\Headers\Tile.h"
#include "..\Headers\Blank.h"
#include "..\Headers\Container.h"
#include "..\Headers\Bag.h"
#include "..\Headers\Board.h"
#include "..\Headers\Player.h"
#include "..\Headers\Singleton.h"
#include "..\Headers\Arbiter.h"
#include "..\Headers\Game.h"
#include "..\Headers\WindowManager.h"

int main() {


    auto& arbiter = Arbiter::instance();
    arbiter.loadSets();
    
    auto& windowManager = WindowManager::instance();
    windowManager.loadingScreen();

    bool nextGame = 1;
    while (nextGame) {
        std::pair<std::string, int> condition = windowManager.chooseConditionScreen();
        std::unique_ptr<Game> game = std::make_unique<Game>(condition.first, condition.second);
        nextGame = game->play();
    }

    return 1;
}