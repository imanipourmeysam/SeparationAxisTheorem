/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Author IMANIPOUR Meysam.                                                                                                                                   //
//  This program use separation axis theorem to findout if two convex shape are colliding or not.                                                              //
//  It asks you first the number of vertices of each shape (shape1 and shape2).                                                                                //
//  For example if your input is 3, it means that you can draw a triangle and you can't draw a squre.                                                          //
//  Each click on the window correspond to one of these vertices.                                                                                              //  
//  Afther drawing the two shapes the program use separation axis theorm to tell you if there is a gap between the two shapes or not.                          //   
//  It's important to not that shapes should be convex. that means the internal angle of each vertex (angle inside the shape) shouldn't be more than 90 degree.//
//  This programm is just for learning purpose and there is no garantee for this :-)                                                                           //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


float dotProduct2f (sf::Vector2f Vec1, sf::Vector2f Vec2);
std::vector<float> getProjectedRange (std::vector<sf::Vector2f> All_vertices, sf::Vector2f Separated_axis);
bool isColliding (std::vector<float> Shape_1_range, std::vector<float> Shape_2_range);

int main()
{
    sf::RenderWindow win(sf::VideoMode(800,600), "Meysam_win");
    size_t Vertices_num;
    std::cout << "enter the vertices number for the first shape ";
    std::cin >> Vertices_num;
    sf::VertexArray Shape_1(sf::LineStrip, Vertices_num + 1);
    std::cout << "\nenter the vertices number for the second shape "; 
    std::cin >> Vertices_num;
    sf::VertexArray Shape_2(sf::LineStrip, Vertices_num + 1);
    std::cout << "Consider that each click correspond to a vertex of your shape\n";
    bool Is_left_mouse_button_pressed = false;
    bool magic = true;
    bool Is_colliding = false;
    int cnt = 0;  
    size_t Vertices_1_iteration_draw   = 0;
    size_t Vertices_2_iteration_draw   = 0;
    size_t Seperation_axis_iteration_1 = 1;
    size_t Seperation_axis_iteration_2 = 1;

    std::vector<sf::Vector2f> Vertices_coor_1;
    std::vector<sf::Vector2f> Vertices_coor_2;
    std::vector<sf::Vector2f> All_separation_axes;

    // main application loop.
    while (win.isOpen())
    {
        sf::Event Event;
        
        // handle events.
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
       
        while (Vertices_coor_1.size() == Shape_1.getVertexCount() - 1 &&  Vertices_1_iteration_draw != Shape_1.getVertexCount())
        {
            if (Vertices_1_iteration_draw == Shape_1.getVertexCount() -1)
                Shape_1[Vertices_1_iteration_draw].position = Vertices_coor_1[0];
            else
                Shape_1[Vertices_1_iteration_draw].position =  Vertices_coor_1[Vertices_1_iteration_draw];
            Vertices_1_iteration_draw++;
        }

        while (Vertices_coor_2.size() == Shape_2.getVertexCount() - 1 &&  Vertices_2_iteration_draw != Shape_2.getVertexCount())
        {
            if (Vertices_2_iteration_draw == Shape_2.getVertexCount() -1)
                Shape_2[Vertices_2_iteration_draw].position = Vertices_coor_2[0];
            else
                Shape_2[Vertices_2_iteration_draw].position =  Vertices_coor_2[Vertices_2_iteration_draw];
            Vertices_2_iteration_draw++;
        }

        if( Vertices_1_iteration_draw == Shape_1.getVertexCount() && Seperation_axis_iteration_1 != Shape_1.getVertexCount())
        {
           for (Seperation_axis_iteration_1; Seperation_axis_iteration_1 != Shape_1.getVertexCount(); Seperation_axis_iteration_1++)
           {
               sf::Vector2f temp = Shape_1[Seperation_axis_iteration_1].position - Shape_1[Seperation_axis_iteration_1-1].position;
               All_separation_axes.push_back(sf::Vector2f(-temp.y, temp.x));
           }

        }

        if( Vertices_2_iteration_draw == Shape_2.getVertexCount() && Seperation_axis_iteration_2 != Shape_2.getVertexCount())
        {
           for (Seperation_axis_iteration_2; Seperation_axis_iteration_2 != Shape_2.getVertexCount(); Seperation_axis_iteration_2++)
           {
               sf::Vector2f temp = Shape_2[Seperation_axis_iteration_2].position - Shape_2[Seperation_axis_iteration_2-1].position;
               All_separation_axes.push_back(sf::Vector2f(-temp.y, temp.x));
           }

        }
       
        if( All_separation_axes.size() == Shape_1.getVertexCount() + Shape_2.getVertexCount() - 2 && cnt == 0)
        {
            // iteratre over the separation axis of type sf::Vector2f.
            for (auto i : All_separation_axes)
            {
                std::vector<float> Min_max_shape1 = getProjectedRange(Vertices_coor_1, i);
                std::vector<float> Min_max_shape2 = getProjectedRange(Vertices_coor_2, i);
                if (isColliding(Min_max_shape1, Min_max_shape2) == false)
                {
                    magic = false;
                }
            }
            cnt++;
             
        }    
        
    if (magic == false && cnt == 1)
    {
        std::cout << "there is a gap\n";
        cnt = 2;
    }
    else if (magic == true && cnt == 1)
    {
        std::cout << "there is no gap\n";
        cnt = 2;
    }

    win.draw(Shape_1);
    win.draw(Shape_2);
    win.display();
   

    }
    return 0;
}

// this function return the value of dot procut between two vector.
float dotProduct2f (sf::Vector2f Vec1, sf::Vector2f Vec2)
{
    return Vec1.x * Vec2.x  + Vec1.y * Vec2.y;
}


// this function return the minimum and the maximum value after projecting all the vertices of a shape.
std::vector<float> getProjectedRange (std::vector<sf::Vector2f> All_vertices, sf::Vector2f Separated_axis)
{
    
    std::vector<sf::Vector2f>::iterator iter = All_vertices.begin(); 
    std::vector<float> Range;
    std::vector<float> Min_max;
    for (iter; iter != All_vertices.end(); iter++)
        Range.push_back(dotProduct2f(*iter, Separated_axis));
    
    Min_max.push_back(*min_element(Range.begin(), Range.end()));
    Min_max.push_back(*max_element(Range.begin(), Range.end()));

    return Min_max;
}

// return true for the two ranges of shapes if they are colliding and if the false indicate that there is a gap. .
bool isColliding (std::vector<float> Shape_1_range, std::vector<float> Shape_2_range)
{
   float Min_shape_1 = *Shape_1_range.begin();
   float Max_shape_1 = *(Shape_1_range.end()-1);
   float Min_shape_2 = *Shape_2_range.begin();
   float Max_shape_2 = *(Shape_2_range.end()-1);
   
   if (Min_shape_1 < Min_shape_2 && Max_shape_1 < Min_shape_2)
       return false;
   else if (Min_shape_2 < Min_shape_1 && Max_shape_2 < Min_shape_1)
       return false;
   else 
      return true; 

}

