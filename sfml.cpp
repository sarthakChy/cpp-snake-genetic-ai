#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <time.h>
#include <random>
#include <stdlib.h>

using namespace std;
using namespace sf;

enum Direction{UP,DOWN,LEFT,RIGHT,Null};

enum State{Play,Dead,Pause};

enum Screen{x=600,y=400};

int gameState;
bool isPause;

int randNum(int x)
    {
        random_device rand;
        mt19937 gen(rand());
        uniform_int_distribution<>dis(0,3);
        return dis(gen);
    }




class Apple
{
private:
    Vector2f position;
    RectangleShape apple;
public:
    Apple(Vector2f startPosition)
    {
        apple.setSize(Vector2f(20,20));

        apple.setFillColor(Color::Blue);

        apple.setPosition(startPosition);
        position=startPosition;

    }

    Vector2f getPosition()
    {
        return position;

    }

    void setPosition(Vector2f newPosition)
    {
        position = newPosition;
    }

    RectangleShape getShape()
    {
        return apple;
    }

    void update()
    {
        apple.setPosition(position);
    }
};



class SnakeSection
{
private:
    Vector2f position;
    RectangleShape section;
public:
    SnakeSection(Vector2f startPosition,char type='b')
    {
        section.setSize(Vector2f(20,20));
        if(type=='h')
        {
            section.setFillColor(Color::Red);
        }
        else{
            section.setFillColor(Color::Green);
        }
        section.setPosition(startPosition);
        position=startPosition;

    }

    Vector2f getPosition()
    {
        return position;

    }

    void setPosition(Vector2f newPosition)
    {
        position = newPosition;
    }

    RectangleShape getShape()
    {
        return section;
    }

    void update()
    {
        section.setPosition(position);
    }
};

class Snake
{
public:

    int snakeDirection;
    int total_moves;
    vector<int> directionQueue;
    vector<int> directionCopy;
    vector<SnakeSection*> snake;
    int fitness;

    Snake(int t_moves)
    {
        snake.clear();
        for(int i=0;i<t_moves;i++)
        {
            directionQueue.push_back(randNum(4));
        }

        directionCopy = directionQueue;

        snakeDirection = randNum(4);


        fitness = 0;

        SnakeSection *h1 = new SnakeSection((Vector2f(100,100)),'h');
        SnakeSection *h2 = new SnakeSection((Vector2f(80,100)));
        SnakeSection *h3 = new SnakeSection((Vector2f(60,100)));
        snake.push_back(h1);
        snake.push_back(h2);
        snake.push_back(h3);
    }

    Snake(int new_total_moves,vector<int>parentGenes)
    {
        snake.clear();

        total_moves = new_total_moves;

        directionQueue = parentGenes;
        directionCopy = directionQueue;

        snakeDirection = randNum(4);

        fitness = 0;

        SnakeSection *h1 = new SnakeSection((Vector2f(100,100)),'h');
        SnakeSection *h2 = new SnakeSection((Vector2f(80,100)));
        SnakeSection *h3 = new SnakeSection((Vector2f(60,100)));
        snake.push_back(h1);
        snake.push_back(h2);
        snake.push_back(h3);
    }

    void printToConsole()
    {
        cout<<"fitness: "<<fitness<<endl;
        for(int direction:directionQueue)
        {
            cout<<direction<<" ";
        }

    }

    int randNum(int x)
    {
        random_device rand;
        mt19937 gen(rand());
        uniform_int_distribution<>dis(0,3);
        return dis(gen);
    }

    vector<int> getDirectionQueue()
    {
        return directionCopy;
    }

