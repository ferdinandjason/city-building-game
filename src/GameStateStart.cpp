#include <utility>

#include "GameStateStart.hpp"
#include "GameStateEditor.hpp"

GameStateStart::GameStateStart(Game* game)
{
    //ctor
    this->game = game;
    sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
    this->view.setSize(pos);
    pos*=0.5f;
    this->view.setCenter(pos);
    this->gameStates=State::START;

//    this->logos.setTexture(this->te)

    /*
     * This Gui will act as a main menu for the game. To add new elements
     * to every othe std::map we've used the [] syntax, but we can't do
     * that here and must use emplace instead. This is because [] works by
     * first creating an empty object of the type you are trying to add
     * (thereby calling an empty constructor), then copying the object to
     * the right of the = into std::map. Gui doesn't have an empty constructor
     * so we use emplace, which works differently.
     *
     * We then place the Gui in the center of the screen before setting the Gui's
     * origin to its center. setOrigin uses local coordinates ((0,0) is the top
     * left of the Gui) unlike setPosition that uses world coordinates. Finally
     * we show the Gui to make it visible and place all the entries in the correct location.
     *
     */

    this->guiSystem.emplace("judul",Gui(sf::Vector2f(410,46),0,false,this->game->styleSheets.at("text2"),
        {std::make_pair("City Building","judul")}));
    pos.y+=60;
    this->guiSystem.at("judul").setOrigin(sf::Vector2f(205,23));
    this->guiSystem.at("judul").setPosition(pos);
    this->guiSystem.at("judul").show();

    this->guiSystem.emplace("menu",Gui(sf::Vector2f(192,32),4,false,game->styleSheets.at("button"),{std::make_pair(" Load Game","load_game")}));
    pos.y+=140;
    this->guiSystem.at("menu").setPosition(pos);
    this->guiSystem.at("menu").setOrigin(96,16);
    this->guiSystem.at("menu").show();

    sf::Vector2i koor=sf::Vector2i(159,3);
    //printf("%d %d\n",koor.y,koor.x);
    this->game->logos.setPosition(this->game->window.mapPixelToCoords(koor));

}

GameStateStart::~GameStateStart()
{
    //dtor
}

void GameStateStart::draw(const float dt)
{
    this->game->window.setView(this->view);

    this->game->window.clear(sf::Color::Black);
    this->game->window.draw(this->game->background);
    this->game->window.draw(this->game->logos);

    for(auto gui:this->guiSystem) this->game->window.draw(gui.second);

    return;
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleinput()
{
    sf::Event event;
    sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window),this->view);

    while(this->game->window.pollEvent(event))
    {
        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                game->window.close();
                break;
            }
            /* Resize the window */
            case sf::Event::Resized:
            {
                this->view.setSize(event.size.width,event.size.height);
                /* Setiing mouse input for guiStart */
                this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0,0)));
                //printf("%d %d\n",this->game->logos.getTexture()->getSize().y,this->game->logos.getTexture()->getSize().x);
                printf("%d\n",event.size.width);
                sf::Vector2i koor=sf::Vector2i(event.size.width/2-this->game->logos.getTexture()->getSize().y/2,event.size.height/2-this->game->logos.getTexture()->getSize().x/2);
                koor.y-=90;
                koor.x-=50;
                printf("%d %d\n",koor.y,koor.x);
                //printf("%d %d\n",koor.y,koor.x);
                this->game->logos.setPosition(this->game->window.mapPixelToCoords(koor));
                sf::Vector2f pos = sf::Vector2f(event.size.width,event.size.height);
                pos*=0.5f;
                pos=this->game->window.mapPixelToCoords(sf::Vector2i(pos),this->view);
                this->guiSystem.at("menu").setPosition(pos);
                this->game->background.setScale(
                    float(event.size.width)/float(this->game->background.getTexture()->getSize().x),
                    float(event.size.height)/float(this->game->background.getTexture()->getSize().y));
                break;
            }
            /* Highlight menu items */
            case sf::Event::MouseMoved:
            {
                this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));
                break;
            }
            /* Click on menu items. */
            case sf::Event::MouseButtonPressed:
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    std::string msg = this->guiSystem.at("menu").activate(mousePos);
                    if(msg == "load_game")
                    {
                        this->loadGame();
                    }
                }
                break;
            }
            /* Key Activate */
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape) this->game->window.close();
                else if(event.key.code == sf::Keyboard::Space) this->loadGame();
                break;
            }
            default: break;
        }
    }

    return;
}

void GameStateStart::loadGame()
{
    this->game->pushState(new GameStateEditor(this->game));

    return;
}


