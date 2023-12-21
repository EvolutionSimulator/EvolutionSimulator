#ifndef CREATURETRACKER_H
#define CREATURETRACKER_H

#include <SFML/Graphics.hpp>
#include <creature.h>
#include <deque>
#include <unordered_map>

class CreatureTracker
{
public:

    CreatureTracker(const Creature& creature);
    void Show();
    void UpdateData(float newValue); // Call this at each simulation step
    void DrawGraph(); // Function to draw the graph
    void DrawInfo();  // Function to draw the information
    void AddCreature(const Creature &creature);
    void UpdateCreatureData(int id, float energy);
    std::unordered_map<int, Creature> trackedCreatures;

private:
    const Creature& trackedCreature_;
    sf::RenderWindow window_;
    sf::Font font_;
    struct DataPoint {
        float value; // value of the variable at a specific simulation step
    };

    std::deque<DataPoint> historicalData; // Store historical data here
};

#endif // CREATURETRACKER_H