    void update(Apple *apple)
    {
        Vector2f thisSectionPosition = snake[0]->getPosition();
        Vector2f lastSectionPosition = thisSectionPosition;
        Vector2f target = apple->getPosition();

        int thisDiffX = abs(thisSectionPosition.x - target.x);
        int thisDiffY = abs(thisSectionPosition.y - target.y);

        int newDiffX;
        int newDiffY;

            //Setting directions
            if(directionCopy.size()!=0)
            {
                switch(snakeDirection)
                {
                    case Direction::RIGHT:
                    {
                        if(directionCopy.front()!=Direction::LEFT){
                            snakeDirection = directionCopy.front();
                        }
                        break;
                    }
                    case Direction::LEFT:
                    {
                         if(directionCopy.front()!=Direction::RIGHT){
                            snakeDirection = directionCopy.front();
                        }
                        break;
                    }
                    case Direction::UP:
                    {    if(directionCopy.front()!=Direction::DOWN){
                            snakeDirection = directionCopy.front();
                        }
                        break;
                    }
                    case Direction::DOWN:
                    {    if(directionCopy.front()!=Direction::UP){
                            snakeDirection = directionCopy.front();
                        }
                        break;
                    }
                }
                //directionCopy.pop_back();
                directionCopy.erase(directionCopy.begin());
            }


            //head

            switch(snakeDirection)
            {
                case Direction::RIGHT :
                {   if(thisSectionPosition.x+20>=Screen::x)
                    {
                        gameState = State::Dead;
                    }
                    else{
                        snake[0]->setPosition(Vector2f(thisSectionPosition.x+20,thisSectionPosition.y));
                        newDiffX = abs(snake[0]->getPosition().x - target.x);
                        newDiffY = abs(snake[0]->getPosition().y - target.y);
                        break;
                    }

                }

                case Direction::LEFT :
                {
                    if(thisSectionPosition.x-20<0)
                    {
                        gameState = State::Dead;
                    }
                    else{
                        snake[0]->setPosition(Vector2f(thisSectionPosition.x-20,thisSectionPosition.y));
                        newDiffX = abs(snake[0]->getPosition().x - target.x);
                        newDiffY = abs(snake[0]->getPosition().y - target.y);
                        break;
                    }

                }

                case Direction::UP :
                {
                    if(thisSectionPosition.y-20<0)
                    {
                        gameState = State::Dead;
                    }
                    else{
                        snake[0]->setPosition(Vector2f(thisSectionPosition.x,thisSectionPosition.y-20));
                        newDiffX = abs(snake[0]->getPosition().x - target.x);
                        newDiffY = abs(snake[0]->getPosition().y - target.y);
                        break;
                    }

                }

                case Direction::DOWN :
                {
                    if(thisSectionPosition.y+20>=Screen::y)
                    {
                        gameState = State::Dead;
                    }
                    else{
                        snake[0]->setPosition(Vector2f(thisSectionPosition.x,thisSectionPosition.y+20));
                        newDiffX = abs(snake[0]->getPosition().x - target.x);
                        newDiffY = abs(snake[0]->getPosition().y - target.y);
                        break;
                    }

                }

            }

            //tail
            for(int s=1;s<snake.size();s++)
            {
                thisSectionPosition = snake[s]->getPosition();
                snake[s]->setPosition(lastSectionPosition);
                lastSectionPosition = thisSectionPosition;
            }

            //update snake section
            for(SnakeSection *s:snake)
            {
                s->update();
            }

            if(directionCopy.size()==0)
            {
                gameState=State::Dead;
            }
            //check if target reach
            if(snake[0]->getPosition()==target)
            {
                gameState = State::Pause;
                delete apple;

            }
            //calculate fitness for each move
            if(newDiffX<thisDiffX || newDiffY<thisDiffY)
            {
                fitness++;
            }
            else{
                fitness--;
            }

    }

};

class SnakePopulation
{
public:
    vector<Snake*> population;
    SnakePopulation(int max_population, int totalMoves)
    {

        for(int i=0;i<max_population;i++)
        {
            Snake *s = new Snake(totalMoves);
            population.push_back(s);
        }

    }
    void populationSort()
    {
        std::sort(population.begin(),population.end(),[](const auto& s1, const auto& s2){ return s1->fitness > s2->fitness;});
    }

};

Snake* create_child(Snake* father,Snake* mother,double parent_ratio,double mutate_propbability)
{


    vector<int>parentGenes;
    int new_total_moves = father->directionQueue.size();

    if(randNum(10)<mutate_propbability)
    {
        for(int i=0;i<new_total_moves;i++)
        {
            if(randNum(100)<parent_ratio)
            {
                parentGenes.push_back(mother->directionQueue[i]);
            }
            else
            {
                parentGenes.push_back(father->directionQueue[i]);
            }
        }
    }
    else
    {
        for(int i=0;i<new_total_moves;i++)
        {
            if(randNum(100)<parent_ratio)
            {
                parentGenes.push_back(mother->directionQueue[i]);
            }
            else
            {
                parentGenes.push_back(father->directionQueue[i]);
            }
        }
    }

    new_total_moves++;
    parentGenes.push_back(randNum(4));
    Snake *child = new Snake(new_total_moves,parentGenes);
    return child;
}

