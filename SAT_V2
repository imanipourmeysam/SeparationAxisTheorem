
////////////////////////////////////////////////////////////////////////////////////////////////////
//  Author IMANIPOUR Meysam.                                                                      //
//  Modified: Shapes move slowly, bounce, show dots, stop on collision.                           //
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

float dotProduct2f(sf::Vector2f Vec1, sf::Vector2f Vec2);
std::vector<float> getProjectedRange(std::vector<sf::Vector2f> All_vertices, sf::Vector2f Separated_axis);
bool isColliding(std::vector<float> Shape_1_range, std::vector<float> Shape_2_range);

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow win(sf::VideoMode(800, 600), "Meysam_win");

    bool keepPlaying = true;

    while (keepPlaying)
    {
        // Reset variables
        std::vector<sf::Vector2f> Vertices_coor_1, Vertices_coor_2;
        sf::VertexArray Shape_1, Shape_2;
        std::vector<sf::Vector2f> All_separation_axes;
        bool Is_colliding = false;
        bool shapes_ready = false;
        size_t Vertices_1_iteration_draw = 0, Vertices_2_iteration_draw = 0;
        sf::Vector2f velocity1, velocity2;

        // Ask for number of vertices
        size_t Vertices_num;
        std::cout << "Enter the vertices number for the first shape: ";
        std::cin >> Vertices_num;
        Shape_1.setPrimitiveType(sf::LineStrip);
        Shape_1.resize(Vertices_num + 1);

        std::cout << "Enter the vertices number for the second shape: ";
        std::cin >> Vertices_num;
        Shape_2.setPrimitiveType(sf::LineStrip);
        Shape_2.resize(Vertices_num + 1);

        std::cout << "Click to place the vertices of each shape (convex only).\n";

        bool Is_left_mouse_button_pressed = false;

        while (win.isOpen())
        {
            sf::Event Event;
            while (win.pollEvent(Event))
            {
                if (Event.type == sf::Event::Closed)
                    win.close();

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    Is_left_mouse_button_pressed = true;

                if (Event.type == sf::Event::MouseButtonReleased && Is_left_mouse_button_pressed)
                {
                    if (Vertices_coor_1.size() != Shape_1.getVertexCount() - 1)
                        Vertices_coor_1.push_back(sf::Vector2f(sf::Mouse::getPosition(win)));
                    else if (Vertices_coor_2.size() != Shape_2.getVertexCount() - 1)
                        Vertices_coor_2.push_back(sf::Vector2f(sf::Mouse::getPosition(win)));

                    Is_left_mouse_button_pressed = false;
                }
            }

            win.clear(sf::Color::Black);

            // Draw Shape 1
            while (Vertices_coor_1.size() == Shape_1.getVertexCount() - 1 && Vertices_1_iteration_draw != Shape_1.getVertexCount())
            {
                if (Vertices_1_iteration_draw == Shape_1.getVertexCount() - 1)
                    Shape_1[Vertices_1_iteration_draw].position = Vertices_coor_1[0];
                else
                    Shape_1[Vertices_1_iteration_draw].position = Vertices_coor_1[Vertices_1_iteration_draw];
                Vertices_1_iteration_draw++;
            }

            // Draw Shape 2
            while (Vertices_coor_2.size() == Shape_2.getVertexCount() - 1 && Vertices_2_iteration_draw != Shape_2.getVertexCount())
            {
                if (Vertices_2_iteration_draw == Shape_2.getVertexCount() - 1)
                    Shape_2[Vertices_2_iteration_draw].position = Vertices_coor_2[0];
                else
                    Shape_2[Vertices_2_iteration_draw].position = Vertices_coor_2[Vertices_2_iteration_draw];
                Vertices_2_iteration_draw++;
            }

            // Once shapes ready, generate separation axes and velocities
            if (!shapes_ready && Vertices_1_iteration_draw == Shape_1.getVertexCount() &&
                Vertices_2_iteration_draw == Shape_2.getVertexCount())
            {
                for (size_t i = 1; i < Shape_1.getVertexCount(); i++)
                {
                    sf::Vector2f temp = Shape_1[i].position - Shape_1[i - 1].position;
                    All_separation_axes.push_back(sf::Vector2f(-temp.y, temp.x));
                }
                for (size_t i = 1; i < Shape_2.getVertexCount(); i++)
                {
                    sf::Vector2f temp = Shape_2[i].position - Shape_2[i - 1].position;
                    All_separation_axes.push_back(sf::Vector2f(-temp.y, temp.x));
                }

                velocity1 = sf::Vector2f(((rand() % 5) - 2) * 0.05f, ((rand() % 5) - 2) * 0.05f);
                velocity2 = sf::Vector2f(((rand() % 5) - 2) * 0.05f, ((rand() % 5) - 2) * 0.05f);

                shapes_ready = true;
            }

            // --- Move shapes and check collisions ---
            if (shapes_ready && !Is_colliding)
            {
                for (auto& v : Vertices_coor_1) v += velocity1;
                for (auto& v : Vertices_coor_2) v += velocity2;

                // Bounce off edges
                for (auto& v : Vertices_coor_1)
                {
                    if (v.x < 0 || v.x > 800) velocity1.x *= -1;
                    if (v.y < 0 || v.y > 600) velocity1.y *= -1;
                }
                for (auto& v : Vertices_coor_2)
                {
                    if (v.x < 0 || v.x > 800) velocity2.x *= -1;
                    if (v.y < 0 || v.y > 600) velocity2.y *= -1;
                }

                // Update shapes
                for (size_t i = 0; i < Vertices_coor_1.size(); i++)
                    Shape_1[i].position = Vertices_coor_1[i];
                Shape_1[Shape_1.getVertexCount() - 1].position = Vertices_coor_1[0];

                for (size_t i = 0; i < Vertices_coor_2.size(); i++)
                    Shape_2[i].position = Vertices_coor_2[i];
                Shape_2[Shape_2.getVertexCount() - 1].position = Vertices_coor_2[0];

                // SAT collision detection
                bool collidingNow = true;
                for (auto axis : All_separation_axes)
                {
                    std::vector<float> Min_max_shape1 = getProjectedRange(Vertices_coor_1, axis);
                    std::vector<float> Min_max_shape2 = getProjectedRange(Vertices_coor_2, axis);
                    if (!isColliding(Min_max_shape1, Min_max_shape2))
                    {
                        collidingNow = false;
                        break;
                    }
                }

                if (collidingNow)
                {
                    Is_colliding = true;

                    for (size_t i = 0; i < Shape_1.getVertexCount(); i++)
                        Shape_1[i].color = sf::Color::Red;
                    for (size_t i = 0; i < Shape_2.getVertexCount(); i++)
                        Shape_2[i].color = sf::Color::Red;

                    velocity1 = sf::Vector2f(0, 0);
                    velocity2 = sf::Vector2f(0, 0);
                }
            }

            // Draw shapes
            win.draw(Shape_1);
            win.draw(Shape_2);

            // Draw vertex dots
            for (auto& v : Vertices_coor_1)
            {
                sf::CircleShape dot(3);
                dot.setFillColor(sf::Color::Yellow);
                dot.setOrigin(3, 3);
                dot.setPosition(v);
                win.draw(dot);
            }
            for (auto& v : Vertices_coor_2)
            {
                sf::CircleShape dot(3);
                dot.setFillColor(sf::Color::Cyan);
                dot.setOrigin(3, 3);
                dot.setPosition(v);
                win.draw(dot);
            }

            win.display();

            // If collided, ask for retry
            if (Is_colliding)
            {
                char answer;
                std::cout << "Collision detected! Try again? (y/n): ";
                std::cin >> answer;
                if (answer == 'n' || answer == 'N')
                    keepPlaying = false;
                break; // restart or exit
            }
        }
    }

    return 0;
}

// --- Helper functions ---
float dotProduct2f(sf::Vector2f Vec1, sf::Vector2f Vec2)
{
    return Vec1.x * Vec2.x + Vec1.y * Vec2.y;
}

std::vector<float> getProjectedRange(std::vector<sf::Vector2f> All_vertices, sf::Vector2f Separated_axis)
{
    std::vector<float> Range;
    for (auto v : All_vertices)
        Range.push_back(dotProduct2f(v, Separated_axis));

    return { *std::min_element(Range.begin(), Range.end()),
             *std::max_element(Range.begin(), Range.end()) };
}

bool isColliding(std::vector<float> Shape_1_range, std::vector<float> Shape_2_range)
{
    float Min_shape_1 = Shape_1_range[0];
    float Max_shape_1 = Shape_1_range[1];
    float Min_shape_2 = Shape_2_range[0];
    float Max_shape_2 = Shape_2_range[1];

    if (Min_shape_1 > Max_shape_2 || Min_shape_2 > Max_shape_1)
        return false;
    return true;
}