int main()
{
    RenderWindow window(VideoMode(Screen::x, Screen::y), " Snake!");

    Apple *apple = new Apple(Vector2f(200,160));

    double population_size = 30.0;

    double parent_ratio = 50.0;
    double mutate_probability = 100.0;
    double transfer_ratio = 15.0;
    double crossover_ratio = 15.0;

    int moves_per_snake = 3;
    int crossover = (crossover_ratio*population_size)/100.0;

    int transferCount = (transfer_ratio*population_size)/100.0;

    int newIndivisualPerGen = population_size - transferCount;

    Vector2f target = apple->getPosition();

    Snake *father;
    Snake *mother;

    gameState = State::Play;
    isPause = false;
    int gen=0;


    SnakePopulation *generation = new SnakePopulation(population_size,moves_per_snake);

    while (window.isOpen())
    {
        sf::Event event;
        int maxFit = 0;
        int nextMax = 0;
        /*if(gen==0)
        {

            generation = new SnakePopulation(population_size,parent_ratio,mutate_probability,transfer_ratio,crossover,moves_per_snake);
        }
        else
        {
            generation = new SnakePopulation(population_size,parent_ratio,mutate_probability,transfer_ratio,crossover,moves_per_snake,father,mother);
        }*/
        gen++;
        cout<<"gen "<<gen<<endl;

        for(int i=0;i<generation->population.size();)
        {
            //close input
            while (window.pollEvent(event))
            {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if(event.type == Event::KeyPressed)
                    {
                        //quit
                        if(Keyboard::isKeyPressed(Keyboard::Escape)){
                            window.close();
                        }
                    }
            }
            if(gameState==State::Play && !isPause)
            {
                //update every frame
                generation->population[i]->update(apple);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //draw every frame
                window.clear();
                for(SnakeSection *section:generation->population[i]->snake)
                {
                    window.draw(section->getShape());
                }
                window.draw(apple->getShape());
                window.display();
            }
            else if(gameState==State::Dead && !isPause)
            {
                gameState = State::Play;
                i++;

            }
            else if(gameState==State::Pause && !isPause){
                isPause = true;
                cout<<"=================================="<<endl;
                cout<<"=================================="<<endl;
                cout<<"=================================="<<endl;
                cout<<"snake winner info:"<<endl;
                cout<<"gen: "<<gen<<endl;
                cout<<"population: "<<i+1<<endl;
                cout<<"fitness: "<<generation->population[i]->fitness<<endl;
                cout<<"x: "<<generation->population[i]->snake[0]->getPosition().x<<endl;
                cout<<"y: "<<generation->population[i]->snake[0]->getPosition().x<<endl;
                cout<<"move set: ";
                for(int j=0;j<generation->population[i]->directionQueue.size();j++)
                {
                    cout<<generation->population[i]->directionQueue[j];
                    cout<<",";
                }

            }
        }
        generation->populationSort();

        for(int i=0;i<generation->population.size();i++)
        {
            cout<<"population: "<<i+1<<endl;
            cout<<"fitness: "<<generation->population[i]->fitness<<endl;
            /*cout<<"x "<<generation->population[i]->snake[0]->getPosition().x<<endl;
            cout<<"y "<<generation->population[i]->snake[0]->getPosition().x<<endl;*/
            cout<<"move set: ";
            for(int j=0;j<generation->population[i]->directionQueue.size();j++)
            {
                cout<<generation->population[i]->directionQueue[j];
                cout<<",";
            }
            cout<<endl;
        }


        vector<Snake*>newPopulation;

        /*for(int i=0;i<transferCount;i++)
        {
            newPopulation.push_back(generation->population[i]);
        }*/

        for(int i=0;i<newIndivisualPerGen;i++)
        {
            mother = generation->population[randNum(crossover+1)];
            father = generation->population[randNum(crossover+1)];

            newPopulation.push_back(create_child(father,mother,parent_ratio,mutate_probability));

        }

        generation->population = newPopulation;
        cout<<"==================================="<<endl;
        for(int i=0;i<generation->population.size();i++)
        {
            cout<<"population: "<<i+1<<endl;
            cout<<"move set: ";
            for(int j=0;j<generation->population[i]->directionQueue.size();j++)
            {
                cout<<generation->population[i]->directionQueue[j];
                cout<<",";
            }
            cout<<endl;

        }

    }
    return 0;
}
